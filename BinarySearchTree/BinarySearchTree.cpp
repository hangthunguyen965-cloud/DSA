// Chương trình quản lý danh mục sản phẩm bằng cây nhị phân tìm kiếm (BST)
#include <algorithm>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <new>
#include <string>
#include <vector>

// Kiểu dữ liệu 

// Thông tin một sản phẩm; khóa so sánh là productId
struct Product {
    int productId;        // mã sản phẩm: > 0 và không trùng
    std::string name;     // tên sản phẩm: không rỗng, tối đa 49 ký tự
    int quantity;         // số lượng: >= 0
    double unitPrice;     // đơn giá: >= 0
};

// Một nút của cây, lưu toàn bộ thông tin một sản phẩm
struct Node {
    Product product;
    Node* left;           // con trái: các mã nhỏ hơn
    Node* right;          // con phải: các mã lớn hơn

    explicit Node(const Product& value)
        : product(value), left(nullptr), right(nullptr) {}
};

// Kết quả của thao tác chèn
enum class InsertResult { inserted, duplicate, outOfMemory };

// Cây nhị phân tìm kiếm 

class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { clear(); }

    // Không cho sao chép để tránh giải phóng cùng một nút hai lần
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    InsertResult insert(const Product& product) { return insertNode(root, product); }
    bool remove(int productId) { return removeNode(root, productId); }
    const Product* search(int productId) const;
    void printInorder() const;
    int countNodes() const { return countNodes(root); }
    int height() const { return height(root); }
    void clear();

private:
    Node* root;

    static InsertResult insertNode(Node*& current, const Product& product);
    static bool removeNode(Node*& current, int productId);
    static void printInorder(const Node* current);
    static int countNodes(const Node* current);
    static int height(const Node* current);
    static void destroy(Node* current);
};

// Chèn đệ quy; dùng tham chiếu tới con trỏ để gán nút mới trực tiếp vào cây
InsertResult BinarySearchTree::insertNode(Node*& current, const Product& product) {
    if (current == nullptr) {
        current = new (std::nothrow) Node(product);   // kiểm tra kết quả cấp phát
        return current == nullptr ? InsertResult::outOfMemory : InsertResult::inserted;
    }
    if (product.productId < current->product.productId) {
        return insertNode(current->left, product);
    }
    if (product.productId > current->product.productId) {
        return insertNode(current->right, product);
    }
    return InsertResult::duplicate;                    // trùng mã: không tạo nút
}

// Tìm kiếm không đệ quy; trả về nullptr nếu không thấy
const Product* BinarySearchTree::search(int productId) const {
    const Node* current = root;
    while (current != nullptr) {
        if (productId == current->product.productId) return &current->product;
        current = (productId < current->product.productId) ? current->left : current->right;
    }
    return nullptr;
}

// Xóa nút theo ba trường hợp: lá, một con, hai con
bool BinarySearchTree::removeNode(Node*& current, int productId) {
    if (current == nullptr) return false;              // không tìm thấy (kể cả cây rỗng)
    if (productId < current->product.productId) return removeNode(current->left, productId);
    if (productId > current->product.productId) return removeNode(current->right, productId);

    if (current->left == nullptr) {                    // lá hoặc chỉ có con phải
        Node* child = current->right;
        delete current;
        current = child;
    } else if (current->right == nullptr) {            // chỉ có con trái
        Node* child = current->left;
        delete current;
        current = child;
    } else {                                           // hai con: dùng nút kế tiếp trung thứ tự
        Node* successor = current->right;
        while (successor->left != nullptr) successor = successor->left;
        current->product = successor->product;         // chép dữ liệu của nút kế tiếp lên nút hiện tại
        removeNode(current->right, successor->product.productId);  // rồi xóa nút kế tiếp
    }
    return true;
}

// Dòng tiêu đề của bảng sản phẩm
void printProductHeader() {
    std::cout << std::left << std::setw(8) << "Ma SP"
              << std::setw(30) << "Ten san pham"
              << std::right << std::setw(10) << "So luong"
              << std::setw(14) << "Don gia" << "\n"
              << std::string(62, '-') << "\n";
}

// Một dòng dữ liệu của bảng sản phẩm
void printProductRow(const Product& product) {
    std::cout << std::left << std::setw(8) << product.productId
              << std::setw(30) << product.name
              << std::right << std::setw(10) << product.quantity
              << std::setw(14) << std::fixed << std::setprecision(2) << product.unitPrice << "\n";
}

