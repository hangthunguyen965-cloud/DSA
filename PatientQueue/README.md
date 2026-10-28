## 1. Bài toán

Chương trình dòng lệnh mô phỏng việc tiếp nhận bệnh nhân tại quầy của một phòng khám nhỏ. Danh sách đăng ký lưu bằng **mảng**; hàng đợi cài đặt bằng **con trỏ (danh sách liên kết)** theo nguyên tắc **FIFO**. Thứ tự sắp xếp có thể **cấu hình linh hoạt** theo nhiều tiêu chí. Chương trình thực hiện lần lượt 5 yêu cầu:

- Khai báo `struct Patient`, khởi tạo 5 bản ghi cố định và in danh sách gốc.
- Sắp xếp bằng **Insertion Sort** theo thứ tự tiêu chí đã cấu hình (mặc định: ưu tiên, rồi thời gian đến, rồi tuổi; nếu bằng hết thì theo mã tăng dần).
- **Tìm kiếm nhị phân** theo mã trên bản sao mảng đã sắp theo mã (tra mã 108 và 999).
- Cài đặt queue bằng con trỏ với `enqueue`/`dequeue`: đưa 5 bệnh nhân vào theo thứ tự đã sắp, lấy ra 2 người đầu, in mã được tiếp nhận và số bệnh nhân còn chờ.
- Hiển thị các phần tử còn lại trong queue theo FIFO.

## 2. Dữ liệu

Mỗi bệnh nhân gồm: mã (`int`), tên (chuỗi tối đa 39 ký tự), mức ưu tiên (`int`, số nhỏ hơn được ưu tiên hơn), thời gian đến (`int`), tuổi (`int`).

Dữ liệu cố định, khởi tạo ngay trong mã nguồn (không đọc từ tệp):

| Mã | Tên | Ưu tiên | Thời gian đến | Tuổi |
|---|---|---|---|---|
| 105 | An | 2 | 10 | 45 |
| 102 | Binh | 1 | 30 | 30 |
| 108 | Chi | 3 | 20 | 60 |
| 101 | Dung | 2 | 5 | 25 |
| 106 | Ha | 1 | 15 | 70 |

Thứ tự sắp xếp do mảng tiêu chí `order` trong hàm `main` quyết định. Mặc định: ưu tiên, rồi thời gian đến, rồi tuổi (đều tăng dần). Nếu tất cả tiêu chí bằng nhau thì mã tăng dần.

## 5. Hướng dẫn sử dụng

Chương trình không có menu và không cần nhập dữ liệu. Khi chạy, nó tự in lần lượt kết quả của 5 yêu cầu:

| Phần in ra | Nội dung |
|---|---|
| 1 | Danh sách gốc (mã, tên, ưu tiên, thời gian đến, tuổi) |
| 2 | Danh sách sau khi sắp xếp theo thứ tự tiêu chí đã cấu hình |
| 3 | Kết quả tra mã 108 và 999 |
| 4 | Mã các bệnh nhân được tiếp nhận và số bệnh nhân còn chờ |
| 5 | Hàng đợi còn lại theo thứ tự FIFO |

Kết quả mẫu (thứ tự mặc định):

```
=== 2. Danh sach sau khi sap xep ===
Ma      Ten     Uu tien Thoi gian den   Tuoi
106     Ha      1       15              70
102     Binh    1       30              30
101     Dung    2       5               25
105     An      2       10              45
108     Chi     3       20              60

=== 3. Tim kiem nhi phan theo ma ===
Tra ma 108: Chi
Tra ma 999: KHONG TIM THAY

=== 4. Tiep nhan 2 benh nhan dau ===
Da tiep nhan ma: 106
Da tiep nhan ma: 102
So benh nhan con cho: 3

=== 5. Hang doi con lai (FIFO) ===
101     Dung    2       5               25
105     An      2       10              45
108     Chi     3       20              60
```

Đổi thứ tự sắp xếp: sửa mảng `order` trong `main`, ví dụ tuổi, rồi ưu tiên, rồi thời gian đến:

```cpp
SortCriterion order[] = { {SORT_AGE, true}, {SORT_PRIORITY, true}, {SORT_ARRIVAL_TIME, true} };
```

