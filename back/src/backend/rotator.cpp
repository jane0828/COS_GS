#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#endif


void * RotatorReadInfo(void * param)
{
    SGS_ROTATOR * rot = (SGS_ROTATOR *)param;
    rot->RotatorReading = true;
    int byte = 0;
    int read_count = 0;
    while(rot->RotatorReading)
    {
        tcflush(rot->descripter, TCIOFLUSH);
        memset(rot->ReadBuf, 0, strlen(rot->ReadBuf));

        if(write(rot->descripter, "C2\r\n", 4) > 0)
        {
            rot->RotatorReady = true;
        }
        else
        {
            rot->callback(SGS_CONMOD_ERR, "Rotator Read Failed. Please Check Again.");
            rot->RotatorReading = false;
            break;
        }

        rot->RotatorReading = true;
        
        sleep(1);
        while(rot->RotatorReading && rot->RotatorReady)
        {
            byte = read(rot->descripter, &rot->ReadBuf[read_count], 1);
            if (byte < 1 && read_count > 3 && rot->ReadBuf[read_count - 1] == '\n')
            {
                char azi[5] = {0}, ele[5] = {0};

                if (rot->ReadBuf[3] == '-')	// AZ=-XXX
                {
                    // Sometime azi / ele can be negative value
                    // in that case, we need calibration

                    strncpy(azi, &rot->ReadBuf[3], 4);
                    rot->az= atoi(azi);

                    fprintf(stderr, "Rotator azimuth calibration needed \n");

                    if (rot->ReadBuf[12] == '-')		// AZ=-XXX  EL=-XXX
                    {
                        strncpy(ele, &rot->ReadBuf[12], 4);
                        rot->el = atoi(ele);

                        fprintf(stderr, "Rotator elevation calibration needed \n");
                    }
                    else								// AZ=-XXX  EL=XXX
                    {
                        strncpy(ele, &rot->ReadBuf[12], 3);
                        rot->el = atoi(ele);
                    }
                }
                else
                {
                    strncpy(azi, &rot->ReadBuf[3], 3);
                    rot->az = atoi(azi);

                    if (rot->ReadBuf[11] == '-')		// AZ=XXX  EL=-XXX
                    {
                        strncpy(ele, &rot->ReadBuf[11], 4);
                        rot->el = atoi(ele);

                        fprintf(stderr, "Rotator elevation calibration needed \n");
                    }
                    else								// AZ=XXX  EL=XXX
                    {
                        strncpy(ele, &rot->ReadBuf[11], 3);
                        rot->el = atoi(ele);
                    }
                }
                read_count = 0;
                sleep(1);
                rot->RotatorReady = false;
            }
            else if (byte > 0)
            {
                read_count++;
            }
            else if (byte < 1)
            {
                rot->callback(SGS_CONMOD_ERR, "Rotator Read Failed.");
                sleep(1);
                break;
            }
        }
    tcflush(rot->descripter,TCIOFLUSH);
    }
    
    if(rot->RotatorReading)
        rot->RotatorReading = false;
    return NULL;
}

void * RotatorSattracker(void * param)
{
    SGS_ROTATOR * rot = (SGS_ROTATOR *)param;
    rot->RotatorEngage = true;

    while(rot->RotatorEngage)
    {
        if(rot->Sat == NULL)
        {
            rot->callback(SGS_CONMOD_ERR, "Cannot Detect Tracking rot->Satellites.");
            break;
        }
    
        if(rot->Sat->Elevation() < 0)
        {
            rot->callback(SGS_CONMOD_DBG, "Change to AOS.");
            rot->az_tag = rot->Sat->AOS_Azimuth();
            rot->el_tag = rot->Sat->AOS_Elevation();
        }
        else
        {
            rot->az_tag = rot->Sat->Azimuth_1secfuture();
            rot->el_tag = rot->Sat->Elevation_1secfuture();
        }
        if(rot->el > 80)
        {
            rot->el_tag = 80;
            rot->Sat->futuretime = 6.0f;
        }
        else
            rot->Sat->futuretime = 3.0f;
        
        if(rot->Sat->CrossPath)
        {
            switch(rot->Sat->CrossPath)
            {
                case(1): {
                    if(rot->az_tag < 90)
                        rot->az_tag += 360;
                    break;
                }
                case(-1): {
                    if(rot->az_tag < 90)
                        rot->az_tag += 360;
                    break;
                }
            }
        }
        if ((abs(rot->az_tag - rot->az) >= rot->tolrnc_az || abs(rot->el_tag - rot->el) >= rot->tolrnc_el))
        {
            

            rot->callback(SGS_CONMOD_DBG, "Azimuth : %d, Elevation : %d\n", rot->az_tag, rot->el_tag);
            rot->RotatorSetTo(rot->az_tag, rot->el_tag);
        }
        else
            rot->callback(SGS_CONMOD_DBG, "Tolerance Filtering...");
        sleep(1);
    }
    return NULL;
}

