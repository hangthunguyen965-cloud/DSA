# DSA – Bài thực hành Cấu trúc dữ liệu và giải thuật

Repo chứa các bài thực hành môn Cấu trúc dữ liệu và giải thuật, viết bằng C++.

| Bài | Nội dung | Thư mục |
|---|---|---|
| Cây nhị phân dạng mảng | Cây quyết định cảnh báo học vụ, lưu trong mảng, `left`/`right` là chỉ số | [TreeArrayPractice](TreeArrayPractice/) |
| Smart Service Desk | Mô phỏng tiếp nhận và phục vụ khách bằng Queue, Stack, List | [SmartServiceDesk](SmartServiceDesk/) |
| Cây nhị phân dùng con trỏ | Cùng bài cây quyết định cảnh báo học vụ, cài đặt bằng con trỏ `Node*` với `new`/`delete` | [TreePointer](TreePointer/) |
| Xét học bổng và xếp hạng | Lọc hồ sơ, xếp hạng bằng Insertion, Selection, Bubble Sort, Quick sort so sánh số lần so sánh và di chuyển | [ScholarshipSorting](ScholarshipSorting/) |
| Tra cứu sản phẩm theo mã | Các thuật toán tìm kiếm và sắp xếp tự cài đặt, không dùng hàm tìm kiếm hay sắp xếp có sẵn của thư viện | [ProductSearch](ProductSearch/) |
| Cây nhị phân tìm kiếm | Chương trình dòng lệnh quản lý danh mục sản phẩm của một cửa hàng nhỏ bằng **cây nhị phân tìm kiếm (BST)** | [BinarySearchTree](BinarySearchTree/) |
| Quản lý danh sách bệnh nhân đã đăng ký tại phòng khám | Chương trình dòng lệnh mô phỏng việc tiếp nhận bệnh nhân tại quầy của một phòng khám nhỏ. Danh sách đăng ký lưu bằng **mảng**, hàng đợi cũng cài đặt bằng **mảng** theo nguyên tắc **FIFO**.  | [PatientQueue](PatientQueue/) |

Mỗi thư mục có `README.md` riêng mô tả bài toán, cách chạy.

**Biên dịch chung:**
```bash
cd tên-thư-mục
g++ -std=c++11 tên-file.cpp -o chuong_trinh
```
