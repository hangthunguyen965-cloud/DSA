#include "student.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

static string trim(const string& s){
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b-a+1);
}

static bool parseDouble(const string& s, double& out){
    if(s.empty()) return false;
    char* end;
    out = strtod(s.c_str(), &end);
    return *end =='\0';
}

static bool parseInt(const string& s, int& out){
    if(s.empty()) return false;
    char* end;
    out = (int)strtol(s.c_str(), &end, 10);
    return *end =='\0';
}

//tach theo dau phay tra ve so cot that(co the>max)
static int tachCot(const string& line, string f[], int maxF){
    int cnt = 0;
    size_t start = 0;
    while (1)
    {
        size_t pos = line.find(',', start);
        if(cnt<maxF)
            f[cnt] = trim(line.substr(start, pos == string::npos ? string::npos : pos - start));
            cnt++;
            if (pos == string::npos) break;
            start = pos + 1;
    }
    return cnt;
    
}

// tra ve "" neu hop le
static string checkLine(const string& line, const Student all[], int nAll, Student& sv){
    string f[6];
    int cols = tachCot(line, f, 6);
    if (cols != 5) return "Sai so cot(can 5, co " +to_string(cols) + ")";
    if (f[0].empty()) return "Ma sv rong!";
    for (int i = 0; i<nAll; i++){
        if (all[i].id == f[0]) return "Ma sv trung: "+f[0];
    }
    double gpa;
    if (!parseDouble(f[2], gpa)) return "GPA khong phai so!";
    if (!(gpa >= 0 && gpa <= 4)) return "GPA ngoai khoang [1,4]";
    int cr, dis;
    if (!parseInt(f[3], cr)) return "Tin chi khong phai so nguyen!";
    if (cr<0) return "Tin chi am";
    if (!parseInt(f[4], dis)) return "Ky luat khong phai so nguyen!";
    if (dis != 0 && dis != 1) return "Ky luat phai 0 va 1";
    sv.id = f[0]; sv.name = f[1];
    sv.gpa = gpa; sv.credits = cr; sv.discipline = dis;
    return "";
}
 
bool readCSV(const string& path, Student all[], int& nAll,
            RejectedLine errs[], int& nErr, int& totalRows) {
    ifstream in(path.c_str());
    if (!in) return false;
    nAll = nErr = totalRows = 0;
    string line;
    int lineNo = 0;
    bool header = true;
    while (getline(in, line)){
        lineNo++;
        if (header) { header = false; continue;} //bo dong tieu de
        if (trim(line).empty()) continue;;
        if (nAll >= MAX_STUS || nErr >= MAX_STUS){
            cout << "Canh bao: vuot MAX_STUDENTS, bo qua cac dong con lai.\n";
            break;
        }
        totalRows++;
        Student sv;
        string reason = checkLine(line, all, nAll, sv);
        if (reason.empty()) {
            sv.inputIndex = totalRows;      // thu tu nhap, dung cho kiem tra tinh on dinh
            all[nAll++] = sv;
        } else {
            errs[nErr++] = {lineNo, trim(line), reason};   // trim de bo ki tu \r
        }
    }
    return true;
}

void printList(const Student a[], int n) {
    cout << left << setw(5) << "STT" << setw(8) << "Ma SV"
         << setw(22) << "Ho ten" << setw(7) << "GPA"
         << setw(9) << "Tin chi" << "Ky luat\n";
    cout << string(58, '-') << "\n";
    for (int i = 0; i < n; i++)
        cout << left << setw(5) << a[i].inputIndex
             << setw(8) << a[i].id
             << setw(22) << a[i].name
             << fixed << setprecision(2) << setw(7) << a[i].gpa
             << setw(9) << a[i].credits
             << a[i].discipline << "\n";
}

void printErr(const RejectedLine errs[], int nErr) {
    for (int i = 0; i < nErr; i++)
        cout << "Dong " << errs[i].lineNo << ": " << errs[i].reason
             << "  [" << errs[i].raw << "]\n";
}
        
    
    
            