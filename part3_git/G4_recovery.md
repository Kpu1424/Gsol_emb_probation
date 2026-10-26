# G4 — Phục hồi thay đổi: restore, revert, reset, stash, reflog

**Mục tiêu:** biết chọn đúng lệnh để "quay lại" tùy theo thay đổi đang nằm ở đâu và đã chia sẻ cho người khác chưa.
**Cần biết trước:** G1–G3. Làm trong `sandbox/sensor-app`.

## Bảng chọn lệnh

| Tình huống | Lệnh | Mất dữ liệu? |
|---|---|---|
| Sửa file, chưa add, muốn bỏ | `git restore <file>` | **Có**: mất thay đổi chưa commit |
| Đã add, muốn bỏ khỏi staging (giữ nội dung) | `git restore --staged <file>` | Không |
| Commit đã **push** bị lỗi, cần hủy | `git revert <commit>` | Không: tạo commit đảo ngược |
| Commit **chưa push**, muốn gộp/sửa lại | `git reset --soft HEAD~1` | Không: thay đổi còn ở staging |
| | `git reset --mixed HEAD~1` (mặc định) | Không: thay đổi còn ở working tree |
| | `git reset --hard <commit>` | **Có**: xóa thay đổi chưa commit |
| Đang làm dở, cần chuyển việc gấp | `git stash push -m "..."` rồi `git stash pop` | Không |
| Lỡ `reset --hard` mất commit | `git reflog` rồi `git reset --hard <hash>` | Lấy lại được commit (không lấy lại được thay đổi chưa từng commit) |

## Thực hành (chạy `git status` và `git log --oneline -5` sau mỗi bước)

```bash
cd part3_git/sandbox/sensor-app

# 1. restore
echo "rac" >> sensor.c;  git restore sensor.c
echo "rac" >> sensor.c;  git add sensor.c;  git restore --staged sensor.c;  git restore sensor.c

# 2. revert (an toàn với commit đã chia sẻ)
echo bug > BUG; git add BUG; git commit -m "Commit loi"
git revert --no-edit HEAD          # BUG biến mất, lịch sử có thêm commit "Revert ..."

# 3. reset 3 chế độ
BEFORE=$(git rev-parse HEAD)
echo a > A; git add A; git commit -m "A"
git reset --soft HEAD~1            # status: A ở staging
git commit -m "A lai"
git reset HEAD~1                   # (--mixed) status: A là untracked
git add A; git commit -m "A lan 3"
git reset --hard "$BEFORE"         # A biến mất

# 4. Cứu commit bằng reflog
git reflog                         # tìm dòng "commit: A lan 3", lấy mã hash bên trái
git reset --hard <hash-vua-tim>    # A quay lại

# 5. stash
echo "dang lam do" >> sensor.c
git stash push -m "wip offset"
git status                         # sạch
git stash list
git stash pop                      # thay đổi quay lại
```

## Lỗi thường gặp
- Dùng `reset --hard` hoặc `restore` khi chưa xem `git status`: mất việc chưa commit, reflog **không** cứu được.
- `reset` một commit **đã push** rồi push lại: phải `--force`, làm hỏng lịch sử của đồng nghiệp. Khi đó dùng `revert`.
- Quên `stash pop`: thay đổi nằm mãi trong `git stash list`.
- `git stash` mặc định **không** cất file untracked (cần `-u`).

## Bài tập phục hồi (chưa có đáp án). Mỗi bài tự tạo tình huống rồi tự cứu
1. Commit 3 commit A, B, C. Hủy **chỉ B** (đã push) mà giữ C.
2. Lỡ `git reset --hard HEAD~2`. Lấy lại cả 2 commit.
3. Đang sửa `sensor.c` (chưa commit) thì phải sửa gấp bug trên main. Cất việc dở, sửa bug, commit, rồi lấy việc dở ra tiếp.
4. Lỡ commit một file `secret.txt` (chưa push). Bỏ nó khỏi commit nhưng **giữ file trên đĩa**, và thêm vào `.gitignore`.
5. Giải thích bằng lời của bạn: vì sao `git restore` một file đã sửa nhưng chưa từng add thì **không** cứu được bằng reflog?

## Tự kiểm tra
- [ ] Với mỗi tình huống trong bảng, chọn đúng lệnh mà không cần nhìn bảng.
- [ ] Phân biệt "lệnh viết lại lịch sử" (reset) và "lệnh thêm lịch sử" (revert).
- [ ] Tự cứu được commit sau `reset --hard` bằng reflog.
