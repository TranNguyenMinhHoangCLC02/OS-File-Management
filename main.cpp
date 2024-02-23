#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "bootSector.h"
#include "entry.h"
#include "time.h"

using namespace std;

bool checkDifferent(vector<string> original, vector<string> storedValues)
{
    for (int i = 0; i < original.size(); i++)
        if (original[i] != storedValues[i])
            return true;
    return false;
}

bool checkFirstEntry(vector<string> storedValues)
{
    for (int i = 0; i < 8; i++)
        if (storedValues[i] == "00")
            return false;
    return true;
}

bool findStringINVector(vector<string> storedValues)
{
    string target = "74 20 65 20 73 20 74 20";
    vector<string> temp;
    getArrayFromHex(target, temp);
    for (int i = 0; i < storedValues.size(); i++)
    {
        if (storedValues[i] == temp[0])
        {
            bool found = true;
            for (int j = 1; j < temp.size(); j++)
                if (storedValues[i + j] != temp[j])
                {
                    found = false;
                    break;
                }
            if (found)
                return true;
        }
    }
    return false; // Added return statement to handle all code paths
}

int main()
{
    BYTE sector[512];
    readSector("\\\\.\\E:", 0, sector);
    BootSector bootSector;
    bootSector.readBootSector(sector);
    DWORD startCluster = bootSector.getSb() + bootSector.getSf() * bootSector.getNf() + bootSector.getstartClusterRDET() * bootSector.getSc() - 2;
    vector<string> storedValues;
    startCluster = bootSector.getSb();
    readSector("\\\\.\\E:", startCluster, 512, storedValues);
    for (int i = 0; i < storedValues.size(); i++)
    {
        if (i % 16 == 0)
            cout << endl;
        cout << storedValues[i] << " ";
    }
    return 0;
}
