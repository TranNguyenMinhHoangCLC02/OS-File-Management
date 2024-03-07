#include "fatTable.h"
#include <cstring>

FatTable::FatTable(const char* diskPath, DWORD Sf, WORD Sb){
    // Read FAT table
    BYTE* data = new BYTE[Sf * 512];
    readSector(diskPath, Sb * 512, data, Sf * 512);
    for(int t = 0; t < Sf * 512; t++){
        fatTable.push_back(data[t]);
    }
    delete[] data;
}

FatTable& FatTable::operator= (const FatTable &fatTable){
        this->fatTable = fatTable.fatTable;
        return *this;
}


vector<int> FatTable::getClusters(int firstCluster) {
    vector<int> clusters;
    int currentCluster = firstCluster;
    
    int eoc_sign[] = {(int)0x00000000, (int)0xFFFFFF0, (int)0xFFFFFFF, (int)0XFFFFFF7, (int)0xFFFFFF8, (int)0xFFFFFFF0};
    while (true){
        bool check = false;
        for(int i : eoc_sign)
            if (i == currentCluster)
            {
                check = true;
                break;
            }
        if (check)
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

vector<vector<int>> FatTable::listClustersOfEntry(vector<int> listFirstCLusters){
    vector<vector<int>> listClusters;
    for(int i : listFirstCLusters){
        // cout << i << " ";
        listClusters.push_back(getClusters(i));
    }
    // cout << "hahsha ";
    return listClusters;
}
