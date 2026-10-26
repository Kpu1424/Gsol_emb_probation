#!/usr/bin/env bash
# Kiểm tra máy host có đủ điều kiện build Yocto hay không. CHỈ ĐỌC, không cài, không tải gì.
#
#   ./check_host.sh [thu_muc_se_build]     (mặc định: thư mục hiện tại)
#
# Ngưỡng tham khảo theo Yocto Project Reference Manual (System Requirements).
# Hãy đối chiếu lại với User Guide của BSP bạn chọn: BSP có thể yêu cầu khác.
set -u
BUILD_DIR="${1:-$PWD}"
WARN=0
ok()   { printf "  [OK]   %s\n" "$*"; }
warn() { printf "  [WARN] %s\n" "$*"; WARN=$((WARN+1)); }

echo "== He dieu hanh =="
. /etc/os-release 2>/dev/null && ok "$PRETTY_NAME ($(uname -m)), kernel $(uname -r)"
[ "$(id -u)" -eq 0 ] && warn "Dang chay voi root: BitBake tu choi build bang root" || ok "Khong phai root"

echo "== Tai nguyen =="
CORES=$(nproc); MEM_GB=$(awk '/MemTotal/{printf "%d", $2/1024/1024}' /proc/meminfo)
FREE_GB=$(df -BG --output=avail "$BUILD_DIR" | tail -1 | tr -dc '0-9')
ok "CPU: $CORES nhan"
[ "$MEM_GB" -ge 8 ]    && ok "RAM: ${MEM_GB} GB"                    || warn "RAM: ${MEM_GB} GB (< 8 GB, build se rat cham hoac loi)"
[ "$FREE_GB" -ge 90 ]  && ok "Dia trong tai $BUILD_DIR: ${FREE_GB} GB" || warn "Dia trong: ${FREE_GB} GB (< 90 GB khuyen nghi)"

echo "== Phien ban cong cu =="
ver_ge() { [ "$(printf '%s\n%s\n' "$2" "$1" | sort -V | head -1)" = "$2" ]; }
check_ver() {   # ten  phien_ban_hien_tai  toi_thieu
  if [ -z "$2" ]; then warn "$1: KHONG TIM THAY"; elif ver_ge "$2" "$3"; then ok "$1 $2 (>= $3)"; else warn "$1 $2 (< $3)"; fi
}
check_ver git     "$(git --version 2>/dev/null | awk '{print $3}')" 1.8.3.1
check_ver tar     "$(tar --version 2>/dev/null | head -1 | awk '{print $NF}')" 1.28
check_ver python3 "$(python3 -c 'import platform;print(platform.python_version())' 2>/dev/null)" 3.8
check_ver gcc     "$(gcc -dumpfullversion 2>/dev/null)" 8.0
check_ver make    "$(make --version 2>/dev/null | head -1 | awk '{print $3}')" 4.0

echo "== Goi host can cho Yocto tren Ubuntu/Debian (dpkg) =="
PKGS="gawk wget git diffstat unzip texinfo gcc build-essential chrpath socat cpio python3 python3-pip
python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 python3-subunit zstd
liblz4-tool file locales libacl1"
MISSING=""
for p in $PKGS; do dpkg -s "$p" >/dev/null 2>&1 || MISSING="$MISSING $p"; done
[ -z "$MISSING" ] && ok "Du tat ca goi" || warn "Thieu:$MISSING  (can admin cai: sudo apt install ...)"

echo "== Locale =="
locale -a 2>/dev/null | grep -qi '^en_US.utf8$' && ok "co en_US.UTF-8" || warn "thieu locale en_US.UTF-8"

echo "== Cong cu tai source BSP =="
command -v repo >/dev/null && ok "repo: $(command -v repo)" || warn "chua co 'repo' (nhieu BSP dung repo manifest)"

echo
echo "So canh bao: $WARN"
echo "Luu y: script khong biet yeu cau rieng cua BSP (vd. phien ban Ubuntu duoc ho tro)."
