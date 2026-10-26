/*
 * Bài 08b (phần con) - chương trình độc lập sẽ được exec bởi fork_exec.
 * Cách dùng: ./sensor_child <channel>
 * Exit status: 0 = đọc thành công, 2 = sai tham số.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Cach dung: %s <channel>\n", argv[0]);
    return 2;
  }
  int channel = atoi(argv[1]);
  printf("  [sensor_child pid=%d] kenh %d = %.1f C\n",
         (int)getpid(), channel, 20.0 + channel * 2.5);
  return 0;
}
