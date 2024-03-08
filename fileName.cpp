#include "fileName.h"

FileName::FileName(vector<string> entry, BPB bpb, const char *diskPath)
    {
        this->header = NTFSAttributeHeader(extractHeader(entry));
        this->entry = entry;
        this->runlist = {};
        if (this->header.getFlagNonResident() == 0)
        {
            this->sizeAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 16, 19 - 16 + 1)));
            this->offset = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 20, 21 - 20 + 1)));
            this->lengthFileName = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, this->offset + 64, 1)));
            this->nameSpace = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, this->offset + 65, 1)));
            this->fileName = convertHexToUTF16(getStringFromVector(this->entry, this->offset + 66, this->lengthFileName));
        }
        else
        {
            int current = 0x20;
            WORD offsetToRunList = hexToDec(convertStringToLittleEdian(getStringFromVector(this->entry, current, 2)));
            current = offsetToRunList;
            while ((this->entry[current] != "00" && this->entry[current + 1] != "00") && current < this->entry.size())
            {
                dataRun run;
                string firstByte = this->entry[current];
                int bytesToParseNum = stoi(firstByte.substr(0, 1));
                int bytesToParseCount = stoi(firstByte.substr(1, 1));
                if (bytesToParseNum == 0)
                    break;
                string runLength = "";
                for (int i = 0; i < bytesToParseNum; i++)
                    runLength += this->entry[current + i];
                run.clusterCount = hexToDec(convertStringToLittleEdian(runLength));
                string runOffset = "";
                for (int i = 0; i < bytesToParseCount; i++)
                    runOffset += this->entry[current + bytesToParseNum + i];
                run.clusterNumber = hexToDec(convertStringToLittleEdian(runOffset));
                this->runlist.push_back(run);
                current += bytesToParseNum + bytesToParseCount;
            }
            int sectorPerCluster = bpb.getSc();
            vector<vector<string>> clusterData;
            for (int i = 0; i < this->runlist.size(); i++)
            {
                vector<string> temp;
                readClusters(charToLPCWSTR(diskPath), runlist[i].clusterNumber, sectorPerCluster, runlist[i].clusterCount * sectorPerCluster * bpb.getSectorSize(), temp);
                clusterData.push_back(temp);
            }
            this->fileName = "";
            for (int i = 0; i < clusterData.size(); i++)
                this->fileName += convertHexToUTF16(getStringFromVector(clusterData[i], 0, clusterData[i].size()));
        }
        
    }

vector<string> extractHeader(vector<string> entry)
{
    vector<string> header;
    for (int i = 0; i < 16; i++)
        header.push_back(entry[i]);
    return header;
}

void FileName::print()
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