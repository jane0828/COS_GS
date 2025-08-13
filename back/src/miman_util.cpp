#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#include <sgs/miman_util.h>
#endif

void FSWTleConverter(Tle& TleInfo, miman_util_fswtle_t * fswtle);
{
    bool in_degree = true;

    DateTime UTCtime = DateTime::Now(true);
    DateTime EpochInfo = TleInfo.Epoch();

    fswtle->fingerprint[0] = 1;
    fswtle->fingerprint[1] = 2;
    fswtle->fingerprint[2] = 0;
    fswtle->fingerprint[3] = 8;

    time_t now, standard;
    struct tm epochtime;
    epochtime.tm_year = 1980 - 1990;
    epochtime.tm_mon = 0;
    epochtime.tm_mday = 1;
    epochtime.tm_hour = 0;
    epochtime.tm_min = 0;
    epochtime.tm_sec = 0;
    epochtime.tm_isdst = 0;
    now = time(NULL);
    standard = mktime(&epochtime);
    fswtle->sec = static_cast<unsigned int>(difftime(standard, now));


    // fswtle->sec =
    //  (uint32_t)(UTCtime.Second() + 60 * UTCtime.Minute() + 60 * 60 * UTCtime.Hour() + 60 * 60 * 24 * UTCtime.DayOfYear(UTCtime.Year(), UTCtime.Month(), UTCtime.Day()));
    fswtle->micsec = (uint32_t)(UTCtime.Microsecond());
    //***************************TBD

    fswtle->epochYear = (uint32_t)(EpochInfo.Year());
    fswtle->epochDay = (uint32_t)(EpochInfo.DayOfYear(EpochInfo.Year(), EpochInfo.Month(), EpochInfo.Day()))-1;

    fswtle->INC = (double)(TleInfo.Inclination(in_degree));
    fswtle->RAAN = (double)(TleInfo.RightAscendingNode(in_degree));
    fswtle->ECC = (double)(TleInfo.Eccentricity());
    fswtle->AOP = (double)(TleInfo.ArgumentPerigee(in_degree));
    fswtle->MA = (double)(TleInfo.MeanAnomaly(in_degree));
    fswtle->MM = (double)(TleInfo.MeanMotion());
    fswtle->padding[0] = 0;
    fswtle->padding[1] = 0;
    fswtle->padding[2] = 0;
    fswtle->padding[3] = 0;
    fswtle->Checksum = gs_crc32(((uint8_t *)(&fswtle->sec)),72);

    console->AddLog(SGS_CONMOD_DBG, "Ping Format - Time\nFingerprint : %u%u%u%u\tSecond : %u\tMicroSecond : %u\t EpochYear : %u\t, EpochDay : %u\n",
                    fswtle->fingerprint[0], fswtle->fingerprint[1], fswtle->fingerprint[2], fswtle->fingerprint[3],
                    fswtle->sec, fswtle->micsec, fswtle->epochYear, fswtle->epochDay);
    console->AddLog(SGS_CONMOD_DBG, "Ping Format - TLE\nInclination : %f\t RAAN : %f\t Eccentricity :%f\t AOP : %f\t MA : %f\t MM : %f\t Checksum : %u\n",
                    fswtle->INC, fswtle->RAAN, fswtle->ECC, fswtle->AOP, fswtle->MA, fswtle->MM, fswtle->Checksum);
}

void * csp_ping_scan(void *) {
	pthread_mutex_lock(&conn_lock);
	uint32_t timeout = 1000;
	uint16_t size = setup->pingsize;
	uint8_t conn_options = 0;
	uint8_t nownode;
	bool scanmode = false;
	uint32_t start, time = 0;
	// uint8_t node = *((uint8_t *) nodedata);
	bool dlstate = State.downlink_mode;
	if((dlstate))
        State.downlink_mode = false;
	while(!State.uplink_mode)
        continue;
	State.uplink_mode = true;
	for(uint8_t i = 0; i < 32; i++)
	{
		int pingResult = csp_ping(i, 2000, size, CSP_O_NONE);
		if(pingResult >= 0)
			console.AddLog("Scanned AX100. Node : %" PRIu8 ", %" PRIu32 "ms.\n", i, pingResult);
	}
	State.uplink_mode = false;
	if(dlstate)
    {
        State.downlink_mode = dlstate;
    }
	pthread_mutex_unlock(&conn_lock);
	return NULL;
}