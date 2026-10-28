#include <iostream>
#include <cstring>
using namespace std;

const int MAX_PATIENTS = 5;
const int NAME_SIZE = 40; // Tối đa 39 ký tự + 1 ký tự kết thúc '\0'

// 1. Kiểu dữ liệu
struct Patient {
    int id;
    char name[NAME_SIZE];
    int priority; // Số nhỏ hơn được ưu tiên hơn
    int age;
    int arrivalTime; // Thời gian đến
};

void setPatient(Patient &p, int id, const char *name, int priority, int age, int arrivalTime) {
    p.id = id;
    strcpy(p.name, name);
    p.priority = priority;
    p.arrivalTime = arrivalTime;
}

void printList(const Patient list[], int n) {
    cout << "Ma\tTen\tUu tien\nThoi gian den\nTuoi\n";
    for (int i = 0; i < n; i++)
        cout << list[i].id << "\t" << list[i].name << "\t" << list[i].priority << "\t" << list[i].arrivalTime << "\t\t" << list[i].age << "\n";
}

// 2. Insertion Sort
// Quy ước: ưu tiên nhỏ hơn đứng trước; bằng nhau thì mã nhỏ hơn đứng trước
// Danh sách các trường có thể dùng để sắp xếp: ưu tiên, thời gian đến, tuổi, mã
enum SortField { SORT_PRIORITY, SORT_ARRIVAL_TIME, SORT_AGE, SORT_ID };

// Một tiêu chí gồm: sắp theo trường nào (field) và tăng hay giảm (ascending)
struct SortCriterion {
    SortField field;
    bool ascending; // true: tăng dần, false: giảm dần
};

// Nhận một bệnh nhân và một trường, trả về giá trị của trường đó. Nhờ vậy hàm so sánh không cần biết cụ thể trường nào
int getFieldValue(const Patient &p, SortField field) {
    switch (field) {
        case SORT_PRIORITY:     return p.priority;
        case SORT_ARRIVAL_TIME: return p.arrivalTime;
        case SORT_AGE:          return p.age;
        default:                return p.id;
    }
}
// Hàm so sánh chính: trả về true nếu bệnh nhân a phải đứng trước b
// Duyệt từng tiêu chí; tiêu chí đầu tiên khác nhau sẽ quyết định thứ tự
bool comesBefore(const Patient &a, const Patient &b,
                 const SortCriterion order[], int orderSize) {
    for (int i = 0; i < orderSize; i++) {
        int x = getFieldValue(a, order[i].field);
        int y = getFieldValue(b, order[i].field);
        if (x != y) return order[i].ascending ? x < y : x > y;
    }
    return a.id < b.id; // Chốt cuối: mã tăng dần
}

// Độ phức tạp thời gian trường hợp xấu nhất: O(n^2*k)
// k là số tiêu chí (mỗi lần so sánh tốn tối đa k bước)
void insertionSort(Patient list[], int n, const SortCriterion order[], int orderSize) {
    for (int i = 1; i < n; i++) {
        Patient key = list[i];
        int j = i - 1;
        while (j >= 0 && comesBefore(key, list[j], order, orderSize) ) {
            list[j + 1] = list[j]; // Dịch phần tử sang phải
            j--;
        }
        list[j + 1] = key;
    }
}

