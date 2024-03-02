#pragma once
#include "bootSector.h"
#include "read.h"

#define FAT_END1 0xFFFFFFF
#define FAT_END2 0xFFFFFF8
#define FAT_BAD 0xFFFFFF7

class FatTable{
private:
    vector<BYTE> fatTable;
public:
    // FatTable();
    FatTable(const char* diskPath);
    // FatTable(WORD Sb, BYTE sector[]);
    vector<int> getClusters(int firstCluster);
    vector<BYTE> getFatTable();
    void printFatTable();
};