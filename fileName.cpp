#include "fileName.h"

FileName::FileName(vector<string> entry)
    {
        this->header = NTFSAttributeHeader(extractHeader(entry));
        this->entry = entry;
        this->sizeAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 16, 19 - 16 + 1)));
        this->offset = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 20, 21 - 20 + 1)));
        this->lengthFileName = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, this->offset + 64, 1)));
        this->nameSpace = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, this->offset + 65, 1)));
        this->fileName = convertHexToUTF16(getStringFromVector(this->entry, this->offset + 66, this->lengthFileName));
    }
void FileName::print()
{
    cout << "File Name: " << this->fileName << endl;
}
string FileName::getFileName()
{
    return this->fileName;
}
int FileName::getSizeAttribute()
{
    return this->sizeAttribute;
}
int FileName::getSizeEntireAttribute()
{
    return this->header.getAttributeSize();
}