HOST=172.31.21.12 USER=contec-3 PASS='12345678' REMOTE_DIR=/home/contec-3/MissionData LOCAL_DIR=~/COS_GS/ftp \
~/COS_GS/ftp_auto_mirror.sh



(crontab -l 2>/dev/null; echo "*/5 * * * * HOST=172.31.21.12 USER=contec-3 PASS=12345678 REMOTE_DIR=/home/contec-3/MissionData LOCAL_DIR=$HOME/COS_GS/ftp $HOME/COS_GS/ftp_auto_mirror.sh >> $HOME/COS_GS/ftp/ftp_sync.log 2>&1") | crontab -
