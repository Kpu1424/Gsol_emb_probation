# Phần 3 — Git (G1 → G4)

Mọi thao tác làm trong **`part3_git/sandbox/`**, một repo thực hành riêng. Không động tới lịch sử của dự án nào khác.

| Bài | File hướng dẫn | Nội dung |
|---|---|---|
| G1 | [G1_basics.md](G1_basics.md) | init, status, add, commit, diff, log |
| G2 | [G2_branch_merge_conflict.md](G2_branch_merge_conflict.md) | branch, merge, xử lý conflict |
| G3 | [G3_remote_bare.md](G3_remote_bare.md) | remote, push, clone, fetch, pull (dùng **local bare repo** thay server) |
| G4 | [G4_recovery.md](G4_recovery.md) | restore, revert, reset, stash, reflog: phục hồi thay đổi |

## Chuẩn bị

```bash
cd part3_git
./setup_sandbox.sh            # tạo sandbox/ và sandbox/remote/sensor-app.git
./setup_sandbox.sh --reset    # làm lại từ đầu (chỉ xóa part3_git/sandbox)
```

> **Lưu ý danh tính commit:** cấu hình git *global* trên máy này đang là `user.name = huypqa`.
> Trong G1 bạn sẽ đặt tên/email **riêng cho repo thực hành** (`git config user.name ...`, không có `--global`),
> để commit không mang tên người khác. Nếu máy dùng chung, đừng đổi cấu hình global khi chưa hỏi.

## Script tự kiểm tra (không thay cho việc bạn tự làm)

`./selftest.sh` chạy tự động **toàn bộ chuỗi lệnh G1–G4** trong một thư mục tạm rồi xóa đi.
Script này chỉ chứng minh các lệnh trong bài chạy đúng với git trên máy này.
Nó **không** chứng minh bạn đã thành thạo.

Kết quả đã chạy (2026-10-23, git 2.34.1): `27 PASS, 0 FAIL`.

## 3 vùng cần nhớ

```
 working tree  --git add-->  staging (index)  --git commit-->  repository (lịch sử)
      ^                            |                                   |
      +----- git restore ----------+                                   |
      +------------------ git restore --source=<commit> ---------------+
```

`git status` luôn cho biết mỗi file đang ở vùng nào. Hãy chạy nó trước và sau **mọi** lệnh khi đang học.