SGS_ROTATOR::SGS_ROTATOR()
{
    memset(this->ControlBuf, 0, sizeof(this->ControlBuf));
    memset(this->ReadBuf, 0, sizeof(this->ReadBuf));
    memset(this->Rotator_devname, 0, sizeof(this->Rotator_devname));
}

SGS_ROTATOR::~SGS_ROTATOR()
{
    if (this->descripter > 0)
    {
        tcflush(this->descripter, TCIOFLUSH);
        tcsetattr(this->descripter, TCSANOW, &this->tio);
        tcflush(this->descripter, TCIOFLUSH);
        close(this->descripter);
    }
    this->RotatorReading = false;
    pthread_join(this->THREAD[0], NULL);
    this->RotatorReady = false;
    close(this->descripter);
}

int SGS_ROTATOR::Initializer()
{
    this->descripter = open(this->Rotator_devname, O_RDWR | O_NOCTTY | O_NDELAY); // Or O_RDWR | O_NOCTTY? // @@ 로테이터 열기
    // open 함수 https://www.it-note.kr/19
    // O_RDWR은 파일 디스크립터인 fd를 읽기와 쓰기 모드로 열기 위한 지정이며
    // O_NOCCTY와 O_NONBLOCK는 시리얼 통신 장치에 맞추어 추가했습니다.

    if (this->descripter < 0)
    {
		return -1;
	}
    // tcgetattr(this->descripter, &this->tio);
    tcflush(this->descripter, TCIOFLUSH);
    switch(this->Rotator_baud)
    {
        case 4800 : this->tio.c_cflag = B4800; break;
        case 9600 : this->tio.c_cflag = B9600; break;
        default : break;
    }
    

    this->tio.c_cflag |= (CLOCAL | CREAD); 	// Receiver and local mode
    this->tio.c_cflag &= ~HUPCL;				// Do not change modem signals
    this->tio.c_cflag &= ~CSIZE;
    this->tio.c_cflag |= CS8;					// 8 bit // @@ S대역 포지셔너(로테이터) 컨트롤러이랑 같은 옵션이듯 // @@ 시리얼 통신 설정방법: https://softtone-someday.tistory.com/15, https://blog.naver.com/choi125496/130034222760
    this->tio.c_cflag &= ~CSTOPB;				// 1 stop bit
    this->tio.c_cflag &= ~PARENB;				// No parity check
    this->tio.c_cflag &= ~CRTSCTS;			// Enable hardware / software flow control

    this->tio.c_iflag &= ~IXON;				// No handshaking

    // Raw output
    this->tio.c_oflag &= ~OPOST; 				// Prevent special interpretathis->tion of output bytes (e.g. newline chars)

    // Canonical input
    this->tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);	// Non-canonical read

    // Initialize control characters
    this->tio.c_cc[VTIME]    = 5;
    this->tio.c_cc[VMIN]     = 1;		// Block 0.5(VTIME) sec after read first(VMIN) character

    tcflush(this->descripter, TCIOFLUSH);
    


    if (tcsetattr(this->descripter, TCSANOW, &this->tio) != 0)
    {
        this->callback(SGS_CONMOD_ERR, "Fail to configure descripter serial port\n");
        return -1;
    }
    	
    return 0;

    pthread_create(&THREAD[0], NULL, RotatorReadInfo, NULL);
}

void SGS_ROTATOR::RotatorSetTo(int azimuth, int elevation)
{   
    this->RotatorReady = false;
    sprintf(ControlBuf, "W%03d %03d\r\n", azimuth, elevation);
    tcflush(this->descripter, TCIOFLUSH);
    int res = write(this->descripter, ControlBuf, 10);
    if(res > 0)
    {
        this->callback(SGS_CONMOD_LOG, "Tracking : Az %d, El %d.", azimuth, elevation);
    }
    else
        this->callback(SGS_CONMOD_ERR, "Write to rotator failed.");
    
    sleep(1);
    this->RotatorReady = true;
    if(res > 0)
    {
        this->callback(SGS_CONMOD_LOG, "Tracking : Az %d, El %d.", azimuth, elevation);
    }
    else
        this->callback(SGS_CONMOD_ERR, "Write to rotator failed.");
    
    sleep(1);
    this->RotatorReady = true;
}
