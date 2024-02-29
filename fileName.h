#include "read.h"
#include "MFT.h"
#include "BPB.h"
#include "entry.h"
#include "headerAttribute.h"

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
    NTFSAttributeHeader header;
    int sizeAttribute;
    int offset;
    int lengthFileName;
    int nameSpace;
    string fileName;
public:
    FileName(vector<string> entry);
    void print();
    string getFileName();
    int getSizeAttribute();
    int getSizeEntireAttribute();
};