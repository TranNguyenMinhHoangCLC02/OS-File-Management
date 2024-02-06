#include "filesystem.h"

// File::File(const char* diskPath, const char* fileName)
// {
//     //find file in root directory
//     //read RDET to find file
//     FAT32 fat32;
//     Entry entry = Entry(fat32, 2, 0);
//     if(entry.getName() != fileName)
//     {
//         cout << "File not found" << endl;
//         return;
//     }
//     else if(entry.getStatus() == 0xE5)
//     {
//         cout << "File is deleted" << endl;
//         return;
//     }
//     else if(entry.getStatus() == 0x00)
//     {
//         cout << "File not found" << endl;
//         return;
//     }
//     this->diskPath = (char*)diskPath;
//     fatTable = FatTable(diskPath);
//     bootSector = BootSector();
//     bootSector.readBootSector(bootSectorBytes);
// }