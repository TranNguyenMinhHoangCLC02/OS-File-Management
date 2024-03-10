#include "fileName.h"

FileName::FileName(vector<string> entry, BPB bpb, const char *diskPath)
    {
        this->header = NTFSAttributeHeader(extractHeader(entry));
        this->entry = entry;
        if (this->header.getFlagNonResident() == 0)
        {
            this->sizeAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 16, 19 - 16 + 1)));
            this->offset = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 20, 21 - 20 + 1)));
            this->lengthFileName = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, this->offset + 64, 1)));
            this->nameSpace = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, this->offset + 65, 1)));
            this->fileName = convertHexToUTF16(getStringFromVector(this->entry, this->offset + 66, this->lengthFileName * 2));
        }
        else
        {
            this->sizeAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 16, 19 - 16 + 1)));
            this->offset = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 20, 21 - 20 + 1)));
            int current = this->offset;
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
                    readSector(charToLPCWSTR(diskPath), clusterRunStr * bpb.getSc() + i, 512, data);
                }
                string realData = convertHexToUTF16(getStringFromVector(data, 0, data.size()));
                for (int i = 0; i < realData.size(); i++)
                    this->fileName.push_back(realData[i]);
                current += bytesToParseCount;
            }
        }
    }

vector<string> extractHeader(vector<string> entry)
{
    vector<string> header;
    for (int i = 0; i < 16; i++)
        header.push_back(entry[i]);
    return header;
}

void FileName::printInfo()
{
    cout << "File Name: " << this->fileName << endl;
}
string FileName::getFileName()
{
    return this->fileName;
}
int FileName::getSizeAttribute()
{
    return this->sizeAttribute;
}
int FileName::getSizeEntireAttribute()
{
    return this->header.getAttributeSize();
}