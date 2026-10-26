# G3 — remote, push, clone, fetch, pull (mô phỏng bằng local bare repo)

**Mục tiêu:** hiểu luồng làm việc nhóm qua một repo trung tâm, khi chưa có server thật.
**Cần biết trước:** G1, G2. Đã chạy `./setup_sandbox.sh` (có `sandbox/remote/sensor-app.git`).

**Bare repository** là repo chỉ có lịch sử, không có thư mục làm việc. Nó đóng vai GitHub/GitLab.
Hai thư mục đóng vai hai người:
- `sandbox/sensor-app`: **bạn**
- `sandbox/teammate`: **đồng nghiệp**

## Các bước

```bash
cd part3_git/sandbox/sensor-app
git remote add origin ../remote/sensor-app.git
git remote -v
git push -u origin main            # -u: ghi nhớ main <-> origin/main

# Đồng nghiệp clone về
cd ..
git clone remote/sensor-app.git teammate
cd teammate
git config user.name "Dong Nghiep"; git config user.email "dn@example.com"
echo "v1" > VERSION
git add VERSION && git commit -m "Them VERSION"
git push

# Quay lại máy bạn
cd ../sensor-app
git fetch                          # TẢI lịch sử mới về origin/main, CHƯA đổi file của bạn
git status                         # "Your branch is behind 'origin/main' by 1 commit"
git log --oneline main..origin/main
git pull                           # = fetch + merge vào nhánh hiện tại
ls                                 # đã có VERSION
```

## Tình huống: push bị từ chối

```bash
# Đồng nghiệp push trước
cd ../teammate && echo note > NOTES && git add NOTES && git commit -m "Them NOTES" && git push
# Bạn commit rồi push sau
cd ../sensor-app && echo x > LOCAL && git add LOCAL && git commit -m "Them LOCAL"
git push                           # rejected (fetch first / non-fast-forward)
git pull --no-rebase               # gộp thay đổi của đồng nghiệp
git push                           # thành công
```

Output dự kiến khi bị từ chối:
```
 ! [rejected]        main -> main (fetch first)
error: failed to push some refs to '../remote/sensor-app.git'
```

## Lỗi thường gặp
- `git push --force` để "sửa" lỗi rejected: **ghi đè** công việc của người khác. Không dùng khi đang học.
- Nhầm `fetch` với `pull`: fetch chỉ tải về, pull tải về **và** gộp.
- Push vào một repo **không bare** đang được checkout: git từ chối. Vì vậy server luôn là bare repo.

## Bài tập (chưa có đáp án)
1. Bạn và đồng nghiệp cùng sửa **một dòng** của `sensor.c` rồi cùng push. Xử lý conflict ở phía người push sau.
2. Tạo nhánh `feature/log` ở máy bạn, push nhánh đó lên origin. Từ `teammate`, lấy nhánh đó về và checkout.
3. So sánh `git pull --no-rebase` với `git pull --rebase` trên cùng một tình huống. Vẽ lịch sử của từng cách.

## Tự kiểm tra
- [ ] Giải thích được `origin`, `origin/main` và `main` là ba thứ khác nhau.
- [ ] Biết đọc "ahead / behind" trong `git status`.
- [ ] Biết vì sao server dùng bare repo.
