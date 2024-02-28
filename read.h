#include <iostream>
#include <windows.h>
#include <iomanip>
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
void readSector(const char *diskPath, DWORD offset, DWORD size, vector<string> &storedValues);
void readSector(const char *diskPath, DWORD offset, BYTE sector[512]);