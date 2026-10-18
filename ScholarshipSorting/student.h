#pragma once
#include <string>

const int MAX_STUS = 1000;

struct Student{
    std::string id, name;
    double gpa;
    int credits;
    int discipline; // 0=khong vi pham, 1 = co vi pham
    int inputIndex;  // thu tu dong du lieu ttrong csv
};

struct SortStats { long comparisons; long moves;};

enum RankMode { RANK_MAIN = 1, RANK_GPA_ONLY = 2};

struct RejectedLine {   //dong loi dinh dang
    int lineNo;
    std::string raw, reason;
};

bool readCSV(const std::string& path, Student all[], int& nAll,
            RejectedLine errs[], int& nErr, int& totalRows);
void printList(const Student a[], int n);
void printErr(const RejectedLine errs[], int nErr);

