# Bài thực hành: Cây nhị phân dùng con trỏ – Cây quyết định cảnh báo học vụ

- **Môn:** Cấu trúc dữ liệu và giải thuật.
- **Ngôn ngữ:** C++ – **Tệp mã nguồn:** `TreeArrayPointer.cpp`

## 1. Bài toán

Chương trình mô phỏng cây quyết định giúp phân loại nhanh tình trạng học vụ của sinh viên. Nút trong lưu câu hỏi Có/Không, nút lá lưu kết luận. Nhánh trái là **Có**, nhánh phải là **Không**. Chức năng và dữ liệu giống bản cài đặt bằng mảng, chỉ khác cách lưu cây: **dùng con trỏ thay cho chỉ số**.

## 2. Cây quyết định mẫu

```
              1: DTB < 2.0?
             /             \
   2: No >= 12 tin chi?   3: DTB >= 3.2?
     /        \             /        \
 4: CB muc 2  5: CB muc 1  6: Khen    7: Theo doi
                              thuong     binh thuong
```

## 3. Biểu diễn cây bằng con trỏ

Mỗi nút được cấp phát bằng `new`. Hai con trỏ `left`, `right` lưu **địa chỉ** của nút con, `nullptr` nghĩa là không có con. Biến `root` trỏ tới nút gốc.

| id | Nội dung | left | right | isLeaf |
|---|---|---|---|---|
| 1 | DTB < 2.0? | nút 2 | nút 3 | 0 |
| 2 | No >= 12 tin chi? | nút 4 | nút 5 | 0 |
| 3 | DTB >= 3.2? | nút 6 | nút 7 | 0 |
| 4 | Canh bao hoc vu muc 2 | nullptr | nullptr | 1 |
| 5 | Canh bao hoc vu muc 1 | nullptr | nullptr | 1 |
| 6 | De xuat khen thuong | nullptr | nullptr | 1 |
| 7 | Theo doi binh thuong | nullptr | nullptr | 1 |

Chương trình có chức năng in bảng này kèm **địa chỉ thật** của từng nút để thấy rõ quan hệ con trỏ.

## 4. Cấu trúc dữ liệu

```cpp
struct Node {
    int id;
    string content;    // câu hỏi hoặc kết luận
    Node* left;        // con trỏ tới nút "Có", nullptr nếu không có
    Node* right;       // con trỏ tới nút "Không", nullptr nếu không có
    bool isLeaf;       // true: kết luận, false: câu hỏi
};

Node* root = nullptr;
int n = 0;             // số nút hiện có
```

## 5. Các hàm chính

| Nhóm | Hàm | Mô tả |
|---|---|---|
| Quản lý bộ nhớ | `taoNut()` | Cấp phát một nút bằng `new`, tăng `n`. |
| | `giaiPhong()` | Duyệt hậu tự, `delete` con trước rồi mới `delete` cha. |
| | `resetCay()` | Giải phóng toàn bộ cây trước khi nạp hoặc nhập cây mới, và khi thoát. |
| Tạo cây | `loadSampleTree()` | Dựng cây mẫu 7 nút từ lá lên gốc. |
| | `nhapCay()` | Nhập thủ công, dùng mảng phụ tạm thời để nối con trỏ, nhập xong thì kiểm tra. |
| Kiểm tra | `kiemTraCay()` | Phát hiện nút có nhiều hơn 1 cha hoặc tạo vòng lặp, id trùng, nút lá có con, nút câu hỏi thiếu nhánh, nút không nối được từ gốc. |
| Hiển thị | `hienThiBang()` | In id, nội dung, id con trái/phải và địa chỉ nút. |
| Duyệt | `preorder / inorder / postorder` | Duyệt tiền tự, trung tự, hậu tự bằng đệ quy, điều kiện dừng `p == nullptr`. |
| Tìm kiếm | `findPath()`, `timNut()` | Tìm nút theo `id`, lưu đường đi bằng quay lui, báo lỗi nếu không tồn tại. |
| Thống kê | `height()`, `countLeaves()` | Chiều cao theo số mức (cây mẫu = 3) và số nút lá (cây mẫu = 4). |
| Ứng dụng | `moPhongTuVan()` | Hỏi `y/n`, `y` đi sang `left`, `n` đi sang `right`, in kết luận và đường đi giải thích. |

Nhập liệu được bảo vệ bởi `docSo()`, `docYN()` và `daCoCay()`.

## 6. Cách biên dịch và chạy

```bash
g++ -std=c++11 TreeArrayPointer.cpp -o tree_ptr
./tree_ptr            # Windows PowerShell: .\tree_ptr.exe
```

**Ví dụ mô phỏng tư vấn:** chọn 1 nạp cây mẫu, chọn 7, trả lời `DTB < 2.0?` → `n`, `DTB >= 3.2?` → `y`.
Kết quả: `De xuat khen thuong`, đường đi `1 -> 3 -> 6`.

## 7. Nhận xét

**Ưu điểm**
- Không phải khai báo trước kích thước cây, cấp phát từng nút khi cần.
- Thêm hoặc xóa nút chỉ cần đổi con trỏ, không phải sắp xếp lại chỉ số.
- Không lãng phí ô trống như mảng.

**Hạn chế**
- Phải tự giải phóng bộ nhớ (`delete`), quên thì rò rỉ bộ nhớ.
- Dễ gặp lỗi con trỏ: truy cập `nullptr`, con trỏ treo, nút có hai cha.
- Khó gỡ lỗi hơn mảng vì không nhìn thấy cây dưới dạng bảng chỉ số.
- Mỗi nút tốn thêm bộ nhớ cho hai con trỏ.
- Chức năng nhập tay vẫn giới hạn tối đa `MAX - 1` nút do dùng mảng phụ.

**So sánh nhanh**

| | Mảng | Con trỏ |
|---|---|---|
| Liên kết con | chỉ số `left`, `right` | địa chỉ `left`, `right` |
| Không có con | `0` | `nullptr` |
| Kích thước | cố định `MAX` | tùy ý, cấp phát động |
| Giải phóng | không cần | bắt buộc `delete` |
| Rủi ro | chỉ số ngoài mảng | con trỏ sai, rò rỉ bộ nhớ |