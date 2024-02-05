#include "bootSector.h"

BootSector::BootSector() {
    Sc = 0;
    Sb = 0;
    Nf = 0;
    Sv = 0;
    Sf = 0;
    startClusterRDET = 0;
    numOfSectorExtra = 0;
    numOfSectorSave = 0;
    typeFAT = 0;
}

void BootSector::readBootSector(BYTE sector[512]) {
        Sc = sector[13];
        Sb = sector[14] + (sector[15] << 8);
        Nf = sector[16];
        Sv = sector[32] + (sector[33] << 8) + (sector[34] << 16) + (sector[35] << 24);
        Sf = sector[36] + (sector[37] << 8) + (sector[38] << 16) + (sector[39] << 24);
        startClusterRDET = sector[44] + (sector[45] << 8) + (sector[46] << 16) + (sector[47] << 24);
        numOfSectorExtra = sector[48] + (sector[49] << 8);
        numOfSectorSave = sector[50] + (sector[51] << 8);
        typeFAT = static_cast<uint64_t>(sector[82]) |
                (static_cast<uint64_t>(sector[83]) << 8) |
                (static_cast<uint64_t>(sector[84]) << 16) |
                (static_cast<uint64_t>(sector[85]) << 24) |
                (static_cast<uint64_t>(sector[86]) << 32) |
                (static_cast<uint64_t>(sector[87]) << 40) |
                (static_cast<uint64_t>(sector[88]) << 48) |
                (static_cast<uint64_t>(sector[89]) << 56);
    }
void BootSector::printBootSector() {
    cout << "Sectors per cluster: " << (int)Sc << endl;
    cout << "Sector of boot sector: " << Sb << endl;
    cout << "Number of FATs: " << (int)Nf << endl;
    cout << "Number of sectors in the volume: " << Sv << endl;
    cout << "Sectors per FAT: " << Sf << endl;
    cout << "Start cluster of RDET: " << startClusterRDET << endl;
    cout << "Number of sectors for extra data: " << numOfSectorExtra << endl;
    cout << "Number of sectors for save data: " << numOfSectorSave << endl;
    string fatTypeString = "";
    for (int i = 0; i < 8; ++i) {
        fatTypeString += static_cast<char>((typeFAT >> (i * 8)) & 0xFF);
    }
    cout << "Type of FAT: " << fatTypeString << endl;
}

int ReadSector(LPCSTR  drive, int readPoint, BYTE sector[512])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("Success!\n");
    }
}

int main(int argc, char ** argv)
{
    BYTE sector[512];
    ReadSector("\\\\.\\D:", 0, sector);
    BootSector bootSector;
    bootSector.readBootSector(sector);
    bootSector.printBootSector();
    system("pause");
    return 0;
}
