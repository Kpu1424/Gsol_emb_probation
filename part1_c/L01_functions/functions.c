/*
 * Bài 01 - Cấu trúc chương trình, hàm, tham số, giá trị trả về
 * Ví dụ xuyên suốt: đọc nhiệt độ từ cảm biến GIẢ LẬP.
 *
 * Build: make        Chạy: ./functions   (hoặc: make run)
 */

#include <stdio.h>   /* 1) #include: lấy khai báo printf */

/* 2) Hằng số dùng chung cho cả file */
#define SAMPLE_COUNT 5

/* 3) KHAI BÁO hàm (prototype): cho compiler biết tên, kiểu tham số,
 *    kiểu trả về. Phần thân (định nghĩa) nằm phía dưới main. */
int   read_fake_raw(int sample_index);
float raw_to_celsius(int raw);
int   is_overheat(float celsius, float limit);
void  print_reading(int index, float celsius);
void  try_reset(int value);

/* 4) main: điểm bắt đầu chương trình. Giá trị trả về gửi cho hệ điều hành. */
int main(void) {
  const float limit = 40.0f;
  int overheat_count = 0;

  for (int i = 0; i < SAMPLE_COUNT; i++) {
    int raw = read_fake_raw(i);           /* gọi hàm, nhận giá trị trả về */
    float celsius = raw_to_celsius(raw);  /* kết quả hàm này làm đầu vào hàm kia */
    print_reading(i, celsius);

    if (is_overheat(celsius, limit)) {
      overheat_count++;
    }
  }
  printf("So lan qua nhiet (> %.1f C): %d\n", limit, overheat_count);

  /* 5) Truyền THAM TRỊ: hàm nhận BẢN SAO, không đổi được biến gốc */
  int counter = 7;
  try_reset(counter);
  printf("Sau try_reset, counter van = %d\n", counter);

  return 0;  /* 0 = thành công. Xem bằng: ./functions; echo $? */
}

/* ===== ĐỊNH NGHĨA các hàm ===== */

/* Giả lập ADC: trả về nhiệt độ đơn vị 0.1 độ C (250 = 25.0 C) */
int read_fake_raw(int sample_index) {
  return 250 + sample_index * 60;
}

/* Đổi đơn vị: chia cho 10.0f (số thực) để KHÔNG bị chia nguyên */
float raw_to_celsius(int raw) {
  return raw / 10.0f;
}

/* Trả về 1 (đúng) hoặc 0 (sai) - C dùng int làm kiểu logic */
int is_overheat(float celsius, float limit) {
  return celsius > limit;
}

/* Hàm void: không trả về giá trị, chỉ làm việc (in ra) */
void print_reading(int index, float celsius) {
  printf("Mau %d: %.1f C\n", index, celsius);
}

void try_reset(int value) {
  value = 0;               /* chỉ đổi bản sao cục bộ */
  printf("Trong try_reset, value = %d\n", value);
}
