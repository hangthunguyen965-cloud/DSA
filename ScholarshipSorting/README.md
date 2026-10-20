# Bài thực hành: Quản lý xét học bổng và công bố bảng xếp hạng sinh viên
- **Môn:** Cấu trúc dữ liệu và giải thuật – **Bài 4:** Các giải thuật sắp xếp
- **Ngôn ngữ:** C++11. Bốn thuật toán (Insertion, Selection, Bubble, Quick Sort 3 pivot) được tự cài đặt, không dùng `qsort()` hay `std::sort()`.
## 1. Bài toán

Chương trình đọc danh sách sinh viên từ file CSV, kiểm tra dữ liệu, lọc hồ sơ đủ điều kiện học bổng, xếp hạng bằng Insertion, Selection, Bubble Sort hoặc Quick sort rồi hiển thị Top K, thống kê và ghi kết quả ra file.

**Quy tắc đủ điều kiện:** GPA ≥ 3.20, tích lũy ≥ 24 tín chỉ, không vi phạm kỷ luật (`discipline = 0`).

**Quy tắc xếp hạng:**
- *Xếp hạng chính:* GPA giảm dần → tín chỉ giảm dần → mã SV tăng dần.
- *Chỉ GPA:* chỉ so GPA giảm dần, dùng để kiểm tra tính ổn định.

**Hai loại hồ sơ bị loại:**
- *Lỗi định dạng:* sai số cột, mã rỗng hoặc trùng, GPA ngoài [0, 4] hoặc không phải số, tín chỉ âm hoặc không phải số nguyên, kỷ luật khác 0 và 1. Chương trình báo kèm số dòng.
- *Không đủ điều kiện:* hợp lệ về định dạng nhưng không đạt quy tắc học bổng, kèm lý do.

Bất biến luôn đúng: **tổng hồ sơ = số lỗi định dạng + số hợp lệ định dạng**.

## 2. Cấu trúc thư mục

```
ScholarshipSorting/
├── main.cpp         Menu và luồng chương trình
├── student.h/.cpp   Struct Student, đọc CSV, kiểm tra từng bản ghi
├── ranking.h/.cpp   Lọc đủ điều kiện, hàm so sánh comesBefore
├── sorts.h/.cpp     Insertion, Selection, Bubble Sort, Quick Sort 3 pivot + đếm so sánh/di chuyển
├── output.h/.cpp    Ghi valid.csv, rejected.csv, ranked.csv, summary.txt
├── students.csv     Dữ liệu đầu vào
└── test_cases.md    Minh chứng kiểm thử
```

## 3. Biên dịch và chạy

```bash
cd ScholarshipSorting
g++ -std=c++11 main.cpp student.cpp ranking.cpp sorts.cpp output.cpp -o scholar
./scholar            # Windows PowerShell: .\scholar.exe
```

## 4. Hướng dẫn sử dụng

| Mục | Chức năng |
|---|---|
| 1 | Đọc CSV (nhập tên file, ví dụ `students.csv`), kiểm tra và lọc điều kiện |
| 2 | Xem dòng lỗi và hồ sơ bị loại kèm lý do |
| 3 | Xếp hạng: chọn thuật toán (1–4) và tiêu chí (chính hoặc chỉ GPA) |
| 4 | Hiển thị Top K |
| 5 | Thống kê số hồ sơ |
| 6 | Ghi file kết quả |
| 7 | Bảng so sánh 4 thuật toán × 2 tiêu chí |

Luồng thường dùng: **1 → 3 → 4 → 6**. Chương trình nhắc nếu chọn sai thứ tự. Sau khi chạy mục 7, cần chọn lại mục 3 trước khi xem Top K hoặc ghi file.

**Trường hợp đặc biệt:** K = 0 không in gì, K lớn hơn số hồ sơ đạt thì chỉ in số hồ sơ có, không ai đủ điều kiện hoặc file trống thì báo thông tin, không bị lỗi.

## 5. File kết quả

File đầu vào không bị ghi đè, chương trình từ chối nếu file đầu vào trùng tên file kết quả.

| File | Nội dung |
|---|---|
| `valid.csv` | Hồ sơ đủ điều kiện, theo thứ tự nhập |
| `rejected.csv` | Cột `type`: `FORMAT` (lỗi định dạng, có số dòng) hoặc `INELIGIBLE` (không đủ điều kiện, có lý do) |
| `ranked.csv` | Bảng xếp hạng, có cột `rank` |
| `summary.txt` | Số liệu hồ sơ, thuật toán, tiêu chí, số so sánh và di chuyển |

## 6. Thiết kế

