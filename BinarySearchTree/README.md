# Bài thực hành: Quản lý danh mục sản phẩm bằng cây nhị phân tìm kiếm (BST)
- **Môn:** Cấu trúc dữ liệu và giải thuật
- **Ngôn ngữ:** C++11. Cây nhị phân tìm kiếm tự cài đặt, không dùng thư viện cấu trúc dữ liệu BST có sẵn.

## 1. Bài toán

Chương trình dòng lệnh quản lý danh mục sản phẩm của một cửa hàng nhỏ bằng **cây nhị phân tìm kiếm (BST)**. Mỗi nút lưu toàn bộ thông tin một sản phẩm, khóa định vị là **mã sản phẩm**. Chương trình hỗ trợ:

- Thêm sản phẩm (kiểm tra dữ liệu và khóa trùng).
- Tìm sản phẩm theo mã.
- Xóa sản phẩm theo mã (đủ ba trường hợp: nút lá, một con, hai con).
- In danh sách theo mã tăng dần (duyệt LNR).
- Đếm số nút và tính chiều cao cây.
- Nạp dữ liệu mẫu từ tệp, và giải phóng toàn bộ nút trước khi thoát.

## 2. Định dạng tệp dữ liệu

```
50|But bi|120|5000
30|Vo o ly|80|12000
...
```

- Mỗi dòng là một sản phẩm, gồm 4 trường ngăn bằng dấu `|`: `Mã SP | Tên SP | Số lượng | Đơn giá`.
- Mã sản phẩm là **khóa**: số nguyên dương, không được trùng. Số lượng là số nguyên không âm. Đơn giá là số không âm. Tên không rỗng, tối đa 49 ký tự.
- Tệp không có dòng tiêu đề và không có dòng ghi số lượng sản phẩm.
- Tệp mẫu là `data/sample_data.txt` (7 sản phẩm, đúng thứ tự chèn của đề). Chương trình luôn đọc tệp này khi chọn mục 6, không nhận tên tệp từ dòng lệnh.

## 3. Cấu trúc thư mục

```
BinarySearchTree/
├── src/
│   └── BinarySearchTree.cpp   Toàn bộ mã nguồn
├── data/
│   └── sample_data.txt          Dữ liệu mẫu (7 sản phẩm)
├── evidence/                    Ảnh minh chứng kiểm thử
├── report.pdf                   Báo cáo
└── readme.md                    Hướng dẫn này
```

## 4. Biên dịch và chạy

Mở terminal tại thư mục gốc `BinarySearchTree/`:

```bash
g++ -std=c++11 -Wall -Wextra -o BinarySearchTree src/BinarySearchTree.cpp
./binary_search_tree                # Windows PowerShell: .\BinarySearchTree.exe
```

Phải chạy từ thư mục gốc để chương trình tìm thấy `data/sample_data.txt`.

## 5. Hướng dẫn sử dụng

| Mục | Chức năng |
|---|---|
| 1 | Thêm sản phẩm (nhập mã, tên, số lượng, đơn giá) |
| 2 | Tìm sản phẩm theo mã |
| 3 | Xóa sản phẩm theo mã |
| 4 | In danh sách theo mã tăng dần (LNR) |
| 5 | Đếm số nút và tính chiều cao |
| 6 | Nạp dữ liệu mẫu từ tệp |
| 0 | Thoát (giải phóng toàn bộ nút) |

Luồng thường dùng: **6 → 4 → 5 → 2 → 3 → 4**. Menu lặp lại sau mỗi thao tác cho đến khi chọn 0.

Kết quả tìm kiếm là một bảng gồm mã, tên, số lượng, đơn giá của sản phẩm; nếu không có thì chương trình báo "Khong tim thay san pham". Toàn bộ thông báo in ra màn hình là tiếng Việt không dấu.

## 6. Thiết kế

```cpp
struct Product { int productId; std::string name; int quantity; double unitPrice; };
struct Node    { Product product; Node* left; Node* right; };
class BinarySearchTree { /* insert, search, remove, printInorder, countNodes, height, clear */ };
```

- Bất biến BST: mọi mã ở cây con trái nhỏ hơn mã của nút, mọi mã ở cây con phải lớn hơn.
- **Chèn:** đệ quy, nhận tham chiếu tới con trỏ để gán nút mới trực tiếp. Mã trùng thì không tạo nút. Cấp phát bằng `new (std::nothrow)` và kiểm tra kết quả.
- **Tìm:** vòng lặp, đi trái hoặc phải theo so sánh mã.
- **Xóa:** nút lá thì xóa và gán `nullptr`; nút một con thì thay bằng con đó; nút hai con thì chép dữ liệu của **nút kế tiếp trung thứ tự** (nút nhỏ nhất của cây con phải) lên nút cần xóa, rồi xóa nút kế tiếp.
- **Duyệt LNR:** trái, nút, phải, nên danh sách luôn tăng dần theo mã.
- **Chiều cao:** tính theo số nút trên đường dài nhất; cây rỗng bằng 0.
- **Giải phóng:** duyệt hậu thứ tự (con trước, cha sau). Lớp cây cấm sao chép để tránh giải phóng một nút hai lần.

## 7. Kiểm tra và xử lý lỗi dữ liệu

Khi nạp tệp, chương trình báo lỗi kèm **số dòng** và bỏ qua dòng lỗi thay vì dừng:

| Tình huống | Cách xử lý |
|---|---|
| Không mở được tệp | Báo "Khong mo duoc tep", cây giữ nguyên |
| Dòng không đủ 4 trường, hoặc số lượng/đơn giá/mã không phải số | Báo "Dong sai dinh dang", bỏ qua dòng đó |
| Mã không dương, tên rỗng hoặc quá 49 ký tự, số lượng hoặc đơn giá âm | Báo lỗi dòng, bỏ qua dòng đó |
| **Trùng mã sản phẩm** | Báo "Ma san pham da ton tai", giữ bản ghi có trước |
| Dòng trống | Bỏ qua |

Chương trình còn xử lý: tệp xuống dòng kiểu Windows, khoảng trắng thừa đầu cuối, cây rỗng (in "Danh sach rong", xóa hoặc tìm đều báo không thấy, thống kê là 0 và 0), nhập sai ở menu hoặc khi nhập sản phẩm, và hết dữ liệu nhập (thoát an toàn, vẫn giải phóng bộ nhớ).

## 8. Nhận xét

- **Độ phức tạp:** chèn, tìm và xóa đều tốn o(h), với h là chiều cao cây. Cây cân đối có h xấp xỉ log₂n nên là o(log n). Với 7 nút của dữ liệu mẫu, chiều cao 3 là mức thấp nhất có thể.
- **Cây suy biến:** nếu chèn mã theo thứ tự tăng dần hoặc giảm dần thì cây thành một nhánh, h = n và các thao tác thành O(n), giống danh sách liên kết. Chương trình không tự cân bằng, nên hiệu năng phụ thuộc thứ tự chèn.
- **Xóa nút hai con** dùng nút kế tiếp trung thứ tự vì nút này lớn hơn mọi mã ở cây con trái và nhỏ hơn mọi mã còn lại ở cây con phải, nên bất biến BST được giữ nguyên.
- **Hạn chế:** các hàm chèn, xóa, duyệt dùng đệ quy, nên cây rất lệch và rất lớn có thể làm tràn ngăn xếp; chỉ nạp dữ liệu từ một tệp cố định; không có chức năng sửa thông tin sản phẩm.