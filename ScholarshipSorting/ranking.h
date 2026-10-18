#ifndef RANKING_H
#define RANKING_H
#include <string>
#include "student.h"

const double MIN_GPA = 3.20;
const int MIN_CREDITS = 24;

struct Ineligible {          // ho so hop le dinh dang nhung khong du dieu kien
    Student s;
    std::string reason;
};

// Bước 3: lọc đủ điều kiện
std::string rejectReason(const Student& s);   // "" neu du dieu kien
void filterEligible(const Student all[], int nAll,
                    Student valid[], int& nValid,
                    Ineligible out[], int& nOut);
void printIneligible(const Ineligible a[], int n);

// Bước 4: quy tắc so sánh dùng chung cho mọi thuật toán
bool comesBefore(const Student& a, const Student& b, RankMode mode);

#endif