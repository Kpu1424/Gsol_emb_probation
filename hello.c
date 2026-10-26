/*
 * hello.c - Bài học C cơ bản: KIỂU DỮ LIỆU + INPUT / OUTPUT
 *
 * Biên dịch:  gcc hello.c -o hello
 * Chạy:       ./hello
 */

#include <stdio.h>   /* thư viện vào/ra chuẩn: printf (in ra), scanf (nhập vào) */

/* ===== PHẦN 1: CẤU TRÚC DỮ LIỆU TỰ ĐỊNH NGHĨA (struct) =====
 * struct = gom nhiều biến khác kiểu thành 1 "hộp" có tên.
 * Ví dụ: 1 sinh viên có tên, tuổi, điểm.
 */
struct SinhVien {
  char  ten[50];  /* mảng ký tự = chuỗi, tối đa 49 ký tự + '\0' */
  int   tuoi;     /* số nguyên */
  float diem;     /* số thực */
};

int main(void) {
  /* ===== PHẦN 2: KIỂU DỮ LIỆU CƠ BẢN =====
   * Mỗi biến = 1 ô nhớ có KIỂU (quyết định chứa gì, chiếm bao nhiêu byte).
   */
  int    so_nguyen = 10;               /* số nguyên: ... -1, 0, 1, 2 ...  */
  float  so_thuc   = 3.14f;            /* số thực (~6 chữ số chính xác)   */
  double so_thuc_2 = 3.14159265358979; /* số thực chính xác hơn (~15 số)  */
  char   ky_tu     = 'A';              /* 1 ký tự, dùng dấu nháy đơn ' '  */

  /* ===== PHẦN 3: OUTPUT - in ra màn hình bằng printf =====
   * Mỗi kiểu có "mã định dạng" riêng:
   *   %d -> int      %f -> float/double     %c -> char
   *   %s -> chuỗi    %zu -> kết quả sizeof  \n -> xuống dòng
   */
  printf("Hello, World!\n\n");

  printf("--- Kieu du lieu co ban ---\n");
  printf("int    : %d (chiem %zu byte)\n", so_nguyen, sizeof(int));
  printf("float  : %.2f (chiem %zu byte)\n", so_thuc, sizeof(float));
  printf("double : %.10f (chiem %zu byte)\n", so_thuc_2, sizeof(double));
  printf("char   : %c (chiem %zu byte)\n\n", ky_tu, sizeof(char));

  /* ===== PHẦN 4: MẢNG (array) =====
   * Mảng = nhiều phần tử CÙNG KIỂU, nằm liền nhau, đánh số từ 0.
   */
  int mang[5] = {1, 2, 3, 4, 5};
  int tong = 0;

  printf("--- Mang ---\n");
  for (int i = 0; i < 5; i++) {
    printf("mang[%d] = %d\n", i, mang[i]);
    tong += mang[i];
  }
  printf("Tong = %d\n\n", tong);

  /* ===== PHẦN 5: INPUT - nhập từ bàn phím bằng scanf =====
   * scanf cần ĐỊA CHỈ của biến (dấu &) để ghi giá trị vào.
   * Riêng chuỗi (mảng char): tên mảng đã là địa chỉ -> không cần &.
   */
  struct SinhVien sv;

  printf("--- Nhap thong tin sinh vien ---\n");
  printf("Nhap ten (khong dau cach): ");
  scanf("%49s", sv.ten);   /* %49s: đọc tối đa 49 ký tự, tránh tràn mảng */

  printf("Nhap tuoi: ");
  scanf("%d", &sv.tuoi);

  printf("Nhap diem: ");
  scanf("%f", &sv.diem);

  /* In lại struct: truy cập từng trường bằng dấu chấm (.) */
  printf("\n--- Ket qua ---\n");
  printf("Ten  : %s\n", sv.ten);
  printf("Tuoi : %d\n", sv.tuoi);
  printf("Diem : %.1f\n", sv.diem);

  if (sv.diem >= 5.0f) {
    printf("=> %s DAU!\n", sv.ten);
  } else {
    printf("=> %s chua dat.\n", sv.ten);
  }

  return 0;  /* 0 = chương trình kết thúc thành công */
}
