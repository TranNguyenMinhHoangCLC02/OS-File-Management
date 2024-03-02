#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "bootSector.h"
#include "time.h"
#include "entry.h"
#include "fatTable.h"
#include <cmath>
using namespace std;

int main()
{
    BYTE sector[512];
    readSector("\\\\.\\F:", 0, sector);
    BootSector bootSector;
    bootSector.readBootSector(sector);
    vector<vector<string>> entries;
    DWORD startSectorOfRDET = bootSector.getSb() + bootSector.getSf() * bootSector.getNf();
    readEntireEntries(startSectorOfRDET, entries);
    removeFaultyEntry(entries);

    Entries entry;
    entry.input(entries);
    FatTable fatTable("\\\\.\\F:", bootSector   );
    entry.print();

    WORD NRDET = entries.size();
    vector<int> listFirstCluster = entry.getListClusters();
    vector<Entry*> e = entry.getEntries();
    for(int i : listFirstCluster){
        cout << i << " ";
        vector<int> listClusterOfEntry = fatTable.getClusters(i);
        for(int j : listClusterOfEntry)
        {
            DWORD offset = bootSector.getSb() + bootSector.getSf() * bootSector.getNf() + ceil( NRDET * 32.0 / 512 ) + (j - 2) * bootSector.getSc();
            cout << offset << " ";
        //     BYTE* data = new BYTE[bootSector.getSc() * 512];
        //     readSector("\\\\.\\F:", offset * 512, data, bootSector.getSc() * 512);
        //     for(int i = 0; i < bootSector.getSc() * 512; i++)
        //     {
        //         cout << hex << setw(2) << setfill('0') << (int)data[i] << " ";
        //     }
        //     delete[] data;
        }
        cout << endl << endl;
    }


    // vector<Entry *> entry;
    // while (entries.empty() == false)
    // {
    //     vector<vector<string>> temp = extractEntry(entries);
    //     Entry *newEntry = new Entry();
    //     newEntry->readEntry(temp);
    //     entry.push_back(newEntry);
    // }
    // for (int i = 0; i < entry.size(); i++)
    // {
    //     entry[i]->print();
    //     entry[i]->printEntry();
    //     cout << endl;
    // }
    // for (int i = 0; i < entry.size(); i++)
    //     delete entry[i];
    
    return 0;
}