// 3. Tìm kiếm nhị phân
// Độ phức tạp thời gian trường hợp xấu nhất: O(log n)
// (mỗi vòng lặp loại bỏ một nửa khoảng tìm kiếm); yêu cầu mảng đã sắp theo mã
int binarySearchById(const Patient list[], int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (list[mid].id == target) return mid;
        if (list[mid].id < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void searchAndPrint(const Patient list[], int n, int target) {
    int pos = binarySearchById(list, n, target);
    cout << "Tra ma " << target << ": ";
    if (pos != -1) cout << list[pos].name << "\n";
    else cout << "KHONG TIM THAY\n";
}

// 4. Queue bằng mảng 
struct Node {
    Patient data;
    Node* next;
    Node(const Patient &p) : data(p), next(nullptr) {}
};


struct Queue {
   Node* front = nullptr;
    Node* rear = nullptr;
    int size = 0;

    Queue() {}
    Queue(const Queue &) = delete;            // Cấm sao chép, tránh giải phóng một nút hai lần
    Queue &operator=(const Queue &) = delete;
    ~Queue() { clear(); }

    bool empty() const { return front == nullptr; }

    // O(1): thêm vào cuối hàng đợi
    void enqueue(const Patient &p) {
        Node* node = new Node(p);
        if (empty()) front = rear = node;
        else { rear->next = node; rear = node; }
        size++;
    }

    // O(1): lấy từ đầu hàng đợi và giải phóng nút

    bool dequeue(Patient &out) {
        if (empty()) return false;
        Node* node = front;
        out = node->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete node;
        size--;
        return true;
    }

    // Hiển thị theo FIFO: duyệt từ front
    void display() const {
        if (empty()) { cout << "(rong)\n"; return; }
        for (Node* p = front; p != nullptr; p = p->next)
            cout << p->data.id << "\t" << p->data.name << "\t" << p->data.priority
                 << "\t" << p->data.arrivalTime << "\t\t" << p->data.age << "\n";
    }

    void clear() { Patient temp; while (dequeue(temp)) {} }
};

int main() {
    // Thứ tự sắp xếp: ưu tiên, rồi thời gian đến, rồi tuổi (đều tăng dần)
    // Muốn đổi thứ tự thì chỉ cần sửa mảng này (và ORDER_SIZE nếu đổi số tiêu chí)
    SortCriterion order[] = {
        {SORT_PRIORITY, true},
        {SORT_ARRIVAL_TIME, true},
        {SORT_AGE, true}
    };
    const int ORDER_SIZE = 3;

    // Yêu cầu 1: dữ liệu cố định, in danh sách gốc
    // Lưu ý: thời gian đến và tuổi là số mẫu tự đặt, hãy thay bằng số thật nếu có
    Patient patients[MAX_PATIENTS];
    setPatient(patients[0], 105, "An",   2, 10, 45);
    setPatient(patients[1], 102, "Binh", 1, 30, 30);
    setPatient(patients[2], 108, "Chi",  3, 20, 60);
    setPatient(patients[3], 101, "Dung", 2,  5, 25);
    setPatient(patients[4], 106, "Ha",   1, 15, 70);

    cout << "=== 1. Danh sach goc ===\n";
    printList(patients, MAX_PATIENTS);

    // Yêu cầu 2: sắp xếp bằng Insertion Sort theo thứ tự đã khai báo ở mảng order
    insertionSort(patients, MAX_PATIENTS, order, ORDER_SIZE);
    cout << "\n=== 2. Danh sach sau khi sap xep ===\n";
    printList(patients, MAX_PATIENTS);

    // Yêu cầu 3: tìm kiếm nhị phân trên bản sao đã sắp theo mã
    // Dùng bản sao để không làm thay đổi thứ tự ưu tiên của mảng gốc
    Patient sortedById[MAX_PATIENTS];
    for (int i = 0; i < MAX_PATIENTS; i++) sortedById[i] = patients[i];
    SortCriterion idOrder[] = { {SORT_ID, true} };
    insertionSort(sortedById, MAX_PATIENTS, idOrder, 1);

    cout << "\n=== 3. Tim kiem nhi phan theo ma ===\n";
    searchAndPrint(sortedById, MAX_PATIENTS, 108);
    searchAndPrint(sortedById, MAX_PATIENTS, 999);

    // Yêu cầu 4: đưa 5 bệnh nhân vào queue theo thứ tự đã sắp, lấy ra 2 người đầu
    Queue waiting;
    for (int i = 0; i < MAX_PATIENTS; i++) waiting.enqueue(patients[i]);

    cout << "\n=== 4. Tiep nhan 2 benh nhan dau ===\n";
    for (int i = 0; i < 2; i++) {
        Patient served;
        if (waiting.dequeue(served))
            cout << "Da tiep nhan ma: " << served.id << "\n";
    }
    cout << "So benh nhan con cho: " << waiting.size << "\n";

    // Yêu cầu 5: hiển thị phần còn lại trong queue theo FIFO
    cout << "\n=== 5. Hang doi con lai (FIFO) ===\n";
    waiting.display();

    return 0;
}