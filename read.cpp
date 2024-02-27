#include <iostream>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "read.h"

using namespace std;

string convertHexToUTF16(string input)
{
    string result = "";
    for (int i = 0; i < input.size(); i += 2)
    {
        string temp = input.substr(i, 2);
        int value = stoi(temp, 0, 16);
        result += (char)value;
    }
    return result;
}

int convertHexadecimalToDecimal(string input)
{
    int result = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] >= '0' && input[i] <= '9')
            result = result * 16 + (input[i] - '0');
        else
            result = result * 16 + (input[i] - 'A' + 10);
    }
    return result;
}

string convertToUpperCase(string input)
{
    for (int i = 0; i < input.size(); i++)
        if (input[i] >= 'a' && input[i] <= 'z')
            input[i] = input[i] - 'a' + 'A';
    return input;
}

string getHexRepresentation(const char* data, size_t size) 
{
    stringstream hexString;
    for (size_t i = 0; i < size; ++i)
        hexString << hex << setw(2) << setfill('0') << static_cast<int>(static_cast<unsigned char>(data[i])) << " ";
    return convertToUpperCase(hexString.str());
}

void getArrayFromHex(const string& hexString, vector<string>& storedValues) 
{
    stringstream hexStream(hexString);
    string temp;
    while (getline(hexStream, temp, ' ')) 
        storedValues.push_back(temp);
}

void readSector(const char *diskPath, DWORD offset, DWORD size, vector<string> &storedValues)
{
    char* buffer = new char[size];
    HANDLE hDevice = CreateFileA(diskPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open disk. Error code: " << GetLastError() << endl;
        return;
    }
    DWORD bytesRead;
    string hexRepresentation;
    SetFilePointer(hDevice, offset, NULL, FILE_BEGIN);
    if (ReadFile(hDevice, buffer, size, &bytesRead, NULL))  
        hexRepresentation = getHexRepresentation(buffer, bytesRead);
    else 
        cerr << "Failed to read VBR. Error code: " << GetLastError() << endl;
    getArrayFromHex(hexRepresentation, storedValues);
    CloseHandle(hDevice);
    delete []buffer;
}

void readSector(const char *diskPath, DWORD offset, BYTE sector[512])
{
    HANDLE hDevice = CreateFileA(diskPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open disk. Error code: " << GetLastError() << endl;
        return;
    }
    SetFilePointer(hDevice, offset, NULL, FILE_BEGIN);
    DWORD bytesRead;
    if (!ReadFile(hDevice, sector, 512, &bytesRead, NULL))
        cerr << "Failed to read VBR. Error code: " << GetLastError() << endl;
    CloseHandle(hDevice);

}