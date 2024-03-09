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
        int current = offset + 32; // offset to run list at byte 32 - 33 from start of attribute
        WORD offsetToRunList = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current, 2)));
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