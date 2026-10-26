/*
 * Bài 08a - fork + waitpid
 * Process cha tạo process con để "đọc cảm biến"; con trả kết quả qua EXIT STATUS (0..255).
 *
 * Build: make   Chạy: ./fork_wait
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* fork, getpid, getppid */
#include <sys/wait.h>   /* waitpid, WIFEXITED, WEXITSTATUS */

int g_reading = 0;      /* sau fork, cha và con có BẢN SAO RIÊNG của biến này */

int main(void) {
  printf("[cha ] pid=%d bat dau, g_reading=%d\n", (int)getpid(), g_reading);
  fflush(stdout);       /* xả bộ đệm TRƯỚC fork, tránh dòng trên bị in 2 lần */

  pid_t pid = fork();   /* từ đây có 2 process cùng chạy tiếp dòng dưới */
  if (pid < 0) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    /* ===== Nhánh CON: fork() trả về 0 ===== */
    g_reading = 42;     /* chỉ sửa bản sao của con */
    printf("[con ] pid=%d ppid=%d, doc cam bien = %d\n",
           (int)getpid(), (int)getppid(), g_reading);
    exit(g_reading);    /* gửi kết quả cho cha qua exit status */
  }

  /* ===== Nhánh CHA: fork() trả về pid của con ===== */
  int status = 0;
  if (waitpid(pid, &status, 0) < 0) {   /* chờ con kết thúc, thu dọn (tránh zombie) */
    perror("waitpid");
    return 1;
  }
  if (WIFEXITED(status)) {
    printf("[cha ] con %d ket thuc, exit status = %d\n", (int)pid, WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("[cha ] con %d bi tin hieu %d giet\n", (int)pid, WTERMSIG(status));
  }
  printf("[cha ] g_reading cua cha van = %d (bo nho KHONG dung chung)\n", g_reading);
  return 0;
}
