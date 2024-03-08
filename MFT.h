#include <iostream>
#include <windows.h>
#include <iomanip>
#include "entry.h"
#include "read.h"
#include "headerAttribute.h"
#include "fileName.h"
#include "DATA.h"
#include "StandardInfo.h"
#include "BPB.h"
#pragma once

using namespace std;

bool checkMFTEntry(vector<string> entry);

void processMFTEntry(vector<string> MFTentry, BPB& bpb, string diskPath);

WORD getStartOffsetOfAtrribute(vector<string> MFTentry);

WORD getFlagsMFTentry(vector<string> MFTentry);