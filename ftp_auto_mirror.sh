#!/usr/bin/env bash
set -euo pipefail
HOST="${HOST:-10.8.0.3}"
USER="${USER:-peter}"
PASS="${PASS:-password}"
PORT="${PORT:-21}"
REMOTE_DIR="${REMOTE_DIR:-/var/log}"
LOCAL_DIR="${LOCAL_DIR:-$HOME/COS_GS/ftp}"
mkdir -p "$LOCAL_DIR"
lftp -u "$USER","$PASS" -p "$PORT" "ftp://$HOST" -e "set net:timeout 20; set net:max-retries 5; set ftp:passive-mode true; set xfer:clobber on; mirror --continue --only-newer --parallel=4 \"$REMOTE_DIR\" \"$LOCAL_DIR\"; bye"
