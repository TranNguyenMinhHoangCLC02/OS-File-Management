#pragma once
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

string convertHexToUTF16(string input);
int convertHexadecimalToDecimal(string input);
string convertNumToBinary(int num);
int decFromSigned2Complement(string input);
string convertToUpperCase(string input);
string getHexRepresentation(const char* data, size_t size);
void getArrayFromHex(const string& hexString, vector<string>& storedValues);
void readSector(LPCWSTR diskPath, unsigned long long offsetSector, DWORD size, vector<string> &storedValues);
void readSector(const char *diskPath, DWORD offset, BYTE sector[512]);
void readSector(const char *diskPath, DWORD offset, BYTE* data, DWORD size);
void readClusters(LPCWSTR diskPath, unsigned long long startingCluster, unsigned long long sectorsPerCluster, DWORD sizeInBytes, vector<string> &storedValues);
