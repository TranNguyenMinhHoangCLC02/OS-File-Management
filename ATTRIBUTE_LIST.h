#include "headerAttribute.h"
#include "BPB.h"
#include <sstream>
#pragma once

struct attributeRecord
{
    vector<string> data; // original data from the disk

    //converted data
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
    vector<string> data; // original data from the disk

    //converted data
    uint16_t clusterCount;
    uint64_t clusterNumber;
};

class $ATTRIBUTE_LIST
{
private:
    vector<string> data;  // original data from the disk

    //converted data
    NTFSAttributeHeader header;
        // lengthAttribute: length of the name of this attribute
        // positionOfAttribute: staring offset of the name of this attribute
    vector<attributeRecord> records;
    vector<dataRun> runlist; // list of data runs in case of non-resident flag
public:
    $ATTRIBUTE_LIST(vector<string> entry, DWORD offset, const char *diskPath, BPB bpb);
    void print(); // print the original data
    void printInfo(); // print the converted data
    NTFSAttributeHeader getHeader();
    vector<attributeRecord> getRecords();
};

DWORD hexToDec(string hex);
string hexToString(string hex);
LPCWSTR charToLPCWSTR(const char* charString);