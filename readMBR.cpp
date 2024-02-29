#include "read.h"

void readMBR(LPCWSTR diskPath, vector<string> &storedValues)
{
    readSector(diskPath, 0, 512, storedValues);
}

void printMBR(vector<string> storedValues)
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
//     const char *diskPath = "\\\\.\\PhysicalDrive0";
//     vector<string> storedValues;
//     readSector(diskPath, 0, 512, storedValues);
//     printMBR(storedValues);
//     system("pause");
//     return 0;
// }