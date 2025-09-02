#include <utl/utl.h>
#include <ifc/ifc.h>

SGS_Filesys::SGS_Filesys() {}
SGS_Filesys::~SGS_Filesys() {}

bool SGS_Filesys::WriteToFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }
    file << data;
    file.close();
    return true;
}

bool SGS_Filesys::ReadFromFile(const std::string& filename, std::string& data) {
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return false;
    }
    std::string line;
    data.clear();
    while (std::getline(file, line)) {
        data += line + "\n";
    }
    file.close();
    return true;
}