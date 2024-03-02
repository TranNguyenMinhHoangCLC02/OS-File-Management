#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "bootSector.h"
#include "time.h"
#include "entry.h"
#include "fatTable.h"
#include <cmath>
#include "BPB.h"
#include "MFT.h"
#include "DATA.h"
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


    // BYTE sector[512];
    // readSector(L"\\\\.\\E:", 0, sector);
    // BootSector bootSector;
    // bootSector.readBootSector(sector);
    // bootSector.printBootSector();
    // vector<vector<string>> entries;
    // DWORD startSectorOfRDET = bootSector.getStartingClusterOfRDET() * bootSector.getSectorPerCluster();
    // readEntireEntries(startSectorOfRDET, entries);
    // removeFaultyEntry(entries);
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
    
    // Entries entry;
    // entry.input(entries);
    // entry.print();
    LPCWSTR disk = L"\\\\.\\C:";
    BPB bpb(disk);
    // bpb.printBPBInfo();
    // bpb.printBPB();
    int MFTStart = bpb.startingClusterOfMFT();
    int Sc = bpb.getSc();
    vector<vector<string>> entries;
    // read_MFT(MFTStart, Sc, disk, entries);

    vector<string> MFT;
    // read_MFT2(MFTStart, Sc, entries);
    readSector(L"\\\\.\\C:", MFTStart * Sc, 1024, MFT);
    for (int i = 0; i < MFT.size(); i++)
    {
        if (i % 16 == 0)
            cout << "\n";
        cout << MFT[i] << " ";
    }
    cout << "\n";
    int MFTSize = 0;
    vector<string> temp = MFT;
    int offsetToAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(temp, 0x14, 0x15 - 0x14 + 1)));
    temp.erase(temp.begin(), temp.begin() + offsetToAttribute);
    for (int i = 0; i < temp.size(); i++)
    {
        if (i % 16 == 0)
            cout << "\n";
        cout << temp[i] << " ";
    }
    cout << "\n";
    while (temp.empty() == false)
    {
        vector<string> temp2;
        for (int i = 0; i < 16; i++)
        {
            temp2.push_back(temp[i]);
        }
        NTFSAttributeHeader header(temp2);
        if (MFTSize == MFTSize + header.getAttributeSize())
            break;
        MFTSize += header.getAttributeSize();
        cout << "current size: " << MFTSize << "\n";
        temp.erase(temp.begin(), temp.begin() + header.getAttributeSize());
    }
    cout << "MFT Size: " << MFTSize << endl;
    MFTStart++;
    for (unsigned long long i = MFTStart * Sc; i < MFTStart * Sc + MFTSize; i += 1)
    {
        vector<string> temp;
        readSector(L"\\\\.\\C:", i, 1024, temp);
        entries.push_back(temp);
    }
    for (int i = 0; i < entries.size(); i++)
    {
        for (int j = 0; j < entries[i].size(); j++)
        {
            if (j % 16 == 0)
                cout << "\n";
            cout << entries[i][j] << " ";
        }
        cout << "\n";
        cout << "\n";
    }
    // printMFT(entries);
    system("pause");

    return 0;
}
