#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <Observer.h>
#include <gts/gts.h>


size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

SGS_TLE::SGS_TLE(std::string _label)
{
    this->geo = new CoordGeodetic(37.564021, 126.934600, 0.05);
    this->obs = new Observer(*(geo));

    this->console = new SGS_CONSOLE();
    this->console->set_catalog(SGS_GTS);

    this->label = _label;
    this->remote = "https://celestrak.org/NORAD/elements/gp.php?GROUP=" + label + "&FORMAT=tle";
    this->local = "bin/tle/" + label;
}


SGS_TLE::SGS_TLE(std::string _local, std::string _remote)
{
    this->geo = new CoordGeodetic(37.564021, 126.934600, 0.05);
    this->obs = new Observer(*(geo));

    this->console = new SGS_CONSOLE();
    this->console->set_catalog(SGS_GTS);

    this->label = "custom url";
    this->remote = _remote;
    this->local = _local;
}

SGS_TLE::~SGS_TLE()
{
    delete this->console;
    delete this->obs;
    delete this->geo;
}

int SGS_TLE::register_handle(SGS_WebSocketClient* _ws)
{
    if (!_ws)
        return SGS_ERROR_NULL;

    this->ws = _ws;

    // Register JSON handler functions using the macro
    // REGISTER_JSON_HANDLER(this->ws, this, set_observer);
    // REGISTER_JSON_HANDLER(this->ws, this, download_tle);
    // REGISTER_JSON_HANDLER(this->ws, this, get_tle_lines);

    return SGS_OK;
}


int SGS_TLE::download_tle()
{
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        console->error("Failed to initialize CURL");
        return -1;
    }

    FILE* fp = fopen(local.c_str(), "wb");
    if (!fp) {
        console->error("Failed to open file: %s", local);
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, remote.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        console->error("CURL error: %s", curl_easy_strerror(res));
        fclose(fp);
        curl_easy_cleanup(curl);
        return -1;
    }

    fclose(fp);
    curl_easy_cleanup(curl);

    console->info("TLE data downloaded successfully to: %s", local);
    return 0;
}

int SGS_TLE::get_tle_lines(char* TLEFilename, bool fds, bool sgp4on)
{
    std::ifstream file(TLEFilename);
    if (!file.is_open()) {
        console->error("Failed to open TLE file: %s", TLEFilename);
        return -1;
    }

    std::string name, line1, line2;

    // 기존 메모리 해제
    if (!Satellites.empty() && count != 0) {
        for (int i = 0; i < count; i++) {
            delete Satellites[i];
        }
        Satellites.clear();
    }

    count = 0;
    // TLE 읽기
    while (std::getline(file, name) && std::getline(file, line1) && std::getline(file, line2)) {
        Tle TleReaden(name, line1, line2);
        Satellites.push_back(new SatelliteObject(TleReaden, *(this->obs)));
        count++;
    }

    file.close();

    console->info("Read %d TLE sets from file.", this->count);
    return 0;
}
