/*
 * Bài 05a - Nhập/xuất bàn phím AN TOÀN: fgets + strtol thay vì scanf trực tiếp
 * Chương trình hỏi ngưỡng cảnh báo, rồi nhập từng giá trị cảm biến đến khi gõ "q".
 *
 * Build: make
 * Chạy tương tác:    ./a_keyboard_io
 * Chạy với dữ liệu sẵn (không cần gõ):  printf '40\n25\nabc\n45\nq\n' | ./a_keyboard_io
 */
#include <stdio.h>
#include <stdlib.h>   /* strtol */
#include <string.h>   /* strcspn */
#include <errno.h>

#define LINE_MAX_LEN 64

/* Đọc 1 dòng từ stdin. Trả về 0 nếu OK, -1 nếu hết dữ liệu (EOF, Ctrl+D). */
static int read_line(const char *prompt, char *buf, size_t size) {
  printf("%s", prompt);
  fflush(stdout);                         /* đảm bảo lời nhắc hiện ra trước khi chờ nhập */
  if (fgets(buf, (int)size, stdin) == NULL) {
    return -1;
  }
  buf[strcspn(buf, "\n")] = '\0';         /* bỏ ký tự xuống dòng ở cuối */
  return 0;
}

/* Chuyển chuỗi -> số nguyên, kiểm tra lỗi đầy đủ. Trả về 0 nếu hợp lệ. */
static int parse_int(const char *text, long *out) {
  char *end = NULL;
  errno = 0;
  long value = strtol(text, &end, 10);
  if (end == text || *end != '\0' || errno == ERANGE) {
    return -1;                            /* rỗng, có ký tự lạ, hoặc tràn số */
  }
  *out = value;
  return 0;
}

int main(void) {
  char line[LINE_MAX_LEN];
  long limit;

  /* Lặp cho đến khi nhập đúng */
  for (;;) {
    if (read_line("Nhap nguong canh bao (do C): ", line, sizeof line) != 0) {
      fprintf(stderr, "\nKhong co du lieu nhap. Thoat.\n");   /* lỗi -> stderr */
      return 1;
    }
    if (parse_int(line, &limit) == 0) break;
    printf("  '%s' khong phai so nguyen, nhap lai.\n", line);
  }

  int count = 0, alarms = 0;
  long sum = 0;
  for (;;) {
    if (read_line("Gia tri cam bien (q de dung): ", line, sizeof line) != 0) break;
    if (strcmp(line, "q") == 0) break;

    long value;
    if (parse_int(line, &value) != 0) {
      printf("  Bo qua '%s' (khong hop le)\n", line);
      continue;
    }
    count++;
    sum += value;
    if (value > limit) {
      alarms++;
      printf("  CANH BAO: %ld > %ld\n", value, limit);
    }
  }

  printf("\nDa nhap %d gia tri hop le, %d canh bao", count, alarms);
  if (count > 0) printf(", trung binh %.2f", (double)sum / count);
  printf("\n");
  return 0;
}
