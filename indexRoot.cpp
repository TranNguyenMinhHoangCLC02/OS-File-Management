#include "indexRoot.h"

indexRoot::indexRoot(vector<string> entry, BPB bpb, const char *diskPath)
{
    this->header = NTFSAttributeHeader(entry);
    this->entry = entry;
    this->sizeAttribute = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 16, 19 - 16 + 1)));
    this->offset = convertHexadecimalToDecimal(convertStringToLittleEdian(getStringFromVector(this->entry, 20, 21 - 20 + 1)));
    for (int i = this->offset; i < this->sizeAttribute; i++)
    {
        if (i % 16 == 0)
        {
            cout << endl;
        }
        cout << this->entry[i] << " ";
    }
}