#include "bootSector.h"
#include "read.h"
#include "entry.h"

// Constructor của lớp BootSector
BootSector::BootSector() {
    // Khởi tạo các biến thành viên của lớp BootSector
    Sc = 0;
    Sb = 0;
    Nf = 0;
    Sv = 0;
    Sf = 0;
    startClusterRDET = 0;
    numOfSectorExtra = 0;
    numOfSectorSave = 0;
    typeFAT = 0;
}

// Hàm để đọc thông tin từ boot sector
void BootSector::readBootSector(BYTE sector[512]) 
{
    Sc = sector[13]; // Offset D trong boot sector
    Sb = sector[14] + (sector[15] << 8); // Offset E trong boot sector, 2 bytes
    Nf = sector[16]; // Offset 10 trong boot sector
    Sv = sector[32] + (sector[33] << 8) + (sector[34] << 16) + (sector[35] << 24); // Offset 20 trong boot sector, 4 bytes
    Sf = sector[36] + (sector[37] << 8) + (sector[38] << 16) + (sector[39] << 24); // Offset 24 trong boot sector, 4 bytes
    startClusterRDET = sector[44] + (sector[45] << 8) + (sector[46] << 16) + (sector[47] << 24); // Offset 2C trong boot sector, 4 bytes
    numOfSectorExtra = sector[48] + (sector[49] << 8); // Offset 30 trong boot sector, 2 bytes 
    numOfSectorSave = sector[50] + (sector[51] << 8); // Offset 32 trong boot sector, 2 bytes
    typeFAT = static_cast<uint64_t>(sector[82]) |
            (static_cast<uint64_t>(sector[83]) << 8) |
            (static_cast<uint64_t>(sector[84]) << 16) |
            (static_cast<uint64_t>(sector[85]) << 24) |
            (static_cast<uint64_t>(sector[86]) << 32) |
            (static_cast<uint64_t>(sector[87]) << 40) |
            (static_cast<uint64_t>(sector[88]) << 48) |
            (static_cast<uint64_t>(sector[89]) << 56); // Offset 52 trong boot sector, 8 bytes
}

// Hàm để in thông tin boot sector
void BootSector::printBootSector() {
    cout << "Sectors per cluster: " << (int)Sc << endl;
    cout << "Sector of boot sector: " << Sb << endl;
    cout << "Number of FATs: " << (int)Nf << endl;
    cout << "Number of sectors in the volume: " << Sv << endl;
    cout << "Sectors per FAT: " << Sf << endl;
    cout << "Start cluster of RDET: " << startClusterRDET << endl;
    cout << "Number of sectors for extra data: " << numOfSectorExtra << endl;
    cout << "Number of sectors for save data: " << numOfSectorSave << endl;

    // Chuyển typeFAT từ dạng số sang dạng chuỗi
    string fatTypeString = "";
    for (int i = 0; i < 8; ++i) {
        fatTypeString += static_cast<char>((typeFAT >> (i * 8)) & 0xFF);
    }
    cout << "Type of FAT: " << fatTypeString << endl;
}