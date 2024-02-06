#include "fatTable.h"

FatTable::FatTable(){
    BootSector bootSector;
    BYTE sector[512];
    readSector("\\\\.\\C:", 0, sector);
    bootSector.readBootSector(sector);
    DWORD Sf = bootSector.getSf(); // Sectors per FAT
    DWORD Nf = bootSector.getNf(); // Number of FATs
    WORD Sb = bootSector.getSb(); // Sector of boot sector
    DWORD startClusterRDET = bootSector.getstartClusterRDET(); // Start cluster of RDET
    fatTable.resize(Sf * 512);
    readSector("\\\\.\\C:", Sb * 512, fatTable.data());
}

FatTable::FatTable(const char* diskPath){
    BootSector bootSector;
    BYTE sector[512];
    readSector(diskPath, 0, sector);
    bootSector.readBootSector(sector);
    DWORD Sf = bootSector.getSf(); // Sectors per FAT
    DWORD Nf = bootSector.getNf(); // Number of FATs
    WORD Sb = bootSector.getSb(); // Sector of boot sector
    DWORD startClusterRDET = bootSector.getstartClusterRDET(); // Start cluster of RDET
    fatTable.resize(Sf * 512);
    readSector(diskPath, Sb * 512, fatTable.data());
}

vector<BYTE> FatTable::getFatTable(){
    return fatTable;
}

void FatTable::printFatTable(){
    for (int i = 0; i < fatTable.size(); i++){
        if (i % 16 == 0){
            cout << endl;
        }
        cout << hex << setw(2) << setfill('0') << (int)fatTable[i] << " ";
    }
    cout << endl;
}

// int main(int argc, char ** argv)
// {
//     FatTable fatTable("\\\\.\\D:");
//     fatTable.printFatTable();
//     system("pause");
//     return 0;
// }