- **Một hàm so sánh duy nhất** `comesBefore(a, b, mode)` cho cả ba thuật toán, nên kết quả đo công bằng. Hàm trả về `false` khi hai phần tử bằng nhau.
- **`inputIndex`** lưu thứ tự xuất hiện trong CSV để kiểm chứng tính ổn định.
- **Luôn sắp xếp trên bản sao** của mảng hợp lệ, nên chạy lại nhiều thuật toán trên cùng dữ liệu gốc.
- **Quy ước đếm:** *so sánh* là mỗi lần gọi `comesBefore`; *di chuyển* là số phép gán phần tử, một lần hoán đổi tính 3.
**Quick Sort 3 pivot (chia dãy thành 4 phần):**
1. Chọn 3 pivot ở vị trí 1/4, 1/2, 3/4 của đoạn đang xét, sắp chúng tăng dần bằng `comesBefore`.
2. Mỗi phần tử còn lại được so với pivot giữa rồi một pivot bên cạnh (2 lần so sánh) để xếp vào 1 trong 4 nhóm: `≤ p0`, `(p0, p1]`, `(p1, p2]`, `> p2`.
3. Xếp lại thành: nhóm 0, p0, nhóm 1, p1, nhóm 2, p2, nhóm 3. Ba pivot đã đứng đúng vị trí cuối cùng.
4. Đệ quy trên 4 nhóm. Đoạn ngắn hơn 8 phần tử chuyển sang Insertion Sort.

Bước phân hoạch dùng một **mảng phụ** cùng kích thước để giữ thứ tự tương đối trong từng nhóm. Cùng quy ước đếm với ba thuật toán còn lại: *so sánh* là mỗi lần gọi `comesBefore`, *di chuyển* là số phép chép phần tử.

## 7. Kết quả

**Dữ liệu mẫu 8 dòng:** 5 hồ sơ đủ điều kiện, 3 bị loại (SV03 thiếu GPA, SV04 có kỷ luật, SV07 thiếu tín chỉ). Xếp hạng chính ra đúng đáp án: SV02, SV05, SV06, SV08, SV01.

**Dữ liệu 108 dòng:** 50 đủ điều kiện, 58 bị loại.

| Thuật toán | Tiêu chí | So sánh | Di chuyển | Ổn định |
|---|---|---|---|---|
| Insertion | Xếp hạng chính | 638 | 691 | – |
| Selection | Xếp hạng chính | 1225 | 141 | – |
| Bubble | Xếp hạng chính | 1180 | 1779 | – |
| Insertion | Chỉ GPA | 561 | 613 | Có |
| Selection | Chỉ GPA | 1225 | 126 | **Không** |
| Bubble | Chỉ GPA | 1170 | 1545 | Có |
| Quick Sort (4) | Xếp hạng chính | 267 | 337 | – |
| Quick Sort (4) | Chỉ GPA | 248 | 321 | **Không** |

## 8. Nhận xét

- Ở xếp hạng chính, không có hai hồ sơ nào bằng nhau hoàn toàn (mã SV phân định), nên cả bốn thuật toán ra cùng một thứ tự.
- **Tính ổn định:** Insertion và Bubble giữ nguyên thứ tự nhập của các hồ sơ cùng GPA. **Selection và Quick Sort không ổn định**: Selection hoán đổi hai phần tử cách xa nhau, còn Quick Sort đưa các phần tử bằng pivot vào nhóm đứng trước pivot dù ban đầu chúng đứng sau.
- **Số phép so sánh:** Selection luôn so sánh nhiều nhất (n(n−1)/2) dù dữ liệu thế nào. Insertion ít hơn rõ rệt vì dừng sớm khi tìm được chỗ chèn.
- **Số phép di chuyển:** Selection ít nhất (tối đa một lần hoán đổi mỗi vòng), Bubble nhiều nhất do hoán đổi từng cặp kề nhau.
- **Lựa chọn:** nếu cần giữ thứ tự nhập khi trùng khóa thì chọn Insertion (ít so sánh hơn Bubble). Nếu chi phí ghi (di chuyển) đắt thì Selection có lợi, đổi lại không ổn định.
- Insertion, Selection và Bubble đều O(n²). Quick Sort trung bình O(n log n) nhưng trường hợp xấu nhất vẫn có thể O(n²) và cần thêm O(n) bộ nhớ cho mảng phụ.
- **Quick Sort cần ít phép so sánh nhất:** 267 so với 638 (Insertion), 1180 (Bubble) và 1225 (Selection) ở xếp hạng chính. Chia 3 pivot thành 4 phần giúp mỗi lần phân hoạch thu hẹp dãy nhanh hơn so với chia đôi, đổi lại mỗi phần tử tốn 2 lần so sánh ở mỗi tầng.
- **Số phép di chuyển của Quick Sort** nhiều hơn Selection vì mỗi lần phân hoạch phải chép phần tử qua mảng phụ rồi chép ngược lại.
- **Lựa chọn thực tế:** cần tốc độ trên dữ liệu lớn thì dùng Quick Sort, cần giữ thứ tự nhập khi trùng khóa thì dùng Insertion Sort (hoặc Bubble Sort), cần ít thao tác ghi nhất thì dùng Selection Sort.