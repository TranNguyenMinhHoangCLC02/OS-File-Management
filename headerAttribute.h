#include "read.h"
#include "entry.h"

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
    NTFSAttributeHeader(vector<string> header);
    void print();
    void printInfo();
    int getTypeID();
    int getAttributeSize();
    int getFlagNonResident();
    int getLengthAttribute();
    int getPositionOfAttribute();
    int getFlag();
    int getAttributeID();
};