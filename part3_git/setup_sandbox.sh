#!/usr/bin/env bash
# Tạo thư mục thực hành Git riêng: part3_git/sandbox/
# Không đụng tới repo nào khác. Chạy lại với --reset để xóa sandbox cũ và làm lại từ đầu.
#
#   ./setup_sandbox.sh          # tạo mới (báo lỗi nếu đã có)
#   ./setup_sandbox.sh --reset  # xóa part3_git/sandbox rồi tạo lại
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
SANDBOX="$HERE/sandbox"

if [ -e "$SANDBOX" ]; then
  if [ "${1:-}" = "--reset" ]; then
    echo "Xoa sandbox cu: $SANDBOX"
    rm -rf -- "$SANDBOX"
  else
    echo "Da co $SANDBOX. Dung --reset neu muon xoa va tao lai." >&2
    exit 1
  fi
fi

mkdir -p "$SANDBOX/remote"
# Bare repository: repo KHÔNG có thư mục làm việc, đóng vai "server" (như GitHub/GitLab)
git init --bare -q -b main "$SANDBOX/remote/sensor-app.git"

echo "Da tao:"
echo "  $SANDBOX/                       <- lam bai G1..G4 o day"
echo "  $SANDBOX/remote/sensor-app.git  <- 'server' gia lap (bare repo) cho bai G3"
echo
echo "Buoc tiep theo: mo part3_git/G1_basics.md"
