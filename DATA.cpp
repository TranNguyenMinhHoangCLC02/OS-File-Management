#include "DATA.h"

DATA::DATA(const char *diskpath, vector<string> entry, BPB bpb, long long offset)
{
    vector<string> headerArr;
    for (int i = 0; i < 16; i++)
    {
        headerArr.push_back(entry[i + offset]);
    }
    header = NTFSAttributeHeader(headerArr);
    this->dataSize = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, 16 + offset, 19 - 16 + 1)));
    this->dataOffset = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, 20 + offset, 21 - 20 + 1)));
    if (header.getFlagNonResident()  == 0)
    {
        for (int i = 0; i < this->dataSize; i++)
        {
            data.push_back(convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, dataOffset + i, 1))));
            cout << data[i] << " ";
        }
    }
    else
    {
        // get data runs of non-resident attribute
        int current = offset + 0x20; // offset to run list at byte 32 - 33 from start of attribute
        WORD offsetToRunList = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current, 2)));
        current = offsetToRunList;
        vector<string> data;
        while (true)
        {
            dataRun run;
            string firstByte = entry[current];
            int bytesToParseNum = stoi(firstByte.substr(0, 1));
            int bytesToParseCount = stoi(firstByte.substr(1, 1));
            if (bytesToParseNum == 0)
                break;
            int clusterRun = convertHexadecimalToDecimal(entry[current + 1]);
            current += 2;
            int clusterRunStr = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current, bytesToParseNum)));
            for (int i = 0; i < clusterRun * bpb.getSc(); i++)
            {
                readSector(charToLPCWSTR(diskpath), clusterRunStr * bpb.getSc() + i, 512, data);
            }
            string realData = convertHexToUTF16(getStringFromVector(data, 0, data.size()));
            for (int i = 0; i < realData.size(); i++)
                this->data.push_back(realData[i]);
            current += bytesToParseCount;
        }
    }
}

void DATA::printInfo()
{
    header.printInfo();
    cout << "Data Size: " << this->dataSize << endl;
    cout << "Data Offset: " << this->dataOffset << endl;
    cout << "Data: \n";
    for(int i = 0; i < this->data.size(); i++)
    {
        cout << this->data[i];
    }
    cout << endl;
}

NTFSAttributeHeader DATA::getHeader()
{
    return this->header;
}

vector<BYTE> DATA::getData()
{
    return this->data;
}