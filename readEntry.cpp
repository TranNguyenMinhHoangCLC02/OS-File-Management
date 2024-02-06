#include "read.h"
#include "bootSector.h"

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

void readEntries(vector<string> &storedValues, DWORD startingSectorOfRDET, vector<vector<string>> &entries)
{
    while (true)
    {
        bool findEmpty = false;
        readSector("\\\\.\\D:", startingSectorOfRDET * 512, 512, storedValues);
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
            entries.push_back(entry);
        }
        if (findEmpty == true)
            break;
    }
}

string convertHexToUTF16(string input)
{
    string result = "";
    for (int i = 0; i < input.size(); i += 2)
    {
        string temp = input.substr(i, 2);
        int value = stoi(temp, 0, 16);
        result += (char)value;
    }
    return result;
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
    return name;
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

string getFullNameFromASetOfEntry(vector<vector<string>> entry)
{
    if (entry.size() == 1)
        return ""; //idk man :v
    string name = "";
    for (int i = entry.size() - 1; i >= 0; i--)
        name += getNameFromSecondaryEntry(entry[i]);
    return name;
}


// int main(int argc, char ** argv)
// {
    // BYTE sector[512];
    // readSector("\\\\.\\D:", 0, sector);
    // BootSector bootSector;
    // bootSector.readBootSector(sector);
    // DWORD startingSectorOfRDET = bootSector.getSb() + bootSector.getNf() * bootSector.getSf();
    // vector<string> storedValues;
    // vector<vector<string>> entries;
    // readEntries(storedValues, startingSectorOfRDET, entries);
    // system("pause");
    // return 0;
// }