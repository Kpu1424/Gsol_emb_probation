/*
 * Ý chính: extern = KHAI BÁO biến toàn cục: "biến này có thật, nằm ở một file .c khác".
 * Build:   (xem main.c)
 * Quan sát: dòng extern không cấp bộ nhớ; bộ nhớ được cấp ở config.c.
 */
#ifndef CONFIG_H
#define CONFIG_H

extern int g_temp_limit;   /* khai báo, KHÔNG có "= 40" */
void config_set_limit(int limit);

#endif
