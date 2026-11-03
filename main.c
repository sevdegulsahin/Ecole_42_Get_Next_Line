#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
int main(void) {
  int fd;
  char *line;
  int i;
  int has_newline;
  fd = open("test.txt", O_RDONLY);
  if (fd < 0) {
    return 1;
  }
  while (((line = get_next_line(fd)) != NULL)) {
    i = 0;
    has_newline = 0;

    // 1. Adım: Satırın sonuna kadar git
    while (line[i])
      i++;

    // 2. Adım: Eğer satırın en sonu \n ise, onu ayır
    if (i > 0 && line[i - 1] == '\n') {
      has_newline = 1;
      i--; // Döngünün \n karakterini basmaması için indeksi 1 geri çek
    }

    // 3. Adım: Harfleri tersten bas (artık \n karakteri aralarında yok)
    while (--i >= 0) {
      printf("%c", line[i]);
    }

    // 4. Adım: Ayırdığımız \n karakterini düzgün görünmesi için en son ekle
    if (has_newline == 1) {
      printf("\n");
    }

    free(line);
  }
  close(fd);
  return (0);
}