#include "DataAttribute.h"

DataAttribute::DataAttribute(vector<string> MFT)
{
    this->header = NTFSAttributeHeader(MFT);
}