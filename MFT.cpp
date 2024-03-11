#include "MFT.h"

bool checkMFTEntry(vector<string> entry)
{
    if (entry[0] == "46" && entry[1] == "49" && entry[2] == "4C" && entry[3] == "45")
        return true;
    return false;
}

WORD getStartOffsetOfAtrribute(vector<string> MFTentry)
{
    return convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(MFTentry, 0x14, 0x15 - 0x14 + 1)));
}

WORD getFlagsMFTentry(vector<string> MFTentry)
{
    return convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(MFTentry, 0x16, 0x17 - 0x16 + 1)));
}

void processMFTEntry(vector<string> MFTentry, BPB &bpb, string diskPath) {
    //extract 16 byte dau de doc header
    //check loai header co phai la 
    WORD startOffset = getStartOffsetOfAtrribute(MFTentry);
    WORD flags = getFlagsMFTentry(MFTentry);

    if (flags == 0x04 || flags == 0x08) {
        cout << "File is deleted" << endl;
        return;
    }

    MFTentry.erase(MFTentry.begin(), MFTentry.begin() + startOffset);  

    NTFSAttributeHeader header(MFTentry);

    while (MFTentry.empty() == false) {
        if (header.getTypeID() != 16 && header.getTypeID() != 32 && header.getTypeID() != 48 && header.getTypeID() != 128) {
        int attributeSize = header.getAttributeSize();
        MFTentry.erase(MFTentry.begin(), MFTentry.begin() + attributeSize);
        }
        else if (header.getTypeID() == 16) {
            StandardInfo standardInfo(MFTentry);
            cout << "flags: " << standardInfo.getFlags() << endl;
            MFTentry.erase(MFTentry.begin(), MFTentry.begin() + standardInfo.getSize());
        }
        else if (header.getTypeID() == 48) {
            FileName fileName(MFTentry, bpb, diskPath.c_str());
            MFTentry.erase(MFTentry.begin(), MFTentry.begin() + fileName.getSizeEntireAttribute());
        }
        else if (header.getTypeID() == 128) {
            DATA data(diskPath.c_str(), MFTentry, bpb, 0);
            MFTentry.erase(MFTentry.begin(), MFTentry.begin() + data.getDataSize());
        }
    }
}