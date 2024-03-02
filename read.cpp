#include <iostream>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "read.h"
#include "math.h"

using namespace std;

string convertHexToUTF16(string input)
{
    string result;
    for (int i = 0; i < input.length(); i += 2)
    {
        string byteString = input.substr(i, 2);
        char byte = static_cast<char>(convertHexadecimalToDecimal(byteString));
        result += byte;
    }
    return result;
}

int convertHexadecimalToDecimal(string input)
{
    int num;
    stringstream(input) >> hex >> num;
    return num;
}

string convertNumToBinary(int num)
{
    string result;
    while (num > 0)
    {
        result = to_string(num % 2) + result;
        num /= 2;
    }
    return result;
}

int decFromSigned2Complement(string input)
{
    int result = 0;
    if (input[0] == '1')
        result += -1 * pow(2, input.size() - 1);
    for (int i = 1; i < input.size(); i++)
        if (input[i] == '1')
            result += pow(2, input.size() - 1 - i);
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

void readSector(LPCWSTR diskPath, unsigned long long offsetSector, DWORD size, vector<string> &storedValues)
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
    LARGE_INTEGER offset;
    offset.QuadPart = offsetSector * 512;
    SetFilePointerEx(hDevice, offset, NULL, FILE_BEGIN); // Use SetFilePointerEx for LARGE_INTEGER offset
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

void readSector(const char *diskPath, DWORD offset, BYTE* data, DWORD size) {
    HANDLE hDevice = CreateFileA(diskPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open disk. Error code: " << GetLastError() << endl;
        return;
    }
    SetFilePointer(hDevice, offset, NULL, FILE_BEGIN);
    DWORD bytesRead;
    if (!ReadFile(hDevice, data, size, &bytesRead, NULL))
        cerr << "Failed to read sector. Error code: " << GetLastError() << endl;
    CloseHandle(hDevice);
}
}

void readClusters(LPCWSTR diskPath, unsigned long long startingCluster, unsigned long long sectorsPerCluster, DWORD sizeInBytes, vector<string> &storedValues)
{
    unsigned long long startingSector = startingCluster * sectorsPerCluster;
    readSector(diskPath, startingSector, sizeInBytes, storedValues);
}
