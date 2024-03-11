#include "fatTable.h"
#include "bootSector.h"
#include "entry.h"
#pragma once

class File1
{
private:
    BootSector bootSector;
    FatTable fatTable;
    string content;
    const char* diskPath;
public:
    File1(const char* diskPath);
    void printContent();
};
