#include "ATTRIBUTE_LIST.h"

$ATTRIBUTE_LIST::$ATTRIBUTE_LIST(vector<string> entry, DWORD offset, const char *diskPath)
{
    this->entry = entry;
    // get 16 first bytes to construct header
    vector<string> header;
    for (int i = 0; i < 16; i++)
        header.push_back(entry[offset + i]);
    this->header = NTFSAttributeHeader(header);

    int current = 16;
    // if no non-resident flag, get the rest of the data to construct records
    if (this->header.getFlagNonResident() == 0)
    {
        // get length of content
        DWORD contentLength = hexToDec(convertStringToLittleEdian(getStringFromVector(entry, offset + current, 4)));
        // get offset to content
        current += 4;
        DWORD contentOffset = hexToDec(convertStringToLittleEdian(getStringFromVector(entry, offset + current, 2)));

        // get list of records
        current = contentOffset;
        while (current < contentOffset + contentLength)
        {
            attributeRecord item;
            item.typeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current, 4)));
            item.recordLength = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current + 4, 2)));
            item.nameLength = convertHexadecimalToDecimal(getStringFromVector(entry, current + 6, 1));
            item.offsetToName = convertHexadecimalToDecimal(getStringFromVector(entry, current + 7, 1));
            item.startingVCN = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current + 8, 8)));
            item.fileReference = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current + 16, 8)));
            item.attributeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, current + 24, 2)));
            item.name = hexToString(convertStringToLittleEdian(getStringFromVector(entry, current + 26, item.nameLength)));
        }
    }
    else // if non-resident flag
    {
        
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