# Bài thực hành: Cây nhị phân dạng mảng – Cây quyết định cảnh báo học vụ

- **Môn:** Cấu trúc dữ liệu và giải thuật 
- **Ngôn ngữ:** C++ (không dùng con trỏ động, không `new`/`malloc`)
- **Tệp mã nguồn:** `TreeArrayPractice.cpp`

## 1. Bài toán

Phòng đào tạo cần một chương trình hỗ trợ phân loại nhanh tình trạng học vụ của sinh viên. Cán bộ trả lời lần lượt các câu hỏi dạng Có/Không, chương trình đi theo nhánh tương ứng cho đến khi gặp kết luận cuối cùng.

- **Nút trong (nút quyết định):** lưu một câu hỏi kiểm tra điều kiện học vụ.
- **Nút lá:** lưu kết luận, ví dụ "Cảnh báo học vụ mức 1", "Đề xuất khen thưởng".
- **Nhánh trái:** lựa chọn **Có**. **Nhánh phải:** lựa chọn **Không**.

## 2. Cây quyết định mẫu

Cây mẫu gồm 7 nút: 3 nút quyết định và 4 nút kết luận.

```
              1: DTB < 2.0?
             /             \
          Có                 Không
         /                     \
 2: No >= 12 tin chi?       3: DTB >= 3.2?
     /        \                /        \
   Có        Không           Có        Không
   /            \            /            \
4: Canh bao  5: Canh bao  6: De xuat    7: Theo doi
   muc 2        muc 1       khen thuong    binh thuong
```

## 3. Biểu diễn cây bằng mảng

Thay vì cấp phát nút bằng con trỏ, chương trình dùng một mảng `Tree[MAX]`. Mỗi phần tử mảng là một nút, và hai trường `left`, `right` lưu **chỉ số** của nút con trái và nút con phải trong chính mảng đó.

| Index | Loại nút | Nội dung | left | right | isLeaf |
|---|---|---|---|---|---|
| 1 | Câu hỏi | DTB < 2.0? | 2 | 3 | 0 |
| 2 | Câu hỏi | No >= 12 tin chi? | 4 | 5 | 0 |
| 3 | Câu hỏi | DTB >= 3.2? | 6 | 7 | 0 |
| 4 | Kết luận | Canh bao hoc vu muc 2 | 0 | 0 | 1 |
| 5 | Kết luận | Canh bao hoc vu muc 1 | 0 | 0 | 1 |
| 6 | Kết luận | De xuat khen thuong | 0 | 0 | 1 |
| 7 | Kết luận | Theo doi binh thuong | 0 | 0 | 1 |

**Quy ước:**
- Gốc của cây ở chỉ số **1** (`root = 1`).
- Chỉ số **0** nghĩa là không có nút con (`EMPTY = 0`), vì vậy ô `Tree[0]` được bỏ trống.
- Các ô chưa dùng có `left = right = 0` (mảng toàn cục nên tự khởi tạo bằng 0).

## 4. Cấu trúc dữ liệu

```cpp
const int MAX = 100;
const int EMPTY = 0;

struct Node {
    int id;            // mã nút
    string content;    // câu hỏi hoặc kết luận
    int left;          // chỉ số nút con trái (trả lời "Có")
    int right;         // chỉ số nút con phải (trả lời "Không")
    bool isLeaf;       // true: nút kết luận, false: nút câu hỏi
};

Node Tree[MAX];
int root = 1;
int n = 0;             // số nút đang dùng
```

- `Tree[i].left = j` nghĩa là nút con trái của nút `i` nằm ở `Tree[j]`.
- Quan hệ cha – con được xác định hoàn toàn bằng chỉ số, không cần con trỏ động.
- Trường `isLeaf` giúp phân biệt câu hỏi với kết luận khi mô phỏng tư vấn.

## 5. Các hàm chính

