#pragma once
#include "bootSector.h"
#include "read.h"

#define FAT_END1 0xFFFFFFFF
#define FAT_END2 0xFFFFFFF8
#define FAT_BAD 0xFFFFFFF7

class FatTable{
private:
    vector<BYTE> fatTable;
public:
    FatTable();
    FatTable(const char* diskPath);
    vector<DWORD> getClusters(DWORD firstCluster);
    vector<BYTE> getFatTable();
    void printFatTable();
};