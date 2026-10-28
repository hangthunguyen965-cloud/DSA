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
};

void setPatient(Patient &p, int id, const char *name, int priority) {
    p.id = id;
    strcpy(p.name, name);
    p.priority = priority;
}

void printList(const Patient list[], int n) {
    cout << "Ma\tTen\tUu tien\n";
    for (int i = 0; i < n; i++)
        cout << list[i].id << "\t" << list[i].name << "\t" << list[i].priority << "\n";
}

// 2. Insertion Sort
// Quy ước: ưu tiên nhỏ hơn đứng trước; bằng nhau thì mã nhỏ hơn đứng trước
bool comesBeforeByPriority(const Patient &a, const Patient &b) {
    if (a.priority != b.priority) return a.priority < b.priority;
    return a.id < b.id;
}

bool comesBeforeById(const Patient &a, const Patient &b) {
    return a.id < b.id;
}

// Độ phức tạp thời gian trường hợp xấu nhất: O(n^2)
// (mảng sắp ngược, mỗi phần tử phải dịch qua toàn bộ phần đã sắp xếp)
void insertionSort(Patient list[], int n,
                   bool (*comesBefore)(const Patient &, const Patient &)) {
    for (int i = 1; i < n; i++) {
        Patient key = list[i];
        int j = i - 1;
        while (j >= 0 && comesBefore(key, list[j])) {
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

// 4. Queue bằng mảng (FIFO, mảng vòng)
struct Queue {
    Patient items[MAX_PATIENTS];
    int front = 0;
    int rear = -1;
    int size = 0;

    bool empty() const { return size == 0; }
    bool full() const { return size == MAX_PATIENTS; }

    bool enqueue(const Patient &p) {
        if (full()) return false;
        rear = (rear + 1) % MAX_PATIENTS;
        items[rear] = p;
        size++;
        return true;
    }

    bool dequeue(Patient &out) {
        if (empty()) return false;
        out = items[front];
        front = (front + 1) % MAX_PATIENTS;
        size--;
        return true;
    }

    // 5. Hiển thị theo FIFO: phần tử vào trước được in trước
    void display() const {
        if (empty()) { cout << "(rong)\n"; return; }
        for (int i = 0; i < size; i++) {
            const Patient &p = items[(front + i) % MAX_PATIENTS];
            cout << p.id << "\t" << p.name << "\t" << p.priority << "\n";
        }
    }
};

int main() {
    // dữ liệu cố định, in danh sách gốc
    Patient patients[MAX_PATIENTS];
    setPatient(patients[0], 105, "An", 2);
    setPatient(patients[1], 102, "Binh", 1);
    setPatient(patients[2], 108, "Chi", 3);
    setPatient(patients[3], 101, "Dung", 2);
    setPatient(patients[4], 106, "Ha", 1);

    cout << "=== 1. Danh sach goc ===\n";
    printList(patients, MAX_PATIENTS);

    // sắp xếp theo ưu tiên, bằng nhau thì theo mã
    insertionSort(patients, MAX_PATIENTS, comesBeforeByPriority);
    cout << "\n=== 2. Danh sach sau khi sap xep ===\n";
    printList(patients, MAX_PATIENTS);

    // tìm kiếm nhị phân trên bản sao đã sắp theo mã
    Patient sortedById[MAX_PATIENTS];
    for (int i = 0; i < MAX_PATIENTS; i++) sortedById[i] = patients[i];
    insertionSort(sortedById, MAX_PATIENTS, comesBeforeById);

    cout << "\n=== 3. Tim kiem nhi phan theo ma ===\n";
    searchAndPrint(sortedById, MAX_PATIENTS, 108);
    searchAndPrint(sortedById, MAX_PATIENTS, 999);

    // đưa 5 bệnh nhân vào queue theo thứ tự ở yêu cầu 2, lấy ra 2 người
    Queue waiting;
    for (int i = 0; i < MAX_PATIENTS; i++) waiting.enqueue(patients[i]);

    cout << "\n=== 4. Tiep nhan 2 benh nhan dau ===\n";
    for (int i = 0; i < 2; i++) {
        Patient served;
        if (waiting.dequeue(served))
            cout << "Da tiep nhan ma: " << served.id << "\n";
    }
    cout << "So benh nhan con cho: " << waiting.size << "\n";

    // hiển thị phần còn lại trong queue
    cout << "\n=== 5. Hang doi con lai (FIFO) ===\n";
    waiting.display();

    return 0;
}