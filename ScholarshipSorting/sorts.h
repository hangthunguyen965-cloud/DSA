#pragma once
#include "student.h"

enum SortAlgo { INSERTION = 1, SELECTION = 2, BUBBLE = 3 };

const char* algoName(SortAlgo algo);
void copyArray(const Student src[], Student dst[], int n);
SortStats sortStudents(Student a[], int n, SortAlgo algo, RankMode mode);
bool isSorted(const Student a[], int n, RankMode mode);