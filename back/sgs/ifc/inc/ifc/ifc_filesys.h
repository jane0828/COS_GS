#ifndef _IFC_FILESYS_H_
#define _IFC_FILESYS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <utl/utl.h>

class SGS_Filesys {
public:
    SGS_Filesys();
    ~SGS_Filesys();

    bool WriteToFile(const std::string& filename, const std::string& data);
    bool ReadFromFile(const std::string& filename, std::string& data);
};

#endif