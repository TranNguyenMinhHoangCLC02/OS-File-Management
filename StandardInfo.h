#include "read.h"
#include "entry.h"
#include "headerAttribute.h"
#pragma once

class StandardInfo {
private: 
    int flag;
    NTFSAttributeHeader header;
public:
    StandardInfo(vector<string> entry);
    void printInfo();
    int getFlags() {return header.getFlag();}
    int getSize(){return header.getAttributeSize();}
};
