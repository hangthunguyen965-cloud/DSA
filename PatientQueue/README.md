# Bài thực hành: Quản lý danh sách bệnh nhân đã đăng ký tại phòng khám
- **Môn:** Cấu trúc dữ liệu và giải thuật
- **Ngôn ngữ:** C++11. Insertion Sort, tìm kiếm nhị phân và hàng đợi đều tự cài đặt trên mảng, không dùng thư viện cấu trúc dữ liệu hay thuật toán có sẵn.

## 1. Bài toán

Chương trình dòng lệnh mô phỏng việc tiếp nhận bệnh nhân tại quầy của một phòng khám nhỏ. Danh sách đăng ký lưu bằng **mảng**, hàng đợi cũng cài đặt bằng **mảng** theo nguyên tắc **FIFO**. Chương trình thực hiện lần lượt 5 yêu cầu:

- Khai báo `struct Patient`, khởi tạo 5 bản ghi cố định và in danh sách gốc.
- Sắp xếp bằng **Insertion Sort** theo mức ưu tiên tăng dần; nếu bằng nhau thì theo mã tăng dần.
- **Tìm kiếm nhị phân** theo mã trên bản sao mảng đã sắp theo mã (tra mã 108 và 999).
- Cài đặt queue bằng mảng với `enqueue`/`dequeue`: đưa 5 bệnh nhân vào theo thứ tự đã sắp, lấy ra 2 người đầu, in mã được tiếp nhận và số bệnh nhân còn chờ.
- Hiển thị các phần tử còn lại trong queue theo FIFO.

## 2. Dữ liệu

Mỗi bệnh nhân gồm: mã (`int`), tên (chuỗi tối đa 39 ký tự), mức ưu tiên (`int`, số nhỏ hơn được ưu tiên hơn).

Dữ liệu cố định, khởi tạo ngay trong mã nguồn (không đọc từ tệp):

| Mã | Tên | Ưu tiên |
|---|---|---|
| 105 | An | 2 |
| 102 | Binh | 1 |
| 108 | Chi | 3 |
| 101 | Dung | 2 |
| 106 | Ha | 1 |

Quy ước sắp xếp: ưu tiên tăng dần; nếu bằng nhau thì mã tăng dần.

## 3. Cấu trúc thư mục

```
patient_queue/
├── src/
│   └── PatientQueue.cpp   Toàn bộ mã nguồn
└── readme.md              Hướng dẫn này
```

## 4. Biên dịch và chạy

Mở terminal tại thư mục gốc `PatientQueue/`:

```bash
g++ -std=c++11 -Wall -Wextra -o PatientQueue src/PatientQueue.cpp
./PatientQueue                # Windows PowerShell: .\PatientQueue.exe
```

## 5. Hướng dẫn sử dụng

Chương trình không có menu và không cần nhập dữ liệu. Khi chạy, nó tự in lần lượt kết quả của 5 yêu cầu:

| Phần in ra | Nội dung |
|---|---|
| 1 | Danh sách gốc (mã, tên, ưu tiên) |
| 2 | Danh sách sau khi sắp xếp theo ưu tiên, bằng nhau theo mã |
| 3 | Kết quả tra mã 108 và 999 |
| 4 | Mã các bệnh nhân được tiếp nhận và số bệnh nhân còn chờ |
| 5 | Hàng đợi còn lại theo thứ tự FIFO |

Kết quả mẫu:

```
=== 2. Danh sach sau khi sap xep ===
Ma      Ten     Uu tien
102     Binh    1
106     Ha      1
101     Dung    2
105     An      2
108     Chi     3

=== 3. Tim kiem nhi phan theo ma ===
Tra ma 108: Chi
Tra ma 999: KHONG TIM THAY

=== 4. Tiep nhan 2 benh nhan dau ===
Da tiep nhan ma: 102
Da tiep nhan ma: 106
So benh nhan con cho: 3

=== 5. Hang doi con lai (FIFO) ===
101     Dung    2
105     An      2
108     Chi     3
```

Toàn bộ thông báo in ra màn hình là tiếng Việt không dấu.

## 6. Thiết kế

```cpp
struct Patient { int id; char name[40]; int priority; };
struct Queue    { Patient items[5]; int front, rear, size; /* empty, full, enqueue, dequeue, display */ };
```

- **Insertion Sort:** lấy từng phần tử `key`, dịch các phần tử đứng trước lớn hơn `key` sang phải rồi chèn `key` vào chỗ trống. Một hàm dùng chung cho hai tiêu chí nhờ con trỏ hàm so sánh: `comesBeforeByPriority` (ưu tiên, rồi mã) và `comesBeforeById` (mã).
- **Tìm kiếm nhị phân:** vòng lặp, so sánh phần tử giữa `mid` với mã cần tìm rồi thu hẹp về nửa trái hoặc nửa phải. Trả về chỉ số, hoặc -1 nếu không có. Tìm trên **bản sao** đã sắp theo mã, không làm thay đổi mảng đã sắp theo ưu tiên dùng cho hàng đợi.
- **Queue bằng mảng vòng:** `front` trỏ phần tử đầu, `rear` trỏ phần tử cuối, chỉ số tăng theo `(chỉ số + 1) % 5`. Nhờ vậy `dequeue` không phải dịch phần tử.
- **Hiển thị FIFO:** duyệt từ `front` đủ `size` phần tử, nên người vào trước được in trước.

## 7. Kiểm tra và xử lý lỗi

| Tình huống | Cách xử lý |
|---|---|
| Tra mã không có trong mảng (999) | Hàm trả về -1, chương trình in "KHONG TIM THAY" |
| `enqueue` khi hàng đợi đầy | Trả về `false`, không ghi đè phần tử |
| `dequeue` khi hàng đợi rỗng | Trả về `false`, không đọc dữ liệu rác |
| Hàng đợi rỗng khi hiển thị | In "(rong)" |

## 8. Nhận xét

- **Insertion Sort:** trường hợp xấu nhất O(n²) khi mảng sắp ngược, vì mỗi phần tử phải dịch qua toàn bộ phần đã sắp xếp. Trường hợp tốt nhất O(n) khi mảng đã sắp sẵn.
- **Tìm kiếm nhị phân:** trường hợp xấu nhất O(log n) vì mỗi vòng lặp loại bỏ một nửa khoảng tìm kiếm. Với 5 phần tử, tra mã 108 và 999 đều tốn 3 lần so sánh. Yêu cầu mảng phải được sắp theo mã trước.
- **Queue bằng mảng vòng:** `enqueue` và `dequeue` đều O(1).
- **Hạn chế:** dung lượng hàng đợi cố định 5 phần tử; dữ liệu cố định trong mã nguồn; hàm gán tên dùng `strcpy` nên không kiểm tra độ dài, chỉ an toàn với tên tối đa 39 ký tự như dữ liệu mẫu.