#include "bootSector.h"
#include "read.h"
#pragma once

class FatTable{
private:
    vector<BYTE> fatTable;
public:
    FatTable();
    FatTable(const char* diskPath);
    vector<BYTE> getFatTable();
    void printFatTable();
};