#pragma once
#include <time.h>
#include <vector>
#include <cstdint>
#include <windows.h>
#include <string>
#include <iostream>
#include "read.h"
#include "bootSector.h"
#include "fatTable.h"

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

class Item;

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
    void setName(string name) { this->name = name; }
    friend string getFullNameFromASetOfEntry(vector<vector<string>> entry);
    friend string getNameFromSecondaryEntry(vector<string> entry);
    Item* getFile(BootSector bootSector, FatTable fatTable);
};

class Item{
    Entry *entry;
public:
    void setEntry(Entry*& entry);
    void virtual addItem(Item* item) = 0;
    void virtual print();
    vector<Item*> virtual getSubFolder(){};
};

class Folder : public Item{
    vector<Item*> subfolder;
public:
    Folder();
    vector<Item*> getSubFolder();
    void addItem(Item* item);
    void print() override;
};

class File : public Item{
    string data;
public:
    // File() : Item (){}
    void addItem(Item* item) {}
    vector<Item*> getSubFolder() {
        vector<Item*> a;
        return a;
    }

};

class Entries
{
private:
    vector<Entry*> entries;
    WORD NRDET; // Number entry of RDET
public:
    void input(vector<vector<string>> entries);
    vector<int> getListClusters();
    vector<vector<DWORD>> getListSector(BootSector bootSector, FatTable fatTable);
    void print();
    WORD getNRDET() { return entries.size(); }
    vector<Entry*> getEntries() {return entries;}
    ~Entries();
    Item* getRootDirectory(BootSector bootSector, FatTable fatTable);  
    Item* getSubDirectory(BootSector bootSector, FatTable fatTable, string name);

    void removeEntry(int index); 
};