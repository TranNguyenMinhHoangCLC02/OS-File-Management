#pragma once
#include <time.h>
#include <vector>
#include <cstdint>
#include <windows.h>
#include <string>
#include <iostream>
#include "read.h"

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
    string name;
    BYTE status; // 0x00: entry empty, 0xE5: file is deleted, else: entry is in use
    uint64_t size;
    DWORD firstCluster;
    EntryAttribute attribute;
    vector<vector<string>> entry;
public:
    Entry();
    void readEntry(vector<vector<string>> entry);
    void printEntry();
    string getName(){ return name; };
    BYTE getStatus(){ return status; };
    friend string getFullNameFromASetOfEntry(vector<vector<string>> entry);
    friend string getNameFromSecondaryEntry(vector<string> entry);
    friend bool checkEmpty(vector<string> entry);
    friend bool checkPrimary(vector<string> entry);
};

class Entries
{
private:
    vector<Entry *> entries;
    vector<vector<string>> entireEntries;
public:
    ~Entries();
    void readEntireEntries(DWORD startingSectorOfRDET);
    friend vector<vector<string>> extractEntry(vector<vector<string>> &entries);
    void readEntries();
};