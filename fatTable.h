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
    FatTable(){}
    FatTable& operator= (const FatTable &fatTable);
    vector<vector<int>> listClustersOfEntry(vector<int> listFirstCLusters);
    FatTable(const char* diskPath, DWORD Sf , WORD Sb);
    vector<int> getClusters(int firstCluster);
};