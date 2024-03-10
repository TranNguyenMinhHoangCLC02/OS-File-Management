#include "read.h"
#include "entry.h"
#include "headerAttribute.h"
#include "BPB.h"
#pragma once

class indexRoot
{
private:
    NTFSAttributeHeader header;
    string indexRootOffset;
    int offset;
    int sizeAttribute;
    vector<string> entry;
public:
    indexRoot()
    {
        this->header = NTFSAttributeHeader();
        this->indexRootOffset = "";
        this->offset = 0;
        this->sizeAttribute = 0;
        this->entry = vector<string>(0);
    }
    indexRoot(vector<string> entry, BPB bpb, const char *diskPath);
    int getSizeAttribute()
    {
        return this->sizeAttribute;
    }
};