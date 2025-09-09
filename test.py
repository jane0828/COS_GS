import socket, struct
s = socket.create_connection(("127.0.0.1", 2001))
payload = b'\x01\x02\x03\x04'   # 예시 페이로드
s.sendall(struct.pack("!H", len(payload)) + payload)
s.close()
