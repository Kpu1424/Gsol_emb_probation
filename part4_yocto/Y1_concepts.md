# Y1 — Khái niệm nền tảng Yocto

**Mục tiêu:** giải thích được các thành phần của một hệ Linux nhúng và vai trò của từng khái niệm Yocto.
**Cần biết trước:** Phần 1 (C, build bằng gcc), biết dùng terminal Linux.

## 1. Host và target

| | Host (máy build) | Target (board) |
|---|---|---|
| Ví dụ trên máy này | Ubuntu 22.04, x86_64 | Board ARM/ARM64 (chưa chốt) |
| Vai trò | Chạy BitBake, compiler, tạo image | Chạy image đã build |

## 2. Cross-compilation: biên dịch trên host, chạy trên target

Thử ngay trên máy này. Máy có sẵn `arm-none-eabi-gcc`, một toolchain ARM **bare-metal** dùng cho vi điều khiển.
Nó **không** phải toolchain Linux mà Yocto sẽ tạo ra, nhưng đủ để thấy ý tưởng:

```bash
cd part4_yocto
echo 'int read_raw(int ch){return 250+ch*3;}' > /tmp/x.c
gcc -c /tmp/x.c -o /tmp/x_host.o
arm-none-eabi-gcc -mcpu=cortex-m4 -c /tmp/x.c -o /tmp/x_arm.o
file /tmp/x_host.o /tmp/x_arm.o
```

Kết quả thực tế đã chạy (2026-10-23):
```
x_arm.o:  ELF 32-bit LSB relocatable, ARM, EABI5 version 1 (SYSV), not stripped
x_host.o: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), not stripped
```
Cùng một mã nguồn nhưng ra mã máy khác nhau. File ARM **không chạy được** trên host x86_64.
Trong Yocto, biến `${CC}` trong recipe trỏ tới cross-compiler đúng với `MACHINE`.

## 3. Chuỗi khởi động của board Linux nhúng

```
Bật nguồn -> ROM code (trong SoC) -> Bootloader (vd. U-Boot, ABL/UEFI...) -> Linux kernel (+ device tree)
          -> mount Root filesystem -> init (systemd/sysvinit) -> chương trình của bạn (/usr/bin/sensor-hello)
```

- **Bootloader**: khởi tạo RAM, clock; nạp kernel vào bộ nhớ rồi nhảy vào kernel.
- **Kernel**: quản lý CPU, bộ nhớ, driver, process.
- **Device tree** (với ARM): mô tả phần cứng cho kernel.
- **Root filesystem (rootfs)**: cây thư mục `/` gồm `/bin`, `/lib`, `/etc`, các thư viện và chương trình.

## 4. Các khái niệm Yocto

| Khái niệm | Là gì | Ví dụ trong bài |
|---|---|---|
| **BitBake** | Công cụ thực thi task (fetch, compile, install, package...) theo metadata | `bitbake sensor-hello` |
| **Metadata** | Recipe, class, file cấu hình | `.bb`, `.bbclass`, `.conf` |
| **Recipe** (`.bb`) | "Công thức" build MỘT phần mềm: lấy source ở đâu, build và cài thế nào | `sensor-hello_1.0.bb` |
| **Layer** (`meta-*`) | Thư mục gom recipe/cấu hình theo chủ đề, có `conf/layer.conf` | `meta-sensor`, `meta-poky`, layer BSP của hãng |
| **BSP layer** | Layer hỗ trợ một board: MACHINE, kernel, bootloader | do hãng chip/board cung cấp |
| **MACHINE** | Board/kiến trúc đích, quyết định toolchain, kernel, bootloader | `qemux86-64` hoặc tên do BSP định nghĩa |
| **DISTRO** | Chính sách hệ thống (init system, tính năng) | `poky`, distro của hãng |
| **Image** | Recipe đặc biệt, gom các package thành rootfs + file flash | `core-image-minimal` |
| **local.conf** | Cấu hình build của bạn (MACHINE, đường dẫn...) | `build/conf/local.conf` |
| **bblayers.conf** | Danh sách layer được dùng | `build/conf/bblayers.conf` |

## Lỗi hiểu nhầm thường gặp
- "Yocto là một bản Linux": sai. Yocto là bộ công cụ để **tự tạo** bản Linux của riêng bạn.
- "Build bằng gcc của máy rồi copy lên board": thường sai kiến trúc, hoặc sai phiên bản thư viện C (glibc).
- Sửa trực tiếp recipe trong layer của hãng: nên dùng `.bbappend` hoặc layer riêng.

## Bài tập (chưa có đáp án)
1. Vẽ chuỗi khởi động ở mục 3 cho board bạn sẽ dùng, sau khi đọc User Guide của board đó.
2. Mở `meta-sensor/recipes-apps/sensor-hello/sensor-hello_1.0.bb`, giải thích từng dòng bằng lời của bạn.
3. Tìm trong một cây poky có sẵn (chỉ đọc): file `.conf` định nghĩa MACHINE `qemux86-64` nằm ở đâu?
   (gợi ý: `find /home/emb01/hlos/poky -name 'qemux86-64.conf'`)
4. Phân biệt: toolchain `arm-none-eabi-gcc` và toolchain Yocto tạo ra cho board Linux ARM64 khác nhau ở điểm nào?

## Tự kiểm tra
- [ ] Giải thích cho người khác vì sao cần cross-compiler.
- [ ] Nói được thứ tự bootloader → kernel → rootfs → ứng dụng, và mỗi bước làm gì.
- [ ] Phân biệt layer, recipe, image, MACHINE.
