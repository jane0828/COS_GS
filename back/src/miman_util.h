#ifndef _MIMAN_UTIL_H_
#define _MIMAN_UTIL_H_
#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

typedef struct{
    uint8_t fingerprint[4];
    uint32_t Checksum;
    
    uint32_t sec;
    uint32_t micsec;

    uint32_t epochYear;
    double epochDay;
    double INC;
    double RAAN;
    double ECC;
    double AOP;
    double MA;
    double MM;
    uint8_t padding[4];
}__attribute__((packed)) miman_util_fswtle_t;

void FSWTleConverter(Tle& TleInfo, miman_util_fswtle_t * fswtle);

#endif