// Duyệt LNR: trái - nút - phải, nên mã luôn tăng dần
void BinarySearchTree::printInorder(const Node* current) {
    if (current == nullptr) return;
    printInorder(current->left);
    printProductRow(current->product);
    printInorder(current->right);
}

void BinarySearchTree::printInorder() const {
    if (root == nullptr) {
        std::cout << "Danh sach rong.\n";
        return;
    }
    printProductHeader();
    printInorder(root);
}

int BinarySearchTree::countNodes(const Node* current) {
    if (current == nullptr) return 0;
    return 1 + countNodes(current->left) + countNodes(current->right);
}

// Chiều cao tính theo số nút trên đường dài nhất; cây rỗng bằng 0
int BinarySearchTree::height(const Node* current) {
    if (current == nullptr) return 0;
    return 1 + std::max(height(current->left), height(current->right));
}

// Giải phóng theo thứ tự hậu thứ tự: con trước, cha sau
void BinarySearchTree::destroy(Node* current) {
    if (current == nullptr) return;
    destroy(current->left);
    destroy(current->right);
    delete current;
}

void BinarySearchTree::clear() {
    destroy(root);
    root = nullptr;
}

// ===================== Kiểm tra và đọc dữ liệu =====================

// Bỏ khoảng trắng đầu cuối (kể cả '\r' của file kết thúc dòng kiểu Windows)
std::string trim(const std::string& text) {
    const char* spaces = " \t\r\n";
    std::size_t first = text.find_first_not_of(spaces);
    if (first == std::string::npos) return "";
    std::size_t last = text.find_last_not_of(spaces);
    return text.substr(first, last - first + 1);
}

// Chuyển chuỗi thành số nguyên; false nếu có ký tự thừa hoặc vượt phạm vi
bool parseInt(const std::string& text, int& value) {
    if (text.empty()) return false;
    char* end = nullptr;
    errno = 0;
    long parsed = std::strtol(text.c_str(), &end, 10);
    if (errno != 0 || *end != '\0' || parsed < INT_MIN || parsed > INT_MAX) return false;
    value = static_cast<int>(parsed);
    return true;
}

// Chuyển chuỗi thành số thực; false nếu có ký tự thừa hoặc giá trị không hữu hạn
bool parseDouble(const std::string& text, double& value) {
    if (text.empty()) return false;
    char* end = nullptr;
    errno = 0;
    double parsed = std::strtod(text.c_str(), &end);
    if (errno != 0 || *end != '\0' || !std::isfinite(parsed)) return false;
    value = parsed;
    return true;
}

// Kiểm tra các ràng buộc của sản phẩm; errorMessage chứa lý do nếu không hợp lệ
bool validateProduct(const Product& product, std::string& errorMessage) {
    if (product.productId <= 0) {
        errorMessage = "Ma san pham phai la so nguyen duong";
    } else if (product.name.empty() || product.name.size() > 49) {
        errorMessage = "Ten san pham khong duoc rong va toi da 49 ky tu";
    } else if (product.quantity < 0) {
        errorMessage = "So luong phai lon hon hoac bang 0";
    } else if (product.unitPrice < 0) {
        errorMessage = "Don gia phai lon hon hoac bang 0";
    } else {
        return true;
    }
    return false;
}

// F1: kiểm tra dữ liệu rồi chèn; message chứa lý do khi thất bại
bool addProduct(BinarySearchTree& tree, const Product& product, std::string& message) {
    if (!validateProduct(product, message)) return false;
    switch (tree.insert(product)) {
        case InsertResult::inserted:
            return true;
        case InsertResult::duplicate:
            message = "Ma san pham da ton tai";
            return false;
        case InsertResult::outOfMemory:
            message = "Khong du bo nho de them san pham";
            return false;
    }
    return false;
}

// Tách chuỗi theo ký tự phân cách
std::vector<std::string> splitFields(const std::string& line, char separator) {
    std::vector<std::string> fields;
    std::size_t start = 0;
    while (true) {
        std::size_t position = line.find(separator, start);
        if (position == std::string::npos) {
            fields.push_back(line.substr(start));
            break;
        }
        fields.push_back(line.substr(start, position - start));
        start = position + 1;
    }
    return fields;
}

