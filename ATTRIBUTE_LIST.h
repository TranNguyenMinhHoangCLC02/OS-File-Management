#include "headerAttribute.h"
#include "BPB.h"
#include <sstream>
#pragma once

struct attributeRecord
{
    int typeID; // 0x00, 4 bytes
    int recordLength; // 0x04, 2 bytes
    int nameLength; // 0x06, 1 byte
    int offsetToName; // 0x07, 1 byte
    DWORD startingVCN; // 0x08, 8 bytes
    DWORD fileReference; // 0x10, 8 bytes
    int attributeID; // 0x18, 2 bytes
    string name; // 0x1A, nameLength bytes. Name is in Unicode
};

struct dataRun
{
    uint16_t clusterCount;
    uint64_t clusterNumber;
};

class $ATTRIBUTE_LIST
{
private:
    vector<string> entry;
    NTFSAttributeHeader header;
        // lengthAttribute: length of the name of this attribute
        // positionOfAttribute: staring offset of the name of this attribute
    vector<attributeRecord> records;
public:
    $ATTRIBUTE_LIST(vector<string> entry, DWORD offset, const char *diskPath, BPB bpb);
    void print();
    void printInfo();
    NTFSAttributeHeader getHeader();
    vector<attributeRecord> getRecords();
};

DWORD hexToDec(string hex);
string hexToString(string hex);
LPCWSTR charToLPCWSTR(const char* charString);