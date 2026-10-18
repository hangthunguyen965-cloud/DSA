#include "ranking.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

string rejectReason(const Student& s) {
    string r;
    if (s.gpa + 1e-9 < MIN_GPA) r += "GPA < 3.20";
    if (s.credits < MIN_CREDITS) { if (!r.empty()) r += "; "; r += "Tin chi < 24"; }
    if (s.discipline == 1)       { if (!r.empty()) r += "; "; r += "Co ky luat"; }
    return r;
}

void filterEligible(const Student all[], int nAll,
                    Student valid[], int& nValid,
                    Ineligible out[], int& nOut) {
    nValid = nOut = 0;
    for (int i = 0; i < nAll; i++) {
        string r = rejectReason(all[i]);
        if (r.empty()) valid[nValid++] = all[i];
        else           out[nOut++] = {all[i], r};
    }
}

void printIneligible(const Ineligible a[], int n) {
    cout << left << setw(5) << "STT" << setw(8) << "Ma SV"
         << setw(22) << "Ho ten" << setw(7) << "GPA"
         << setw(9) << "Tin chi" << setw(8) << "Ky luat" << "Ly do\n";
    cout << string(80, '-') << "\n";
    for (int i = 0; i < n; i++)
        cout << left << setw(5) << a[i].s.inputIndex
             << setw(8) << a[i].s.id
             << setw(22) << a[i].s.name
             << fixed << setprecision(2) << setw(7) << a[i].s.gpa
             << setw(9) << a[i].s.credits
             << setw(8) << a[i].s.discipline
             << a[i].reason << "\n";
}

// true neu a phai dung truoc b (nghiem ngat). Bang nhau thi tra false.
bool comesBefore(const Student& a, const Student& b, RankMode mode) {
    if (fabs(a.gpa - b.gpa) > 1e-9) return a.gpa > b.gpa;   // GPA giam dan
    if (mode == RANK_GPA_ONLY) return false;                // hoa
    if (a.credits != b.credits) return a.credits > b.credits; // tin chi giam dan
    return a.id < b.id;                                     // maSV tang dan
}