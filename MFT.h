#include <iostream>
#include <windows.h>
#include <iomanip>
#include "entry.h"
using namespace std;

void ReadSect2(LPCWSTR disk, BYTE*& DATA, unsigned int _nsect);

void read_MFT(unsigned int MFTStart, unsigned int sectors_per_cluster, LPCWSTR disk, vector<vector<string>>& entries);

int getBytes(BYTE* MFT, int offset, int size);

bool checkMFTEntry(BYTE* MFT);

void printMFT(vector<vector<string>> entries);

string byteArrayToHexString(const BYTE* data, size_t size);

void read_MFT2(unsigned int MFTStart, unsigned int sectors_per_cluster, vector<vector<string>> &entries);

bool checkMFTEntry(vector<string> entry);