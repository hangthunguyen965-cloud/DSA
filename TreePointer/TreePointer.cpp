#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

// ===== Cấu trúc dữ liệu =====
const int MAX = 100;   // giới hạn số nút nhập tay và độ dài mảng phụ

struct Node {
    int id;
    string content;
    Node* left;        // con trỏ tới nút con "Có", nullptr = không có
    Node* right;       // con trỏ tới nút con "Không", nullptr = không có
    bool isLeaf;
};

Node* root = nullptr;
int n = 0;             // số nút đang có

// ===== Hàm hỗ trợ nhập an toàn =====
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
    if (root == nullptr) {
        cout << "Chua co cay. Hay chon 1 (cay mau) hoac 2 (nhap tay) truoc.\n";
        return false;
    }
    return true;
}

// ===== Tạo nút, giải phóng cây =====
Node* taoNut(int id, const string& content, bool isLeaf,
             Node* l = nullptr, Node* r = nullptr) {
    Node* p = new Node{id, content, l, r, isLeaf};
    n++;
    return p;
}

void giaiPhong(Node* p) {      // hậu tự: xóa con trước, xóa cha sau
    if (p == nullptr) return;
    giaiPhong(p->left);
    giaiPhong(p->right);
    delete p;
}

void resetCay() {
    giaiPhong(root);
    root = nullptr;
    n = 0;
}

// ===== Cây mẫu =====
void loadSampleTree() {
    resetCay();
    // dựng từ lá lên gốc vì cha cần biết địa chỉ con
    Node* n4 = taoNut(4, "Canh bao hoc vu muc 2", true);
    Node* n5 = taoNut(5, "Canh bao hoc vu muc 1", true);
    Node* n6 = taoNut(6, "De xuat khen thuong",   true);
    Node* n7 = taoNut(7, "Theo doi binh thuong",  true);
    Node* n2 = taoNut(2, "No >= 12 tin chi?", false, n4, n5);
    Node* n3 = taoNut(3, "DTB >= 3.2?",       false, n6, n7);
    root = taoNut(1, "DTB < 2.0?", false, n2, n3);
    cout << "Da nap cay mau " << n << " nut.\n";
}

// ===== Kiểm tra cây hợp lệ =====
void duyetKiemTra(Node* p, Node* seen[], int& cnt, bool& ok) {
    if (p == nullptr) return;
    for (int k = 0; k < cnt; k++) {
        if (seen[k] == p) {
            cout << "Loi: nut id " << p->id << " co nhieu hon 1 cha hoac tao vong lap.\n";
            ok = false;
            return;               // dừng để không lặp vô hạn
        }
        if (seen[k]->id == p->id) {
            cout << "Loi: id " << p->id << " bi trung.\n";
            ok = false;
        }
    }
    if (cnt >= MAX) { cout << "Loi: vuot qua MAX nut.\n"; ok = false; return; }
    seen[cnt++] = p;
    if (p->isLeaf && (p->left != nullptr || p->right != nullptr)) {
        cout << "Loi: nut la id " << p->id << " khong duoc co con.\n"; ok = false;
    }
    if (!p->isLeaf && (p->left == nullptr || p->right == nullptr)) {
        cout << "Loi: nut cau hoi id " << p->id << " thieu nhanh Co/Khong.\n"; ok = false;
    }
    duyetKiemTra(p->left, seen, cnt, ok);
    duyetKiemTra(p->right, seen, cnt, ok);
}

bool kiemTraCay() {
    if (root == nullptr) { cout << "Cay rong.\n"; return false; }
    Node* seen[MAX];
    int cnt = 0;
    bool ok = true;
    duyetKiemTra(root, seen, cnt, ok);
    if (ok && cnt != n) {
        cout << "Loi: co " << (n - cnt) << " nut khong noi vao cay tu goc.\n";
        ok = false;
    }
    if (ok) cout << "Cay hop le.\n";
    return ok;
}

// ===== Nhập cây thủ công =====
void nhapCay() {
    resetCay();
    int soNut = docSo("So nut (1-" + to_string(MAX - 1) + "): ", 1, MAX - 1);
    Node* nodes[MAX] = {nullptr};   // mảng phụ, chỉ dùng trong lúc nhập để nối con trỏ
    int lt[MAX], rt[MAX];

    for (int i = 1; i <= soNut; i++) {
        cout << "--- Nut " << i << " ---\n";
        int id = docSo("id: ", 1, 9999);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string nd;
        cout << "Noi dung: ";
        getline(cin, nd);
        lt[i] = docSo("con trai la nut thu may (0 = khong co): ", 0, soNut);
        rt[i] = docSo("con phai la nut thu may (0 = khong co): ", 0, soNut);
        bool leaf = docSo("isLeaf (1 = ket luan, 0 = cau hoi): ", 0, 1) == 1;
        nodes[i] = taoNut(id, nd, leaf);
    }
    for (int i = 1; i <= soNut; i++) {
        nodes[i]->left  = lt[i] ? nodes[lt[i]] : nullptr;
        nodes[i]->right = rt[i] ? nodes[rt[i]] : nullptr;
    }
    root = nodes[1];                 // nút nhập đầu tiên là gốc

    if (!kiemTraCay()) {
        for (int i = 1; i <= soNut; i++) delete nodes[i];
        root = nullptr;
        n = 0;
        cout << "Cay khong hop le, da huy. Hay nhap lai.\n";
    }
}

