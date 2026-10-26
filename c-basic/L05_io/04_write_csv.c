/*
 * Ý chính: Ghi file văn bản: fopen("w") -> fprintf từng dòng -> fclose; luôn kiểm tra fopen có trả về NULL không.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 04_write_csv.c -o 04_write_csv && ./04_write_csv && cat sensor.csv
 * Quan sát: chương trình báo đã ghi 3 dòng; "cat sensor.csv" cho thấy 1 dòng tiêu đề + 3 dòng số.
 */
#include <stdio.h>

int main(void) {
  int temps[3] = {25, 26, 27};
  int volts[3] = {3300, 3290, 3310};

  FILE *f = fopen("sensor.csv", "w");   /* "w": tạo file mới; nếu đã có thì XÓA nội dung cũ */
  if (f == NULL) {
    perror("Khong mo duoc sensor.csv");  /* perror in kèm lý do hệ điều hành báo (vd: Permission denied) */
    return 1;
  }

  fprintf(f, "id,temp,voltage\n");       /* dòng tiêu đề */
  for (int i = 0; i < 3; i++) {
    fprintf(f, "%d,%d,%d\n", i, temps[i], volts[i]);   /* giống printf, nhưng ghi vào file f */
  }
  fclose(f);                             /* đóng file: đẩy phần còn trong bộ đệm xuống đĩa */

  printf("Da ghi 3 dong du lieu vao sensor.csv\n");
  return 0;
}
