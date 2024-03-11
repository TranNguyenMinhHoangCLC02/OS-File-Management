#include "read.h"
#include "entry.h"
#include "headerAttribute.h"
#pragma once

class StandardInfo {
private: 
    int flag; // Cờ thuộc tính
    NTFSAttributeHeader header;  // Header của thuộc tính
    int offset; // Offset bắt đầu phần nội dung thuộc tính
    vector<string> entry; 
public:
    // Constructor cho lớp StandardInfo
    StandardInfo(vector<string> entry);

    // Hàm để in thông tin
    void printInfo();

    // Getter cho cờ thuộc tính
    int getFlags() {return header.getFlag();}

    // Getter cho kích thước thuộc tính
    int getSize(){return header.getAttributeSize();}
};
