# G1 — init, status, add, commit, diff, log

**Mục tiêu:** tạo repo, theo dõi file, hiểu 3 vùng working tree / staging / repository.
**Cần biết trước:** lệnh shell cơ bản (`cd`, `ls`, `cat`, `echo`).

## Các bước (gõ từng lệnh, đọc output sau mỗi lệnh)

```bash
cd part3_git/sandbox
git init -b main sensor-app          # tạo repo, nhánh mặc định tên main
cd sensor-app

# Danh tính CHỈ cho repo này (không --global)
git config user.name  "Ten Cua Ban"
git config user.email "email-cua-ban@example.com"

cat > sensor.c <<'EOF'
#define TEMP_OFFSET 0
int read_temp(void) { return 250 + TEMP_OFFSET; }
EOF

git status                 # sensor.c: Untracked
git add sensor.c
git status                 # sensor.c: Changes to be committed (đã stage)
git commit -m "Them sensor.c"

echo '/* doc nhiet do */' >> sensor.c
git diff                   # khác biệt: working tree  vs  staging
git add sensor.c
git diff                   # rỗng: không còn gì chưa stage
git diff --staged          # khác biệt: staging  vs  commit cuối
git commit -m "Them chu thich"

git log --oneline          # 2 commit, mới nhất ở trên
git log -p -1              # nội dung thay đổi của commit cuối
```

## Output dự kiến (rút gọn; mã hash khác trên máy bạn)

```
$ git status --short        (sau khi tạo file)
?? sensor.c
$ git status --short        (sau git add)
A  sensor.c
$ git log --oneline
b1c2d3e (HEAD -> main) Them chu thich
a0b1c2d Them sensor.c
```

## Lỗi thường gặp
- `Author identity unknown`: chưa đặt `user.name`/`user.email` cho repo.
- `git commit` mở trình soạn thảo lạ (vim): dùng `-m "..."`, hoặc thoát vim bằng `Esc` rồi `:wq`.
- Sửa file xong `git commit` luôn mà quên `git add`: commit không có thay đổi đó (`git status` sẽ cho thấy).
- Chạy `git init` nhầm trong thư mục cha (ví dụ cả `emb_probation`): luôn kiểm tra `pwd` trước.

## Bài tập (chưa có đáp án)
1. Tạo file `sensor.h` chứa prototype `int read_temp(void);`, commit riêng.
2. Sửa **2 file** cùng lúc nhưng chỉ commit **1 file**. Dùng `git status` chứng minh file kia vẫn chưa commit.
3. Tạo file `.gitignore` để git bỏ qua mọi file `*.o` và file thực thi `sensor_app`. Kiểm chứng bằng `git status`.
4. Dùng `git log` với một tùy chọn để xem **file nào** thay đổi trong mỗi commit (gợi ý: tìm trong `git log --help`).

## Tự kiểm tra
- [ ] Giải thích được `git diff` và `git diff --staged` so sánh hai vùng nào.
- [ ] Nhìn `git status --short`, biết ý nghĩa `??`, `A `, ` M`, `M `.
- [ ] Biết vì sao nên commit nhỏ, mỗi commit một ý.
