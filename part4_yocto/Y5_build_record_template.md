# Y5 — Hồ sơ tái lập build (mẫu)

Sao chép file này thành `records/<ngay>-<board>.md` cho **mỗi** lần build quan trọng. Mục tiêu: người khác (hoặc bạn sau 6 tháng) build lại được **đúng** image đó.

## 1. Tổng quan
- Ngày giờ: 
- Người thực hiện: 
- Mục đích build: 
- Kết quả: thành công / lỗi (ghi bước lỗi)

## 2. Host
```bash
cat /etc/os-release | head -2; uname -a; nproc; free -h; df -h <build-dir>
./check_host.sh <build-dir> > host-check.txt
```
(dán output)

## 3. Nguồn và revision
- User Guide: tên, phiên bản, ngày
- Manifest: URL, branch, file
- Revision chốt:
```bash
repo manifest -r -o manifest-pinned.xml          # nếu dùng repo
bitbake-layers show-layers > layers.txt
for l in $(bitbake-layers show-layers | awk 'NR>3{print $2}'); do
  echo "$l $(git -C $l rev-parse HEAD 2>/dev/null) $(git -C $l status --porcelain 2>/dev/null | wc -l) thay doi chua commit"
done > layer-revisions.txt
```

## 4. Cấu hình
- Sao lưu `conf/local.conf`, `conf/bblayers.conf` (hoặc `diff` với bản mặc định của BSP)
- MACHINE = 
- DISTRO = 
- Image = 
- Các dòng tự thêm (vd. `IMAGE_INSTALL:append = " sensor-hello"`):

## 5. Lệnh đã chạy (đúng thứ tự)
```
source ...
bitbake ...
```

## 6. Log
- Build: `logs/build-<ngay>.log` (từ `tee`), và `tmp*/log/cooker/<MACHINE>/` của BitBake
- Lỗi task: `tmp*/work/<arch>/<recipe>/<ver>/temp/log.do_<task>`
- Flash: 
- Boot serial: 

## 7. Kết quả
```bash
ls -l tmp*/deploy/images/<MACHINE>/
sha256sum <file-image-da-flash>
```
| File | sha256 |
|---|---|
| | |

## 8. Checklist User Guide (từ Y3)
(dán bảng đã điền)

## 9. Ghi chú / sự cố và cách xử lý
