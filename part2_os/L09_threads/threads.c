/*
 * Bài 09 - pthread_create và pthread_join
 * 3 thread, mỗi thread giả lập 1 cảm biến: đọc N mẫu và tính trung bình.
 * Mỗi thread nhận 1 struct RIÊNG làm tham số và ghi kết quả vào đó.
 *
 * Build: make   Chạy: ./threads
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <time.h>      /* nanosleep */

#define N_SENSORS 3
#define N_SAMPLES 5

typedef struct {
  int    channel;      /* đầu vào  */
  int    base_x10;     /* đầu vào  */
  double average;      /* đầu ra: thread ghi vào */
} sensor_task_t;

static void sleep_ms(long ms) {
  struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
  nanosleep(&ts, NULL);
}

/* Hàm thread: nhận void*, trả về void* (quy ước của pthread) */
static void *sensor_thread(void *arg) {
  sensor_task_t *task = arg;            /* ép lại về đúng kiểu */
  long sum = 0;
  for (int i = 0; i < N_SAMPLES; i++) {
    int sample = task->base_x10 + i;
    sum += sample;
    sleep_ms(10 * (task->channel + 1)); /* giả lập thời gian đọc khác nhau */
  }
  task->average = sum / 10.0 / N_SAMPLES;
  printf("  [thread kenh %d] xong, trung binh %.2f C\n", task->channel, task->average);
  return NULL;
}

int main(void) {
  pthread_t threads[N_SENSORS];
  sensor_task_t tasks[N_SENSORS];       /* MỖI thread 1 phần tử riêng -> không tranh chấp */

  for (int i = 0; i < N_SENSORS; i++) {
    tasks[i].channel = i;
    tasks[i].base_x10 = 200 + i * 50;
    tasks[i].average = 0.0;
    int rc = pthread_create(&threads[i], NULL, sensor_thread, &tasks[i]);
    if (rc != 0) {                      /* pthread trả mã lỗi, KHÔNG đặt errno */
      fprintf(stderr, "pthread_create loi %d\n", rc);
      return 1;
    }
  }
  printf("[main] da tao %d thread, cho join...\n", N_SENSORS);

  for (int i = 0; i < N_SENSORS; i++) {
    pthread_join(threads[i], NULL);     /* chờ thread i kết thúc */
  }

  /* Sau join mới chắc chắn đọc kết quả an toàn */
  for (int i = 0; i < N_SENSORS; i++) {
    printf("[main] kenh %d: %.2f C\n", tasks[i].channel, tasks[i].average);
  }
  return 0;
}
