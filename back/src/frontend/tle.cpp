#ifndef _SCL_INCLUDE_H_
#include <sgs/include.h>
#endif

SGS_TLE::SGS_TLE(uint32_t idx, const char* label, bool ref)
{
    memset(this->_local, 0, sizeof(this->_local));
    memset(this->_remote, 0, sizeof(this->_remote));
    memset(this->label, 0, sizeof(this->label));
    memset(this->Satellites, NULL, sizeof(this->Satellites));

	strcpy_s(this->label, label);
	this->webref = ref;
    this->idx = idx;
    if(this->webref)
        snprintf(this->_remote, sizeof(this->_remote), "https://celestrak.org/NORAD/elements/gp.php?GROUP=%s&FORMAT=tle", this->label);
    snprintf(this->_local, sizeof(this->_local), "./bin/tle/%s.dat", label);
}

SGS_TLE::~SGS_TLE()
{

}

size_t SGS_TLE::write_data(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int SGS_TLE::DownloadTLE()
{
    int size_l = MultiByteToWideChar(CP_UTF8, 0,this->_local, -1, NULL, 0);
    int size_r = MultiByteToWideChar(CP_UTF8, 0,this->_remote, -1, NULL, 0);
    wchar_t* localURL = new wchar_t[size_l];
    wchar_t* remoteURL = new wchar_t[size_r];
    MultiByteToWideChar(CP_UTF8, 0, this->_local, -1, localURL, size_l);
    MultiByteToWideChar(CP_UTF8, 0, this->_remote, -1, remoteURL, size_r);
    return URLDownloadToFile(0, remoteURL, localURL, 0, 0);
}

void * SGS_TLE::Import()
{
    int result = 0;
    //State.Display_TLE = false;
    //State.Display_load = true;
    if (this->webref)
        result = this->DownloadTLE();
    if (result == S_OK)
        console->AddLog(SCL_CONMOD_OK, "TLE Download Completed.");
    else
    {
        console->AddLog(SCL_CONMOD_ERR, "TLE Download Failed.(EC %d)", result);
        return NULL;
    }
    memset(this->Satellites, 0, sizeof(this->Satellites));
    if ((this->count = this->ReadTLELines(this->_local, false, this->sgp4on)) <= 0)
        console->AddLog(SCL_CONMOD_ERR, "Invalid type of TLE File.");

    this->imported = true;
    return NULL;
}

int SGS_TLE::ReadTLELines(char* TLEFilename, bool fds, bool sgp4on)
{
    std::string TLEResultName;
    std::string TLEResultLine1;
    std::string TLEResultLine2;
    std::ifstream TLEDATA;
    TLEDATA.open(TLEFilename);

    DateTime start_date = DateTime::Now(true);
    DateTime end_date(start_date.AddDays(7.0));
    int res = 0;

    while (getline(TLEDATA, TLEResultName) && getline(TLEDATA, TLEResultLine1) && getline(TLEDATA, TLEResultLine2))
    {
        TLEResultName.erase(std::find_if(TLEResultName.rbegin(), TLEResultName.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultName.end());
        TLEResultLine1.erase(std::find_if(TLEResultLine1.rbegin(), TLEResultLine1.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultLine1.end());
        TLEResultLine2.erase(std::find_if(TLEResultLine2.rbegin(), TLEResultLine2.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultLine2.end());
        try
        {
            this->Satellites[res] = new SatelliteObject(*(this->obs));
            this->Satellites[res]->tle = new Tle(TLEResultName, TLEResultLine1, TLEResultLine2);
        }
        catch (const std::exception& e)
        {
            console->AddLog(SCL_CONMOD_DBG, "TLE Load Error. Index : %d, Error : %s", res, e.what());
            delete Satellites[res]->tle;
            delete Satellites[res];
            continue;
        }
        try
        {
            this->Satellites[res]->Initializer(this->sgp4on);
        }
        catch (const std::exception& e)
        {
            console->AddLog(SCL_CONMOD_DBG, "SGP4 Error during importing. Skip this satellite and continue. Error : %s", e.what());
            delete Satellites[res]->tle;
            delete Satellites[res];
            continue;
        }
        res++;
    }
    TLEDATA.close();
    return 0;
}
