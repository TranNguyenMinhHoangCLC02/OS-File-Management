#include "read.h"
#include "entry.h"
#pragma once

class NTFSAttributeHeader
{
private:
    vector<string> header;
    int typeID;
    int attributeSize;
    int flagNonResident;
    int lengthAttribute;
    int positionOfAttribute;
    int flag;
    int AttributeID;
public:
    NTFSAttributeHeader()
    {
        this->typeID = 0;
        this->attributeSize = 0;
        this->flagNonResident = 0;
        this->lengthAttribute = 0;
        this->positionOfAttribute = 0;
        this->flag = 0;
        this->AttributeID = 0;
    }
    NTFSAttributeHeader(vector<string> header);
    void printInfo();
    int getTypeID();
    int getAttributeSize();
    int getFlagNonResident();
    int getLengthAttribute();
    int getPositionOfAttribute();
    int getFlag();
    int getAttributeID();
};