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
void readEntireEntries(const char* diskPath, DWORD startSectorOfRDET, vector<vector<string>> &entries);
vector<vector<string>> extractEntry(vector<vector<string>> &entries);
void removeFaultyEntry(vector<vector<string>> &entries);
string convertStringToLittleEdian(string input);
string getStringFromVector(vector<string> storedValues, int start, int length);

class Item;
class File;
class Folder;

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
    Entry& operator=(const Entry& entry);
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
    File* getFile(BootSector bootSector, FatTable fatTable);
};

class Item{
    Entry *entry;
public:
    void setEntry(Entry* entry);
    void virtual print(int level = 0);
    vector<Item*> virtual getSubfolder(){}
    void virtual deleteItem();
};

class Folder : public Item{
    vector<Item*> subfolder;
public:
    Folder();
    void addItem(Item* item);
    vector<Item*> getSubfolder() { return subfolder; }
    void print(int level = 0);
    void deleteItem();
};

class File : public Item{
    string data;
public:
    void setData(string data){ this->data = data; }
    void print(int level = 0);
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
    Folder* getRootDirectory(BootSector bootSector, FatTable fatTable, const char* diskPath);  
    Folder* getSubDirectory(BootSector bootSector, FatTable fatTable, const char* diskPath, string name);

    void removeEntry(int index); 
};