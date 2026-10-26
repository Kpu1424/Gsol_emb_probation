# Recipe: cách BitBake lấy source, build và cài chương trình sensor-hello
# Tên file <tên>_<phiên bản>.bb  ->  PN = sensor-hello, PV = 1.0
SUMMARY = "Sensor Hello World (bai hoc Yocto)"
DESCRIPTION = "Chuong trinh C in Hello World va du lieu cam bien gia lap"
LICENSE = "MIT"
# Checksum của file giấy phép MIT có sẵn trong poky (meta/files/common-licenses/MIT)
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# Source nằm ngay trong layer, thư mục files/
SRC_URI = "file://sensor_hello.c"

# Thư mục source sau khi unpack. Đúng cho dunfell/kirkstone/scarthgap.
# Từ release styhead (5.1) trở đi phải dùng: S = "${UNPACKDIR}"
S = "${WORKDIR}"

# ${CC} là CROSS-COMPILER cho MACHINE đã chọn (không phải gcc của host).
# Truyền ${LDFLAGS} để tránh lỗi QA "No GNU_HASH in the ELF binary".
do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} ${S}/sensor_hello.c -o sensor-hello
}

# Cài vào ${D} (thư mục ảo của rootfs). ${bindir} thường là /usr/bin
do_install() {
    install -d ${D}${bindir}
    install -m 0755 sensor-hello ${D}${bindir}/
}
