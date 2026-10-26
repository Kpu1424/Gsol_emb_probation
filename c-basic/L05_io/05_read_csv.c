/*
 * Ý chính: Đọc file văn bản từng dòng bằng fgets, tách các cột bằng sscanf; sscanf trả về số cột tách được.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 05_read_csv.c -o 05_read_csv && ./05_read_csv   (chạy 04_write_csv trước để có sensor.csv)
 * Quan sát: dòng tiêu đề bị bỏ qua (sscanf không tách được 3 số); 3 dòng dữ liệu được in lại; trung bình temp = 26.
 */
#include <stdio.h>

int main(void) {
  FILE *f = fopen("sensor.csv", "r");   /* "r": chỉ đọc, file phải có sẵn */
  if (f == NULL) {
    perror("Khong mo duoc sensor.csv (hay chay 04_write_csv truoc)");
    return 1;
  }

  char line[64];
  int id, temp, voltage;
  int count = 0, sum = 0;
  while (fgets(line, sizeof(line), f) != NULL) {   /* NULL = đã đọc hết file */
    if (sscanf(line, "%d,%d,%d", &id, &temp, &voltage) != 3) {
      printf("Bo qua dong: %s", line);              /* tiêu đề là chữ, không phải 3 số */
      continue;
    }
    printf("id = %d, temp = %d C, voltage = %d mV\n", id, temp, voltage);
    count++;
    sum += temp;
  }
  fclose(f);

  if (count > 0) {   /* tránh chia cho 0 khi file không có dòng dữ liệu nào */
    printf("Doc duoc %d dong du lieu, trung binh temp = %d C\n", count, sum / count);
  }
  return 0;
}