// Nạp dữ liệu mẫu từ tệp, mỗi dòng: productId|name|quantity|unitPrice
void loadSampleData(BinarySearchTree& tree, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cout << "Khong mo duoc tep: " << filePath << "\n";
        return;
    }
    std::string line;
    int lineNumber = 0, loaded = 0, skipped = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        line = trim(line);
        if (line.empty()) continue;

        std::vector<std::string> fields = splitFields(line, '|');
        Product product;
        std::string message;
        bool ok = fields.size() == 4
               && parseInt(trim(fields[0]), product.productId)
               && parseInt(trim(fields[2]), product.quantity)
               && parseDouble(trim(fields[3]), product.unitPrice);
        if (!ok) {
            message = "Dong sai dinh dang";
        } else {
            product.name = trim(fields[1]);
            ok = addProduct(tree, product, message);
        }
        if (ok) {
            ++loaded;
        } else {
            ++skipped;
            std::cout << "Bo qua dong " << lineNumber << ": " << message << "\n";
        }
    }
    std::cout << "Da nap " << loaded << " san pham, bo qua " << skipped << " dong.\n";
}

// ===================== Giao diện dòng lệnh =====================

// Hiện lời nhắc và đọc một dòng; false khi hết dữ liệu nhập (EOF)
bool readLine(const std::string& prompt, std::string& line) {
    std::cout << prompt;
    if (!std::getline(std::cin, line)) return false;
    line = trim(line);
    return true;
}

// F1: nhập một sản phẩm từ bàn phím
void handleAdd(BinarySearchTree& tree) {
    Product product;
    std::string text, message;

    if (!readLine("Nhap ma san pham: ", text) || !parseInt(text, product.productId)) {
        std::cout << "Ma san pham khong hop le.\n";
        return;
    }
    if (!readLine("Nhap ten san pham: ", product.name)) return;
    if (!readLine("Nhap so luong: ", text) || !parseInt(text, product.quantity)) {
        std::cout << "So luong khong hop le.\n";
        return;
    }
    if (!readLine("Nhap don gia: ", text) || !parseDouble(text, product.unitPrice)) {
        std::cout << "Don gia khong hop le.\n";
        return;
    }
    if (addProduct(tree, product, message)) {
        std::cout << "Da them san pham co ma " << product.productId << ".\n";
    } else {
        std::cout << message << ".\n";
    }
}

// F2: tìm sản phẩm theo mã
void handleSearch(const BinarySearchTree& tree) {
    std::string text;
    int productId;
    if (!readLine("Nhap ma can tim: ", text) || !parseInt(text, productId)) {
        std::cout << "Ma san pham khong hop le.\n";
        return;
    }
    const Product* found = tree.search(productId);
    if (found == nullptr) {
        std::cout << "Khong tim thay san pham.\n";
        return;
    }
    printProductHeader();
    printProductRow(*found);
}

// F3: xóa sản phẩm theo mã
void handleRemove(BinarySearchTree& tree) {
    std::string text;
    int productId;
    if (!readLine("Nhap ma can xoa: ", text) || !parseInt(text, productId)) {
        std::cout << "Ma san pham khong hop le.\n";
        return;
    }
    if (tree.remove(productId)) {
        std::cout << "Da xoa san pham co ma " << productId << ".\n";
    } else {
        std::cout << "Khong tim thay san pham.\n";
    }
}

// F5: đếm số nút và tính chiều cao
void handleStatistics(const BinarySearchTree& tree) {
    std::cout << "So nut = " << tree.countNodes()
              << "; chieu cao = " << tree.height() << "\n";
}

// F6: hiển thị menu
void printMenu() {
    std::cout << "\n===== QUAN LY DANH MUC SAN PHAM (BST) =====\n"
              << "1. Them san pham\n"
              << "2. Tim san pham theo ma\n"
              << "3. Xoa san pham theo ma\n"
              << "4. In danh sach theo ma tang dan (LNR)\n"
              << "5. Dem so nut va tinh chieu cao\n"
              << "6. Nap du lieu mau tu tep\n"
              << "0. Thoat\n";
}

int main() {
    const std::string sampleDataPath = "data/sample_data.txt";
    BinarySearchTree tree;
    std::string text;

    while (true) {                                     // F6: lặp cho đến khi chọn Thoát
        printMenu();
        if (!readLine("Chon chuc nang: ", text)) break;
        int choice;
        if (!parseInt(text, choice)) {
            std::cout << "Lua chon khong hop le.\n";
            continue;
        }
        if (choice == 0) break;
        switch (choice) {
            case 1: handleAdd(tree); break;
            case 2: handleSearch(tree); break;
            case 3: handleRemove(tree); break;
            case 4: tree.printInorder(); break;
            case 5: handleStatistics(tree); break;
            case 6: loadSampleData(tree, sampleDataPath); break;
            default: std::cout << "Lua chon khong hop le.\n";
        }
    }

    tree.clear();                                      // F7: giải phóng toàn bộ nút
    std::cout << "Da giai phong bo nho. Tam biet!\n";
    return 0;
}