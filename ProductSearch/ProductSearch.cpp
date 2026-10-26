#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
using namespace std;

const int MAX_PRODUCTS = 1000;

struct Product {
    string id, name;
    long price;
    int quantity;
};

struct SearchResult {
    bool found;         // co tim thay hay khong
    int position;       // vi tri (tinh tu 1) trong mang dang tim, 0 neu khong thay
    long comparisons;   // so lan so sanh khoa
};

static Product original[MAX_PRODUCTS];   // mang goc, dung thu tu nhap hang
static Product sortedCopy[MAX_PRODUCTS]; // ban sao da sap xep tang dan theo ma
static int productCount = 0;
static bool loaded = false, sortedReady = false;
static long sortComparisons = 0, sortMoves = 0;   // chi phi tien xu ly sap xep

// ===== Ham ho tro =====
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static bool parseLong(const string& s, long& out) {
    if (s.empty()) return false;
    char* end;
    out = strtol(s.c_str(), &end, 10);
    return *end == '\0';
}

// tach theo dau '|', tra ve so truong that
static int splitFields(const string& line, string fields[], int maxFields) {
    int total = 0;
    size_t start = 0;
    while (true) {
        size_t pos = line.find('|', start);
        if (total < maxFields)
            fields[total] = trim(line.substr(start, pos == string::npos ? string::npos : pos - start));
        total++;
        if (pos == string::npos) break;
        start = pos + 1;
    }
    return total;
}

