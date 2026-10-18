#include "student.h"
#include "ranking.h"
#include "sorts.h"
#include "output.h"
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

static Student all[MAX_STUS], valid[MAX_STUS], work[MAX_STUS];
static Ineligible rej[MAX_STUS];
static RejectedLine errs[MAX_STUS];
static RunInfo info;
static bool loaded = false;

static int docSo(const string& msg, int lo, int hi) {
    int x;
    while (true) {
        cout << msg;
        if (cin >> x && x >= lo && x <= hi) return x;
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Nhap sai, can so tu " << lo << " den " << hi << ".\n";
    }
}

static bool coDuLieu() {
    if (!loaded) { cout << "Chua nap du lieu. Hay chon 1 truoc.\n"; return false; }
    return true;
}

// F1 + F2
static void napDuLieu() {
    string path;
    cout << "Ten file CSV (vi du students.csv): ";
    cin >> path;
    int nAll, nErr, total;
    if (!readCSV(path, all, nAll, errs, nErr, total)) {
        cout << "Khong mo duoc file " << path << "\n";
        return;
    }
    int nValid, nRej;
    filterEligible(all, nAll, valid, nValid, rej, nRej);
    info.inputFile = path;
    info.total = total; info.nErr = nErr; info.nAll = nAll;
    info.nValid = nValid; info.nRej = nRej;
    info.ranked = false;
    loaded = true;
    cout << "Tong ho so: " << total << ", loi dinh dang: " << nErr
         << ", hop le dinh dang: " << nAll << "\n";
    cout << "Du dieu kien: " << nValid << ", khong du dieu kien: " << nRej << "\n";
    for (int i = 0; i < nErr; i++)
        cout << "  Dong " << errs[i].lineNo << ": " << errs[i].reason << "\n";
}

static void xemBiLoai() {
    if (!coDuLieu()) return;
    cout << "\n--- Dong loi dinh dang (" << info.nErr << ") ---\n";
    for (int i = 0; i < info.nErr; i++)
        cout << "Dong " << errs[i].lineNo << ": " << errs[i].reason
             << "  [" << errs[i].raw << "]\n";
    cout << "\n--- Khong du dieu kien (" << info.nRej << ") ---\n";
    if (info.nRej > 0) printIneligible(rej, info.nRej);
}

// F3 + F4
static void xepHang() {
    if (!coDuLieu()) return;
    cout << "1. Insertion Sort\n2. Selection Sort\n3. Bubble Sort\n";
    int al = docSo("Thuat toan: ", 1, 3);
    cout << "1. Xep hang chinh (GPA, tin chi, ma SV)\n"
         << "2. Chi GPA giam dan (kiem tra tinh on dinh)\n";
    int md = docSo("Tieu chi: ", 1, 2);
    copyArray(valid, work, info.nValid);        // luon sap xep tren ban sao
    info.stats = sortStudents(work, info.nValid, (SortAlgo)al, (RankMode)md);
    info.algo = (SortAlgo)al;
    info.mode = (RankMode)md;
    info.ranked = true;
    cout << "Da xep " << info.nValid << " ho so bang " << algoName(info.algo)
         << ": so sanh = " << info.stats.comparisons
         << ", di chuyen = " << info.stats.moves << "\n";
}

// F5: Top K
static void hienTopK() {
    if (!coDuLieu()) return;
    if (!info.ranked) { cout << "Chua xep hang. Hay chon 3 truoc.\n"; return; }
    int k = docSo("K (0 - 100000): ", 0, 100000);
    if (info.nValid == 0) { cout << "Khong co ho so nao du dieu kien.\n"; return; }
    if (k == 0) { cout << "K = 0: khong hien thi sinh vien nao.\n"; return; }
    int show = (k > info.nValid) ? info.nValid : k;
    if (k > info.nValid)
        cout << "K lon hon so ho so dat (" << info.nValid << "), chi in " << show << " sinh vien.\n";
    cout << left << setw(6) << "Hang" << setw(8) << "Ma SV" << setw(22) << "Ho ten"
         << setw(7) << "GPA" << setw(9) << "Tin chi" << "Ky luat\n" << string(58, '-') << "\n";
    for (int i = 0; i < show; i++)
        cout << left << setw(6) << (i + 1) << setw(8) << work[i].id << setw(22) << work[i].name
             << fixed << setprecision(2) << setw(7) << work[i].gpa
             << setw(9) << work[i].credits << work[i].discipline << "\n";
}

