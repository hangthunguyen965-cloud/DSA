# Bài thực hành: Tra cứu sản phẩm theo mã (tìm kiếm tuần tự và nhị phân)
- **Môn:** Cấu trúc dữ liệu và giải thuật 
- **Ngôn ngữ:** C++11. Các thuật toán tìm kiếm và sắp xếp tự cài đặt, không dùng hàm tìm kiếm hay sắp xếp có sẵn của thư viện.

## 1. Bài toán

Chương trình đọc danh sách sản phẩm từ tệp văn bản, cho phép tra cứu sản phẩm theo **mã sản phẩm** bằng hai cách rồi so sánh chi phí:

- **Tìm kiếm tuần tự** trên mảng gốc, giữ đúng thứ tự nhập hàng.
- **Tìm kiếm nhị phân** trên một **bản sao** đã sắp xếp tăng dần theo mã.

Chương trình đếm số lần so sánh khóa của mỗi cách và đo cả **chi phí tiền xử lý** (sắp xếp bản sao) để biết khi nào nhị phân thật sự có lợi.

## 2. Định dạng tệp dữ liệu

```
12
SP108|Chuot_khong_day|185000|26
SP103|Ban_phim_co|750000|8
...
```

- Dòng đầu là số lượng sản phẩm **N**.
- Mỗi dòng tiếp theo có 4 trường ngăn bằng dấu `|`: `Mã SP | Tên SP | Đơn giá | Số lượng`.
- Mã sản phẩm là **khóa**, không được trùng. Đơn giá và số lượng là số nguyên không âm.
- Đề đặt tên tệp là `san_pham.txt`. Tệp mẫu trong bài này là `san_pham_1.txt` (12 sản phẩm).

## 3. Cấu trúc thư mục

```
ProductSearch/
├── ProductSearch.cpp   Toàn bộ mã nguồn
├── san_pham_1.txt      Dữ liệu mẫu
└── README.md
```

## 4. Biên dịch và chạy

```bash
cd ProductSearch
g++ -std=c++11 ProductSearch.cpp -o product_search
./product_search san_pham_1.txt        # Windows PowerShell: .\product_search.exe san_pham_1.txt
```

Tên tệp truyền vào dòng lệnh được dùng làm tên mặc định, ở menu mục 1 chỉ cần nhập `-`.

## 5. Hướng dẫn sử dụng

| Mục | Chức năng |
|---|---|
| 1 | Đọc tệp, kiểm tra dữ liệu và in danh sách theo thứ tự nhập hàng |
| 2 | Tra cứu tuần tự trên mảng gốc |
| 3 | Sắp xếp bản sao (Insertion Sort) và in, kèm chi phí tiền xử lý |
| 4 | Tra cứu nhị phân trên bản sao đã sắp xếp (tự sắp xếp nếu chưa làm) |
| 5 | Bảng so sánh hai giải thuật trên 6 truy vấn |
| 0 | Thoát |

Luồng thường dùng: **1 → 2 → 3 → 4 → 5**. Nhập mã sản phẩm phân biệt chữ hoa, chữ thường (ví dụ `SP103`).

Kết quả mỗi lần tra cứu gồm: có tìm thấy hay không, thông tin sản phẩm (mã, tên, đơn giá, số lượng), **vị trí** (tính từ 1) trong mảng đang tìm và **số lần so sánh khóa**.

## 6. Thiết kế

```cpp
struct Product      { string id, name; long price; int quantity; };
struct SearchResult { bool found; int position; long comparisons; };
```

- `original[]`: mảng gốc, đúng thứ tự nhập hàng, dùng cho tìm tuần tự. Mảng này **không bị thay đổi** khi sắp xếp.
- `sortedCopy[]`: bản sao được sắp xếp bằng Insertion Sort tự cài, dùng cho tìm nhị phân.
- **Quy ước đếm:** mỗi lần so sánh mã sản phẩm với khóa cần tìm tính **1 lần**. Tìm tuần tự so từng phần tử, tìm nhị phân so một lần ở mỗi vòng lặp (so sánh ba chiều: bằng, nhỏ hơn, lớn hơn).
- **Chi phí sắp xếp** đếm riêng: số lần so sánh và số phép di chuyển phần tử.

