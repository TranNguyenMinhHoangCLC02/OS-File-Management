#include "ATTRIBUTE_LIST.h"

$ATTRIBUTE_LIST::$ATTRIBUTE_LIST(vector<string> entry, DWORD offset, const char *diskPath, BPB bpb)
{
    // get 16 first bytes to construct header
    vector<string> header;
    for (int i = 0; i < 16; i++)
        header.push_back(entry[offset + i]);
    this->header = NTFSAttributeHeader(header);

    long long current = offset + 16;
    // if no non-resident flag, get the rest of the data to construct records
    if (this->header.getFlagNonResident() == 0)
    {
        // get length of content
        DWORD contentLength = hexToDec(convertStringToLittleEdian(getStringFromVector(entry, offset + current, 4)));
        // get offset to content
        current += 4;
        DWORD contentOffset = hexToDec(convertStringToLittleEdian(getStringFromVector(entry, offset + current, 2))); // offset from start of header (start of attribute))

        // get list of records
        current = offset + contentOffset;
        while (current < contentOffset + contentLength)
        {
            attributeRecord item;
            item.typeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current, 4)));
            item.recordLength = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current + 4, 2)));
            item.nameLength = convertHexadecimalToDecimal(getStringFromVector(entry, current + 6, 1));
            item.offsetToName = convertHexadecimalToDecimal(getStringFromVector(entry, current + 7, 1));
            item.startingVCN = hexToDec(convertStringToLittleEdian(getStringFromVector(entry, current + 8, 8)));
            item.fileReference = hexToDec(convertStringToLittleEdian(getStringFromVector(entry, current + 16, 8)));
            item.attributeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current + 24, 2)));
            item.name = hexToString(convertStringToLittleEdian(getStringFromVector(entry, current + item.offsetToName, item.nameLength)));
            item.data = vector<string>(entry.begin() + current, entry.begin() + current + item.recordLength);
            this->records.push_back(item);
            current += item.recordLength;
        }

        this->data = vector<string>(entry.begin() + offset, entry.begin() + offset + contentOffset + contentLength);
    }
    else // if non-resident flag
    {
        current += 16;
        WORD offsetToRunList = hexToDec(convertStringToLittleEdian(getStringFromVector(entry, current, 2)));
        current = offset + offsetToRunList;

        runlist = {};  // list of data runs
        while ((entry[current] != "00" && entry[current + 1] != "00") && current < entry.size())
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
            run.clusterCount = hexToDec(convertStringToLittleEdian(runLength));
            string runOffset = "";
            for (int i = 0; i < bytesToParseCount; i++)
                runOffset += entry[current + bytesToParseNum + i];
            run.clusterNumber = hexToDec(convertStringToLittleEdian(runOffset));
            run.data = vector<string>(entry.begin() + current, entry.begin() + current + bytesToParseNum + bytesToParseCount + 1);
            runlist.push_back(run);
            current += bytesToParseNum + bytesToParseCount + 1;
        }

        this->data = vector<string>(entry.begin() + offset, entry.begin() + current + 1);

        // read clusters provided by data runs
        int sectorPercluster = bpb.getSc();
        vector<string> clusterData;
        for (int i = 0; i < runlist.size(); i++)
            readClusters(charToLPCWSTR(diskPath), runlist[i].clusterNumber, sectorPercluster, runlist[i].clusterCount * sectorPercluster * bpb.getSectorSize(), clusterData);

        // get list of records
        current = 0;
        while (current < clusterData.size())
        {
            attributeRecord item;
            item.typeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(clusterData, current, 4)));
            item.recordLength = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(clusterData, current + 4, 2)));
            item.nameLength = convertHexadecimalToDecimal(getStringFromVector(clusterData, current + 6, 1));
            item.offsetToName = convertHexadecimalToDecimal(getStringFromVector(clusterData, current + 7, 1));
            item.startingVCN = hexToDec(convertStringToLittleEdian(getStringFromVector(clusterData, current + 8, 8)));
            item.fileReference = hexToDec(convertStringToLittleEdian(getStringFromVector(clusterData, current + 16, 8)));
            item.attributeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(clusterData, current + 24, 2)));
            item.name = hexToString(convertStringToLittleEdian(getStringFromVector(clusterData, current + item.offsetToName, item.nameLength)));
            item.data = vector<string>(clusterData.begin() + current, clusterData.begin() + current + item.recordLength);
            this->records.push_back(item);
            current += item.recordLength;
        }
    }
}

void $ATTRIBUTE_LIST::print()
{
    for (int i = 0; i < this->data.size(); i++)
        cout << this->data[i];
}

void $ATTRIBUTE_LIST::printInfo()
{
    this->header.printInfo();
    cout << endl;

    if (this->header.getFlagNonResident() == 1)
    {
        for (int i = 0; i < this->runlist.size(); i++)
        {
            cout << "Run " << i << endl;
            cout << "Cluster Count: " << this->runlist[i].clusterCount << endl;
            cout << "Cluster Number: " << this->runlist[i].clusterNumber << endl;
        }
        cout << endl;
    }

    for (int i = 0; i < this->records.size(); i++)
    {
        cout << "Record " << i << endl;
        cout << "Type ID: " << this->records[i].typeID << endl;
        cout << "Record Length: " << this->records[i].recordLength << endl;
        cout << "Name Length: " << this->records[i].nameLength << endl;
        cout << "Offset to Name: " << this->records[i].offsetToName << endl;
        cout << "Starting VCN: " << this->records[i].startingVCN << endl;
        cout << "File Reference: " << this->records[i].fileReference << endl;
        cout << "Attribute ID: " << this->records[i].attributeID << endl;
        cout << "Name: " << this->records[i].name << endl << endl;
    }
}

DWORD hexToDec(string hex)
{
    DWORD num;
    stringstream(hex) >> hex >> num;
    return num;
}

string hexToString(string hex)
{
    string result = "";
    for (int i = 0; i < hex.size(); i += 2)
    {
        string byte = hex.substr(i, 2);
        char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
        result.push_back(chr);
    }
    return result;
}

LPCWSTR charToLPCWSTR(const char* charString)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, charString, -1, nullptr, 0);
    if (size == 0) {
        cerr << "Failed to get required buffer size. Error code: " << GetLastError() << std::endl;
        return nullptr;
    }

    vector<wchar_t> buffer(size);
    if (MultiByteToWideChar(CP_UTF8, 0, charString, -1, buffer.data(), size) == 0) {
        cerr << "Failed to convert string. Error code: " << GetLastError() << std::endl;
        return nullptr;
    }

    return buffer.data();
}