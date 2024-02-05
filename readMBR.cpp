#include <iostream>
#include <windows.h>
#include <iomanip>
#include <vector>

using namespace std;

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

int main()
{
    const char* diskPath = "\\\\.\\PhysicalDrive0"; 
    string hexRepresentation;
    HANDLE hDevice = CreateFileA(diskPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open disk. Error code: " << GetLastError() << endl;
        return 1;
    }

    const DWORD vbrSize = 512;
    char vbr[vbrSize];
    vector<string> storedValues;
    DWORD bytesRead;
    if (ReadFile(hDevice, vbr, vbrSize, &bytesRead, NULL)) 
        hexRepresentation = getHexRepresentation(vbr, bytesRead);
    else 
        cerr << "Failed to read VBR. Error code: " << GetLastError() << endl;

    CloseHandle(hDevice);
    getArrayFromHex(hexRepresentation, storedValues);
    int count = 0;
    cout << "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D ";
    count += 14;
    for (int i = 446; i < storedValues.size(); i++)
    {
        cout << storedValues[i] << " ";
        count++;
        if (count % 16 == 0)
            cout << endl;
    }
    system("pause");
    return 0;
}