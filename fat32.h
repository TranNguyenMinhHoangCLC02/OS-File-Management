#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FAT32
{
private:
    HANDLE device;
    DWORD bytesRead;
    BYTE sector[512];
    int retCode;
public:
    FAT32();
    void openDevice(const char* devicePath);
    void closeDevice();
    bool readSector(DWORD sectorNumber);
    bool readCluster(DWORD clusterNumber);
    bool readDirectory(DWORD clusterNumber);
    bool readFile(DWORD clusterNumber, string fileName);
};