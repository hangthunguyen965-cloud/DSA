#include "student.h"
#include <iostream>
using namespace std;

static Student all[MAX_STUS];
static RejectedLine errs[MAX_STUS];

int main(int argc, char* argv[]) {
    string path = (argc > 1) ? argv[1] : "students.csv";
    int nAll, nErr, total;
    if (!readCSV(path, all, nAll, errs, nErr, total)) {
        cout << "Khong mo duoc file " << path << "\n";
        return 1;
    }
    cout << "Tong ho so: " << total << ", hop le dinh dang: " << nAll
         << ", loi: " << nErr << "\n\n";
    printList(all, nAll);
    if (nErr > 0) { cout << "\n--- Dong loi ---\n"; printErr(errs, nErr); }
    return 0;
}