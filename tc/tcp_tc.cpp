#include "tcp_tc.h"

struct __attribute__ ((packed)) ExpediteInterface::_TC_HEADER{
    uint32_t npreamble;
    uint32_t nMsglen;
    uint32_t nMsgType;
};

struct ExpediteInterface::_TM_GPS_MCC{
    int16_t year;
    int8_t month;
    int8_t mday;
    int16_t yday;
    int8_t wday;
    int8_t hour;
    int8_t min;
    int8_t sec;
    int32_t frac;
    int32_t offs_from_utc;
};

const char ExpediteInterface::mcu_days_of_month[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},    //윤년 x
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}     //윤년
};

uint32_t ExpediteInterface::reverseCRC(uint32_t crc){
    int i;
    uint32_t res = 0;
    for (i = 0; i < 32; i++){
        if((crc >> i) & 1){
            res |= (1 << (31 - i));
        }
    }
    return res;
}

uint32_t ExpediteInterface::GetCRC32(const std::vector<uint8_t> &u8vIn){
    uint32_t POLY = 0x04C11DB7;    //LSB부터 계산할 때는 0xEDB88320 쓰던데,,,,,흠,,,,
    uint32_t crc = 0xFFFFFFFF;    //crc가 0xFF or 0x00 => crc를 뒤집어도 동일, 즉 crc(shift register)를 뒤집으나 안 뒤집나 초반 crc ^= i 의 결과는 동일, 
                                  //이 결과를 뒤집어진 poly와 계산하므로 뒤집어진 crc 구해짐
                                  //원래는 crc의 최상단 비트와 데이터의 입력 비트를 xor 계산하지만 poly를 뒤집으면 crc의 최하단 비트와 계산 => 뒤집어진 결과

    for(uint8_t i : u8vIn){
        crc ^= i;   //어차피 나중에 8비트 shift를 통해 사라지므로 최종 crc값에는 영향을 주지 않음 -> xor을 통해 8비트만 xor 값 => 어차피 시프트를 통해 사라짐 => 최종값에는 영향을 주지 않음
        for (size_t j = 0; j < 8; j++){
            if(crc & 1){
                crc = (crc >> 1) ^ POLY;
            }
            else{
                crc = (crc >> 1);
            }
        }
    }
    crc ^= 0xFFFFFFFF;    //<GPT 피셜> 초기 crc가 1의 값으로 차있던 것을 보정 -> crc가 0으로 차있었으면 해줄 필요 x => 초기 리스트가 비어있었거나 비어있지 않았거나로 생각하면 되나? 
    return crc;
}

void ExpediteInterface::EncapsulateTC(std::vector<uint8_t> pnData, std::vector<uint8_t> &pnOut){
    TC_HEADER sTCheader;
    sTCheader.npreamble = PREAMBLE;
    sTCheader.nMsgType = 1;
    sTCheader.nMsglen = sizeof(TC_HEADER) + pnData.size() + 2 * sizeof(uint32_t);

    uint32_t m_nPostamble = POSTAMBLE;

    pnOut.clear();
    pnOut.resize(pnOut.size() + sizeof(TC_HEADER));
    memcpy(&pnOut[pnOut.size() - sizeof(TC_HEADER)], &sTCheader, sizeof(TC_HEADER));

    pnOut.resize(pnOut.size() + pnData.size());
    memcpy(&pnOut[pnOut.size() - pnData.size()], &pnData[0], pnData.size());

    uint32_t m_nCRC32 = GetCRC32(pnOut);
    pnOut.resize(pnOut.size() + sizeof(m_nCRC32));
    memcpy(&pnOut[pnOut.size() - sizeof(m_nCRC32)], &m_nCRC32, sizeof(m_nCRC32));

    pnOut.resize(pnOut.size() + sizeof(m_nPostamble));
    memcpy(&pnOut[pnOut.size() - sizeof(m_nPostamble)], &m_nPostamble, sizeof(m_nPostamble));
}

int ExpediteInterface::mcu_is_leap_year(int y){
    return ((((y % 4) == 0) && ((y % 100) != 0 )) || ((y % 400) == 0));
}

int ExpediteInterface::mcu_day_of_year(int day, int month, int year){
    const char *t;
    t = mcu_days_of_month[mcu_is_leap_year(year)];
    while(--month){
        day += *t++;
    }

    return day;
}

void ExpediteInterface::mcu_date_of_year(int year, int day_num, TM_GPS_MCC *tm){
    int i;
    const char *t;

    while (day_num < 1){
        day_num += mcu_day_of_year(31, 12, --year);
    }

    t = mcu_days_of_month[mcu_is_leap_year(year)];

    for(i = 1; i <13 && day_num > *t; i++){
        day_num -= *t++;
    }

    if (i > 12){
        mcu_date_of_year(++year, day_num, tm);
    }
    else{
        tm->mday = (int8_t) day_num;
        tm->month = (int8_t) i;
        tm->year = (int16_t) year;
    }
}

uint8_t ExpediteInterface::BCDToDecimal(uint8_t BCD){
    uint8_t hi = (BCD >> 4) & 0x0F;
    uint8_t lo = BCD & 0x0F;
    return static_cast<uint8_t>(hi*10 + lo);
}

QString ExpediteInterface::IRIGtoTime(uint8_t *irig){
   std::vector<uint8_t> irig_vec;
    irig_vec.resize(8);
    memcpy(&irig_vec[0], irig, sizeof(uint8_t)*8);
    for (size_t ele = 0 ; ele < irig_vec.size(); ++ele){
        irig_vec[ele] = BCDToDecimal(irig_vec[ele]);
    }
    TM_GPS_MCC *timeinfo = new TM_GPS_MCC;
    int years = 1900 + irig_vec[0];
    int days = irig_vec[2] * 100 + irig_vec[1] + 1;
    int hours = irig_vec[3];
    int mins = irig_vec[4];
    int sec = irig_vec[5];
    int ms = irig_vec[6] * 10 + irig_vec[7];
    mcu_date_of_year(years, days, timeinfo);
    QString s;
    QTextStream out(&s);
    printf("yy / mm / dd : %d / %d / %d , %d : %d : %d.%d \n", years, timeinfo->month, timeinfo->mday, hours, mins, sec, ms);
    return s ; 
}

bool ExpediteInterface::VerifyCRC32(const std::vector<uint8_t> &u8vIn){
    if ( u8vIn . size () < 2 * sizeof ( uint32_t ) ) return 0;
    std :: vector < uint8_t > crc32_v ( u8vIn . end () - 2 * sizeof ( uint32_t ) , u8vIn . end
    () - sizeof ( uint32_t ) ) ;
    uint32_t m_nCRC32 = crc32_v [0] | ( crc32_v [1] << 8) | ( crc32_v [2] << 16) | (
    crc32_v [3] << 24) ;
    std :: vector < uint8_t > data_v ( u8vIn . begin () , u8vIn . end () - 2* sizeof ( uint32_t
    ));
    uint32_t m_nCRC32_vari = GetCRC32 ( data_v ) ;
    return m_nCRC32 == m_nCRC32_vari ;
}

