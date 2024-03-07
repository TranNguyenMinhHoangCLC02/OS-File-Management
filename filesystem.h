#include "fatTable.h"
#include "bootSector.h"
// #include "fat32.h"
#include "entry.h"

class File
{
private:
    BootSector bootSector;
    // BYTE bootSectorBytes[512];
    FatTable fatTable;
    string content;
    const char* diskPath;
public:
    File(const char* diskPath);
    void printContent();
};
