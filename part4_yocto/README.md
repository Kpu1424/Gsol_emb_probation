# Phần 4 — Yocto (Y1 → Y5)

| Bài | File | Trạng thái môi trường |
|---|---|---|
| Y1 Khái niệm: host/target, cross-compile, bootloader, kernel, rootfs, BitBake, layer, recipe, image, MACHINE | [Y1_concepts.md](Y1_concepts.md) | Làm được ngay |
| Y2 Chọn platform, đọc User Guide, build image theo BSP | [Y2_bsp_build_plan.md](Y2_bsp_build_plan.md) | **BỊ CHẶN**: chưa chốt board / User Guide / phiên bản BSP |
| Y3 Boot image trên board + checklist User Guide | [Y2_bsp_build_plan.md](Y2_bsp_build_plan.md#y3--boot-trên-board-và-checklist) | **BỊ CHẶN**: cần board, ảnh đã build, công cụ flash; mình KHÔNG flash |
| Y4 Custom layer + recipe Hello World, đưa vào image, chạy trên board | [Y4_custom_layer.md](Y4_custom_layer.md), [meta-sensor/](meta-sensor/) | Layer đã viết và BitBake **parse** thành công; **chưa** build/chạy |
| Y5 Lưu cấu hình, revision, lệnh, log để tái lập | [Y5_build_record_template.md](Y5_build_record_template.md) | Mẫu sẵn sàng, điền khi build thật |

## Đã kiểm tra trên máy này (2026-10-23)

`./check_host.sh` → 0 cảnh báo. Ubuntu 22.04.5, 16 nhân, 31 GB RAM, 539 GB trống, đủ gói host theo danh sách của Yocto, có `repo`.

Trên máy có sẵn **2 cây BSP Qualcomm** (chỉ đọc, KHÔNG build/sửa trong đó):

| Đường dẫn | Release poky | Ghi chú |
|---|---|---|
| `/home/emb01/hlos` | kirkstone (4.0) | commit `b318c38` "HSPTek Init OS source code for AIBOX8550", build dir `build-qti-distro-ubuntu-fullstack-debug` |
| `/home/emb01/linkbase-openq-u20/qrb5165-openq_build-dir_v2.1` | dunfell (3.1) | QRB5165. Dunfell không liệt kê Ubuntu 22.04 trong danh sách host được hỗ trợ; tên thư mục gợi ý build trên Ubuntu 20.04 |

Hai cây này có thể là platform của đơn vị bạn, nhưng **mình chưa coi là đã chọn**. Bạn cần xác nhận với người hướng dẫn (xem Y2).

## Kiểm chứng layer (không build)

Dùng poky kirkstone ở `/home/emb01/hlos/poky` **chỉ để đọc**, với build dir riêng trong thư mục tạm:
`bitbake-layers add-layer meta-sensor` + `bitbake -e sensor-hello` → parse OK, `PN=sensor-hello`, `PV=1.0`, `MACHINE=qemux86-64`.
Số file thay đổi trong `hlos/poky` trước và sau đều là 8 (đã có sẵn từ trước, không do bài học tạo).
Checksum `LIC_FILES_CHKSUM` của MIT khớp file giấy phép trong cả hai cây poky.

## Không làm (theo yêu cầu)
- Không tải source BSP, không chạy build lớn, không cài gói hệ thống, không flash board.
- Có thể luyện với QEMU (`MACHINE = "qemux86-64"`, `runqemu`). Yocto tự build QEMU nên không cần QEMU của host.
  Nhưng việc này vẫn cần tải poky và build vài giờ, vài chục GB, nên chờ bạn đồng ý.
  **Chạy được trên QEMU không tính là đạt đầu ra "chạy trên board thật".**
