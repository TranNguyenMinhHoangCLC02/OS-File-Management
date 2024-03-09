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
        // readSector(charToLPCWSTR(diskPath.c_str()), MFTStart * Sc, 1024, MFT);
        // for (int i = 0; i < MFT.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << MFT[i] << " ";
        // }
        // cout << "\n";
        // int MFTSize = 0;
        // vector<string> temp = MFT;
        // int offsetToAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(temp, 0x14, 0x15 - 0x14 + 1)));
        // temp.erase(temp.begin(), temp.begin() + offsetToAttribute);
        // for (int i = 0; i < temp.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << temp[i] << " ";
        // }
        // cout << "\n";
        // StandardInfo itemInfo(temp);
        // itemInfo.printInfo();
        // temp.erase(temp.begin(), temp.begin() + itemInfo.getSize());
        // // for (int i = 0; i < temp.size(); i++)
        // // {
        // //     if (i % 16 == 0)
        // //         cout << "\n";
        // //     cout << temp[i] << " ";
        // // }
        // FileName fileName(temp, bpb, diskPath.c_str());
        // // fileName.printInfo();
        // temp.erase(temp.begin(), temp.begin() + fileName.getSizeAttribute());
        // // for (int i = 0; i < temp.size(); i++)
        // // {
        // //     if (i % 16 == 0)
        // //         cout << "\n";
        // //     cout << temp[i] << " ";
        // // }
        // // cout << "\n";
        // vector<string> nextEntry;
        // readSector(charToLPCWSTR(diskPath.c_str()), MFTStart * Sc + 2, 1024, nextEntry);
        // vector<string> temp2 = nextEntry;
        // for (int i = 0; i < temp2.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << temp2[i] << " ";
        // }
        // cout << "\n";
        // int offsetToAttribute2 = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(temp2, 0x14, 0x15 - 0x14 + 1)));
        // temp2.erase(temp2.begin(), temp2.begin() + offsetToAttribute2);
        // // cout << nextEntry.size() << endl;
        // for (int i = 0; i < temp2.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << temp2[i] << " ";
        // }
        // cout << "\n";
        // StandardInfo itemInfo2(temp2);
        // itemInfo2.printInfo();
        // temp2.erase(temp2.begin(), temp2.begin() + itemInfo2.getSize());
        // for (int i = 0; i < temp2.size(); i++)
        // {
        //     if (i % 16 == 0)
        //         cout << "\n";
        //     cout << temp2[i] << " ";
        // }
        // FileName fileName2(temp2, bpb, diskPath.c_str());
        // fileName2.printInfo();
        // temp2.erase(temp2.begin(), temp2.begin() + fileName2.getSizeAttribute());
        DWORD count = 78;
        // while (true)
        // {
        //     readSector(charToLPCWSTR(diskPath.c_str()), (MFTStart * Sc + count), 1024, MFT);
        //     cout << MFTStart * Sc + count << endl;
        //     for (int i = 0; i < MFT.size(); i++)
        //     {
        //         if (i % 16 == 0)
        //             cout << "\n";
        //         cout << MFT[i] << " ";
        //     }
        //     cout << "\n";
        //     cout << "MFT " << count << endl;
        //     count += 2;
        //     int MFTSize = 0;
        //     vector<string> temp = MFT;
        //     int offsetToAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(temp, 0x14, 0x15 - 0x14 + 1)));
        //     temp.erase(temp.begin(), temp.begin() + offsetToAttribute);
        //     StandardInfo itemInfo(temp);
        //     itemInfo.printInfo();
        //     temp.erase(temp.begin(), temp.begin() + itemInfo.getSize());
        //     NTFSAttributeHeader header(temp);
        //     if (header.getTypeID() == 0x30)
        //     {
        //         FileName fileName(temp, bpb, diskPath.c_str());
        //         fileName.printInfo();
        //         temp.erase(temp.begin(), temp.begin() + fileName.getSizeEntireAttribute());
        //         if (fileName.getFileName().find('$') == string::npos)
        //         {
        //             for (int i = 0; i < temp.size(); i++)
        //             {
        //                 if (i % 16 == 0)
        //                     cout << "\n";
        //                 cout << temp[i] << " ";
        //             }
        //             cout << "\n";
        //         }
        //         NTFSAttributeHeader header2(temp);
        //         temp.erase(temp.begin(), temp.begin() + header2.getAttributeSize());
        //         for (int i = 0; i < temp.size(); i++)
        //         {
        //             if (i % 16 == 0)
        //                 cout << "\n";
        //             cout << temp[i] << " ";
        //         }
        //         cout << "\n";
        //         break;
        //     }
        //     MFT.clear();
        // }
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
                    // data.print();
                    MFT.erase(MFT.begin(), MFT.begin() + data.getDataSize());
                    break;
                }
                else if (header.getTypeID() == 0x90)
                {
                    temp.erase(temp.begin(), temp.begin() + header.getAttributeSize());
                }
            }
        }
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