## 7. Kiểm tra và xử lý lỗi dữ liệu

Chương trình báo lỗi kèm **số dòng** và bỏ qua dòng lỗi thay vì dừng:

| Tình huống | Cách xử lý |
|---|---|
| Không mở được tệp, tệp rỗng | Báo lỗi, không nạp dữ liệu |
| Dòng đầu không phải số N hợp lệ | Báo lỗi, không nạp dữ liệu |
| Sai số trường, mã hoặc tên rỗng | Báo lỗi dòng, bỏ qua dòng đó |
| Đơn giá hoặc số lượng không phải số, hoặc âm | Báo lỗi dòng, bỏ qua dòng đó |
| **Trùng mã sản phẩm** | Báo lỗi dòng, giữ bản ghi xuất hiện trước |
| Ít dòng hơn N | Cảnh báo, dùng số dòng thực có |
| Nhiều dòng hơn N | Cảnh báo, bỏ qua phần dư |

Chương trình còn xử lý tệp có BOM của UTF-8, xuống dòng kiểu Windows, dòng trống, danh sách rỗng và nhập sai ở menu.

## 8. Kết quả với dữ liệu mẫu (12 sản phẩm)

**Chi phí tiền xử lý (Insertion Sort):** 40 lần so sánh, 53 phép di chuyển.

**Tra cứu từng mã:**

| Mã | Tuần tự | Nhị phân |
|---|---|---|
| `SP103` (có) | vị trí 2, 2 lần so sánh | vị trí 3, 2 lần so sánh |
| `SP114` (có) | – | vị trí 12, 4 lần so sánh |
| `SP104` (không có) | 12 lần so sánh | 3 lần so sánh |

**Bảng so sánh 6 truy vấn (số lần so sánh):**

| Truy vấn | Kết quả | Tuần tự | Nhị phân |
|---|---|---|---|
| SP108 (đầu mảng gốc) | Có | 1 | 3 |
| SP114 (giữa mảng gốc) | Có | 7 | 4 |
| SP109 (cuối mảng gốc) | Có | 12 | 4 |
| SP101 (nhỏ nhất) | Có | 4 | 3 |
| SP000 | Không có | 12 | 3 |
| SP114X | Không có | 12 | 4 |
| **Tổng** | | **48** | **21** |

## 9. Nhận xét

- **Tuần tự:** đơn giản, không cần sắp xếp trước, độ phức tạp O(n). Nhanh khi khóa nằm gần đầu mảng (SP108 chỉ mất 1 lần so sánh, ít hơn nhị phân), nhưng khi **không tìm thấy** thì luôn phải duyệt hết n phần tử.
- **Nhị phân:** mỗi vòng loại một nửa vùng tìm, độ phức tạp O(log n), nên ổn định ở mức 3 đến 4 lần so sánh cho cả truy vấn không có trong dữ liệu. Đổi lại cần dữ liệu đã sắp xếp.
- **Chi phí tiền xử lý:** sắp xếp bản sao tốn 40 lần so sánh (Insertion Sort, O(n²)) và thêm O(n) bộ nhớ. Với 6 truy vấn, tổng chi phí nhị phân là 21 + 40 = 61, vẫn cao hơn tuần tự (48). Trung bình mỗi truy vấn tuần tự tốn 8,0 lần so sánh, nhị phân tốn 3,5, nên nhị phân bắt đầu có lợi từ khoảng **9 truy vấn** trở lên (tính theo số lần so sánh).
- **Lựa chọn:** danh sách nhỏ hoặc chỉ tra cứu vài lần thì dùng tìm tuần tự. Danh sách lớn hoặc tra cứu nhiều lần liên tiếp thì nên sắp xếp một lần rồi dùng tìm nhị phân.
- Bản sao giúp **giữ nguyên thứ tự nhập hàng** của mảng gốc, đúng yêu cầu bài toán.