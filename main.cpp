#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "bootSector.h"
#include "time.h"
#include "entry.h"
#include "BPB.h"
#include "MFT.h"

using namespace std;

int main()
{
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
    bpb.printBPBInfo();
    bpb.printBPB();
    int MFTStart = bpb.startingClusterOfMFT();
    int Sc = bpb.getSc();
    vector<vector<string>> entries;
    // read_MFT(MFTStart, Sc, disk, entries);
    // vector<string> MFT;
    read_MFT2(MFTStart, Sc, entries);
    printMFT(entries);
    system("pause");
    return 0;

}
