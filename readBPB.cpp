#include <iostream>
#include <windows.h>
#include <iomanip>
#include <vector>

using namespace std;

string getHexRepresentation(const char* data, size_t size) 
{
    stringstream hexString;
    for (size_t i = 0; i < size; ++i)
        hexString << hex << setw(2) << setfill('0') << static_cast<int>(static_cast<unsigned char>(data[i])) << " ";
    return hexString.str();
}

void getArrayFromHex(const string& hexString, vector<string>& storedValues) 
{
    stringstream hexStream(hexString);
    string temp;
    while (getline(hexStream, temp, ' ')) 
        storedValues.push_back(temp);
}

int findNTFSIdentifier(vector<string> storedValues)
{
    for (int i = 0; i < storedValues.size() - 4; i++)
        if (storedValues[i] == "80" && storedValues[i + 1] == "00" && storedValues[i + 2] == "80" && storedValues[i+3] == "00")
            return i;
}

int findBeginningOfBPB(vector<string> storedValues)
{
    return findNTFSIdentifier(storedValues) - 25;
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

int getSectorSize(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[beginningOfBPB + i];
    return convertHexadecimalToDecimal(temp);
}

int getSc(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2;
    return convertHexadecimalToDecimal(storedValues[current]);
}

int getst(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2;
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getHeadNumber(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2 + 2;
    string temp = "";
    for (int i = 1; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getBeginningSectorOfLogicalPartition(vector<string> storedValues)
{
    int beginningOfBPB = findBeginningOfBPB(storedValues);
    int current = beginningOfBPB + 2 + 1 + 2 + 1 + 2 + 2 + 1 + 2 + 2 + 2;
    string temp = "";
    for (int i = 3; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getSv(vector<string> storedValues)
{
    int beginningOfNTFS = findNTFSIdentifier(storedValues);
    int current = beginningOfNTFS + 4;
    string temp = "";
    for (int i = 7; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int startingClusterOfMFT(vector<string> storedValues)
{
    int beginningOfNTFS = findNTFSIdentifier(storedValues);
    int current = beginningOfNTFS + 4 + 8;
    string temp = "";
    for (int i = 7; i >= 0; i--)
        temp += storedValues[current + i];
    return convertHexadecimalToDecimal(temp);
}

int getSizeOfAMFTEntry(vector<string> storedValues)
{
    int beginningOfNTFS = findNTFSIdentifier(storedValues);
    int current = beginningOfNTFS + 4 + 8 + 8 + 8;
    string temp = "";
    temp += storedValues[current];
    return convertHexadecimalToDecimal(temp);
}

int main() {
    const char* diskPath = "\\\\.\\C:"; 
    string hexRepresentation;
    HANDLE hDevice = CreateFileA(diskPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open disk. Error code: " << GetLastError() << endl;
        return 1;
    }

    const DWORD vbrSize = 512;  // Tui đoán thế thôi chớ không biết kích thước cụ thể, chủ yếu để tìm cái BPB thôi
    char vbr[vbrSize];
    vector<string> storedValues;
    DWORD bytesRead;
    if (ReadFile(hDevice, vbr, vbrSize, &bytesRead, NULL)) 
        hexRepresentation = getHexRepresentation(vbr, bytesRead);
    else 
        cerr << "Failed to read VBR. Error code: " << GetLastError() << endl;

    CloseHandle(hDevice);
    getArrayFromHex(hexRepresentation, storedValues);
    cout << "Sector size = " << getSectorSize(storedValues) << endl;
    cout << "Sc = " << getSc(storedValues) << endl;
    cout << "st = " << getst(storedValues) << endl;
    cout << "h = " << getHeadNumber(storedValues) << endl;
    cout << "Beginning sector of logical partition = " << getBeginningSectorOfLogicalPartition(storedValues) << endl;
    cout << "Sv = " << getSv(storedValues) << endl;
    cout << "Starting cluster of MFT = " << startingClusterOfMFT(storedValues) << endl;
    cout << "Size of a MFT entry = " << getSizeOfAMFTEntry(storedValues) << endl;
    system("pause");
    return 0;
}
