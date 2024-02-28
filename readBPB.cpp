#include "BPB.h"

int BPB::findNTFSIdentifier()
{
    for (int i = 0; i < storedValues.size() - 4; i++)
        if (storedValues[i] == "80" && storedValues[i + 1] == "00" && storedValues[i + 2] == "80" && storedValues[i+3] == "00")
            return i;
    return -1;
}

int BPB::findBeginningOfBPB()
{
    return findNTFSIdentifier() - 25;
}

int BPB::getSectorSize()
{
    int beginningOfBPB = findBeginningOfBPB();
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[beginningOfBPB + i];
    return convertHexadecimalToDecimal(temp);
}

int BPB::getSc()
{
    int beginningOfBPB = findBeginningOfBPB();
    int current = beginningOfBPB + 2;
    return convertHexadecimalToDecimal(storedValues[current]);
}

int BPB::getst()
{
    int beginningOfBPB = findBeginningOfBPB();
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2;
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int BPB::getHeadNumber()
{
    int beginningOfBPB = findBeginningOfBPB();
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2 + 2;
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int BPB::getBeginningSectorOfLogicalPartition()
{
    int beginningOfBPB = findBeginningOfBPB();
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2 + 2 + 2;
    string temp = "";
    for (int i = 3; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int BPB::getSv()
{
    int beginningOfNTFS = findNTFSIdentifier();
    int current = beginningOfNTFS + 4;
    string temp = "";
    for (int i = 7; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int BPB::startingClusterOfMFT()
{
    int beginningOfNTFS = findNTFSIdentifier();
    int current = beginningOfNTFS + 4 + 8;
    string temp = "";
    for (int i = 7; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int BPB::getSizeOfAMFTEntry()
{
    int beginningOfNTFS = findNTFSIdentifier();
    int current = beginningOfNTFS + 4 + 8 + 8 + 8;
    string temp = "";
    temp += storedValues[current];
    return convertHexadecimalToDecimal(temp);
}

void readBPB(const char *diskPath, vector<string> &storedValues)
{
    readSector(diskPath, 0, 512, storedValues);
}

void BPB::printBPBInfo()
{
    cout << "Sector size = " << getSectorSize() << endl;
    cout << "Sc = " << getSc() << endl;
    cout << "st = " << getst() << endl;
    cout << "h = " << getHeadNumber() << endl;
    cout << "Beginning sector of logical partition = " << getBeginningSectorOfLogicalPartition() << endl;
    cout << "Sv = " << getSv() << endl;
    cout << "Starting cluster of MFT = " << startingClusterOfMFT() << endl;
    cout << "Size of a MFT entry = " << getSizeOfAMFTEntry() << endl;
}

void BPB::printBPB()
{
    for (int i = 0; i < storedValues.size(); i++)
    {
        cout << storedValues[i] << " ";
        if ((i + 1) % 16 == 0)
            cout << endl;
    }
}

// int main()
// {
//     const char *diskPath = "\\\\.\\C:";
//     vector<string> storedValues;
//     readBPB(diskPath, storedValues);
//     printBPBInfo(storedValues);
//     system("pause");
//     return 0;
// }