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
#include "DATA.h"
#include "fileName.h"
#include "standardInfo.h"
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
        bootSector.printBootSector();

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
        cout << "BPB info: \n";
        bpb.printBPBInfo();
        int MFTStart = bpb.startingClusterOfMFT();
        int Sc = bpb.getSc();
        system("pause");
        vector<vector<string>> entries;
        vector<string> MFT;
        DWORD count = 78;
        while (true)
        {
            readSector(charToLPCWSTR(diskPath.c_str()), (MFTStart * Sc + count), 1024, MFT);
            int countZero = 0;
            for (int i = 0; i < MFT.size(); i++)
            {
                if (MFT[i] == "00")
                    countZero++;
            }
            if (countZero == MFT.size())
                break;
            entries.push_back(MFT);
            MFT.clear();
            count += 2;
        }
        for (int i = 0; i < entries.size(); i++)
        {
            vector<string> temp = entries[i];
            int offsetToAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(temp, 0x14, 0x15 - 0x14 + 1)));
            temp.erase(temp.begin(), temp.begin() + offsetToAttribute);
            while (temp.empty() == false)
            {
                if ((temp[0] == "FF" && temp[1] == "FF"))
                    break;
                NTFSAttributeHeader header(temp);
                if (header.getTypeID() != 0x10 && header.getTypeID() != 0x30 && header.getTypeID() != 0x80 && header.getTypeID() != 0x90)
                {
                    temp.erase(temp.begin(), temp.begin() + header.getAttributeSize());
                }
                else if (header.getTypeID() == 0x10)
                {
                    StandardInfo itemInfo(temp);
                    itemInfo.printInfo();
                    temp.erase(temp.begin(), temp.begin() + itemInfo.getSize());
                }
                else if (header.getTypeID() == 0x30)
                {
                    FileName fileName(temp, bpb, diskPath.c_str());
                    fileName.printInfo();
                    temp.erase(temp.begin(), temp.begin() + fileName.getSizeEntireAttribute());
                }
                else if (header.getTypeID() == 0x80)
                {
                    DATA data(diskPath.c_str(), temp, bpb, 0);
                    data.printInfo();
                    MFT.erase(MFT.begin(), MFT.begin() + data.getDataSize());
                    break;
                }
                else if (header.getTypeID() == 0x90)
                {
                    // indexRoot indexRoot(temp, bpb, diskPath.c_str());
                    temp.erase(temp.begin(), temp.begin() + header.getAttributeSize());
                }
            }
        }
    }
    system("pause");

    return 0;
}
