#!/usr/bin/env bash
set -euo pipefail
umask 077

NAME="${1:-aclgs_test}"               # ← 기본 클라이언트 이름
SRV="165.132.142.126"                 # 서버 공인 IP/도메인
PORT="1194"

CA_PATH="/etc/openvpn/server/ca.crt"  # server.conf 의 ca 경로
TA="/etc/openvpn/server/ta.key"       # server.conf: tls-auth ... 0

PKI="$HOME/easy-rsa/pki"
CRT="$PKI/issued/$NAME.crt"
KEY="$PKI/private/$NAME.key"
OUT="${NAME}_inline.ovpn"

pem() { sed -n "/-----BEGIN $1-----/,/-----END $1-----/p" "$2"; }

# 존재 확인
sudo test -r "$CA_PATH"; sudo test -r "$TA"
[[ -r "$CRT" && -r "$KEY" ]]

{
cat <<EOF
client
dev tun
proto udp
remote $SRV $PORT
resolv-retry infinite
nobind
persist-key
persist-tun
remote-cert-tls server
auth SHA512
data-ciphers AES-256-GCM:AES-128-GCM:AES-256-CBC
data-ciphers-fallback AES-256-CBC
verb 3
key-direction 1
<ca>
EOF
sudo sed -n '/-----BEGIN CERTIFICATE-----/,/-----END CERTIFICATE-----/p' "$CA_PATH"
cat <<'EOF'
</ca>
<cert>
EOF
pem "CERTIFICATE" "$CRT"
cat <<'EOF'
</cert>
<key>
EOF
pem "PRIVATE KEY" "$KEY"
cat <<'EOF'
</key>
<tls-auth>
EOF
sudo sed -n '/-----BEGIN OpenVPN Static key V1-----/,/-----END OpenVPN Static key V1-----/p' "$TA"
cat <<'EOF'
</tls-auth>
EOF
} > "$OUT"

chmod 600 "$OUT"
echo "생성됨: $OUT"
