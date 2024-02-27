#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>

class BootSector {
private:
    BYTE Sc; // Sectors per cluster
    WORD Sb; // Sector of boot sector
    BYTE Nf; // Number of FATs
    DWORD Sv; // Number of sectors in the volume
    DWORD Sf; // Sectors per FAT
    DWORD startClusterRDET; // Start cluster of RDET
    WORD numOfSectorExtra; // Number of sectors for extra data
    WORD numOfSectorSave; // Number of sectors for save data
    uint64_t typeFAT; // Type of FAT
public:
    BootSector();
    void readBootSector(BYTE sector[512]);
    void printBootSector();
    BYTE getstartClusterRDET() { return startClusterRDET; }
    DWORD getSf() { return Sf; }
    DWORD getNf() { return Nf; }
    WORD getSb() { return Sb; }
    BYTE getSc() { return Sc; }
};