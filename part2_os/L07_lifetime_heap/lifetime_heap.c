/*
 * Bài 07 - Vòng đời biến và cấp phát/giải phóng bộ nhớ động
 * Ví dụ: bộ đệm mẫu cảm biến có kích thước quyết định LÚC CHẠY.
 *
 * Build: make   Chạy: ./lifetime_heap [so_mau]    vd: ./lifetime_heap 8
 */
#include <stdio.h>
#include <stdlib.h>

/* ===== 1. Vòng đời ===== */
int g_total_reads = 0;   /* toàn cục: sống suốt chương trình */

int next_sample_id(void) {
  static int s_next_id = 0;   /* static cục bộ: khởi tạo 1 LẦN, giữ giá trị giữa các lần gọi */
  int local = 0;              /* automatic: tạo mới mỗi lần gọi, mất khi hàm return */
  local++;
  s_next_id++;
  g_total_reads++;
  printf("  next_sample_id: local=%d, s_next_id=%d\n", local, s_next_id);
  return s_next_id;
}

/* SAI (chỉ để đọc, không gọi): trả về địa chỉ biến cục bộ -> con trỏ treo (dangling)
 *   int *bad(void) { int x = 5; return &x; }   // x đã chết khi hàm return
 */

/* ĐÚNG: hàm tạo bộ đệm trên heap, NGƯỜI GỌI có trách nhiệm free */
int *create_buffer(size_t n) {
  int *buf = malloc(n * sizeof *buf);   /* sizeof *buf: tự đúng nếu đổi kiểu buf */
  if (buf == NULL) {
    return NULL;                         /* malloc có thể thất bại -> luôn kiểm tra */
  }
  for (size_t i = 0; i < n; i++) {
    buf[i] = 250 + (int)i;               /* malloc KHÔNG xóa về 0, phải tự gán */
  }
  return buf;                            /* vùng heap vẫn sống sau khi hàm return */
}

int main(int argc, char *argv[]) {
  size_t n = 4;
  if (argc > 1) {
    long v = strtol(argv[1], NULL, 10);
    if (v <= 0 || v > 1000000) {
      fprintf(stderr, "so_mau phai trong khoang 1..1000000\n");
      return 1;
    }
    n = (size_t)v;
  }

  printf("== 1. static cuc bo vs automatic ==\n");
  for (int i = 0; i < 3; i++) next_sample_id();
  printf("  g_total_reads = %d\n", g_total_reads);

  printf("\n== 2. malloc: cap phat %zu mau ==\n", n);
  int *buf = create_buffer(n);
  if (buf == NULL) { perror("malloc"); return 1; }
  printf("  buf[0]=%d buf[%zu]=%d\n", buf[0], n - 1, buf[n - 1]);

  printf("\n== 3. realloc: tang gap doi ==\n");
  size_t new_n = n * 2;
  int *tmp = realloc(buf, new_n * sizeof *buf);   /* gán vào biến TẠM: nếu lỗi, buf cũ vẫn còn */
  if (tmp == NULL) {
    free(buf);
    perror("realloc");
    return 1;
  }
  buf = tmp;
  for (size_t i = n; i < new_n; i++) buf[i] = 0;  /* phần mới chưa được khởi tạo */
  printf("  kich thuoc moi %zu, buf[0]=%d van giu nguyen\n", new_n, buf[0]);

  printf("\n== 4. calloc: cap phat + xoa ve 0 ==\n");
  int *zeros = calloc(n, sizeof *zeros);
  if (zeros == NULL) { free(buf); perror("calloc"); return 1; }
  printf("  zeros[0]=%d\n", zeros[0]);

  printf("\n== 5. free: tra lai bo nho ==\n");
  free(buf);
  buf = NULL;      /* tránh dùng lại con trỏ đã free (use-after-free) */
  free(zeros);
  zeros = NULL;
  free(NULL);      /* free(NULL) hợp lệ, không làm gì */
  printf("  Da giai phong. Moi malloc/calloc/realloc thanh cong can DUNG 1 free.\n");
  return 0;
}
