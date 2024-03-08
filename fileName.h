#include "read.h"
#include "BPB.h"
#include "entry.h"
#include "headerAttribute.h"
#include "ATTRIBUTE_LIST.h"
#pragma once

vector<string> extractHeader(vector<string> entry);

class FileName
{
private:
    vector<string> entry;
    vector<dataRun> runlist;
    NTFSAttributeHeader header;
    int sizeAttribute;
    int offset;
    int lengthFileName;
    int nameSpace;
    string fileName;
public:
    FileName()
    {
        this->header = NTFSAttributeHeader();
        this->entry = {};
        this->runlist = {};
        this->sizeAttribute = 0;
        this->offset = 0;
        this->lengthFileName = 0;
        this->nameSpace = 0;
        this->fileName = "";
    }
    FileName(vector<string> entry, BPB bpb, const char *diskPath);
    void printInfo();
    string getFileName();
    int getSizeAttribute();
    int getSizeEntireAttribute();
};