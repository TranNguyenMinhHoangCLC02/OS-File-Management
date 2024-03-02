#include "fatTable.h"

// FatTable::FatTable(){
//     BootSector bootSector;
//     BYTE sector[512];
//     readSector("\\\\.\\C:", 0, sector);
//     bootSector.readBootSector(sector);
//     DWORD Sf = bootSector.getSf(); // Sectors per FAT
//     DWORD Nf = bootSector.getNf(); // Number of FATs
//     WORD Sb = bootSector.getSb(); // Sector of boot sector
//     DWORD startClusterRDET = bootSector.getstartClusterRDET(); // Start cluster of RDET
//     fatTable.resize(Sf * 512);
//     readSector("\\\\.\\C:", Sb * 512, fatTable.data());
// }

FatTable::FatTable(const char* diskPath){
    BootSector bootSector;
    BYTE sector[512];
    readSector(diskPath, 0, sector);
    bootSector.readBootSector(sector);
    DWORD Sf = bootSector.getSf(); // Sectors per FAT
    DWORD Nf = bootSector.getNf(); // Number of FATs
    WORD Sb = bootSector.getSb(); // Sector of boot sector
    BYTE Nf = bootSector.getNf();

    // Read FAT table
    BYTE* data = new BYTE[Sf * 512];
    readSector(diskPath, Sb * 512, data, Sf * 512);
    for(int t = 0; t < Sf * 512; t++){
        fatTable.push_back(data[t]);
    }
    delete[] data;
}

// vector<BYTE> FatTable::getFatTable(){
//     return fatTable;
// }

// FatTable::FatTable(const char* diskPath) {
//     // Read boot sector to get necessary information
//     ifstream disk(diskPath, ios::binary);
//     if (!disk.is_open()) {
//         cerr << "Error: Unable to open disk." << endl;
//         return;
//     }

//     disk.seekg(Sb * 512); // Move to the sector of the boot sector
//     // Read FAT table
//     for (DWORD i = 0; i < Sf; ++i) {
//         BYTE sector[512];
//         disk.read(reinterpret_cast<char*>(sector), 512);
//         // Assume FAT32, each entry is 4 bytes
//         for (int j = 0; j < 512; j += 4) {
//             DWORD entry = (sector[j] & 0xFF) | ((sector[j + 1] & 0xFF) << 8) | ((sector[j + 2] & 0xFF) << 16) | ((sector[j + 3] & 0xFF) << 24);
//             fatTable.push_back(entry);
//         }
//     }

//     disk.close();
// }

void FatTable::printFatTable(){
    for (int i = 0; i <  1000 ; i++){
        if (i % 16 == 0){
            cout << endl;
        }
        cout << hex << setw(2) << setfill('0') << (int)fatTable[i] << " ";
    }
    cout << endl;
}

vector<int> FatTable::getClusters(int firstCluster) {
    vector<int> clusters;
    int currentCluster = firstCluster;
    int eoc_sign[] = {(int)0x00000000, (int)0xFFFFFF0, (int)0xFFFFFFF, (int)0XFFFFFF7, (int)0xFFFFFF8, (int)0xFFFFFFF0};
    while (true){
        for(int i : eoc_sign)
            if (i == currentCluster)
                break;
        clusters.push_back(currentCluster);
        int INDEX = 0;
        for(int i = 3; i >= 0; i--){
            INDEX *= 256;
            INDEX += fatTable[currentCluster * 4 + i];
        }
        currentCluster = INDEX;
    }

    return clusters;
}

// eoc_sign = [0x00000000, 0xFFFFFF0, 0xFFFFFFF, 0XFFFFFF7, 0xFFFFFF8, 0xFFFFFFF0]
// if n in eoc_sign:
//     return []

// next_cluster = n
// chain = [next_cluster]

// while True:
//     # Phần tử FAT 2 ứng với cluster số 1
//     next_cluster = read_number_buffer(self.fat_table_buffer, next_cluster * 4, 4)
//     if next_cluster in eoc_sign:
//         break 
//     else:
//         chain.append(next_cluster)
// return chain 

// int main(int argc, char ** argv)
// {
//     FatTable fatTable("\\\\.\\F:");
//     fatTable.printFatTable();
//     system("pause");
//     return 0;
// }