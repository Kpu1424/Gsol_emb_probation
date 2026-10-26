/*
 * Ý chính: Đọc cả dòng bằng fgets rồi đổi sang số bằng strtol, kiểm tra cả dòng có đúng là một số hợp lệ không.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 03_fgets_strtol_right.c -o 03_fgets_strtol_right && ./03_fgets_strtol_right < 03_fgets_strtol_right.in
 * Quan sát: "abc", "25abc", số quá lớn bị từ chối kèm lý do; "25" hợp lệ; hết input (EOF) thì dừng gọn.
 */
#include <stdio.h>
#include <stdlib.h>   /* strtol */
#include <errno.h>    /* errno, ERANGE */

int main(void) {
  char line[32];

  printf("Nhap nhiet do, moi dong mot so:\n");
  while (fgets(line, sizeof(line), stdin) != NULL) {   /* NULL = hết dữ liệu (EOF, hoặc Ctrl+D khi gõ tay) */
    printf("Doc duoc dong: %s", line);                  /* line đã có sẵn '\n' ở cuối */

    char *end;
    errno = 0;
    long value = strtol(line, &end, 10);   /* end trỏ tới ký tự đầu tiên KHÔNG thuộc về số */

    if (end == line) {
      printf("  Tu choi: khong co chu so nao\n");
    } else if (errno == ERANGE) {
      printf("  Tu choi: so qua lon, vuot gioi han cua long\n");
    } else if (*end != '\n' && *end != '\0') {
      printf("  Tu choi: co ky tu thua '%c' sau so\n", *end);
    } else {
      printf("  Hop le: nhiet do = %ld C\n", value);
    }
  }
  printf("Het du lieu (EOF), ket thuc.\n");
  return 0;
}
