# Y2 — Build image theo User Guide và BSP · Y3 — Boot trên board

> **TRẠNG THÁI: BỊ CHẶN.** Chưa thể bắt đầu cho đến khi điền đủ phần A. Không tải source, không build lớn trước khi có thông tin này.

## A. Thông tin bắt buộc (bạn điền, xác nhận với người hướng dẫn)

| Mục | Giá trị | Nguồn |
|---|---|---|
| Board (tên, revision phần cứng) | _chưa có_ | |
| SoC | _chưa có_ | |
| User Guide (tên tài liệu, phiên bản, ngày) | _chưa có_ | |
| Phiên bản BSP / tag release | _chưa có_ | |
| Release Yocto (dunfell / kirkstone / scarthgap...) | _chưa có_ | |
| Cách lấy source (repo manifest URL + branch + file xml / tarball) | _chưa có_ | |
| `MACHINE` | _chưa có_ | |
| `DISTRO` | _chưa có_ | |
| Image target (vd. `core-image-minimal`, image của hãng) | _chưa có_ | |
| Host OS mà User Guide hỗ trợ | _chưa có_ | so với host: Ubuntu 22.04 |
| Yêu cầu tài nguyên của User Guide (đĩa, RAM) | _chưa có_ | so với host: 539 GB trống, 31 GB RAM |
| Công cụ flash + cách vào chế độ flash (fastboot, QDL/EDL, UUU, SD card...) | _chưa có_ | |
| Cổng debug serial (UART, baudrate) | _chưa có_ | |
| Quyền truy cập: tài khoản tải BSP, license, mạng nội bộ | _chưa có_ | |

**Ứng viên đã thấy trên máy (chỉ đọc, chưa xác nhận):** QCS8550 / AIBOX8550 (`/home/emb01/hlos`, kirkstone) và
QRB5165 (`/home/emb01/linkbase-openq-u20/...`, dunfell). Nếu đơn vị chọn một trong hai, **hỏi trước** xem có được
dùng lại `downloads/` và `sstate-cache/` có sẵn không, và **build vào thư mục build riêng**, không ghi vào build dir của người khác.

## B. Quy trình chung (chỉ làm sau khi có A; User Guide luôn ưu tiên hơn file này)

```bash
# 1. Kiểm tra host
./check_host.sh /duong/dan/se/build

# 2. Lấy source đúng phiên bản (ví dụ dạng repo; dùng URL/branch/manifest TRONG User Guide)
mkdir -p ~/yocto/<board> && cd ~/yocto/<board>
repo init -u <manifest-url> -b <branch> -m <manifest.xml>
repo sync -j8
repo manifest -r -o manifest-pinned.xml      # chốt revision từng project (tái lập, Y5)

# 3. Môi trường build (script do BSP cung cấp, hoặc của poky)
source <script-setup-cua-BSP> <build-dir>    # hoặc: source poky/oe-init-build-env build
grep -E '^(MACHINE|DISTRO)' conf/local.conf

# 4. Build image và lưu log
bitbake <image> 2>&1 | tee ~/yocto/<board>/logs/build-$(date +%Y%m%d-%H%M).log
ls tmp*/deploy/images/${MACHINE}/
```

## Y3 — Boot trên board và checklist

Flash theo **đúng** User Guide. Phần này bạn tự làm trên board; mình không flash thiết bị.

Checklist mẫu. Thay bằng checklist trong User Guide, ghi kết quả thật và bằng chứng (log serial):

| # | Hạng mục | Kết quả | Bằng chứng |
|---|---|---|---|
| 1 | Flash thành công, không lỗi | ☐ | log công cụ flash |
| 2 | Bootloader in ra serial | ☐ | `logs/boot-serial.log` |
| 3 | Kernel boot, phiên bản đúng (`uname -a`) | ☐ | |
| 4 | Đăng nhập được shell | ☐ | |
| 5 | `cat /etc/os-release` khớp DISTRO/phiên bản | ☐ | |
| 6 | Mạng / thiết bị ngoại vi theo User Guide | ☐ | |
| 7 | ... (các mục khác trong User Guide) | ☐ | |

Ghi log serial: `picocom -b <baud> /dev/ttyUSB0 --logfile boot-serial.log` (nếu đã có picocom), hoặc `script -f boot.log`.

## Lỗi thường gặp
- Build theo hướng dẫn của một phiên bản BSP khác: patch/manifest không khớp, lỗi khó hiểu.
- Hết dung lượng đĩa giữa chừng: kiểm tra trước, và dùng `INHERIT += "rm_work"` nếu User Guide cho phép.
- Chạy `bitbake` trong terminal chưa `source` môi trường: `bitbake: command not found`.
- Flash nhầm image hoặc nhầm partition: đọc kỹ bảng partition trong User Guide.

## Bài tập (khi đã có board)
1. Điền đủ bảng A, ghi nguồn cho từng dòng.
2. So sánh yêu cầu host của User Guide với kết quả `check_host.sh`. Ghi ra mọi điểm không khớp.
3. Sau khi boot: chạy `uname -a`, `cat /proc/cpuinfo | head`, `df -h`, và giải thích từng output.

## Tự kiểm tra
- [ ] Biết image nằm ở đâu sau khi build, và file nào dùng để flash.
- [ ] Biết MACHINE được đặt ở đâu và vì sao không nên tự đổi tùy tiện.
- [ ] Có log build + log boot đủ để người khác lặp lại (Y5).
