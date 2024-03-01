#include "MFT.h"
#include "headerAttribute.h"
#include "read.h"
#include "ATTRIBUTE_LIST.h"
#pragma once

class $DATA
{
private:
    int dataSize;
    int dataOffset;
    NTFSAttributeHeader header;
    vector<BYTE> data;
public:
    $DATA(const char* diskPath, vector<string> entry, BPB bpb, long long offset);
    void print();
    void printInfo();
    NTFSAttributeHeader getHeader();
    vector<BYTE> getData();
};