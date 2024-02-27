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
    cout << Sf << endl;
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
    for (int i = 0; i < 1000; i++){
        if (i % 16 == 0){
            cout << endl;
        }
        cout << hex << setw(2) << setfill('0') << (int)fatTable[i] << " ";
    }
    cout << endl;
}

vector<DWORD> FatTable::getClusters(DWORD firstCluster) {
    vector<DWORD> clusters;
    DWORD currentCluster = firstCluster;

    while (currentCluster != FAT_END1 && currentCluster != FAT_END2 && currentCluster != FAT_BAD) {
        clusters.push_back(currentCluster);
        DWORD INDEX = 0;
        for(int i = 3; i >= 0; i--){
            INDEX *= 256;
            INDEX += fatTable[currentCluster + i];
        }
        currentCluster = fatTable[currentCluster];
    }

    return clusters;
}
// int main(int argc, char ** argv)
// {
//     FatTable fatTable("\\\\.\\F:");
//     fatTable.printFatTable();
//     system("pause");
//     return 0;
// }