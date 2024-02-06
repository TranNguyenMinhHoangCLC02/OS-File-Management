#include "read.h"

int findNTFSIdentifier(vector<string> storedValues)
{
    for (int i = 0; i < storedValues.size() - 4; i++)
        if (storedValues[i] == "80" && storedValues[i + 1] == "00" && storedValues[i + 2] == "80" && storedValues[i+3] == "00")
            return i;
}

int findBeginningOfBPB(vector<string> storedValues)
{
    return findNTFSIdentifier(storedValues) - 25;
}

int getSectorSize(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[beginningOfBPB + i];
    return convertHexadecimalToDecimal(temp);
}

int getSc(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2;
    return convertHexadecimalToDecimal(storedValues[current]);
}

int getst(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2;
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getHeadNumber(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2 + 2;
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getBeginningSectorOfLogicalPartition(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2 + 2 + 2;
    string temp = "";
    for (int i = 3; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getSv(vector<string> storedValues)
{
    int beginningOfNTFS = findNTFSIdentifier(storedValues);
    int current = beginningOfNTFS + 4;
    string temp = "";
    for (int i = 7; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int startingClusterOfMFT(vector<string> storedValues)
{
    int beginningOfNTFS = findNTFSIdentifier(storedValues);
    int current = beginningOfNTFS + 4 + 8;
    string temp = "";
    for (int i = 7; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getSizeOfAMFTEntry(vector<string> storedValues)
{
    int beginningOfNTFS = findNTFSIdentifier(storedValues);
    int current = beginningOfNTFS + 4 + 8 + 8 + 8;
    string temp = "";
    temp += storedValues[current];
    return convertHexadecimalToDecimal(temp);
}

void readBPB(const char *diskPath, vector<string> &storedValues)
{
    readSector(diskPath, 0, 512, storedValues);
}

void printBPBInfo(vector<string> storedValues)
{
    cout << "Sector size = " << getSectorSize(storedValues) << endl;
    cout << "Sc = " << getSc(storedValues) << endl;
    cout << "st = " << getst(storedValues) << endl;
    cout << "h = " << getHeadNumber(storedValues) << endl;
    cout << "Beginning sector of logical partition = " << getBeginningSectorOfLogicalPartition(storedValues) << endl;
    cout << "Sv = " << getSv(storedValues) << endl;
    cout << "Starting cluster of MFT = " << startingClusterOfMFT(storedValues) << endl;
    cout << "Size of a MFT entry = " << getSizeOfAMFTEntry(storedValues) << endl;
}

void printBPB(vector<string> storedValues)
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