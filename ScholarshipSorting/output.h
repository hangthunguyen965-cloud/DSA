#ifndef OUTPUT_H
#define OUTPUT_H
#include <string>
#include "student.h"
#include "ranking.h"
#include "sorts.h"

struct RunInfo {
    std::string inputFile;
    int total, nErr, nAll, nValid, nRej;
    bool ranked;
    SortAlgo algo;
    RankMode mode;
    SortStats stats;
};

bool writeValid(const std::string& path, const Student a[], int n);
bool writeRejected(const std::string& path, const RejectedLine errs[], int nErr,
                   const Ineligible rej[], int nRej);
bool writeRanked(const std::string& path, const Student a[], int n);
bool writeSummary(const std::string& path, const RunInfo& r);

#endif