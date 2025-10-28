import socket

# Expedite 모사 장비 (TCP 서버)
HOST = "0.0.0.0"
PORT = 2001

hex_data = """
D4 C3 B2 A1 D0 00 00 00 00 00 00 00 C5 52 02 08
32 04 89 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 86 83 7F 01 08 25 C0 00 00 95 00 0F
46 4D DB EA 00 00 00 00 00 00 1E 00 00 00 00 00
04 00 70 CC 14 86 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 BB AB 02 98 EB F1 A0 F8 2B 3C 4D 5E
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