// F5: thong ke
static void thongKe() {
    if (!coDuLieu()) return;
    cout << "File dau vao:           " << info.inputFile << "\n"
         << "Tong ho so:             " << info.total << "\n"
         << "Loi dinh dang:          " << info.nErr << "\n"
         << "Hop le dinh dang:       " << info.nAll << "\n"
         << "  Du dieu kien:         " << info.nValid << "\n"
         << "  Khong du dieu kien:   " << info.nRej << "\n"
         << "Kiem tra: " << info.nErr << " + " << info.nAll << " = " << (info.nErr + info.nAll)
         << (info.nErr + info.nAll == info.total ? "  (khop tong)\n" : "  (LECH!)\n");
}

// On dinh: cac phan tu lien tiep cung GPA thi inputIndex phai tang dan
static bool onDinhTheoGPA(const Student a[], int n) {
    for (int i = 1; i < n; i++)
        if (a[i].gpa == a[i - 1].gpa && a[i].inputIndex < a[i - 1].inputIndex) return false;
    return true;
}

// bang do cho bao cao: 3 thuat toan x 2 tieu chi tren cung du lieu goc
static void soSanh() {
    if (!coDuLieu()) return;
    cout << left << setw(16) << "Thuat toan" << setw(18) << "Tieu chi"
         << setw(10) << "So sanh" << setw(10) << "Di chuyen" << "On dinh\n"
         << string(64, '-') << "\n";
    for (int md = 1; md <= 2; md++)
        for (int al = 1; al <= 3; al++) {
            copyArray(valid, work, info.nValid);
            SortStats st = sortStudents(work, info.nValid, (SortAlgo)al, (RankMode)md);
            cout << left << setw(16) << algoName((SortAlgo)al)
                 << setw(18) << (md == 1 ? "Xep hang chinh" : "Chi GPA")
                 << setw(10) << st.comparisons << setw(10) << st.moves
                 << (md == 2 ? (onDinhTheoGPA(work, info.nValid) ? "Co" : "KHONG") : "-") << "\n";
        }
    info.ranked = false;     // work da bi ghi de, phai xep lai truoc khi hien Top K / ghi file
}

// F6
static void ghiFile() {
    if (!coDuLieu()) return;
    if (!info.ranked) { cout << "Chua xep hang (hoac vua chay so sanh). Hay chon 3 truoc.\n"; return; }
    const string in = info.inputFile;
    if (in == "valid.csv" || in == "rejected.csv" || in == "ranked.csv" || in == "summary.txt") {
        cout << "File dau vao trung ten file ket qua, doi ten file dau vao de khong bi ghi de.\n";
        return;
    }
    bool ok = writeValid("valid.csv", valid, info.nValid);
    ok = writeRejected("rejected.csv", errs, info.nErr, rej, info.nRej) && ok;
    ok = writeRanked("ranked.csv", work, info.nValid) && ok;
    ok = writeSummary("summary.txt", info) && ok;
    cout << (ok ? "Da ghi valid.csv, rejected.csv, ranked.csv, summary.txt\n"
                : "Loi: khong ghi duoc mot so file.\n");
}

int main() {
    int c;
    do {
        cout << "\n===== XET HOC BONG =====\n"
             << "1. Doc CSV va loc dieu kien\n"
             << "2. Xem ho so loi / bi loai\n"
             << "3. Xep hang (chon thuat toan, tieu chi)\n"
             << "4. Hien thi Top K\n"
             << "5. Thong ke\n"
             << "6. Ghi file ket qua\n"
             << "7. So sanh 3 thuat toan (bang do)\n"
             << "0. Thoat\n";
        c = docSo("Chon: ", 0, 7);
        switch (c) {
            case 1: napDuLieu(); break;
            case 2: xemBiLoai(); break;
            case 3: xepHang(); break;
            case 4: hienTopK(); break;
            case 5: thongKe(); break;
            case 6: ghiFile(); break;
            case 7: soSanh(); break;
        }
    } while (c != 0);
    return 0;
}