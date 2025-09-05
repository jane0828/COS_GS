#include "tcp_tc.h"

int main(int argc, char** argv){

    if (argc != 3){
        printf("%s \n", "The number of parameter is incorrect / argv[1] = ip / argv[2] = port");
        return 1;
    }    //ip와 port 입력 확인

    ExpediteInterface tcp_tc;

    std::vector<uint8_t> pnData;
    std::vector<uint8_t> pnOut;

    int client_socket;
    struct sockaddr_in in;
    uint8_t data;
    
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){    //AF_INET : IPV4 주소 사용, SOCK_STREAM : TCP 사용
        perror("socket creation failed\n");
        return 1;
    }    //ip와 연결할 socket(fd) 생성
    else{
        printf("socket created\n");
    }

    memset(&in, 0, sizeof(in));    //ip 정보를 담을 socket 구조체 초기화
    in.sin_family = AF_INET;    //ipv4 사용
    in.sin_port = htons(atoi(argv[2]));    //입력받은 port 사용 => 연결할 server의 port
    if (inet_pton(AF_INET, argv[1], &in.sin_addr) != 1){    //입력받은 ip 주소 사용 => 연결할 서버의 ip
        perror("ip address error\n");
        return 1;
    }
    else{
        printf("set ip success\n");
    }

    if (connect(client_socket, (struct sockaddr *)&in, sizeof(in)) == -1){    //socket 구조체의 정보를 사용해서 client_socket(fd)으로 서버와 연결
        perror("connection fail\n");
        return 1;
    }
    else{
        printf("connection success\n");
    }

    printf("Telecommand Data (input -1 for finish) : ");
    while(scanf("%hhx", &data) && data != 0xFF){    //telecommand 데이터(hex) 입력 -> -1 입력 되기 전까지의 값들 데이터 vector에 추가
        pnData.push_back(data);
        //pnData.insert(pnData.begin(), data);
    }

    size_t Datasize = pnData.size();    //입력 데이터 개수 확인 => N bytes

    tcp_tc.EncapsulateTC(pnData, pnOut);    //패킷 구성 => crc 부분 확인 필요...만약 오류나면 polynomial 바꾸고 결과값 뒤집어보기(reverseCRC 함수)

    send(client_socket, pnOut.data(), pnOut.size(), 0);    //전송

    // std::vector<uint8_t> received;
    // received.resize(tcp_tc.pnOut.size() + 12);

    // read(client_socket, received.data(), tcp_tc.pnOut.size()+12);

    // std::vector<uint8_t> Preamble(received.begin(), received.begin() + 4);
    // std::stringstream ss;
    // std::copy(Preamble.begin(), Preamble.begin()+4, std::ostream_iterator<uint8_t>(ss, " "));
    // std::string result = ss.str();
    // printf("%s", ss.c_str());
    // std::vector<uint8_t> Total_Length(received.begin()+4, received.begin()+8);

    uint8_t received[pnOut.size() + 12];

    read(client_socket, received, pnOut.size() + 12);    //응답 확인

    uint32_t preamble;
    printf("Preamble :\n");
    memcpy(&preamble, received, sizeof(uint32_t));
    printf("0x%08x\n", preamble);

    uint32_t totallength;
    printf("Total Length :\n");
    memcpy(&totallength, &received[4], sizeof(uint32_t));
    printf("0x%08x\n", totallength);

    uint32_t messagetype;
    printf("Message Type :\n");
    memcpy(&messagetype, &received[8], sizeof(uint32_t));
    printf("0x%08x\n", messagetype);

    printf("Timetag :\n");
    QString s = tcp_tc.IRIGtoTime(&received[12]);

    uint32_t status;
    printf("Status :\n");
    memcpy(&status, &received[20], sizeof(uint32_t));
    printf("0x%08x\n", status);

    printf("Echoed Telecommand :\n");
    for (size_t j = 24; j < 24 + Datasize; j++){
        printf("0x%02hhx ", received[j]);
    }
    printf("\n");

    uint32_t crc32;
    printf("CRC32 :\n");
    memcpy(&crc32, &received[24+Datasize], sizeof(uint32_t));
    printf("0x%08x\n", crc32);

    uint32_t postamble;
    printf("Postamble :\n");
    memcpy(&postamble, &received[28+Datasize], sizeof(uint32_t));
    printf("0x%08x\n", postamble);

    close(client_socket);
    return 0;

    // printf("Preamble :\n");
    // for (int i = 0; i < 4; i ++){
    //     printf("%hhx", received[i]);
    // }
    // printf("\n");

    // printf("Total length :\n");
    // for (int i = 4; i < 8; i++){
    //     printf("%hhx", received[i]);
    // }
    // printf("\n");

    // printf("Message type :\n");
    // for (int i = 8, j = 3; i < 12; i++, j-=2){
    //     printf("%hhx", received[i]);
    // }    
    // printf("\n");

    // printf("Timetag :\n");
    // QString s = tcp_tc.IRIGtoTime(&received[12]);
    // printf("%s \n", qPrintable(s));

    // printf("Status :\n");
    // printf("%hhx \n", received[23]);

    // printf("Echoed Telecommand :\n");
    // for (size_t j = 24; j < 24 + Datasize; j++){
    //     printf("%hhx ", received[j]);
    // }
    // printf("\n");

    // printf("CRC32 :\n");
    // for (size_t j = 24 + Datasize; j < 28 + Datasize; j++){
    //     printf("%hhx", received[j]);
    // }
    // printf("\n");

    // printf("Postamble :\n");
    // for (size_t j = 28 + Datasize; j < 32 + Datasize; j++){
    //     printf("%hhx", received[j]);
    // }
    // printf("\n");
}
