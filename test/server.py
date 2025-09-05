#!/usr/bin/env python3
import socket

HOST = "10.8.0.1"   # 모든 인터페이스에서 수신 (VPN만 받으려면 "10.8.0.1")
PORT = 5555

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((HOST, PORT))
    s.listen(5)
    print(f"LISTEN {HOST}:{PORT}")
    while True:
        conn, addr = s.accept()
        print("CONN from", addr)
        with conn:
            while True:
                data = conn.recv(4096)
                if not data:
                    break
                print("RX", len(data), data[:64])
                conn.sendall(b"ACK:" + data)
