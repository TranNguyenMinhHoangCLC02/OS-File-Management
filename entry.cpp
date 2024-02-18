#include "entry.h"

bool checkPrimary(vector<string> entry)
{
    if (entry[11] == "0F")
        return false;
    return true;
}

bool checkEmpty(vector<string> entry)
{
    for (int i = 0; i < entry.size(); i++)
        if (entry[i] != "00")
            return false;
    return true;
}

string getNameFromSecondaryEntry(vector<string> entry)
{
    string name = "";
    for (int i = 1; i <= 10; i++)
        name += entry[i];
    for (int i = 14; i <= 14 + 12; i++)
        name += entry[i];
    for (int i = 28; i <= 28 + 4; i++)
        name += entry[i];
    return convertHexToUTF16(name);
}

string getFullNameFromASetOfEntry(vector<vector<string>> entry)
{
    if (entry.size() == 1)
        return ""; //idk man :v
    string name = "";
    for (int i = entry.size() - 1; i >= 0; i--)
        name += getNameFromSecondaryEntry(entry[i]);
    return name;
}

Entry::Entry() {
    name = "";
    status = 0;
    size = 0;
    firstCluster = 0;
    attribute = (EntryAttribute)0;
}

void Entry::readEntry(vector<vector<string>> entry)
{
    this->entry = entry;
    vector<string> primary = this->entry[this->entry.size() - 1];
    this->status = convertHexadecimalToDecimal(primary[0]);
    if (this->status == 0 || this->status == 0xE5)
        return;
    if (this->entry.size() > 1)
        this->name = getFullNameFromASetOfEntry(this->entry);
    else
    {
        for (int i = 0; i < 8; i++)
        this->name += primary[i];
        this->name = convertHexToUTF16(this->name);
        this->name += '.';
        string extension = "";
        for (int i = 8; i < 11; i++)
            extension += primary[i];
        extension = convertHexToUTF16(extension);
        this->name += extension;
    }
    this->attribute = (EntryAttribute)convertHexadecimalToDecimal(primary[11]);
    string temp = "";
    for (int i = 28 + 3; i >= 28; i--)
        temp += primary[i];
    this->size = convertHexadecimalToDecimal(temp);
    temp = "";
    for (int i = 14 + 1; i >= 14; i--)
        temp += primary[i];
    for (int i = 26 + 1; i >= 26; i--)
        temp += primary[i];
    this->firstCluster = convertHexadecimalToDecimal(temp);
}

void Entry::printEntry()
{
    if (name == "")
        return;
    cout << "Name: " << name << endl;
    cout << "Status: " << (int)status << endl;
    cout << "Attribute: " << (int)attribute << endl;
    cout << "Size: " << size << " bytes" << endl;
    cout << "First cluster: " << firstCluster << endl;
    cout << endl;
}

void Entries::readEntireEntries(DWORD startingSectorOfRDET)
{
    vector<string> storedValues;
    DWORD currentSector = startingSectorOfRDET;
    while (true)
    {
        bool findEmpty = false;
        readSector("\\\\.\\D:", currentSector * 512, 512, storedValues);
        vector<string> entry;
        for (int i = 0; i < storedValues.size(); i += 32)
        {
            for (int j = 0; j < 32; j++)
                entry.push_back(storedValues[i + j]);
            if (checkEmpty(entry))
            {
                findEmpty = true;
                break;
            }
            this->entireEntries.push_back(entry);
        }
        if (findEmpty == true)
            break;
        currentSector += 1;
    }
}

vector<vector<string>> extractEntry(vector<vector<string>> &entries)
{
    vector<vector<string>> result;
    for (int i = 0; i < entries.size(); i++)
    {
        result.push_back(entries[i]);
        entries.erase(entries.begin() + i);
        if (checkPrimary(entries[i]) == true)
            break;
    }
    return result;
}

void Entries::readEntries()
{
    vector<vector<string>> temp = this->entireEntries;
    while (temp.size() > 0)
    {
        vector<vector<string>> entry = extractEntry(temp);
        Entry *newEntry = new Entry();
        newEntry->readEntry(entry);
        this->entries.push_back(newEntry);
    }
}

Entries::~Entries()
{
    for (int i = 0; i < this->entries.size(); i++)
        delete this->entries[i];
}