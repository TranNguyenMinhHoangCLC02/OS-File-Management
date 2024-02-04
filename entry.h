#pragma once
#include "fat32.h"
#include <time.h>
#include <vector>

using namespace std;

enum EntryAttribute {
    ReadOnly = 0x01,   // Bit 0: Read-only
    Hidden = 0x02,     // Bit 1: Hidden
    System = 0x04,     // Bit 2: System
    VolLabel = 0x08,   // Bit 3: Volume Label
    Directory = 0x10,  // Bit 4: Directory
    Archive = 0x20     // Bit 5: Archive
};

class Entry {
private:
    string _name;
    BYTE _status; // 0x00: entry empty, 0xE5: file is deleted, else: entry is in use
    time_t _creationTime;
    time_t _lastModifiedTime;
    uint64_t _size;
    uint16_t _firstCluster;
    EntryAttribute _attribute;
public:
    Entry(FAT32 fat32, DWORD clusterNumber, DWORD offset);
};
