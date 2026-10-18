# DSA – Bài thực hành Cấu trúc dữ liệu và giải thuật

Repo chứa các bài thực hành môn Cấu trúc dữ liệu và giải thuật, viết bằng C++.

| Bài | Nội dung | Thư mục |
|---|---|---|
| Cây nhị phân dạng mảng | Cây quyết định cảnh báo học vụ, lưu trong mảng, `left`/`right` là chỉ số | [TreeArrayPractice](TreeArrayPractice/) |
| Smart Service Desk | Mô phỏng tiếp nhận và phục vụ khách bằng Queue, Stack, List | [SmartServiceDesk](SmartServiceDesk/) |
| Cây nhị phân dùng con trỏ | Cùng bài cây quyết định cảnh báo học vụ, cài đặt bằng con trỏ `Node*` với `new`/`delete` | [TreePointer](TreePointer/) |
| Xét học bổng và xếp hạng | Lọc hồ sơ, xếp hạng bằng Insertion, Selection, Bubble Sort, so sánh số lần so sánh và di chuyển | [ScholarshipSorting](ScholarshipSorting/) |

Mỗi thư mục có `README.md` riêng mô tả bài toán, cách chạy.

**Biên dịch chung:**
```bash
cd tên-thư-mục
g++ -std=c++11 tên-file.cpp -o chuong_trinh
```
