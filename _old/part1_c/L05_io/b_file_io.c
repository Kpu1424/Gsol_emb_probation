/*
 * Bài 05b - Đọc/ghi file
 *   Phần 1: ghi log cảm biến dạng TEXT (CSV), rồi đọc lại và tính trung bình.
 *   Phần 2: ghi/đọc dạng NHỊ PHÂN (binary) bằng fwrite/fread.
 *
 * Build: make   Chạy: ./b_file_io   (tạo sensor_log.csv và sensor_log.bin trong thư mục hiện tại)
 */
#include <stdio.h>
#include <stdint.h>

#define CSV_PATH "sensor_log.csv"
#define BIN_PATH "sensor_log.bin"
#define N 5

typedef struct {
  uint32_t timestamp_ms;
  int16_t  temp_x10;
} record_t;

static int write_csv(const char *path) {
  FILE *f = fopen(path, "w");          /* "w": tạo mới / ghi đè */
  if (f == NULL) {
    perror("fopen ghi CSV");           /* in lý do lỗi của hệ điều hành */
    return -1;
  }
  fprintf(f, "timestamp_ms,temp_x10\n");
  for (int i = 0; i < N; i++) {
    fprintf(f, "%d,%d\n", i * 1000, 250 + i * 3);   /* dữ liệu giả lập */
  }
  fclose(f);                           /* đóng file: đẩy dữ liệu xuống đĩa, giải phóng tài nguyên */
  return 0;
}

static int read_csv(const char *path) {
  FILE *f = fopen(path, "r");          /* "r": chỉ đọc, file phải tồn tại */
  if (f == NULL) {
    perror("fopen doc CSV");
    return -1;
  }
  char line[128];
  int line_no = 0, n = 0;
  long sum = 0;
  while (fgets(line, sizeof line, f) != NULL) {     /* đọc từng dòng đến hết file */
    line_no++;
    unsigned long ts;
    int temp;
    if (sscanf(line, "%lu,%d", &ts, &temp) != 2) {  /* sscanf trả về số trường đọc được */
      printf("  dong %d bo qua: %s", line_no, line);
      continue;
    }
    printf("  t=%5lu ms  temp=%d.%d C\n", ts, temp / 10, temp % 10);
    sum += temp;
    n++;
  }
  fclose(f);
  if (n > 0) printf("  -> %d ban ghi, trung binh %.2f C\n", n, sum / 10.0 / n);
  return 0;
}

static int write_and_read_binary(const char *path) {
  record_t out[N];
  for (int i = 0; i < N; i++) {
    out[i].timestamp_ms = (uint32_t)(i * 1000);
    out[i].temp_x10 = (int16_t)(250 + i * 3);
  }

  FILE *f = fopen(path, "wb");         /* "b": binary (trên Linux không khác, trên Windows có khác) */
  if (f == NULL) { perror("fopen wb"); return -1; }
  size_t written = fwrite(out, sizeof(record_t), N, f);   /* ghi nguyên khối bộ nhớ */
  fclose(f);
  printf("  fwrite: %zu ban ghi x %zu byte = %zu byte\n",
         written, sizeof(record_t), written * sizeof(record_t));

  record_t in[N];
  f = fopen(path, "rb");
  if (f == NULL) { perror("fopen rb"); return -1; }
  size_t got = fread(in, sizeof(record_t), N, f);
  fclose(f);
  printf("  fread : %zu ban ghi, in[4].temp_x10 = %d\n", got, in[4].temp_x10);
  printf("  Chu y: file nhi phan chua ca PADDING va thu tu byte cua MAY NAY,\n"
         "         may khac (kien truc/compiler khac) co the doc sai.\n");
  return 0;
}

int main(void) {
  printf("== Ghi CSV: %s ==\n", CSV_PATH);
  if (write_csv(CSV_PATH) != 0) return 1;

  printf("== Doc lai CSV ==\n");
  if (read_csv(CSV_PATH) != 0) return 1;

  printf("== Binary: %s ==\n", BIN_PATH);
  if (write_and_read_binary(BIN_PATH) != 0) return 1;

  printf("== Thu mo file khong ton tai ==\n");
  read_csv("khong_co_file_nay.csv");   /* cố ý: xem perror in gì */
  return 0;
}
