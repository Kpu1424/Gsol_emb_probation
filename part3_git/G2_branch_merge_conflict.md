# G2 — branch, merge, xử lý conflict

**Mục tiêu:** làm việc song song trên nhánh, gộp nhánh, tự tay giải quyết conflict.
**Cần biết trước:** G1 (repo `sandbox/sensor-app` có ít nhất 2 commit).

## Phần A: merge không conflict (fast-forward)

```bash
cd part3_git/sandbox/sensor-app
git switch -c feature/readme       # tạo nhánh mới và chuyển sang
echo "# Sensor app" > README.md
git add README.md && git commit -m "Them README"
git switch main
git merge feature/readme           # "Fast-forward": main chỉ việc tiến lên
git log --oneline --graph --all
git branch -d feature/readme       # xóa nhánh đã gộp
```

## Phần B: tạo conflict có chủ đích

Hai nhánh cùng sửa **một dòng** `TEMP_OFFSET`:

```bash
git switch -c feature/calib
sed -i 's/TEMP_OFFSET 0/TEMP_OFFSET -3/' sensor.c
git commit -am "Hieu chuan offset -3"

git switch main
sed -i 's/TEMP_OFFSET 0/TEMP_OFFSET 5/' sensor.c
git commit -am "Offset 5 theo phan cung moi"

git merge feature/calib            # CONFLICT
git status                         # sensor.c: both modified
cat sensor.c
```

Output dự kiến của `cat sensor.c`:
```
<<<<<<< HEAD
#define TEMP_OFFSET 5
=======
#define TEMP_OFFSET -3
>>>>>>> feature/calib
...
```

## Phần C: giải quyết conflict

1. Mở `sensor.c` trong VS Code (có nút *Accept Current / Incoming / Both*), hoặc sửa tay.
2. Quyết định giá trị đúng (ví dụ giữ `5`), **xóa hết** các dòng `<<<<<<<`, `=======`, `>>>>>>>`.
3. Đánh dấu đã giải quyết và hoàn tất merge:

```bash
git add sensor.c
git commit                         # message mặc định "Merge branch 'feature/calib'"
git log --oneline --graph --all    # thấy 2 nhánh nhập lại
```

Muốn bỏ dở merge đang conflict: `git merge --abort`.

## Lỗi thường gặp
- Commit khi còn sót `<<<<<<<` trong file: kiểm tra bằng `grep -n '<<<<<<<' -r .` trước khi `git add`.
- Merge nhầm chiều: `git merge X` gộp X **vào nhánh hiện tại**. Luôn kiểm tra `git branch` trước.
- Chọn "Accept Both" một cách máy móc: code có 2 dòng `#define` trùng nhau, build lỗi.

## Bài tập (chưa có đáp án)
1. Tạo conflict ở **2 chỗ** trong cùng một file, giải quyết với kết quả là giá trị **mới hoàn toàn** (không phải của bên nào).
2. Tạo conflict rồi dùng `git merge --abort`. Kiểm chứng repo quay về trạng thái trước merge.
3. Dùng `git log --oneline --graph --all` và vẽ lại sơ đồ nhánh ra giấy.
4. Tìm hiểu: vì sao merge ở phần A không tạo merge commit mà phần C thì có?

## Tự kiểm tra
- [ ] Đọc được 3 phần của khối conflict: HEAD, phần ngăn cách, nhánh được merge.
- [ ] Biết `git status` trong lúc merge cho thấy những gì.
- [ ] Giải thích được fast-forward và merge commit khác nhau thế nào.
