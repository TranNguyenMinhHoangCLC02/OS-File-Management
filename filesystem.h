#include "fatTable.h"
#include "bootSector.h"
// #include "fat32.h"
#include "entry.h"
#pragma once

class File1
{
private:
    BootSector bootSector;
    // BYTE bootSectorBytes[512];
    FatTable fatTable;
    string content;
    const char* diskPath;
public:
    File1(const char* diskPath);
    void printContent();
};