static int readInt(const string& msg, int lo, int hi) {
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

static bool hasData() {
    if (!loaded) { cout << "Chua doc du lieu. Hay chon 1 truoc.\n"; return false; }
    if (productCount == 0) { cout << "Danh sach rong, khong co gi de tim.\n"; return false; }
    return true;
}

// ===== Chuc nang 1: doc tep =====
static bool readFile(const string& path) {
    ifstream in(path.c_str());
    if (!in) { cout << "Khong mo duoc file " << path << "\n"; return false; }

    string line;
    if (!getline(in, line)) { cout << "File rong, khong co dong dau (so luong N).\n"; return false; }
    if (line.size() >= 3 && (unsigned char)line[0] == 0xEF &&
        (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF)
        line.erase(0, 3);                                   // bo BOM cua UTF-8
    long declared;
    if (!parseLong(trim(line), declared) || declared < 0 || declared > MAX_PRODUCTS) {
        cout << "Dong 1 phai la so luong N trong khoang 0.." << MAX_PRODUCTS
             << ", dang la: [" << trim(line) << "]\n";
        return false;
    }

    productCount = 0;
    sortedReady = false;
    int lineNo = 1, dataLines = 0, skipped = 0, extra = 0;
    while (getline(in, line)) {
        lineNo++;
        line = trim(line);
        if (line.empty()) continue;
        dataLines++;
        if (dataLines > declared) { extra++; continue; }    // dong du lieu vuot qua N

        string fields[5];
        int cols = splitFields(line, fields, 5);
        string error;
        Product p;
        long price, quantity;
        if (cols != 4)                           error = "Sai so truong (can 4, co " + to_string(cols) + ")";
        else if (fields[0].empty())              error = "Ma san pham rong";
        else if (fields[1].empty())              error = "Ten san pham rong";
        else if (!parseLong(fields[2], price) || price < 0)       error = "Don gia khong hop le: " + fields[2];
        else if (!parseLong(fields[3], quantity) || quantity < 0) error = "So luong khong hop le: " + fields[3];
        else {
            for (int i = 0; i < productCount; i++)
                if (original[i].id == fields[0]) { error = "Trung ma san pham: " + fields[0]; break; }
        }
        if (!error.empty()) {
            cout << "  Dong " << lineNo << ": " << error << "  -> bo qua\n";
            skipped++;
            continue;
        }
        p.id = fields[0]; p.name = fields[1]; p.price = price; p.quantity = (int)quantity;
        original[productCount++] = p;
    }

    if (dataLines < declared)
        cout << "Canh bao: N = " << declared << " nhung chi co " << dataLines << " dong du lieu.\n";
    if (extra > 0)
        cout << "Canh bao: co " << extra << " dong du lieu vuot qua N, da bo qua.\n";
    cout << "Doc xong: N khai bao = " << declared << ", doc hop le = " << productCount
         << ", bo qua do loi = " << skipped << "\n";
    loaded = true;
    return true;
}

static void printList(const Product a[], int n, const string& title) {
    cout << "\n--- " << title << " (" << n << " san pham) ---\n";
    cout << left << setw(5) << "STT" << setw(8) << "Ma SP" << setw(22) << "Ten SP"
         << setw(12) << "Don gia" << "So luong\n" << string(56, '-') << "\n";
    for (int i = 0; i < n; i++)
        cout << left << setw(5) << (i + 1) << setw(8) << a[i].id << setw(22) << a[i].name
             << setw(12) << a[i].price << a[i].quantity << "\n";
}

// ===== Chuc nang 2: tim kiem tuan tu tren mang goc =====
static SearchResult sequentialSearch(const string& key) {
    SearchResult r = {false, 0, 0};
    for (int i = 0; i < productCount; i++) {
        r.comparisons++;                                    // moi lan so sanh khoa tinh 1
        if (original[i].id == key) { r.found = true; r.position = i + 1; return r; }
    }
    return r;
}

// ===== Chuc nang 3: sap xep ban sao (Insertion Sort tu cai) =====
static void sortCopy() {
    for (int i = 0; i < productCount; i++) sortedCopy[i] = original[i];
    sortComparisons = sortMoves = 0;
    for (int i = 1; i < productCount; i++) {
        Product key = sortedCopy[i]; sortMoves++;
        int j = i - 1;
        while (j >= 0) {
            sortComparisons++;
            if (sortedCopy[j].id.compare(key.id) <= 0) break;
            sortedCopy[j + 1] = sortedCopy[j]; sortMoves++;
            j--;
        }
        sortedCopy[j + 1] = key; sortMoves++;
    }
    sortedReady = true;
}

// ===== Chuc nang 3: tim kiem nhi phan tren ban sao da sap xep =====
static SearchResult binarySearch(const string& key) {
    SearchResult r = {false, 0, 0};
    int lo = 0, hi = productCount - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        r.comparisons++;                                    // 1 lan so sanh khoa moi vong
        int c = sortedCopy[mid].id.compare(key);
        if (c == 0) { r.found = true; r.position = mid + 1; return r; }
        if (c < 0) lo = mid + 1; else hi = mid - 1;
    }
    return r;
}

// ===== Chuc nang 4: thong bao ket qua =====
static void printResult(const string& method, const string& arrayName, const string& key,
                        const SearchResult& r, const Product arr[]) {
    if (r.found) {
        const Product& p = arr[r.position - 1];
        cout << "[" << method << "] TIM THAY ma " << key << "\n"
             << "  Ma: " << p.id << " | Ten: " << p.name
             << " | Don gia: " << p.price << " | So luong: " << p.quantity << "\n"
             << "  Vi tri trong " << arrayName << ": " << r.position << "\n"
             << "  So lan so sanh khoa: " << r.comparisons << "\n";
    } else {
        cout << "[" << method << "] KHONG TIM THAY ma " << key << "\n"
             << "  So lan so sanh khoa: " << r.comparisons << "\n";
    }
}

static string readKey() {
    string key;
    cout << "Nhap ma san pham can tim (vi du SP103): ";
    if (!(cin >> key)) exit(0);
    return trim(key);
}

static void runSequentialSearch() {
    if (!hasData()) return;
    string key = readKey();
    printResult("Tuan tu", "mang goc", key, sequentialSearch(key), original);
}

static void runSortCopy() {
    if (!hasData()) return;
    sortCopy();
    printList(sortedCopy, productCount, "Ban sao da sap xep tang dan theo ma");
    cout << "Chi phi tien xu ly (Insertion Sort): " << sortComparisons << " lan so sanh, "
         << sortMoves << " phep di chuyen\n";
}

static void runBinarySearch() {
    if (!hasData()) return;
    if (!sortedReady) {
        cout << "Chua sap xep ban sao, tu dong sap xep truoc.\n";
        sortCopy();
        cout << "Chi phi tien xu ly: " << sortComparisons << " lan so sanh, "
             << sortMoves << " phep di chuyen\n";
    }
    string key = readKey();
    printResult("Nhi phan", "mang da sap xep", key, binarySearch(key), sortedCopy);
}

// ===== Bao cao: so sanh hai giai thuat tren nhieu truy van =====
static void compareAlgorithms() {
    if (!hasData()) return;
    if (!sortedReady) sortCopy();

    string queries[6] = {
        original[0].id, original[productCount / 2].id, original[productCount - 1].id, sortedCopy[0].id,  // 4 truy van co trong du lieu
        "SP000", original[productCount / 2].id + "X"                                                    // 2 truy van khong co
    };
    cout << "\n" << left << setw(10) << "Truy van" << setw(14) << "Ket qua"
         << setw(16) << "Tuan tu (ss)" << "Nhi phan (ss)\n" << string(52, '-') << "\n";
    long totalSeq = 0, totalBin = 0;
    for (int i = 0; i < 6; i++) {
        SearchResult a = sequentialSearch(queries[i]), b = binarySearch(queries[i]);
        totalSeq += a.comparisons; totalBin += b.comparisons;
        cout << left << setw(10) << queries[i] << setw(14) << (a.found ? "Co" : "Khong co")
             << setw(16) << a.comparisons << b.comparisons << "\n";
    }
    cout << string(52, '-') << "\n" << left << setw(24) << "Tong 6 truy van"
         << setw(16) << totalSeq << totalBin << "\n";
    cout << "\nChi phi tien xu ly sap xep: " << sortComparisons << " so sanh, "
         << sortMoves << " di chuyen\n";
    cout << "Tong chi phi nhi phan (6 truy van + sap xep, chi tinh so sanh): "
         << (totalBin + sortComparisons) << " so voi tuan tu: " << totalSeq << "\n";
    double avgSeq = totalSeq / 6.0, avgBin = totalBin / 6.0;
    if (avgSeq > avgBin) {
        long breakEven = (long)(sortComparisons / (avgSeq - avgBin)) + 1;
        cout << "Trung binh moi truy van: tuan tu " << fixed << setprecision(2) << avgSeq
             << ", nhi phan " << avgBin << " so sanh\n"
             << "Uoc tinh nhi phan co loi tu khoang " << breakEven
             << " truy van tro len (tinh theo so sanh).\n";
    } else {
        cout << "Voi bo truy van nay nhi phan khong tiet kiem hon tuan tu.\n";
    }
}

int main(int argc, char* argv[]) {
    string defaultPath = (argc > 1) ? argv[1] : "san_pham.txt";
    int choice;
    do {
        cout << "\n===== TRA CUU SAN PHAM =====\n"
             << "1. Doc tep va in danh sach\n"
             << "2. Tra cuu tuan tu (mang goc)\n"
             << "3. Sap xep ban sao va in\n"
             << "4. Tra cuu nhi phan (mang da sap xep)\n"
             << "5. So sanh hai giai thuat\n"
             << "0. Thoat\n";
        choice = readInt("Chon: ", 0, 5);
        switch (choice) {
            case 1: {
                cout << "Ten tep (nhap '-' de dung " << defaultPath << "): ";
                string path;
                cin >> path;
                if (path == "-") path = defaultPath;
                if (readFile(path) && productCount > 0) printList(original, productCount, "Danh sach theo thu tu nhap hang");
                break;
            }
            case 2: runSequentialSearch(); break;
            case 3: runSortCopy(); break;
            case 4: runBinarySearch(); break;
            case 5: compareAlgorithms(); break;
        }
    } while (choice != 0);
    return 0;
}