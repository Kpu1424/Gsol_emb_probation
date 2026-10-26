/*
 * Bài 08b - fork + exec + waitpid
 * Cha tạo 3 con, mỗi con exec chương trình ./sensor_child với kênh khác nhau.
 * exec THAY THẾ toàn bộ chương trình của process con bằng chương trình mới (giữ nguyên pid).
 *
 * Build: make   Chạy: ./fork_exec   (cần ./sensor_child cùng thư mục)
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N_CHILD 3

static pid_t spawn_sensor(const char *channel_text) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return -1;
  }
  if (pid == 0) {
    /* argv của chương trình mới: phần tử đầu là tên, kết thúc bằng NULL */
    char *const args[] = { "sensor_child", (char *)channel_text, NULL };
    execv("./sensor_child", args);
    /* Chỉ tới được dòng này khi exec THẤT BẠI */
    perror("execv ./sensor_child");
    _exit(127);   /* _exit trong con: không xả lại bộ đệm stdio của cha */
  }
  return pid;
}

int main(void) {
  const char *channels[N_CHILD] = { "0", "1", "9" };
  pid_t pids[N_CHILD];

  printf("[cha pid=%d] tao %d process con\n", (int)getpid(), N_CHILD);
  fflush(stdout);

  for (int i = 0; i < N_CHILD; i++) {
    pids[i] = spawn_sensor(channels[i]);
  }

  /* Các con chạy SONG SONG: thứ tự dòng in của con có thể khác nhau mỗi lần chạy */
  int failures = 0;
  for (int i = 0; i < N_CHILD; i++) {
    if (pids[i] < 0) { failures++; continue; }
    int status;
    waitpid(pids[i], &status, 0);
    int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    printf("[cha] con pid=%d (kenh %s) exit=%d\n", (int)pids[i], channels[i], code);
    if (code != 0) failures++;
  }
  printf("[cha] xong, %d con loi\n", failures);
  return failures ? 1 : 0;
}
