/*
 * Bài 10a - RACE CONDITION (CHƯƠNG TRÌNH NÀY CÓ LỖI, CỐ Ý)
 * 4 thread cùng tăng 1 bộ đếm "số mẫu đã nhận" dùng chung, KHÔNG khóa.
 *
 * QUAN TRỌNG: kể cả khi kết quả in ra đúng, chương trình vẫn SAI.
 * Đây là data race -> hành vi không xác định theo chuẩn C. Đúng vài lần không chứng minh gì.
 *
 * Build: make   Chạy: ./race   (chạy nhiều lần: make race5)
 */
#include <stdio.h>
#include <pthread.h>

#define N_THREADS  4
#define N_INCREMENT 1000000

long g_sample_count = 0;   /* DỮ LIỆU DÙNG CHUNG giữa các thread */

static void *receiver(void *arg) {
  (void)arg;
  for (int i = 0; i < N_INCREMENT; i++) {
    g_sample_count++;      /* KHÔNG nguyên tử: đọc -> cộng 1 -> ghi; thread khác có thể chen giữa */
  }
  return NULL;
}

int main(void) {
  pthread_t t[N_THREADS];
  for (int i = 0; i < N_THREADS; i++) pthread_create(&t[i], NULL, receiver, NULL);
  for (int i = 0; i < N_THREADS; i++) pthread_join(t[i], NULL);

  long expected = (long)N_THREADS * N_INCREMENT;
  printf("Mong doi: %ld, thuc te: %ld, mat: %ld\n",
         expected, g_sample_count, expected - g_sample_count);
  return 0;
}