// ===== Hiển thị bảng nút =====
void inDong(Node* p) {
    if (p == nullptr) return;
    cout << left << setw(6) << p->id
         << setw(26) << p->content
         << setw(8) << (p->left  ? to_string(p->left->id)  : string("NULL"))
         << setw(8) << (p->right ? to_string(p->right->id) : string("NULL"))
         << setw(8) << p->isLeaf
         << static_cast<void*>(p) << "\n";
    inDong(p->left);
    inDong(p->right);
}

void hienThiBang() {
    if (!daCoCay()) return;
    cout << "\nroot = " << static_cast<void*>(root) << ", n = " << n << "\n";
    cout << left << setw(6) << "id" << setw(26) << "Noi dung"
         << setw(8) << "left" << setw(8) << "right"
         << setw(8) << "isLeaf" << "Dia chi nut\n";
    cout << string(75, '-') << "\n";
    inDong(root);
}

// ===== Duyệt cây =====
void preorder(Node* p) {
    if (p == nullptr) return;
    cout << p->id << " - " << p->content << "\n";
    preorder(p->left);
    preorder(p->right);
}
void inorder(Node* p) {
    if (p == nullptr) return;
    inorder(p->left);
    cout << p->id << " - " << p->content << "\n";
    inorder(p->right);
}
void postorder(Node* p) {
    if (p == nullptr) return;
    postorder(p->left);
    postorder(p->right);
    cout << p->id << " - " << p->content << "\n";
}

void duyetCay() {
    if (!daCoCay()) return;
    cout << "\n[Tien tu]\n";  preorder(root);
    cout << "\n[Trung tu]\n"; inorder(root);
    cout << "\n[Hau tu]\n";   postorder(root);
}

// ===== Tìm nút + đường đi =====
int findPath(Node* current, int targetId, Node* path[], int& len) {
    if (current == nullptr) return 0;
    path[len++] = current;
    if (current->id == targetId) return 1;
    if (findPath(current->left, targetId, path, len)) return 1;
    if (findPath(current->right, targetId, path, len)) return 1;
    len--;   // quay lui
    return 0;
}

void inPath(Node* path[], int len) {
    for (int k = 0; k < len; k++) {
        cout << path[k]->id;
        if (k < len - 1) cout << " -> ";
    }
    cout << "\n";
}

void timNut() {
    if (!daCoCay()) return;
    int targetId = docSo("id can tim: ", 1, 9999);
    Node* path[MAX];
    int len = 0;
    if (findPath(root, targetId, path, len)) {
        Node* p = path[len - 1];
        cout << "Tim thay nut id " << p->id << ": " << p->content
             << " (dia chi " << static_cast<void*>(p) << ")\n";
        cout << "Duong di tu goc: ";
        inPath(path, len);
    } else {
        cout << "Khong tim thay nut co id = " << targetId << ".\n";
    }
}

// ===== Chiều cao + đếm lá =====
int height(Node* p) {
    if (p == nullptr) return 0;
    int hl = height(p->left), hr = height(p->right);
    return 1 + (hl > hr ? hl : hr);
}
int countLeaves(Node* p) {
    if (p == nullptr) return 0;
    if (p->left == nullptr && p->right == nullptr) return 1;
    return countLeaves(p->left) + countLeaves(p->right);
}
void thongKe() {
    if (!daCoCay()) return;
    cout << "Chieu cao (tinh theo so muc): " << height(root) << "\n";
    cout << "So nut la: " << countLeaves(root) << "\n";
}

// ===== Mô phỏng tư vấn =====
void moPhongTuVan() {
    if (!daCoCay()) return;
    Node* path[MAX];
    int len = 0;
    Node* cur = root;
    while (cur != nullptr && !cur->isLeaf && len < MAX - 1) {
        path[len++] = cur;
        cur = (docYN(cur->content) == 'y') ? cur->left : cur->right;
    }
    if (cur == nullptr || !cur->isLeaf) {
        cout << "Du lieu cay bi loi.\n";
        return;
    }
    path[len++] = cur;
    cout << "Ket luan: " << cur->content << "\n";
    cout << "Duong di giai thich: ";
    inPath(path, len);
}

// ===== Menu =====
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
        case 0: resetCay(); cout << "Da giai phong bo nho. Tam biet!\n"; break;
    }
}

int main() {
    int choice;
    do {
        cout << "\n===== TREE POINTER =====\n"
             << "1. Khoi tao cay mau\n"
             << "2. Nhap cay thu cong\n"
             << "3. Hien thi bang nut\n"
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