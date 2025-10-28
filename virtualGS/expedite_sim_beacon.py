import socket

# Expedite 모사 장비 (TCP 서버)
HOST = "0.0.0.0"
PORT = 2001

hex_data = """
D4 C3 B2 A1 C3 00 00 00 00 00 00 00 C5 52 02 05
38 13 30 07 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 86 83 7E 01 08 1A C0 49 00 88 00 0F
50 1E F2 53 00 00 07 07 07 07 07 00 00 00 00 00
00 00 00 00 00 00 00 0A 00 02 00 13 50 0F 00 5F
F9 E2 23 7E 00 00 00 21 FF 89 FF 45 00 02 00 00
00 05 00 00 36 1C 01 00 00 00 02 7A 06 01 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 D8 51 2F 93 FC E8 7E FF 2B
3C 4D 5E 
""".strip()

data_bytes = bytes.fromhex(hex_data.replace("\n", " "))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    print(f"[ExpediteSim] TCP 서버 대기 중... {HOST}:{PORT}")

    conn, addr = s.accept()
    with conn:
        print(f"[ExpediteSim] 연결 수립: {addr}")

        while True:
            cmd = input("데이터를 보내겠습니까? (y/n) > ").strip().lower()
            if cmd == "y":
                conn.sendall(data_bytes)
                print(f"[ExpediteSim] {len(data_bytes)} bytes 전송 완료")
            elif cmd == "n":
                print("[ExpediteSim] 종료")
                break
