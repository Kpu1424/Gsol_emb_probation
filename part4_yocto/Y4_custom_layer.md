# Y4 — Custom layer + recipe Hello World, đưa vào image, chạy trên board

**Mục tiêu:** tự tạo layer `meta-sensor` chứa recipe `sensor-hello`, build ra package, đưa vào image và chạy trên board.
**Cần biết trước:** Y1. Phần build/chạy cần Y2 xong (có build dir của BSP).

## Cấu trúc đã tạo

```
meta-sensor/
├── conf/layer.conf                               # khai báo layer, độ ưu tiên, release tương thích
└── recipes-apps/sensor-hello/
    ├── sensor-hello_1.0.bb                       # recipe
    └── files/sensor_hello.c                      # source C
```

Những dòng quan trọng:
- `layer.conf` → `LAYERSERIES_COMPAT_meta-sensor`: **phải** chứa tên release của BSP, nếu không BitBake báo lỗi không tương thích.
- `.bb` → `SRC_URI = "file://sensor_hello.c"`: BitBake tìm file trong `files/` cạnh recipe.
- `.bb` → `${CC} ${CFLAGS} ${LDFLAGS}`: cross-compiler cho MACHINE, kèm cờ link chuẩn.
- `.bb` → `do_install` cài vào `${D}${bindir}`, tức `/usr/bin/sensor-hello` trên board.

## Đã kiểm chứng (2026-10-23)
- `sensor_hello.c` build bằng gcc host và chạy được (in `arch=x86_64`, đúng vì đó là host).
- Với poky **kirkstone** (đọc từ `/home/emb01/hlos/poky`, build dir tạm riêng): `bitbake-layers add-layer` và `bitbake -e sensor-hello` **parse thành công**.
- **Chưa** chạy `bitbake sensor-hello` (cần build cả toolchain, mất nhiều giờ), **chưa** đưa vào image, **chưa** chạy trên board.

## Các bước (sau khi có build dir của BSP từ Y2)

```bash
source <script-setup-cua-BSP> <build-dir>

# 1. Thêm layer (ghi vào conf/bblayers.conf)
bitbake-layers add-layer /home/emb01/tuan/emb/emb_probation/part4_yocto/meta-sensor
bitbake-layers show-layers | grep meta-sensor

# 2. Build riêng recipe
bitbake sensor-hello
oe-pkgdata-util list-pkg-files sensor-hello      # mong đợi: /usr/bin/sensor-hello
file tmp*/work/*/sensor-hello/1.0-r0/image/usr/bin/sensor-hello   # phải là kiến trúc của board

# 3. Đưa vào image: thêm vào conf/local.conf
#    kirkstone trở lên:   IMAGE_INSTALL:append = " sensor-hello"
#    dunfell:             IMAGE_INSTALL_append = " sensor-hello"
#    (chú ý DẤU CÁCH đầu chuỗi; cú pháp ':' và '_' khác nhau giữa các release)
bitbake <image>

# 4. Kiểm tra image có chương trình trước khi flash
grep sensor-hello tmp*/deploy/images/${MACHINE}/*.manifest

# 5. Flash + boot theo User Guide (Y3), rồi trên board:
sensor-hello
```

Output dự kiến trên board (`host`, `kernel`, `arch` tùy board):
```
Hello World from sensor-hello!
host=<hostname-board> kernel=<phien-ban-kernel> arch=aarch64
sample 0: 25.0 C
sample 1: 25.3 C
sample 2: 25.6 C
```

## Lỗi thường gặp
- `Layer meta-sensor is not compatible with the core layer...`: sửa `LAYERSERIES_COMPAT`.
- `No GNU_HASH in the ELF binary`: quên `${LDFLAGS}` khi link.
- `license-checksum` lỗi: md5 trong `LIC_FILES_CHKSUM` không khớp file license.
- `Nothing PROVIDES 'sensor-hello'`: layer chưa được thêm, hoặc tên file `.bb` sai mẫu `tên_phiênbản.bb`.
- Thiếu dấu cách trong `" sensor-hello"`: tên package dính vào package trước đó.
- Dùng `IMAGE_INSTALL:append` trên dunfell (hoặc `_append` trên kirkstone+): không có tác dụng hoặc lỗi cú pháp.
- Từ release styhead (5.1) trở lên: `S = "${WORKDIR}"` bị từ chối, phải dùng `S = "${UNPACKDIR}"`.

## Bài tập (chưa có đáp án)
1. Thêm một `Makefile` vào `files/`, sửa recipe để build bằng `oe_runmake` thay vì gọi `${CC}` trực tiếp.
2. Thêm tham số dòng lệnh `-n <so_mau>` cho chương trình, tăng `PV` lên `1.1` (đổi tên file `.bb`).
3. Tạo `sensor-hello_%.bbappend` trong một layer thứ hai để cài thêm file cấu hình `/etc/sensor-hello.conf`.
4. Chạy `bitbake -e sensor-hello | grep ^WORKDIR=` và giải thích đường dẫn đó gồm những phần nào.

## Tự kiểm tra
- [ ] Giải thích được BitBake chạy những task nào cho recipe này (`bitbake -c listtasks sensor-hello`).
- [ ] Chứng minh được binary là kiến trúc của board, không phải x86_64.
- [ ] Chạy được `sensor-hello` trên **board thật** và có log serial làm bằng chứng. (QEMU không tính.)
