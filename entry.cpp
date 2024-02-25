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

void removeFaultyEntry(vector<vector<string>> &entries)
{
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i][0] == "E5")
        {
            entries.erase(entries.begin() + i);
            i--;
        }
    }
}

string getStringFromVector(vector<string> storedValues, int start, int length)
{
    string result;
    for (int i = start; i < start + length; i++)
        result += storedValues[i];
    return result;
}

string convertStringToLittleEdian(string input)
{
    string result = "";
    string copy = input;
    vector<string> temp;
    while (copy != "")
    {
        temp.push_back(copy.substr(0, 2));
        copy.erase(0, 2);
    }
    for (int i = temp.size() - 1; i >= 0; i--)
        result += temp[i];
    return result;
}

string getNameFromSecondaryEntry(vector<string> entry)
{
    string name = "";
    // for (int i = 1; i < 0xA; i++)
    // {
    //     if (entry[i] == "FF")
    //         break;
    //     name += entry[i];
    // }
    // for (int i = 0xE; i < 0x1A; i++)
    // {
    //     if (entry[i] == "FF")
    //         break;
    //     name += entry[i];
    // }
    // for (int i = 0x1C; i < 0x1F; i++)
    // {
    //     if (entry[i] == "FF")
    //         break;
    //     name += entry[i];
    // }
    name += getStringFromVector(entry, 1, 0xA);
    name += getStringFromVector(entry, 0xE, 0xC);
    name += getStringFromVector(entry, 0x1C, 0x4);
    while (name.find("FF") != string::npos)
        name.erase(name.find("FF"), 2);
    return convertHexToUTF16(name);
}

string getFullNameFromASetOfEntry(vector<vector<string>> entry)
{
    if (entry.size() == 1)
        return "";
    string name = "";
    for (int i = entry.size() - 2; i >= 0; i--)
        name += getNameFromSecondaryEntry(entry[i]);
    return name;
}

Entry::Entry()
{
    name = "";
    status = 0;
    size = 0;
    firstCluster = 0;
    attribute = (EntryAttribute)0;
}

void Entry::print()
{
    for (int i = 0; i < entries.size(); i++)
    {
        for (int j = 0; j < entries[i].size(); j++)
        {
            if (j % 16 == 0)
                cout << endl;
            cout << entries[i][j] << " ";
        }
        cout << endl;
    }
}

void Entry::readEntry(vector<vector<string>> entry)
{
    this->entries = entry;
    vector<string> primaryEntry = this->entries[this->entries.size() - 1];
    this->status = (convertHexadecimalToDecimal(primaryEntry[0]));
    if (this->entries.size() > 1)
        this->name = getFullNameFromASetOfEntry(this->entries);
    else
    {
        for (int i = 0; i < 0x8; i++)
            if (primaryEntry[i] != "20" && primaryEntry[i] != "00")
                this->name += primaryEntry[i];
        this->name = convertHexToUTF16(this->name);
    }
    string extension = getStringFromVector(primaryEntry, 0x8, 3);
    extension = convertHexToUTF16(extension);
    if (extension != "" && extension != "   ")
        this->name += "." + extension;
    this->attribute = (EntryAttribute)convertHexadecimalToDecimal(primaryEntry[0xB]);
    string temp = convertStringToLittleEdian(getStringFromVector(primaryEntry, 0x1C, 4));
    this->size = convertHexadecimalToDecimal(temp);
    temp = convertStringToLittleEdian(getStringFromVector(primaryEntry, 0x1A, 2)) + convertStringToLittleEdian(getStringFromVector(primaryEntry, 0x14, 2));
    this->firstCluster = convertHexadecimalToDecimal(temp);
}

void readEntireEntries(DWORD startSectorOfRDET, vector<vector<string>> &entries)
{
    vector<string> storedValues;
    while (true)
    {
        readSector("\\\\.\\E:", startSectorOfRDET * 512, 512, storedValues);
        if (checkEmpty(storedValues) || storedValues.empty())
            break;
        for (int i = 0; i < storedValues.size(); i += 32)
        {
            vector<string> entry;
            for (int j = 0; j < 32; j++)
                entry.push_back(storedValues[i + j]);
            if (checkEmpty(entry))
                break;
            entries.push_back(entry);
        }
        startSectorOfRDET++;
        storedValues.clear();
    }
}

vector<vector<string>> extractEntry(vector<vector<string>> &entries)
{
    vector<vector<string>> result;
    for (int i = 0; i < entries.size(); i++)
    {
        result.push_back(entries[i]);
        if (checkPrimary(entries[i]))
        {
            entries.erase(entries.begin() + i);
            break;
        }
        entries.erase(entries.begin() + i);
        i--;
    }
    return result;
}

void Entry::printEntry()
{
    cout << "Name: " << name << endl;
    cout << "Status: " << (int)status << endl;
    cout << "Attribute: " << (int)attribute << endl;
    cout << "Size: " << size << " bytes" << endl;
    cout << "First cluster: " << firstCluster << endl;
    cout << endl;

}

void Entries::input(vector<vector<string>> entries)
{
    while (entries.empty() == false)
    {
        vector<vector<string>> temp = extractEntry(entries);
        Entry *newEntry = new Entry();
        newEntry->readEntry(temp);
        this->entries.push_back(newEntry);
    }
}

void Entries::print()
{
    for (int i = 0; i < entries.size(); i++)
    {
        entries[i]->print();
        cout << "\n";
        entries[i]->printEntry();
    }
}

Entries::~Entries()
{
    for (int i = 0; i < entries.size(); i++)
        delete entries[i];
}