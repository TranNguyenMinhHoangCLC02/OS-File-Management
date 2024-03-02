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

bool checkEmpty(vector<string> entry);
bool checkPrimary(vector<string> entry);
void readEntireEntries(DWORD startSectorOfRDET, vector<vector<string>> &entries);
vector<vector<string>> extractEntry(vector<vector<string>> &entries);
void removeFaultyEntry(vector<vector<string>> &entries);
string convertStringToLittleEdian(string input);
string getStringFromVector(vector<string> storedValues, int start, int length);

class Entry
{
private:
    string name;
    BYTE status;
    int size;
    int firstCluster;
    EntryAttribute attribute;
    vector<vector<string>> entries;
public:
    Entry();
    void readEntry(vector<vector<string>> entry);
    void printEntry();
    void print();
    EntryAttribute getAttribute(){ return attribute;}
    int getFirstCluster(){ return firstCluster; };
    string getName() { return name; };
    BYTE getStatus() { return status; };
    int getSize() { return size; }
    friend string getFullNameFromASetOfEntry(vector<vector<string>> entry);
    friend string getNameFromSecondaryEntry(vector<string> entry);
};

class Entries
{
private:
    vector<Entry*> entries;
    WORD NRDET; // Number entry of RDET
public:
    void input(vector<vector<string>> entries);
    vector<int> getListClusters();
    void print();
    WORD getNRDET() { return entries.size(); }
    vector<Entry*> getEntries() {return entries;}
    ~Entries();
};