#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <stdlib.h>
#include <QString>
#include <QTextStream>


#define PREAMBLE 0xA1B2C3D4
#define POSTAMBLE ~0xA1B2C3D4

class ExpediteInterface{
    private :
        struct _TC_HEADER;
        struct _TM_GPS_MCC;

        typedef struct _TC_HEADER TC_HEADER;

        typedef struct _TM_GPS_MCC TM_GPS_MCC;

        static const char mcu_days_of_month[2][12];

    public :        
        uint32_t GetCRC32(const std::vector<uint8_t> &u8vIn);

        void EncapsulateTC(std::vector<uint8_t> pnData, std::vector<uint8_t> &pnOut);

        int mcu_is_leap_year(int y);

        int mcu_day_of_year(int day, int month, int year);

        void mcu_date_of_year(int year, int day_num, TM_GPS_MCC *tm);

        uint8_t BCDToDecimal(uint8_t BCD);

        QString IRIGtoTime(uint8_t *irig);

        uint32_t reverseCRC(uint32_t crc);

        bool VerifyCRC32(const std::vector<uint8_t> &u8vIn);
         
};