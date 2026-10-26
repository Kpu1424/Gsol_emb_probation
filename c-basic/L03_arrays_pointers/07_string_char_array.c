/*
 * Ý chính: Chuỗi trong C là mảng char kết thúc bằng ký tự '\0'; strlen đếm ký tự trước '\0', sizeof đếm cả mảng.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 07_string_char_array.c -o 07_string_char_array
 * Quan sát: name[4] = 0 (ký tự '\0'); strlen = 4 nhưng sizeof = 5; sửa name[0] thì chuỗi đổi thành "WEMP".
 */
#include <stdio.h>
#include <string.h>   /* strlen */

int main(void) {
  char name[] = "TEMP";   /* 4 chữ + '\0' do compiler tự thêm = mảng 5 phần tử */

  for (int i = 0; i < 4; i++) {
    printf("name[%d] = '%c' (ma ASCII %d)\n", i, name[i], name[i]);
  }
  printf("name[4] = %d (ky tu '\\0' danh dau het chuoi)\n", name[4]);

  printf("strlen(name) = %zu\n", strlen(name));
  printf("sizeof(name) = %zu\n", sizeof(name));

  name[0] = 'W';   /* mảng char của mình nên sửa được từng ký tự */
  printf("Sau khi sua name[0]: %s\n", name);
  return 0;
}
