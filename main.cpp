#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "bootSector.h"
#include "time.h"
#include "entry.h"

using namespace std;

int main()
{
    BYTE sector[512];
    readSector("\\\\.\\E:", 0, sector);
    BootSector bootSector;
    bootSector.readBootSector(sector);
    bootSector.printBootSector();
    vector<vector<string>> entries;
    DWORD startSectorOfRDET = bootSector.getSb() + bootSector.getSf() * bootSector.getNf();
    readEntireEntries(startSectorOfRDET, entries);
    removeFaultyEntry(entries);
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
    Entries entry;
    entry.input(entries);
    entry.print();
    return 0;
}
