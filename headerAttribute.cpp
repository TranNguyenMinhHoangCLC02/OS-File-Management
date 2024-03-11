#include "headerAttribute.h"

NTFSAttributeHeader::NTFSAttributeHeader(vector<string> header)
{
    this->header = header;
    this->typeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->header, 0, 3 - 0 + 1)));
    this->attributeSize = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->header, 4, 7 - 4 + 1)));
    this->flagNonResident = convertHexadecimalToDecimal(getStringFromVector(this->header, 8, 1));
    this->nameLength = convertHexadecimalToDecimal(getStringFromVector(this->header, 9, 1));
    this->positionOfAttributeName = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->header, 10, 11 - 10 + 1)));
    this->flag = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->header, 12, 13 - 12 + 1)));
    this->AttributeID = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->header, 14, 15 - 14 + 1)));
}

void NTFSAttributeHeader::printInfo()
{
    cout << "Type ID: " << this->typeID << endl;
    cout << "Attribute Size: " << this->attributeSize << endl;
    cout << "Flag Non Resident: " << this->flagNonResident << endl;
    cout << "Length Attribute Name: " << this->nameLength << endl;
    cout << "Position of Attribute Name: " << this->positionOfAttributeName << endl;
    cout << "Flag: " << this->flag << endl;
    cout << "Attribute ID: " << this->AttributeID << endl;
    system("pause");
}

int NTFSAttributeHeader::getTypeID()
{
    return this->typeID;
}

int NTFSAttributeHeader::getAttributeSize()
{
    return this->attributeSize;
}

int NTFSAttributeHeader::getFlagNonResident()
{
    return this->flagNonResident;
}

int NTFSAttributeHeader::getNameLength()
{
    return this->nameLength;
}

int NTFSAttributeHeader::getPositionOfAttributeName()
{
    return this->positionOfAttributeName;
}

int NTFSAttributeHeader::getFlag()
{
    return this->flag;
}

int NTFSAttributeHeader::getAttributeID()
{
    return this->AttributeID;
}