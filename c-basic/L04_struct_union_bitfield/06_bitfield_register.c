/*
 * Ý chính: Bit field cho mỗi thành viên chỉ chiếm vài bit, dùng để mô phỏng thanh ghi trạng thái; giá trị vượt số bit bị CẮT.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 06_bitfield_register.c -o 06_bitfield_register
 * Quan sát: sizeof = 4 dù chỉ dùng 8 bit; raw = 0x99; gán mode = 5 (nhị phân 101) đọc lại chỉ còn 1 (nhị phân 01), raw thành 0x95.
 * WARNING CỐ Ý: gcc báo -Woverflow ở dòng "reg.bits.mode = 5" vì 5 không vừa 2 bit.
 */
#include <stdio.h>

struct status_reg {
  unsigned int enable  : 1;   /* 1 bit: 0..1  */
  unsigned int error   : 1;   /* 1 bit: 0..1  */
  unsigned int mode    : 2;   /* 2 bit: 0..3  */
  unsigned int channel : 4;   /* 4 bit: 0..15 */
};

/* Xem cùng vùng nhớ như một số nguyên, giống đọc nguyên thanh ghi một lần */
union status_view {
  struct status_reg bits;
  unsigned int raw;
};

int main(void) {
  union status_view reg;
  reg.raw = 0;   /* xóa hết bit về 0 */

  printf("sizeof(struct status_reg) = %zu byte\n", sizeof(struct status_reg));

  reg.bits.enable = 1;
  reg.bits.mode = 2;
  reg.bits.channel = 9;
  printf("enable = %u, error = %u, mode = %u, channel = %u -> raw = 0x%02X\n",
         reg.bits.enable, reg.bits.error, reg.bits.mode, reg.bits.channel, reg.raw);

  reg.bits.mode = 5;   /* 5 cần 3 bit, trường chỉ có 2 bit -> chỉ giữ 2 bit thấp (01) */
  printf("Sau khi gan mode = 5: mode = %u, raw = 0x%02X\n", reg.bits.mode, reg.raw);
  return 0;
}
