#include "MFT.h"

void ReadSect2(LPCWSTR disk, BYTE *&DATA, unsigned int _nsect)
{
    DWORD dwBytesRead(0);

    HANDLE hFloppy = CreateFileW(disk, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                 NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL); // Handle to template

    if (hFloppy != NULL)
    {
        LARGE_INTEGER li;
        li.QuadPart = _nsect * 512;
        SetFilePointerEx(hFloppy, li, 0, FILE_BEGIN);
        // Read the boot sector
        if (!ReadFile(hFloppy, DATA, 512, &dwBytesRead, NULL))
        {
            printf("Error in reading floppy disk\n");
        }

        CloseHandle(hFloppy);
        // Close the handle
    }
}

int getBytes(BYTE *MFT, int offset, int size)
{
    int result = 0;
    for (int i = 0; i < size; i++)
    {
        result += MFT[offset + i] << (i * 8);
    }
    return result;
}

bool checkMFTEntry(BYTE *MFT)
{
    if (to_string(MFT[0]) == "46" && to_string(MFT[1]) == "49" && to_string(MFT[2]) == "4C" && to_string(MFT[3]) == "45")
        return true;
    return false;
}

bool checkMFTEntry(vector<string> entry)
{
    if (entry[0] == "46" && entry[1] == "49" && entry[2] == "4C" && entry[3] == "45")
        return true;
    return false;
}

string byteArrayToHexString(const BYTE *data, size_t size)
{
    stringstream hexString;
    for (size_t i = 0; i < size; ++i)
        hexString << hex << setw(2) << setfill('0') << static_cast<int>(data[i]) << " ";
    return hexString.str();
}

vector<string> readMFTEntry(const BYTE *entry, size_t entrySize)
{
    // Implement parsing logic based on your MFT structure
    // This is just a placeholder, modify it according to your actual structure
    vector<string> fields;
    fields.push_back(byteArrayToHexString(entry, entrySize)); // Example: Store the entire entry as a hex string
    // Parse other fields and add them to the 'fields' vector

    return fields;
}

void read_MFT(unsigned int MFTStart, unsigned int sectors_per_cluster, LPCWSTR disk, vector<vector<string>> &entries)
{
    BYTE *MFT = new BYTE[512];
    MFTStart *= sectors_per_cluster;
    ReadSect2(disk, MFT, MFTStart);

    // entries.push_back(readMFTEntry(MFT, 512));
    vector<string> fields1 = readMFTEntry(MFT, 512);

    delete[] MFT;

    BYTE *MFT2 = new BYTE[512];
    MFTStart += 1;
    ReadSect2(disk, MFT2, MFTStart);

    vector<string> fields2 = readMFTEntry(MFT2, 512);

    delete[] MFT2;

    fields1.insert(fields1.end(), fields2.begin(), fields2.end());
    entries.push_back(fields1);

    while (true)
    {
        int MFTStart = MFTStart + 1;
        MFT = new BYTE[512];
        ReadSect2(disk, MFT, MFTStart);

        if (!checkMFTEntry(MFT))
        {
            delete[] MFT;
            break;
        }

        // entries.push_back(readMFTEntry(MFT, 512));
        fields1 = readMFTEntry(MFT, 512);

        delete[] MFT;

        MFT2 = new BYTE[512];
        MFTStart += 1;
        ReadSect2(disk, MFT2, MFTStart);

        fields2 = readMFTEntry(MFT2, 512);

        delete[] MFT2;

        fields1.insert(fields1.end(), fields2.begin(), fields2.end());
        entries.push_back(fields1);
    }
}

void read_MFT2(unsigned int MFTStart, unsigned int sectors_per_cluster, vector<vector<string>> &entries) {
    vector<string> storedValues1;
    vector<string> storedValues2;
    MFTStart *= sectors_per_cluster;
    readSector(L"\\\\.\\C:", MFTStart, 512, storedValues1);
    if (checkMFTEntry(storedValues1)) {
        entries.push_back(storedValues1);
        MFTStart+=1;
        readSector(L"\\\\.\\C:", MFTStart, 512, storedValues2);
        entries.push_back(storedValues2);
        storedValues1.clear();
        storedValues2.clear();
    }
    MFTStart+=2;
    int i = 0;
    while (i < 10) {
        readSector(L"\\\\.\\C:", MFTStart, 512, storedValues1);
        // if (checkMFTEntry(storedValues1)) {
        //     entries.push_back(storedValues1);
        //     MFTStart+=1;
        //     readSector(L"\\\\.\\C:", MFTStart, 512, storedValues2);
        //     entries.push_back(storedValues2);
        //     storedValues1.clear();
        //     storedValues2.clear();
        // }
        // else {
        //     break;
        // }

        entries.push_back(storedValues1);
        MFTStart+=1;
        readSector(L"\\\\.\\C:", MFTStart, 512, storedValues2);
        entries.push_back(storedValues2);
        storedValues1.clear();
        storedValues2.clear();
        MFTStart+=2;
        i++;
    }
}

void printMFT(vector<vector<string>> entries)
{
    for (int i = 0; i < entries.size(); i++)
    {
        for (int j = 0; j < entries[i].size(); j++)
            cout << entries[i][j] << " ";
        cout << endl;
    }
}