| Hàm | Mô tả | Độ phức tạp |
|---|---|---|
| `loadSampleTree()` | Nạp cây mẫu 7 nút vào mảng. | O(n) |
| `nhapCay()` | Nhập cây thủ công: id, nội dung, left, right, isLeaf, có kiểm tra giá trị nhập. | O(n) |
| `kiemTraCay()` | Kiểm tra chỉ số con nằm trong `[0, n]`, nút lá không có con, nút câu hỏi đủ 2 nhánh, mỗi nút (trừ gốc) có đúng 1 cha. | O(n) |
| `hienThiBang()` | In bảng mảng gồm index, id, nội dung, left, right, isLeaf. | O(n) |
| `preorder / inorder / postorder` | Duyệt tiền tự, trung tự, hậu tự bằng đệ quy, tham số là chỉ số mảng, điều kiện dừng `i == 0`. | O(n) |
| `findPath()` | Tìm nút theo `id` và lưu đường đi từ gốc vào mảng `path[]`. Dùng quay lui: nếu nhánh không chứa nút cần tìm thì giảm `len` để bỏ nút hiện tại khỏi đường đi. | O(n) |
| `timNut()` | Tìm theo chỉ số hoặc theo id, in nút tìm được và đường đi, báo lỗi nếu không tồn tại. | O(n) |
| `height()` | Tính chiều cao cây theo **số mức** (cây mẫu có chiều cao 3). | O(n) |
| `countLeaves()` | Đếm nút lá (cây mẫu có 4 nút lá: 4, 5, 6, 7). | O(n) |
| `moPhongTuVan()` | Bắt đầu từ gốc, hỏi người dùng `y/n`: `y` đi sang `left`, `n` đi sang `right`, dừng khi gặp nút lá rồi in kết luận và đường đi giải thích. | O(h) |

**Xử lý lỗi:**
- `docSo()` chặn nhập sai kiểu hoặc ngoài khoảng cho phép.
- `docYN()` chỉ chấp nhận `y` hoặc `n`.
- `daCoCay()` báo lỗi thân thiện khi chưa khởi tạo cây.
- `kiemTraCay()` phát hiện chỉ số trỏ ra ngoài mảng, nút trỏ về chính nó, nút có nhiều hơn một cha.

## 6. Cách biên dịch và chạy

```bash
g++ -std=c++11 TreeArrayPractice.cpp -o tree
./tree            # Windows PowerShell: .\tree.exe
```

Menu chương trình:

| Lựa chọn | Chức năng |
|---|---|
| 1 | Khởi tạo cây mẫu |
| 2 | Nhập cây thủ công |
| 3 | Hiển thị bảng mảng |
| 4 | Duyệt tiền tự / trung tự / hậu tự |
| 5 | Tìm nút và in đường đi |
| 6 | Tính chiều cao và đếm nút lá |
| 7 | Mô phỏng tư vấn học vụ |
| 8 | Kiểm tra cây hợp lệ |
| 0 | Thoát |

**Ví dụ chạy mô phỏng tư vấn:** chọn 1 để nạp cây mẫu, chọn 7 rồi trả lời lần lượt:

- `DTB < 2.0?` → `n` (đi sang nút 3)
- `DTB >= 3.2?` → `y` (đi sang nút 6)

Kết quả: `Ket luan: De xuat khen thuong`, đường đi giải thích: `1 -> 3 -> 6`.

## 7. Nhận xét

**Ưu điểm**
- Dễ hình dung quan hệ cha – con vì chỉ cần nhìn bảng chỉ số.
- Truy cập một nút trong thời gian hằng số qua `Tree[i]`.
- Không cấp phát động nên không lo rò rỉ bộ nhớ, dễ kiểm soát và gỡ lỗi.
- Phù hợp với cây có dữ liệu cố định như cây quyết định mẫu.

**Hạn chế**
- Kích thước tối đa phải xác định trước (`MAX`), cây lớn hơn thì không chứa được.
- Thêm hoặc xóa nút phải sửa chỉ số `left`/`right` thủ công và dễ sai.
- Xóa nút để lại ô trống trong mảng, gây lãng phí nếu không tái sử dụng.
- Không tự đảm bảo cây hợp lệ, nên phải có hàm `kiemTraCay()` để phát hiện lỗi liên kết.

**Hướng mở rộng:** đọc dữ liệu cây từ file văn bản, cập nhật nội dung nút hoặc đổi liên kết `left`/`right`, so sánh với cách cài đặt bằng con trỏ.