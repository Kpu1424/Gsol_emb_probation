/*
 * Ý chính: fwrite ghi nguyên các byte của struct trong bộ nhớ, gồm cả padding và thứ tự byte của máy này -> file không portable.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 06_binary_file.c -o 06_binary_file && ./06_binary_file
 * Quan sát: file 8 byte dù dữ liệu chỉ 5 byte; 3 byte "00" sau id là padding; 3300 = 0x00000CE4 được ghi ngược thành E4 0C 00 00.
 */
#include <stdio.h>
#include <string.h>   /* memset */

struct record {
  char id;       /* 1 byte, sau nó compiler chèn 3 byte padding (xem L04 04_padding_sizeof.c) */
  int voltage;   /* 4 byte */
};

int main(void) {
  struct record r;
  memset(&r, 0, sizeof(r));   /* xóa cả byte padding về 0, nếu không chúng chứa giá trị rác bất kỳ */
  r.id = 1;
  r.voltage = 3300;

  FILE *f = fopen("record.bin", "wb");   /* "b": nhị phân (Linux không khác, Windows có khác) */
  if (f == NULL) { perror("Khong mo duoc record.bin"); return 1; }
  fwrite(&r, sizeof(r), 1, f);            /* ghi 1 khối sizeof(r) byte, y nguyên như trong RAM */
  fclose(f);

  unsigned char bytes[16];
  f = fopen("record.bin", "rb");
  if (f == NULL) { perror("Khong mo duoc record.bin"); return 1; }
  size_t n = fread(bytes, 1, sizeof(bytes), f);   /* đọc lại từng byte, trả về số byte đọc được */
  fclose(f);

  printf("sizeof(struct record) = %zu, du lieu that = 1 + 4 = 5 byte\n", sizeof(struct record));
  printf("record.bin co %zu byte:", n);
  for (size_t i = 0; i < n; i++) {
    printf(" %02X", bytes[i]);
  }
  printf("\n");
  return 0;
}
