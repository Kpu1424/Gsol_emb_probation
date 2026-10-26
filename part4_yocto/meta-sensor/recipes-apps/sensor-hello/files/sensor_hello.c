/*
 * sensor_hello.c - Hello World chạy trên board (được build bởi Yocto recipe sensor-hello)
 * In thông tin hệ thống để chứng minh chương trình chạy trên TARGET, kèm vài mẫu cảm biến giả lập.
 */
#include <stdio.h>
#include <sys/utsname.h>

int main(void) {
  struct utsname u;

  printf("Hello World from sensor-hello!\n");
  if (uname(&u) == 0) {
    printf("host=%s kernel=%s arch=%s\n", u.nodename, u.release, u.machine);
  }
  for (int i = 0; i < 3; i++) {
    int raw = 250 + i * 3;   /* giả lập, đơn vị 0.1 C */
    printf("sample %d: %d.%d C\n", i, raw / 10, raw % 10);
  }
  return 0;
}
