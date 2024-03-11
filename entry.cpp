#include "entry.h"
#include "read.h"
#include "ATTRIBUTE_LIST.h"

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

Entry& Entry::operator=(const Entry& entry){
    this->name = entry.name;
    this->status = entry.status;
    this->size = entry.size;
    this->firstCluster = entry.firstCluster;
    this->attribute = entry.attribute;
    this->entries = entry.entries;
    return *this;
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
    this->attribute = (EntryAttribute)convertHexadecimalToDecimal(primaryEntry[0xB]);
    string temp = convertStringToLittleEdian(getStringFromVector(primaryEntry, 0x1C, 4));
    this->size = convertHexadecimalToDecimal(temp);
    temp =  convertStringToLittleEdian(getStringFromVector(primaryEntry, 0x14, 2)) + convertStringToLittleEdian(getStringFromVector(primaryEntry, 0x1A, 2));
    this->firstCluster = convertHexadecimalToDecimal(temp);
}

void readEntireEntries(const char* diskPath, DWORD startSectorOfRDET, vector<vector<string>> &entries)
{
    vector<string> storedValues;
    while (true)
    {
        readSector(charToLPCWSTR(diskPath), startSectorOfRDET, 512, storedValues);
        if (checkEmpty(storedValues) || storedValues.empty())
            break;
        for (int i = 0; i < storedValues.size(); i += 32)
        {
            vector<string> entry;
            for (int j = 0; j < 32; j++) {
                entry.push_back(storedValues[i + j]);
                // cout << storedValues[i+j] << " ";
            }
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
        if(newEntry->getAttribute() != 0x16)
            this->entries.push_back(newEntry);
        else 
            delete newEntry;
    }
}

vector<int> Entries::getListClusters(){
    vector<int> listClusters;
    for(int i = 0; i < entries.size(); i++){
        listClusters.push_back(entries[i]->getFirstCluster());
    }
    return listClusters;
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

void Entries::removeEntry(int index){
    delete []entries[index];
    entries.erase(entries.begin() + index);
}

Folder* Entries::getSubDirectory(BootSector bootSector, FatTable fatTable, const char* diskPath, string name){
    entries.erase(entries.begin() + 1);
    entries[0]->setName(name);
    return getRootDirectory(bootSector, fatTable, diskPath);
}

File* Entry::getFile(BootSector bootSector, FatTable fatTable){
    File* a = new File;
    return  a;
}

vector<vector<DWORD>> Entries::getListSector(BootSector bootSector, FatTable fatTable){
    vector<int> list_First_Clusters = getListClusters();
    vector<vector<int>> list_Clusters_Of_Entry = fatTable.listClustersOfEntry(list_First_Clusters);
    

    vector<vector<DWORD>> list_Sector_Of_Entry;
    for(int i = 0; i < list_Clusters_Of_Entry.size(); i++){
        vector<DWORD> listSector;
        for(int j = 0; j < list_Clusters_Of_Entry[i].size(); j++){
            DWORD offset = bootSector.getSb() + bootSector.getSf() * bootSector.getNf() + (list_Clusters_Of_Entry[i][j]- 2) * bootSector.getSc();
            listSector.push_back(offset);
        }
        list_Sector_Of_Entry.push_back(listSector);
    }
    return list_Sector_Of_Entry;
}


Folder* Entries::getRootDirectory(BootSector bootSector, FatTable fatTable, const char* diskPath){
    vector<vector<DWORD>> list_Sector = this->getListSector(bootSector, fatTable);
// đoạn này chừng làm gọn lại là truyền vào list first cluster trả ra vecvtor<vector<int>> 
// list sector của mỗi cái vector<int> là dãy cluster bắt dầu từ first cluster đó

    Folder* res = new Folder;
    res->setEntry(entries[0]);

    for(int i = 1; i < list_Sector.size(); i++){
        Item* item;
        if(entries[i]->getAttribute() == Directory){
            for(int j = 0; j < list_Sector[i].size() ; j++){
                vector<vector<string>> s_entries;
                DWORD startSectorOfSDET = list_Sector[i][j];
                readEntireEntries(diskPath, startSectorOfSDET, s_entries);
                removeFaultyEntry(s_entries);
                Entries entry;
                entry.input(s_entries); 
    
                item = entry.getSubDirectory(bootSector, fatTable, diskPath, entries[i]->getName());
            }
        } else if(entries[i]->getAttribute() == Archive){
            File* f = entries[i]->getFile(bootSector, fatTable);
            vector<BYTE> binary_data;
            for(int j = 0; j < list_Sector[i].size() ; j++){
                int size = 0;
                BYTE* data = new BYTE[bootSector.getSc() * 512];
                readSector(diskPath, list_Sector[i][j] * 512, data, bootSector.getSc() * 512);
                for(int t = 0; t < bootSector.getSc() * 512 && size < entries[i]->getSize(); t++){
                    binary_data.push_back(data[t]);
                    size ++;
                }
                delete[] data;
            }
            string content = "";
            for(int i = 0; i < binary_data.size(); i++)
            {
                content += decimalToHex(binary_data[i]) ;
            }
            f->setData(convertHexToUTF16(content));
            item = f;
            item->setEntry(entries[i]);
            
        }
        res->addItem(item);
    }
    return res;
}

void Item::setEntry(Entry* entry){
    this->entry = new Entry;
    this->entry = entry;
}


Folder::Folder(){}

void Folder::print(int level){
    Item::print(level);

    for(int i = 0; i < subfolder.size(); i++)
        subfolder[i]->print(level + 1);
}

void File::print(int level){
    Item::print(level);
    for(int i = 0; i < level; i++)
        cout << "  ";
    cout << "content: ";
    if(data != "")
        cout <<data;
    else
        cout << "Khong co gi het nha:))";
    cout << endl << endl;
}


void Folder::addItem(Item* item){
    this->subfolder.push_back(item);
}

void Folder::deleteItem(){
    for(int i = 0; i < subfolder.size(); i++)
        subfolder[i]->deleteItem();
    Item::deleteItem();
}

void Item::deleteItem(){
    delete entry;
}


void Item::print(int level){
    for(int i = 0; i < level; i++)
        cout << "  ";
    cout << entry->getName() << endl << endl;
}