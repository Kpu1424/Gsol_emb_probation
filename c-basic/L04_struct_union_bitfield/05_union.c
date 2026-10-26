/*
 * Ý chính: Mọi thành viên của union dùng CHUNG một vùng nhớ: ghi vào thành viên này thì đọc thành viên kia thấy thay đổi.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 05_union.c -o 05_union
 * Quan sát: 2 địa chỉ giống nhau; sizeof = 4; ghi value = 0x11223344 thì bytes = 44 33 22 11 (máy little-endian). Địa chỉ sẽ khác trên máy bạn.
 */
#include <stdio.h>

union word {
  unsigned int value;       /* nhìn 4 byte như 1 số (unsigned int = 4 byte trên máy này) */
  unsigned char bytes[4];   /* nhìn chính 4 byte đó như 4 ô riêng */
};

int main(void) {
  union word w;

  printf("Dia chi w.value: %p\n", (void *)&w.value);
  printf("Dia chi w.bytes: %p\n", (void *)&w.bytes);
  printf("sizeof(union word) = %zu byte (bang thanh vien lon nhat, khong phai tong)\n", sizeof(union word));

  w.value = 0x11223344;
  printf("Ghi w.value = 0x%08X\n", w.value);
  /* bytes[0] = 44: byte thấp nằm ở địa chỉ thấp -> máy little-endian */
  printf("Doc w.bytes = %02X %02X %02X %02X\n", w.bytes[0], w.bytes[1], w.bytes[2], w.bytes[3]);

  w.bytes[0] = 0xFF;   /* sửa 1 byte cũng là sửa một phần của value */
  printf("Sau khi ghi w.bytes[0] = 0xFF: w.value = 0x%08X\n", w.value);
  return 0;
}
