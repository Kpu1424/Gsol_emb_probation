/*
 * Ý chính: Header chỉ công khai 2 hàm; biến count bên trong counter.c KHÔNG được khai báo ở đây.
 * Build:   (xem main_right.c)
 * Quan sát: file khác chỉ đụng tới count qua 2 hàm này.
 */
#ifndef COUNTER_H
#define COUNTER_H

void counter_increment(void);
int counter_get(void);

#endif
