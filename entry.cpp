#include "entry.h"

Entry::Entry(FAT32 fat32, DWORD clusterNumber, DWORD offset) {
    fat32.readCluster(clusterNumber);
    BYTE* sector = fat32.getSector();
    BYTE* entry = sector + offset;

    _status = entry[0]; // get status of entry
    if (_status == 0x00)
    {
        _name = "";
        return;
    }
    if (_status == 0xE5)
    {
        _name = "DELETED";
        return;
    }

    _attribute = (EntryAttribute)entry[11]; // get attribute of entry
    _size = *(DWORD*)(entry + 28); // get size of entry
    _creationTime = *(DWORD*)(entry + 14); // get creation time of entry
    _lastModifiedTime = *(DWORD*)(entry + 22); // get last modified time of entry

    // get first cluster form offset 20 and 26
    _firstCluster = *(WORD*)(entry + 20) + (*(WORD*)(entry + 26) << 16);

    _name = ""; // get name of entry
    for (int i = 0; i < 8; i++)
    {
        if (entry[i] == ' ')
            break;
        _name += entry[i];
    }
    if (_attribute & Directory) // if entry is a directory
    {
        _name += ".";
        for (int i = 8; i < 11; i++)
        {
            if (entry[i] == ' ')
                break;
            _name += entry[i];
        }
    }
}

void Entry::printEntry()
{
    if (_name == "")
        return;
    cout << "Name: " << _name << endl;
    cout << "Status: " << (int)_status << endl;
    cout << "Attribute: " << (int)_attribute << endl;
    cout << "Size: " << _size << " bytes" << endl;
    cout << "First cluster: " << _firstCluster << endl;
    cout << "Creation time: " << ctime(&_creationTime);
    cout << "Last modified time: " << ctime(&_lastModifiedTime);
    cout << endl;
}