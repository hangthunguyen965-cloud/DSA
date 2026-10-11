#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

const int MAX = 100;
const int EMPTY = 0;

struct Node {
    int id;
    string content;
    int left, right;
    bool isLeaf;
};

Node Tree[MAX];   // global variable (neu chua dung tu = 0)
int root = 1;
int n = 0;

//ham ho tro nhap an toan
int docSo(const string& msg, int lo, int hi) {
    int x;
    while (true) {
        cout << msg;
        if (cin >> x && x >= lo && x <= hi) return x;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Nhap sai, can so tu " << lo << " den " << hi << ".\n";
    }
}

char docYN(const string& msg) {
    char c;
    while (true) {
        cout << msg << " (y/n): ";
        cin >> c;
        if (c == 'y' || c == 'Y') return 'y';
        if (c == 'n' || c == 'N') return 'n';
        cout << "  Chi nhap y hoac n.\n";
    }
}

bool daCoCay() {
    if (n <= 0) {
        cout << "Chua co cay. Hay chon 1 (cay mau) hoac 2 (nhap chay) truoc.\n";
        return false;
    }
    return true;
}

//khoi tao, cay mau, ham nhap
void resetCay() {
    for (int i = 0; i < MAX; i++) Tree[i] = {0, "", 0, 0, false};
    n = 0; root = 1;
}

void loadSampleTree() {
    resetCay();
    n = 7; root = 1;
    Tree[1] = {1, "DTB < 2.0?",            2, 3, false};
    Tree[2] = {2, "No >= 12 tin chi?",     4, 5, false};
    Tree[3] = {3, "DTB >= 3.2?",           6, 7, false};
    Tree[4] = {4, "Canh bao hoc vu muc 2", 0, 0, true};
    Tree[5] = {5, "Canh bao hoc vu muc 1", 0, 0, true};
    Tree[6] = {6, "De xuat khen thuong",   0, 0, true};
    Tree[7] = {7, "Theo doi binh thuong",  0, 0, true};
    cout << "Da nap cay mau 7 nut, root = " << root << ".\n";
}

// Kiem tra du lieu: cho so hop le, khong qua MAX
bool kiemTraCay() {
    bool ok = true;
    int cha[MAX] = {0};
    if (n < 1 || n >= MAX) { cout << "Loi: n khong hop le.\n"; return false; }
    for (int i = 1; i <= n; i++) {
        int l = Tree[i].left, r = Tree[i].right;
        if (l < 0 || l > n || r < 0 || r > n) {
            cout << "Loi: nut " << i << " co con tro ra ngoai mang.\n"; ok = false; continue;
        }
        if (l == i || r == i) { cout << "Loi: nut " << i << " tro ve chinh no.\n"; ok = false; }
        if (Tree[i].isLeaf && (l != 0 || r != 0)) {
            cout << "Loi: nut la " << i << " khong duoc co con.\n"; ok = false;
        }
        if (!Tree[i].isLeaf && (l == 0 || r == 0)) {
            cout << "Loi: nut cau hoi " << i << " thieu nhanh Co/Khong.\n"; ok = false;
        }
        if (l) cha[l]++;
        if (r) cha[r]++;
    }
    for (int i = 1; i <= n; i++) {
        if (i == root && cha[i] != 0) { cout << "Loi: goc co cha.\n"; ok = false; }
        if (i != root && cha[i] != 1) {
            cout << "Loi: nut " << i << " co " << cha[i] << " cha (can dung 1).\n"; ok = false;
        }
    }
    if (ok) cout << "Cay hop le.\n";
    return ok;
}

void nhapCay() {
    resetCay();
    n = docSo("So nut (1-" + to_string(MAX - 1) + "): ", 1, MAX - 1);
    for (int i = 1; i <= n; i++) {
        cout << "--- Nut " << i << " ---\n";
        Tree[i].id = docSo("id: ", 1, 9999);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Noi dung: ";
        getline(cin, Tree[i].content);
        Tree[i].left   = docSo("left (0 = khong co): ", 0, n);
        Tree[i].right  = docSo("right (0 = khong co): ", 0, n);
        Tree[i].isLeaf = docSo("isLeaf (1 = ket luan, 0 = cau hoi): ", 0, 1) == 1;
    }
    kiemTraCay();
}

// Hien thi bang mang
void hienThiBang() {
    if (!daCoCay()) return;
    cout << "\nroot = " << root << ", n = " << n << "\n";
    cout << left << setw(7) << "Index" << setw(6) << "id"
         << setw(26) << "Noi dung" << setw(7) << "left"
         << setw(7) << "right" << "isLeaf\n";
    cout << string(60, '-') << "\n";
    for (int i = 1; i <= n; i++)
        cout << left << setw(7) << i << setw(6) << Tree[i].id
             << setw(26) << Tree[i].content << setw(7) << Tree[i].left
             << setw(7) << Tree[i].right << Tree[i].isLeaf << "\n";
}

