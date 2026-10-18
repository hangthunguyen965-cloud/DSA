#include "output.h"
#include <fstream>
#include <iomanip>
using namespace std;

static void ghiSV(ofstream& f, const Student& s) {
    f << s.id << "," << s.name << "," << fixed << setprecision(2) << s.gpa
      << "," << s.credits << "," << s.discipline;
}

bool writeValid(const string& path, const Student a[], int n) {
    ofstream f(path.c_str());
    if (!f) return false;
    f << "id,name,gpa,credits,discipline\n";
    for (int i = 0; i < n; i++) { ghiSV(f, a[i]); f << "\n"; }
    return true;
}

bool writeRejected(const string& path, const RejectedLine errs[], int nErr,
                   const Ineligible rej[], int nRej) {
    ofstream f(path.c_str());
    if (!f) return false;
    f << "type,ref,id,name,gpa,credits,discipline,reason\n";
    for (int i = 0; i < nErr; i++)
        f << "FORMAT," << errs[i].lineNo << ",,,,,,\"" << errs[i].reason << "\"\n";
    for (int i = 0; i < nRej; i++) {
        f << "INELIGIBLE," << rej[i].s.inputIndex << ",";
        ghiSV(f, rej[i].s);
        f << ",\"" << rej[i].reason << "\"\n";
    }
    return true;
}

bool writeRanked(const string& path, const Student a[], int n) {
    ofstream f(path.c_str());
    if (!f) return false;
    f << "rank,id,name,gpa,credits,discipline\n";
    for (int i = 0; i < n; i++) { f << (i + 1) << ","; ghiSV(f, a[i]); f << "\n"; }
    return true;
}

bool writeSummary(const string& path, const RunInfo& r) {
    ofstream f(path.c_str());
    if (!f) return false;
    f << "File dau vao: " << r.inputFile << "\n"
      << "Tong ho so: " << r.total << "\n"
      << "Loi dinh dang: " << r.nErr << "\n"
      << "Hop le dinh dang: " << r.nAll << "\n"
      << "Du dieu kien: " << r.nValid << "\n"
      << "Khong du dieu kien: " << r.nRej << "\n"
      << "Kiem tra: " << r.nErr << " + " << r.nAll << " = " << (r.nErr + r.nAll)
      << " (tong " << r.total << ")\n";
    if (r.ranked)
        f << "Thuat toan: " << algoName(r.algo) << "\n"
          << "Tieu chi: " << (r.mode == RANK_MAIN ? "xep hang chinh" : "chi GPA giam dan") << "\n"
          << "So sanh: " << r.stats.comparisons << "\n"
          << "Di chuyen: " << r.stats.moves << "\n";
    else
        f << "Chua xep hang.\n";
    return true;
}