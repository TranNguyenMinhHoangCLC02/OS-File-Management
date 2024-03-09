#include "StandardInfo.h"

StandardInfo::StandardInfo(vector<string> entry)
{
    this->entry = entry;
    this->header = NTFSAttributeHeader(entry);
    this->offset = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 20, 21 - 20 + 1)));
    this->flag = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, this->offset + 32, 35 - 32 + 1)));
}

void StandardInfo::printInfo()
{
    cout << "Standard Information Attribute" << endl;
    cout << "Flag: " << this->flag << endl;
    switch (this->flag)
    {
        case 0x0001:
            cout << "Read Only" << endl;
            break;
        case 0x0002:
            cout << "Hidden" << endl;
            break;
        case 0x0004:
            cout << "System" << endl;
            break;
        case 0x0020:
            cout << "Archive" << endl;
            break;
        case 0x0040:
            cout << "Device" << endl;
            break;
        case 0x0080:
            cout << "Normal" << endl;
            break;
        case 0x0100:
            cout << "Temporary" << endl;
            break;
        case 0x0200:
            cout << "Sparse File" << endl;
            break;
        case 0x0400:
            cout << "Reparse Point" << endl;
            break;
        case 0x0800:
            cout << "Compressed" << endl;
            break;
        case 0x1000:
            cout << "Offline" << endl;
            break;
        case 0x2000:
            cout << "Not Content Indexed" << endl;
            break;
        case 0x4000:
            cout << "Encrypted" << endl;
            break;
        default:
            cout << "Unknown" << endl;
            break;
    }
}