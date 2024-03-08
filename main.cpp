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
    cout << "Input file system (FAT32, NTFS): ";
    string fileSystem;
    getline(cin, fileSystem);
    
    if (fileSystem != "FAT32" && fileSystem != "NTFS")
    {
        cout << "Invalid file system" << endl;
        return 0;
    }

    string diskName;
    cout << "Input disk path(C, D, E...): ";
    getline(cin, diskName);
    string diskPath = "\\\\.\\" + diskName + ":";

    if (fileSystem == "FAT32") {
        BYTE sector[512];
        readSector(diskPath.c_str(), 0, sector);
        BootSector bootSector;
        bootSector.readBootSector(sector);

        FatTable fatTable(diskPath.c_str(), bootSector.getSf(), bootSector.getSb());

        // đọc RDET
        DWORD startSectorOfRDET = bootSector.getSb() + bootSector.getSf() * bootSector.getNf();
        vector<vector<string>> entries;
        readEntireEntries(diskPath.c_str() ,startSectorOfRDET, entries);
        removeFaultyEntry(entries);
        Entries entry;
        entry.input(entries);

        // build tree
        Item* root = entry.getRootDirectory(bootSector, fatTable, diskPath.c_str());
        root->print();
        root->deleteItem();
    }

    else {
        BPB bpb(charToLPCWSTR(diskPath.c_str()));
        bpb.printBPB();
        bpb.printBPBInfo();
        int MFTStart = bpb.startingClusterOfMFT();
        int Sc = bpb.getSc();
        vector<vector<string>> entries;

        vector<string> MFT;
        readSector(charToLPCWSTR(diskPath.c_str()), MFTStart * Sc, 1024, MFT);
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
        // for (int i = 0; i < temp.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << temp[i] << " ";
        // }
        // cout << "\n";
        StandardInfo itemInfo(temp);
        // itemInfo.printInfo();
        temp.erase(temp.begin(), temp.begin() + itemInfo.getSize());
        // for (int i = 0; i < temp.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << temp[i] << " ";
        // }
        FileName fileName(temp, bpb, diskPath.c_str());
        // fileName.printInfo();
        temp.erase(temp.begin(), temp.begin() + fileName.getSizeAttribute());
        // for (int i = 0; i < temp.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << temp[i] << " ";
        // }
        // cout << "\n";
        vector<string> nextEntry;
        readSector(charToLPCWSTR(diskPath.c_str()), MFTStart * Sc + 2, 1024, nextEntry);
        vector<string> temp2 = nextEntry;
        for (int i = 0; i < temp2.size(); i++)
        {
            if (i % 16 == 0)
                cout << "\n";
            cout << temp2[i] << " ";
        }
        cout << "\n";
        int offsetToAttribute2 = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(temp2, 0x14, 0x15 - 0x14 + 1)));
        temp2.erase(temp2.begin(), temp2.begin() + offsetToAttribute2);
        // cout << nextEntry.size() << endl;
        for (int i = 0; i < temp2.size(); i++)
        {
            if (i % 16 == 0)
                cout << "\n";
            cout << temp2[i] << " ";
        }
        cout << "\n";
    }

    // đọc MFT (không xóa nhưng những dòng dưới)

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
    // printMFT(entries);
    system("pause");

    return 0;
}
