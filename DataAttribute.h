#include "MFT.h"
#include "headerAttribute.h"
#include <iostream>
using namespace std;

class DataAttribute{
    private:
        NTFSAttributeHeader header;
    public:
        DataAttribute(vector<string> MFT);
};