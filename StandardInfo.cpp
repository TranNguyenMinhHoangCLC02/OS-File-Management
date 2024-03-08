#include "StandardInfo.h"

StandardInfo::StandardInfo(vector<string> entry)
{
    this->header = NTFSAttributeHeader(entry);
    this->flag = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(entry, 0x20, 0x23 - 0x20 + 1)));
}

void StandardInfo::printInfo()
{
    cout << "Standard Information Attribute" << endl;
    // switch (this->flags)
    // {
    //     case 0x0001:
    //         cout << "Read Only" << endl;
    //         break;
    //     case 0x0002:
    //         cout << "Hidden" << endl;
    //         break;
    //     case 0x0004:
    //         cout << "System" << endl;
    //         break;
    //     case 0x0020:
    //         cout << "Archive" << endl;
    //         break;
    //     case 0x0040:
    //         cout << "Device" << endl;
    //         break;
    //     case 0x0080:
    //         cout << "Normal" << endl;
    //         break;
    //     case 0x0100:
    //         cout << "Temporary" << endl;
    //         break;
    //     case 0x0200:
    //         cout << "Sparse File" << endl;
    //         break;
    //     case 0x0400:
    //         cout << "Reparse Point" << endl;
    //         break;
    //     case 0x0800:
    //         cout << "Compressed" << endl;
    //         break;
    //     case 0x1000:
    //         cout << "Offline" << endl;
    //         break;
    //     case 0x2000:
    //         cout << "Not Content Indexed" << endl;
    //         break;
    //     case 0x4000:
    //         cout << "Encrypted" << endl;
    //         break;
    //     default:
    //         cout << "Unknown" << endl;
    //         break;
    // }
}