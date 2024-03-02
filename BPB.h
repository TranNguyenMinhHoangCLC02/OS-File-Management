#include "read.h"
#include "entry.h"
#pragma once

class BPB
{
private:
    vector<string> storedValues;
public:
    BPB(LPCWSTR diskPath) {readSector(diskPath, 0, 512, storedValues);}
    int findNTFSIdentifier();
    int findBeginningOfBPB();
    int getSectorSize();
    int getSc();
    int getst();
    int getHeadNumber();
    int getBeginningSectorOfLogicalPartition();
    int getSv();
    int startingClusterOfMFT();
    int getSizeOfAMFTEntry();
    void printBPBInfo();
    void printBPB();
};