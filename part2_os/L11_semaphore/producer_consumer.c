/*
 * Bài 11 - Semaphore và bài toán producer–consumer
 * Producer: "driver cảm biến" sinh mẫu và đẩy vào hàng đợi vòng (ring buffer) 4 ô.
 * Consumer: "bộ xử lý" lấy mẫu ra và tính trung bình.
 *
 *   sem_empty: số ô TRỐNG còn lại  (khởi tạo = BUF_SIZE) -> producer chờ khi đầy
 *   sem_full : số ô ĐÃ CÓ dữ liệu  (khởi tạo = 0)        -> consumer chờ khi rỗng
 *   mutex    : bảo vệ chỉ số head/tail khi có nhiều producer/consumer
 *
 * Build: make   Chạy: ./producer_consumer
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUF_SIZE   4
#define N_SAMPLES  10
#define STOP_VALUE -1      /* giá trị đặc biệt báo consumer dừng */

static int buffer[BUF_SIZE];
static int head = 0, tail = 0;           /* head: chỗ ghi tiếp, tail: chỗ đọc tiếp */
static sem_t sem_empty, sem_full;
static pthread_mutex_t buf_lock = PTHREAD_MUTEX_INITIALIZER;

static void sleep_ms(long ms) {
  struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
  nanosleep(&ts, NULL);
}

static void put_item(int value) {
  sem_wait(&sem_empty);                  /* giảm số ô trống; nếu = 0 thì CHỜ */
  pthread_mutex_lock(&buf_lock);
  buffer[head] = value;
  head = (head + 1) % BUF_SIZE;
  pthread_mutex_unlock(&buf_lock);
  sem_post(&sem_full);                   /* tăng số ô có dữ liệu, đánh thức consumer */
}

static int get_item(void) {
  sem_wait(&sem_full);                   /* nếu buffer rỗng thì CHỜ */
  pthread_mutex_lock(&buf_lock);
  int value = buffer[tail];
  tail = (tail + 1) % BUF_SIZE;
  pthread_mutex_unlock(&buf_lock);
  sem_post(&sem_empty);                  /* trả lại 1 ô trống */
  return value;
}

static void *producer(void *arg) {
  (void)arg;
  for (int i = 0; i < N_SAMPLES; i++) {
    int sample = 250 + (i * 7) % 20;     /* mẫu giả lập, đơn vị 0.1 C */
    put_item(sample);
    printf("  [producer] day mau %2d: %d\n", i, sample);
    sleep_ms(5);                         /* producer nhanh hơn consumer */
  }
  put_item(STOP_VALUE);
  return NULL;
}

static void *consumer(void *arg) {
  double *avg_out = arg;
  long sum = 0;
  int n = 0;
  for (;;) {
    int sample = get_item();
    if (sample == STOP_VALUE) break;
    sum += sample;
    n++;
    printf("  [consumer]          lay %d (tong %d mau)\n", sample, n);
    sleep_ms(20);                        /* xử lý chậm -> buffer sẽ đầy, producer phải chờ */
  }
  *avg_out = n ? sum / 10.0 / n : 0.0;
  return NULL;
}

int main(void) {
  /* sem_init(sem, pshared=0: dùng giữa các thread cùng process, giá trị ban đầu) */
  if (sem_init(&sem_empty, 0, BUF_SIZE) != 0 || sem_init(&sem_full, 0, 0) != 0) {
    perror("sem_init");
    return 1;
  }

  pthread_t prod, cons;
  double average = 0.0;
  pthread_create(&cons, NULL, consumer, &average);
  pthread_create(&prod, NULL, producer, NULL);
  pthread_join(prod, NULL);
  pthread_join(cons, NULL);

  printf("[main] trung binh %d mau = %.2f C\n", N_SAMPLES, average);
  sem_destroy(&sem_empty);
  sem_destroy(&sem_full);
  return 0;
}