// Duyet cay
void preorder(int i) {
    if (i == EMPTY) return;
    cout << Tree[i].id << " - " << Tree[i].content << "\n";
    preorder(Tree[i].left);
    preorder(Tree[i].right);
}
void inorder(int i) {
    if (i == EMPTY) return;
    inorder(Tree[i].left);
    cout << Tree[i].id << " - " << Tree[i].content << "\n";
    inorder(Tree[i].right);
}
void postorder(int i) {
    if (i == EMPTY) return;
    postorder(Tree[i].left);
    postorder(Tree[i].right);
    cout << Tree[i].id << " - " << Tree[i].content << "\n";
}

void duyetCay() {
    if (!daCoCay()) return;
    cout << "\n[Tien tu]\n";  preorder(root);
    cout << "\n[Trung tu]\n"; inorder(root);
    cout << "\n[Hau tu]\n";   postorder(root);
}

// Tim kiem duong di
int findPath(int current, int targetId, int path[], int &len) {
    if (current == EMPTY) return 0;
    path[len++] = current;
    if (Tree[current].id == targetId) return 1;
    if (findPath(Tree[current].left, targetId, path, len)) return 1;
    if (findPath(Tree[current].right, targetId, path, len)) return 1;
    len--;   // quay lui
    return 0;
}

void inPath(int path[], int len) {
    for (int k = 0; k < len; k++) {
        cout << path[k];
        if (k < len - 1) cout << " -> ";
    }
    cout << "\n";
}

void timNut() {
    if (!daCoCay()) return;
    cout << "1. Tim theo chi so\n2. Tim theo id\n";
    int c = docSo("Chon: ", 1, 2);
    int targetId;
    if (c == 1) {
        int i = docSo("Chi so (1-" + to_string(n) + "): ", 1, n);
        targetId = Tree[i].id;
    } else {
        targetId = docSo("id can tim: ", 1, 9999);
    }
    int path[MAX], len = 0;
    if (findPath(root, targetId, path, len)) {
        int idx = path[len - 1];
        cout << "Tim thay: Tree[" << idx << "] = " << Tree[idx].content << "\n";
        cout << "Duong di tu goc: ";
        inPath(path, len);
    } else {
        cout << "Khong tim thay nut co id = " << targetId << ".\n";
    }
}

//chieu cao+dem la
int height(int i) {
    if (i == EMPTY) return 0;
    int hl = height(Tree[i].left), hr = height(Tree[i].right);
    return 1 + (hl > hr ? hl : hr);
}
int countLeaves(int i) {
    if (i == EMPTY) return 0;
    if (Tree[i].left == EMPTY && Tree[i].right == EMPTY) return 1;
    return countLeaves(Tree[i].left) + countLeaves(Tree[i].right);
}
void thongKe() {
    if (!daCoCay()) return;
    cout << "Chieu cao (tinh theo so muc): " << height(root) << "\n";
    cout << "So nut la: " << countLeaves(root) << "\n";
}

//Mo phong tu van
void moPhongTuVan() {
    if (!daCoCay()) return;
    int path[MAX], len = 0, cur = root;
    while (cur != EMPTY && !Tree[cur].isLeaf && len < MAX - 1) {
        path[len++] = cur;
        cur = (docYN(Tree[cur].content) == 'y') ? Tree[cur].left : Tree[cur].right;
    }
    if (cur == EMPTY || !Tree[cur].isLeaf) {
        cout << "Du lieu cay bi loi.\n";
        return;
    }
    path[len++] = cur;
    cout << "Ket luan: " << Tree[cur].content << "\n";
    cout << "Duong di giai thich: ";
    inPath(path, len);
}

//Menu
void processChoice(int c) {
    switch (c) {
        case 1: loadSampleTree(); break;
        case 2: nhapCay(); break;
        case 3: hienThiBang(); break;
        case 4: duyetCay(); break;
        case 5: timNut(); break;
        case 6: thongKe(); break;
        case 7: moPhongTuVan(); break;
        case 8: if (daCoCay()) kiemTraCay(); break;
        case 0: cout << "Tam biet!\n"; break;
    }
}

int main() {
    int choice;
    do {
        cout << "\n===== TREE ARRAY =====\n"
             << "1. Khoi tao cay mau\n"
             << "2. Nhap cay thu cong\n"
             << "3. Hien thi bang mang\n"
             << "4. Duyet tien tu / trung tu / hau tu\n"
             << "5. Tim nut va in duong di\n"
             << "6. Tinh chieu cao va dem nut la\n"
             << "7. Mo phong tu van hoc vu\n"
             << "8. Kiem tra cay hop le\n"
             << "0. Thoat\n";
        choice = docSo("Chon: ", 0, 8);
        processChoice(choice);
    } while (choice != 0);
    return 0;
}