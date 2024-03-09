#include "headerAttribute.h"
#include "read.h"
#include "ATTRIBUTE_LIST.h"
#pragma once

class DATA
{
private:
    int dataSize;
    int dataOffset;
    NTFSAttributeHeader header;
    vector<BYTE> data;
public:
    DATA(const char* diskPath, vector<string> entry, BPB bpb, long long offset);
    void printInfo();
    NTFSAttributeHeader getHeader();
    vector<BYTE> getData();
    int getDataSize(){ return header.getAttributeSize(); }
};