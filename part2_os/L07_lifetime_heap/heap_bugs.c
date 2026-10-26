/*
 * Bài 07b - CÁC LỖI BỘ NHỚ (cố ý) để quan sát bằng AddressSanitizer (ASan)
 * KHÔNG viết code như thế này trong chương trình thật.
 *
 * Build: make heap_bugs     (đã bật -fsanitize=address)
 * Chạy:  ./heap_bugs leak | uaf | double | overflow
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Cach dung: %s leak|uaf|double|overflow\n", argv[0]);
    return 1;
  }
  const char *mode = argv[1];
  int *samples = malloc(4 * sizeof *samples);
  if (samples == NULL) return 1;

  if (strcmp(mode, "leak") == 0) {
    samples = NULL;               /* mất con trỏ duy nhất -> không bao giờ free được: RÒ RỈ */
    printf("Da lam mat con tro (leak). ASan bao khi chuong trinh ket thuc.\n");
  } else if (strcmp(mode, "uaf") == 0) {
    free(samples);
    printf("Doc sau khi free: %d\n", samples[0]);   /* USE-AFTER-FREE */
  } else if (strcmp(mode, "double") == 0) {
    free(samples);
    free(samples);                /* DOUBLE FREE */
  } else if (strcmp(mode, "overflow") == 0) {
    samples[4] = 1;               /* ghi vượt biên: chỉ có samples[0..3] */
    free(samples);
  } else {
    printf("Che do khong biet: %s\n", mode);
    free(samples);
  }
  return 0;
}
