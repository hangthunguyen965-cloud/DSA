#include "sorts.h"
#include "ranking.h"

static void swapStudent(Student& x, Student& y, SortStats& st) {
    Student tmp = x; x = y; y = tmp;
    st.moves += 3;                       // 1 lần hoán đổi = 3 phép gán
}

// Insertion Sort trên đoạn a[lo..hi] (dùng cho Insertion Sort và đoạn nhỏ của Quick Sort)
static void insertionRange(Student a[], int lo, int hi, RankMode mode, SortStats& st) {
    for (int i = lo + 1; i <= hi; i++) {
        Student key = a[i]; st.moves++;
        int j = i - 1;
        while (j >= lo) {
            st.comparisons++;
            if (!comesBefore(key, a[j], mode)) break;   // dừng khi bằng nhau => ổn định
            a[j + 1] = a[j]; st.moves++;
            j--;
        }
        a[j + 1] = key; st.moves++;
    }
}

static void insertionSort(Student a[], int n, RankMode mode, SortStats& st) {
    insertionRange(a, 0, n - 1, mode, st);
}

static void selectionSort(Student a[], int n, RankMode mode, SortStats& st) {
    for (int i = 0; i < n - 1; i++) {
        int best = i;
        for (int j = i + 1; j < n; j++) {
            st.comparisons++;
            if (comesBefore(a[j], a[best], mode)) best = j;
        }
        if (best != i) swapStudent(a[i], a[best], st);  // hoán đổi nhảy xa => không ổn định
    }
}

static void bubbleSort(Student a[], int n, RankMode mode, SortStats& st) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            st.comparisons++;
            if (comesBefore(a[j + 1], a[j], mode)) {    // chỉ đổi khi thật sự đứng trước
                swapStudent(a[j], a[j + 1], st);
                swapped = true;
            }
        }
        if (!swapped) break;                            // dừng sớm nếu đã có thứ tự
    }
}

// ===== Quick Sort 3 pivot: chia dãy thành 4 phần =====
const int QUICK_CUTOFF = 8;          // đoạn ngắn hơn ngưỡng này thì dùng Insertion Sort
static Student buf[MAX_STUS];        // mảng phụ cho bước phân hoạch
static int grp[MAX_STUS];            // nhóm (0..3) của từng phần tử, -1 = pivot

static void orderPair(Student& x, Student& y, RankMode mode, SortStats& st) {
    st.comparisons++;
    if (comesBefore(y, x, mode)) swapStudent(x, y, st);
}

static void quick4(Student a[], int lo, int hi, RankMode mode, SortStats& st) {
    int n = hi - lo + 1;
    if (n < QUICK_CUTOFF) { insertionRange(a, lo, hi, mode, st); return; }

    // 1. Chọn 3 pivot ở 1/4, 1/2, 3/4 đoạn rồi sắp chúng tăng dần (3 lần so sánh)
    int idx[3] = { lo + n / 4, lo + n / 2, lo + (3 * n) / 4 };
    Student p[3] = { a[idx[0]], a[idx[1]], a[idx[2]] };
    st.moves += 3;
    orderPair(p[0], p[1], mode, st);
    orderPair(p[1], p[2], mode, st);
    orderPair(p[0], p[1], mode, st);

    // 2. Phân loại từng phần tử còn lại vào 1 trong 4 nhóm (2 lần so sánh mỗi phần tử)
    //    nhóm 0: <= p0 | nhóm 1: (p0, p1] | nhóm 2: (p1, p2] | nhóm 3: > p2
    int cnt[4] = {0, 0, 0, 0};
    for (int k = lo; k <= hi; k++) {
        if (k == idx[0] || k == idx[1] || k == idx[2]) { grp[k] = -1; continue; }
        int g;
        st.comparisons++;
        if (comesBefore(p[1], a[k], mode)) {
            st.comparisons++;
            g = comesBefore(p[2], a[k], mode) ? 3 : 2;
        } else {
            st.comparisons++;
            g = comesBefore(p[0], a[k], mode) ? 1 : 0;
        }
        grp[k] = g;
        cnt[g]++;
    }

    // 3. Xếp lại: nhóm0, p0, nhóm1, p1, nhóm2, p2, nhóm3 (giữ thứ tự tương đối trong mỗi nhóm)
    int start[4];
    start[0] = lo;
    start[1] = start[0] + cnt[0] + 1;
    start[2] = start[1] + cnt[1] + 1;
    start[3] = start[2] + cnt[2] + 1;
    int next[4] = { start[0], start[1], start[2], start[3] };
    for (int k = lo; k <= hi; k++)
        if (grp[k] >= 0) buf[next[grp[k]]++] = a[k];
    buf[start[1] - 1] = p[0];
    buf[start[2] - 1] = p[1];
    buf[start[3] - 1] = p[2];
    for (int k = lo; k <= hi; k++) a[k] = buf[k];
    st.moves += 2 * n;                   // n phép ghi vào buf + n phép chép ngược

    // 4. Đệ quy trên 4 phần (3 pivot đã đứng đúng vị trí cuối cùng)
    for (int g = 0; g < 4; g++)
        quick4(a, start[g], start[g] + cnt[g] - 1, mode, st);
}

const char* algoName(SortAlgo algo) {
    switch (algo) {
        case INSERTION: return "Insertion Sort";
        case SELECTION: return "Selection Sort";
        case BUBBLE:    return "Bubble Sort";
        default:        return "Quick Sort (4)";
    }
}

void copyArray(const Student src[], Student dst[], int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

SortStats sortStudents(Student a[], int n, SortAlgo algo, RankMode mode) {
    SortStats st = {0, 0};
    if (n <= 1) return st;
    if (algo == INSERTION)      insertionSort(a, n, mode, st);
    else if (algo == SELECTION) selectionSort(a, n, mode, st);
    else if (algo == BUBBLE)    bubbleSort(a, n, mode, st);
    else                        quick4(a, 0, n - 1, mode, st);
    return st;
}

bool isSorted(const Student a[], int n, RankMode mode) {
    for (int i = 1; i < n; i++)
        if (comesBefore(a[i], a[i - 1], mode)) return false;
    return true;
}