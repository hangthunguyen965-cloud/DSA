#include "sorts.h"
#include "ranking.h"

static void swapStudent(Student& x, Student& y, SortStats& st) {
    Student tmp = x; x = y; y = tmp;
    st.moves += 3;                       // 1 lan hoan doi = 3 phep gan
}

static void insertionSort(Student a[], int n, RankMode mode, SortStats& st) {
    for (int i = 1; i < n; i++) {
        Student key = a[i]; st.moves++;
        int j = i - 1;
        while (j >= 0) {
            st.comparisons++;
            if (!comesBefore(key, a[j], mode)) break;   // dung khi bang nhau => on dinh
            a[j + 1] = a[j]; st.moves++;
            j--;
        }
        a[j + 1] = key; st.moves++;
    }
}

static void selectionSort(Student a[], int n, RankMode mode, SortStats& st) {
    for (int i = 0; i < n - 1; i++) {
        int best = i;
        for (int j = i + 1; j < n; j++) {
            st.comparisons++;
            if (comesBefore(a[j], a[best], mode)) best = j;
        }
        if (best != i) swapStudent(a[i], a[best], st);  // hoan doi nhay xa => khong on dinh
    }
}

static void bubbleSort(Student a[], int n, RankMode mode, SortStats& st) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            st.comparisons++;
            if (comesBefore(a[j + 1], a[j], mode)) {    // chi doi khi thuc su dung truoc
                swapStudent(a[j], a[j + 1], st);
                swapped = true;
            }
        }
        if (!swapped) break;                            // dung som neu da co thu tu
    }
}

const char* algoName(SortAlgo algo) {
    switch (algo) {
        case INSERTION: return "Insertion Sort";
        case SELECTION: return "Selection Sort";
        default:        return "Bubble Sort";
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
    else                        bubbleSort(a, n, mode, st);
    return st;
}

bool isSorted(const Student a[], int n, RankMode mode) {
    for (int i = 1; i < n; i++)
        if (comesBefore(a[i], a[i - 1], mode)) return false;
    return true;
}