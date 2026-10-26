/*
 * Bài 10b - Sửa race condition bằng mutex
 * Cùng bài toán với race.c, nhưng mọi truy cập g_sample_count đều nằm trong vùng khóa.
 *
 * Build: make   Chạy: ./mutex
 */
#include <stdio.h>
#include <pthread.h>

#define N_THREADS  4
#define N_INCREMENT 1000000

long g_sample_count = 0;
pthread_mutex_t g_count_lock = PTHREAD_MUTEX_INITIALIZER;   /* khởi tạo tĩnh */

static void *receiver(void *arg) {
  (void)arg;
  for (int i = 0; i < N_INCREMENT; i++) {
    pthread_mutex_lock(&g_count_lock);     /* chỉ 1 thread được vào cùng lúc */
    g_sample_count++;                      /* VÙNG GĂNG (critical section): càng ngắn càng tốt */
    pthread_mutex_unlock(&g_count_lock);   /* quên unlock -> các thread khác chờ mãi (deadlock) */
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
  pthread_mutex_destroy(&g_count_lock);
  return 0;
}
