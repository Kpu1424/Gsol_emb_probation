/*
 * Bài 06 - Các vùng nhớ của một process: text, data, BSS, stack, heap
 * Chương trình in địa chỉ của từng loại biến, rồi tra /proc/self/maps (Linux)
 * để xem địa chỉ đó thuộc vùng nào trong MÔI TRƯỜNG ĐANG CHẠY.
 *
 * Build: make   Chạy: ./memory_layout
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ---- Biến toàn cục ---- */
int  g_calib_offset = 15;           /* có giá trị khởi tạo khác 0  -> .data */
int  g_sample_buffer[1024];         /* không khởi tạo (=0)         -> .bss  */
const char g_sensor_name[] = "TEMP_01";  /* hằng                   -> .rodata (thường đi cùng text) */

void read_sensor(void) { }          /* mã lệnh của hàm                -> .text */

/* Tìm dòng trong /proc/self/maps chứa địa chỉ addr và in ra */
static void where_is(const char *label, const void *addr) {
  FILE *f = fopen("/proc/self/maps", "r");
  if (f == NULL) { perror("fopen /proc/self/maps"); return; }

  uintptr_t a = (uintptr_t)addr;
  char line[512];
  while (fgets(line, sizeof line, f) != NULL) {
    unsigned long start, end;
    char perms[8];
    if (sscanf(line, "%lx-%lx %7s", &start, &end, perms) != 3) continue;
    if (a >= start && a < end) {
      /* Tên mapping nằm ở cột cuối (nếu có): file thực thi, [heap], [stack]... */
      char *name = strrchr(line, ' ');
      name = (name != NULL) ? name + 1 : line;
      name[strcspn(name, "\n")] = '\0';
      printf("  %-26s %p  perms=%s  %s\n", label, addr, perms,
             *name ? name : "(anonymous)");
      break;
    }
  }
  fclose(f);
}

static void stack_demo(int depth) {
  int local_in_deeper_call = depth;   /* mỗi lần gọi hàm có stack frame riêng */
  printf("  depth %d: &local = %p\n", depth, (void *)&local_in_deeper_call);
  if (depth < 2) stack_demo(depth + 1);
}

int main(void) {
  static int s_call_count = 0;        /* static cục bộ, khởi tạo 0 -> .bss */
  static int s_threshold = 40;        /* static cục bộ, khởi tạo khác 0 -> .data */
  int local_reading = 253;            /* biến cục bộ (automatic) -> stack */
  int *heap_samples = malloc(16 * sizeof *heap_samples);   /* cấp phát động -> heap */
  if (heap_samples == NULL) { perror("malloc"); return 1; }

  (void)s_call_count; (void)s_threshold;

  printf("== Dia chi va vung nho (tra theo /proc/self/maps) ==\n");
  where_is("text: read_sensor()",     (const void *)(uintptr_t)read_sensor);
  where_is("rodata: g_sensor_name",   g_sensor_name);
  where_is("data: g_calib_offset",    &g_calib_offset);
  where_is("data: static s_threshold",&s_threshold);
  where_is("bss: g_sample_buffer",    g_sample_buffer);
  where_is("bss: static s_call_count",&s_call_count);
  where_is("heap: heap_samples",      heap_samples);
  where_is("stack: local_reading",    &local_reading);

  printf("\n== Stack: moi lan goi ham tao frame moi ==\n");
  stack_demo(0);
  printf("  (Tren x86_64 Linux stack thuong lon len theo huong dia chi GIAM,\n"
         "   nhung khong phai kien truc nao cung vay.)\n");

  printf("\nGoi y: chay lai vai lan - dia chi doi do ASLR (ngau nhien hoa dia chi).\n");
  free(heap_samples);
  return 0;
}
