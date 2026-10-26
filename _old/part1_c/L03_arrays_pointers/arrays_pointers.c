/*
 * Bài 03 - Mảng, chuỗi, con trỏ, truyền dữ liệu vào hàm
 * Ví dụ: một mảng mẫu nhiệt độ (đơn vị 0.1 C) từ cảm biến giả lập.
 *
 * Build: make   Chạy: ./arrays_pointers
 */
#include <stdio.h>
#include <string.h>   /* strlen */

#define N_SAMPLES 6

/* Mảng truyền vào hàm sẽ "suy biến" (decay) thành CON TRỎ tới phần tử đầu.
 * Vì vậy LUÔN truyền kèm số phần tử n. 'const' = hàm hứa không sửa dữ liệu. */
double average(const int *data, size_t n) {
  long sum = 0;
  for (size_t i = 0; i < n; i++) {
    sum += data[i];          /* data[i] tương đương *(data + i) */
  }
  printf("  [average] sizeof(data) = %zu (kich thuoc CON TRO, khong phai mang)\n",
         sizeof(data));
  return n ? (double)sum / n : 0.0;
}

/* Muốn hàm "trả về" nhiều giá trị: truyền ĐỊA CHỈ biến của người gọi */
void find_min_max(const int *data, size_t n, int *out_min, int *out_max) {
  *out_min = data[0];   /* *out_min: ghi vào biến mà out_min trỏ tới */
  *out_max = data[0];
  for (size_t i = 1; i < n; i++) {
    if (data[i] < *out_min) *out_min = data[i];
    if (data[i] > *out_max) *out_max = data[i];
  }
}

/* Không có const -> hàm ĐƯỢC PHÉP sửa mảng gốc */
void apply_offset(int *data, size_t n, int offset) {
  for (size_t i = 0; i < n; i++) {
    data[i] += offset;
  }
}

int main(void) {
  /* ===== 1. Mảng ===== */
  int samples[N_SAMPLES] = {251, 248, 260, 255, 249, 270};
  size_t count = sizeof(samples) / sizeof(samples[0]);   /* chỉ đúng ở nơi khai báo mảng */

  printf("== Mang ==\n");
  printf("sizeof(samples) = %zu byte, so phan tu = %zu\n", sizeof(samples), count);
  printf("Trung binh = %.1f (x0.1 C)\n", average(samples, count));

  /* ===== 2. Con trỏ ===== */
  printf("\n== Con tro ==\n");
  int *p = samples;          /* tên mảng -> địa chỉ phần tử đầu, giống &samples[0] */
  printf("p tro toi samples[0]: *p = %d\n", *p);
  printf("*(p + 2) = %d (giong samples[2])\n", *(p + 2));
  printf("Dia chi samples[0] = %p, samples[1] = %p (cach nhau sizeof(int) = %zu)\n",
         (void *)&samples[0], (void *)&samples[1], sizeof(int));

  int min_v, max_v;
  find_min_max(samples, count, &min_v, &max_v);   /* & = lấy địa chỉ */
  printf("min = %d, max = %d\n", min_v, max_v);

  apply_offset(samples, count, -5);                /* hàm sửa trực tiếp mảng gốc */
  printf("Sau apply_offset(-5): samples[0] = %d\n", samples[0]);

  /* ===== 3. Chuỗi = mảng char kết thúc bằng '\0' ===== */
  printf("\n== Chuoi ==\n");
  char name[16] = "TEMP_01";                       /* mảng, sửa được */
  const char *unit = "C";                          /* trỏ tới chuỗi hằng, KHÔNG được sửa */
  printf("name = \"%s\", strlen = %zu, sizeof = %zu\n", name, strlen(name), sizeof(name));

  name[5] = '9';                                   /* sửa 1 ký tự trong mảng */
  printf("Sau khi sua name[5]: %s\n", name);

  char label[32];
  /* snprintf: ghi chuỗi có giới hạn kích thước -> không tràn bộ đệm */
  snprintf(label, sizeof(label), "%s=%d.%d%s", name, samples[0] / 10, samples[0] % 10, unit);
  printf("label = %s\n", label);

  return 0;
}
