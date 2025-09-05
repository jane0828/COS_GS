#include "tcp_tc.h"

int main(int argc, char** argv){
    std::string ip = argv[1];
    int port = atoi(argv[2]);

    ExpediteInterface tcp_tc;

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);    //IPV4와 tcp를 사용하는 소켓 생성
    if(server_sock == -1){
        printf("Fail to create socket \n");
        close(server_sock);
        return 1;
    }
    else{
        printf("create socket success \n");
    }

    struct sockaddr_in serv_addr;    //서버의 주소체계를 담을 sockaddr_in 구조체
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); //모든 ip에 들어오는 요청을 받고 싶다면 INADDR_ANY 사용

    if(bind(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){    //소켓과 서버의 주소체계를 담은 sockaddr_in 구조체 연결(바인드)
        close(server_sock);
        printf("socket binding error \n");
        return 1;
    }
    else{
        printf("socket binding success \n");
    }

    int lis = listen(server_sock, 5);    //서버 소켓에 들어오는 요청 수신 (최대 5개로 설정해놓음)
    if (lis != -1){
        printf("listen success \n");
    }

    struct sockaddr_in client_addr;    //서버와의 연결을 요청한 client의 주소값을 담을 구조체
    socklen_t clen = sizeof(client_addr);

    int new_fd = accept(server_sock, (struct sockaddr*)&client_addr, &clen);    //서버 소켓으로 들어온 요청을 승인하고 client의 주소체계를 client_addr에 저장하여 서버-클라이언트 fd생성

    if (new_fd < 0){
        perror("accept error \n");
        printf("%d \n", new_fd);
        close(server_sock);
        close(new_fd);
        return 1;
    }
    else{
        printf("accept success \n");
    }

    std::vector<uint8_t> read_buf(1000);
    int valread;
    int totalread = 0;
    std::vector<uint8_t> totalread_buf;
    int readsize;

    while (1){
        valread = read(new_fd, read_buf.data(), read_buf.size());
        totalread += valread;
        totalread_buf.resize(totalread_buf.size() + valread);
        memcpy(&totalread_buf[totalread_buf.size() - valread], read_buf.data(), valread);

        if (totalread > 8){
            readsize = totalread_buf[4] | totalread_buf[5] << 8 | totalread_buf[6] << 16 | totalread_buf[7] << 24;    //수신패킷의 총 길이
        }

        if (totalread >= readsize){
            printf("readsize : %d \n", readsize);    //수신패킷의 길이 이상의 데이터가 들어오면 read 종료
            break;
        }
        
    }

    if (totalread <= 0){
        printf("read error \n");
    }

    read_buf.resize(readsize);    //수신패킷의 길이만큼으로 버퍼 크기 조정
    
    uint32_t totalLength = readsize + 12;    //ack 패킷 구성

    std::vector<uint8_t> ack_buf(totalLength);

    memcpy(ack_buf.data(), read_buf.data(), sizeof(uint64_t));

    memcpy(ack_buf.data()+4, &totalLength, sizeof(uint32_t));

    //ack_buf[11] = 0x02;
    uint32_t message_type = 0x02;

    memcpy(ack_buf.data() + 8, &message_type, sizeof(uint32_t));

    memset(ack_buf.data() + 12, 0, sizeof(uint64_t));

    if (tcp_tc.VerifyCRC32(read_buf) == true){
        ack_buf[23] = 0x00;
    }
    else{
        ack_buf[23] = 0x01;
    }

    std::vector<uint8_t> data_v(read_buf.begin() + 12, read_buf.end()-8);

    memcpy(ack_buf.data()+24, data_v.data(), data_v.size());

    std::vector<uint8_t> crc_data(read_buf.begin(), read_buf.end() - 2 * sizeof(uint32_t));

    uint32_t CRC = tcp_tc.GetCRC32(crc_data);

    memcpy(&ack_buf[ack_buf.size() - 8], &CRC, sizeof(uint32_t));

    uint32_t postamble = POSTAMBLE;

    memcpy(&ack_buf[ack_buf.size() - 4], &postamble, sizeof(uint32_t));

    if(send(new_fd, ack_buf.data(), ack_buf.size(), 0) == (ssize_t)ack_buf.size()){    //ack 데이터를 서버-클라이언트 fd로 send
        printf("send Success\n");
    };

    for(size_t i = 0; i < ack_buf.size(); i++){
        printf("0x%02x ", ack_buf[i]);
    }
    printf("\n");

    close(server_sock);
    close(new_fd);

    return 0;
}