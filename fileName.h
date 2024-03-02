#include "read.h"
#include "MFT.h"
#include "BPB.h"
#include "entry.h"
#include "headerAttribute.h"
#include "ATTRIBUTE_LIST.h"

vector<string> extractHeader(vector<string> entry)
{
    vector<string> header;
    for (int i = 0; i < 16; i++)
        header.push_back(entry[i]);
    return header;
}

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
    void print();
    string getFileName();
    int getSizeAttribute();
    int getSizeEntireAttribute();
};