#include <utl/utl.h>
#include <ifc/ifc.h>

static void * SGS_IPServerTask(void * param)
{
    SGS_IP * ip = (SGS_IP *)param;
    socklen_t socklen = sizeof(ip->clnt_addr);
    ip->clnt_sock = accept(ip->serv_sock, (sockaddr *)&ip->clnt_addr, &socklen);
    
    if(ip->clnt_sock == -1)
    	ip->console->error("ServerTask SocketAccept Failure.");
        
    char test_msg[64];
    snprintf(test_msg, 64, "Dangun!!");
    write(ip->clnt_sock, test_msg, sizeof(test_msg));
    

    return NULL;
}

static void * SGS_IPClientTask(void * param)
{
    SGS_IP * ip = (SGS_IP *)param;

    return NULL;
}

SGS_IP::SGS_IP(int _mode, int _dst, int _port)
{
    this->mode = _mode;
    this->port = _port;
    switch(_mode) {
    case SGS_IO_IP_MODE_SERVER: {
        this->InitializeServer();
        break;
    }
    case SGS_IO_IP_MODE_CLIENT: {
        this->InitializeClient();
        break;
    }
    default: {
        
        break;
    }
    }
}

SGS_IP::~SGS_IP()
{
    close(this->clnt_sock);
    close(this->serv_sock);
}

int SGS_IP::InitializeServer()
{
    if((this->descripter = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        this->console->error("Socket Creation Error\n");
        return -1;
    }
    
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = INADDR_ANY;
    this->serv_addr.sin_port = htons(this->port);

    if((bind(this->descripter, (const sockaddr *)&this->serv_addr, sizeof(this->serv_addr)) < 0))
    {
        this->console->error("Bind Failed.\n");
        return -3;
    }
    
    if(listen(this->descripter, 3) < 0)
    {
        this->console->error("Listen Error.\n");
        return -4;
    }
    
    return 0;
}

int SGS_IP::InitializeClient()
{
    int str_len;
    
    this->clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    if(this->clnt_sock == -1)
        this->console->error("ClientTask SocketAccept Failure.");
    
    memset(&this->serv_addr, 0, sizeof(serv_addr));
    this->serv_addr.sin_family = AF_INET;
    // this->serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // this->serv_addr.sin_port = htons(atoi(argv[2]));
    return 0;
}

