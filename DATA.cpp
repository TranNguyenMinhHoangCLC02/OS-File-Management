#include "DATA.h"

DATA::DATA(const char *diskpath, vector<string> entry, BPB bpb, long long offset)
{
    vector<string> headerArr;
    for (int i = 0; i < 16; i++)
    {
        headerArr.push_back(entry[i + offset]);
    }
    header = NTFSAttributeHeader(headerArr);
    this->dataSize = convertHexadecimalToDecimal(entry[16 + offset] + entry[17 + offset] + entry[18 + offset] + entry[19 + offset]);
    this->dataOffset = convertHexadecimalToDecimal(entry[20 + offset] + entry[21 + offset]);
    if (!header.getFlagNonResident())
    {
        for (int i = 0; i < data.size(); i++)
        {
            data.push_back(convertHexadecimalToDecimal(entry[i + this->dataOffset + offset]));
        }
    }
    else
    {
        // get data runs of non-resident attribute
        int current = offset + 32; // offset to run list at byte 32 - 33 from start of attribute
        WORD offsetToRunList = convertHexadecimalToDecimal(entry[current] + entry[current + 1]);
        current = offset + offsetToRunList;
        vector<dataRun> runs;
        while (entry[current] != "00" && entry[current + 1] != "00")
        {
            dataRun run;
            string firstByte = entry[current];
            int bytesToParseNum = stoi(firstByte.substr(0, 1));
            int bytesToParseCount = stoi(firstByte.substr(1, 1));
            if (bytesToParseNum == 0)
                break;
            string runLength = "";
            for (int i = 0; i < bytesToParseNum; i++)
                runLength += entry[current + i];
            run.clusterCount = convertHexadecimalToDecimal(runLength);
            string runOffset = "";
            for (int i = 0; i < bytesToParseCount; i++)
                runOffset += entry[current + bytesToParseNum + i];
            run.clusterNumber = convertHexadecimalToDecimal(runOffset);
            runs.push_back(run);
            current += bytesToParseNum + bytesToParseCount;
        }

        // read cluster from the data run cluster number by the total cluster count
        for (int i = 0; i < runs.size(); i++)
        {
            vector<string> cluster;
            readClusters(charToLPCWSTR(diskpath), runs[i].clusterNumber, bpb.getSc(),
                         runs[i].clusterCount * bpb.getSc() * bpb.getSectorSize(), cluster);
            for (int j = 0; j < cluster.size(); j++)
            {
                data.push_back(convertHexadecimalToDecimal(cluster[j]));
            }
        }
    }
}

void DATA::print()
{
    header.printInfo();
    cout << "Data Size: " << this->dataSize << endl;
    cout << "Data Offset: " << this->dataOffset << endl;
    for (int i = 0; i < this->data.size(); i++)
    {
        cout << this->data[i];
    }
}

void DATA::printInfo()
{
    header.printInfo();
    cout << "Data Size: " << this->dataSize << endl;
    cout << "Data Offset: " << this->dataOffset << endl;
    for(int i = 0; i < this->data.size(); i++)
    {
        cout << (char)this->data[i] << " ";
    }
}

NTFSAttributeHeader DATA::getHeader()
{
    return this->header;
}

vector<BYTE> DATA::getData()
{
    return this->data;
}