Toàn bộ thông báo in ra màn hình là tiếng Việt không dấu.

## 6. Thiết kế

```cpp
struct Patient { int id; char name[40]; int priority; int arrivalTime; int age; };
struct SortCriterion { SortField field; bool ascending; };
struct Node  { Patient data; Node* next; };
struct Queue { Node* front; Node* rear; int size; /* empty, enqueue, dequeue, display, clear */ };
```

- **Thứ tự sắp xếp linh hoạt:** thứ tự là một mảng `SortCriterion`, mỗi tiêu chí gồm trường (`SORT_PRIORITY`, `SORT_ARRIVAL_TIME`, `SORT_AGE`, `SORT_ID`) và chiều tăng hoặc giảm. Hàm `comesBefore` duyệt từng tiêu chí, tiêu chí đầu tiên cho giá trị khác nhau sẽ quyết định thứ tự; nếu bằng hết thì so sánh theo mã. Đổi thứ tự chỉ cần đổi mảng, không phải sửa thuật toán.
- **Insertion Sort:** lấy từng phần tử `key`, dịch các phần tử đứng trước mà `key` phải đứng trước sang phải, rồi chèn `key` vào chỗ trống. Thuật toán nhận mảng tiêu chí làm tham số nên dùng chung cho mọi thứ tự.
- **Tìm kiếm nhị phân:** vòng lặp, so sánh phần tử giữa `mid` với mã cần tìm rồi thu hẹp về nửa trái hoặc nửa phải. Trả về chỉ số, hoặc -1 nếu không có. Tìm trên **bản sao** đã sắp theo mã (tiêu chí `SORT_ID`), không làm thay đổi mảng đã sắp theo ưu tiên dùng cho hàng đợi.
- **Queue bằng con trỏ:** danh sách liên kết đơn. `enqueue` thêm nút mới vào `rear`, `dequeue` lấy nút ở `front` rồi `delete`. Hàm hủy gọi `clear` để giải phóng toàn bộ nút; lớp cấm sao chép để tránh giải phóng một nút hai lần.
- **Hiển thị FIFO:** duyệt từ `front` theo con trỏ `next`, nên người vào trước được in trước.

## 7. Kiểm tra và xử lý lỗi

| Tình huống | Cách xử lý |
|---|---|
| Tra mã không có trong mảng (999) | Hàm trả về -1, chương trình in "KHONG TIM THAY" |
| `dequeue` khi hàng đợi rỗng | Trả về `false`, không đọc dữ liệu rác |
| Hàng đợi rỗng khi hiển thị | In "(rong)" |
| Hai bệnh nhân bằng nhau ở mọi tiêu chí | So sánh theo mã tăng dần nên thứ tự luôn xác định |
| Kết thúc chương trình | Hàm hủy của `Queue` giải phóng toàn bộ nút còn lại |

## 8. Nhận xét

- **Insertion Sort:** trường hợp xấu nhất O(n²·k) với k là số tiêu chí, vì mảng sắp ngược thì mỗi phần tử phải dịch qua toàn bộ phần đã sắp và mỗi lần so sánh tốn tối đa k bước. Trường hợp tốt nhất O(n·k) khi mảng đã sắp sẵn.
- **Tìm kiếm nhị phân:** trường hợp xấu nhất O(log n) vì mỗi vòng lặp loại bỏ một nửa khoảng tìm kiếm. Với 5 phần tử, tra mã 108 và 999 đều tốn 3 lần so sánh. Yêu cầu mảng phải được sắp theo mã trước.
- **Queue bằng con trỏ:** `enqueue` và `dequeue` đều O(1); không giới hạn số phần tử nhưng mỗi nút cần cấp phát và giải phóng bộ nhớ.
- **Hạn chế:** thứ tự sắp xếp đổi bằng cách sửa mảng `order` trong mã nguồn rồi biên dịch lại, chưa chọn được lúc chạy; dữ liệu cố định trong mã nguồn; hàm gán tên dùng `strcpy` nên không kiểm tra độ dài, chỉ an toàn với tên tối đa 39 ký tự như dữ liệu mẫu.