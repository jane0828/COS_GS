#!/usr/bin/env python3
# ccsds_sat_sim.py
import socket, re, sys, time, struct
from typing import List, Tuple

HOST = "0.0.0.0"
PORT = 2001
MARKER = b"\x08\x78"   # msg_id == 0x0878

USE_LEN_PREFIX = False     # True면 각 프레임 앞에 2바이트 길이(빅엔디언)를 붙여 보냅니다.
TX_PACE_SEC    = 0.0005   # 프레임 사이 텀(초). 0 으로 해도 됨. 디버깅 때만 살짝 둬도 OK.

SEG_MAP = {0: "CONT", 1: "FIRST", 2: "LAST", 3: "UNSEG"}

def parse_c_arrays(path: str) -> List[bytes]:
    """test.c 안의 static const unsigned char pktXXX[...] = { ... }; 들을 파싱해서 raw 바이트 배열 리스트로 반환"""
    import re
    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        src = f.read()

    # 배열 블록 추출: 이름까지 캡처, 공백/개행에 관대
    blocks = re.findall(
        r"static\s+const\s+unsigned\s+char\s+(\w+)\s*\[\s*\d+\s*\]\s*=\s*\{(.*?)\}\s*;",
        src, flags=re.S
    )
    print(f"[load] C-array blocks found: {len(blocks)}")

    pkts: List[bytes] = []
    for i, (name, body) in enumerate(blocks):
        hexes = re.findall(r"0x([0-9A-Fa-f]{2})", body)
        if not hexes:
            print(f"[load][skip] #{i:04d} {name}: no hex bytes found")
            continue
        buf = bytes(int(h, 16) for h in hexes)
        pkts.append(buf)
        print(f"[load][block] #{i:04d} {name}: {len(buf)} bytes")

    return pkts

def extract_ccsds_frame(buf: bytes) -> Tuple[bytes, int, int]:
    """
    buf 안에서 msg_id(0x0878)를 찾아 CCSDS 프레임을 잘라낸다.
    반환: (frame_bytes, seg_flag(상위2bit), seq_count(하위8bit))
    - 총길이 = 6 + (packet_length + 1)  (CCSDS Primary Header 규칙)
    - 데이터 TM이면 timecode(6) + padding(4) + payload가 뒤따름
    - EOF(데이터 없음) TM이면 보통 12바이트(Primary 6 + timecode 6)가 될 것(구현은 packet_length에 따름)
    """
    idx = buf.find(MARKER)
    if idx < 0 or idx + 6 > len(buf):
        raise ValueError("CCSDS msg_id (0x0878) not found in packet")
    # Primary Header: [0..1]=msg_id, [2..3]=seq, [4..5]=packet_length
    seq_hi = buf[idx + 2]
    seq_lo = buf[idx + 3]
    seg_flag = (seq_hi >> 6) & 0x03   # 01:first, 00:cont, 10:last, 11:unseg
    seq_count = seq_lo               # 네가 말한 0x00, 0x01, ...
    packet_length = (buf[idx + 4] << 8) | (buf[idx + 5] << 0)
    total_len = 6 + (packet_length + 1)
    # 범위 방어: pcap/UDP payload 길이와 딱 맞을 텐데 혹시 모자라면 남은 만큼만
    end = min(idx + total_len, len(buf))
    frame = buf[idx:end]
    return frame, seg_flag, seq_count

def load_frames_from_cfile(path: str) -> List[bytes]:
    raw_pkts = parse_c_arrays(path)
    frames: List[bytes] = []
    for i, p in enumerate(raw_pkts):
        try:
            f, seg, cnt = extract_ccsds_frame(p)
            frames.append(f)
            # 디버깅 로그(선택)
            plen = (f[4] << 8) | f[5]
            print(f"[load] #{i:04d} {SEG_MAP.get(seg,'?'):>5} seq=0x{cnt:02X} "
                  f"len={plen} total={len(f)} bytes")
        except Exception as e:
            print(f"[load][skip] #{i:04d} {e}")
    return frames

def _extract_seq(frame: bytes) -> int | None:
    """프레임 안에서 seq(마커 직후 1바이트) 추출: ... 08 78 [seg] [seq] [len_hi] [len_lo] ..."""
    idx = frame.find(MARKER)
    if idx < 0 or idx + 3 >= len(frame):
        return None
    return frame[idx + 3]

def run_server(frames: list[bytes]):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen(1)
        print(f"[SatSim] TCP 서버 대기중... {HOST}:{PORT}")

        while True:
            try:
                conn, addr = s.accept()
                print(f"[SatSim] 연결됨: {addr}")
                with conn:
                    # 전송 성능/레이턴시 향상
                    try:
                        conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                    except OSError:
                        pass

                    while True:
                        cmd = input("데이터를 보내겠습니까? (y=전부, 숫자=그 개수, n/q=종료) > ").strip().lower()
                        if cmd in ("n", "q"):
                            print("[SatSim] 전송 없이 연결 종료")
                            try:
                                conn.shutdown(socket.SHUT_WR)
                            except OSError:
                                pass
                            break  # 연결 종료

                        try:
                            send_n = len(frames) if cmd == "y" else int(cmd)
                        except:
                            print("[SatSim] 입력은 y/n/q 또는 숫자(보낼 개수)")
                            continue

                        send_n = max(0, min(send_n, len(frames)))
                        if send_n == 0:
                            print("[SatSim] 보낼 프레임이 없습니다.")
                            try:
                                conn.shutdown(socket.SHUT_WR)
                            except OSError:
                                pass
                            break

                        # === 전송 시작: 절대 스킵/정렬 없음 ===
                        sent = 0
                        missing = []
                        for i, f in enumerate(frames[:send_n]):
                            # 길이 프리픽스(선택)
                            payload = (struct.pack(">H", len(f)) + f) if USE_LEN_PREFIX else f
                            seq = _extract_seq(f)

                            try:
                                conn.sendall(payload)
                            except Exception as e:
                                print(f"[tx][error] idx={i:04d} seq={None if seq is None else f'0x{seq:02X}'}: {e} → 1회 재시도")
                                # 1회 재시도
                                conn.sendall(payload)

                            sent += 1
                            if seq is None:
                                missing.append(i)
                                print(f"[tx] #{i:04d} seq=?    len={len(f)} {'(+2)' if USE_LEN_PREFIX else ''}")
                            else:
                                print(f"[tx] #{i:04d} seq=0x{seq:02X} len={len(f)} {'(+2)' if USE_LEN_PREFIX else ''}")

                            if TX_PACE_SEC > 0:
                                time.sleep(TX_PACE_SEC)

                        print(f"[SatSim] {sent} 프레임 전송 완료 — 연결 종료")
                        if missing:
                            print(f"[tx][note] seq 파싱 실패 index: {missing}")

                        # 전송 완료 후 연결 종료
                        try:
                            conn.shutdown(socket.SHUT_WR)
                        except OSError:
                            pass
                        break  # 연결 종료

                # with 블록을 벗어나면 conn.close() 자동 수행
                print("[SatSim] 연결 정리 완료. 다음 클라이언트 대기...")

            except KeyboardInterrupt:
                print("\n[SatSim] 사용자 종료")
                break

if __name__ == "__main__":
    cfile = "test_with_0x55.c" if len(sys.argv) < 2 else sys.argv[1]
    frames = load_frames_from_cfile(cfile)
    if not frames:
        print("[SatSim] 보낼 CCSDS 프레임이 없습니다.")
        sys.exit(1)
    run_server(frames)
