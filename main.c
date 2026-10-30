#include "get_next_line.h"
#include <fcntl.h> // open() fonksiyonu için
#include <stdio.h> // printf için

int main(void) {
  int fd;
  char *line;

  // 1. Oluşturduğumuz dosyayı "sadece okuma" (O_RDONLY) modunda açıyoruz.

  if (fd == -1) {
    printf("Hata: Dosya acilamadi!\n");
    return (1);
  }

  // 2. get_next_line fonksiyonunu bir döngü içinde çağırıyoruz.
  // Fonksiyon okunacak bir şey kalmadığında NULL döndüreceği için döngü o zaman
  // duracak[cite: 8].
  while ((line = get_next_line(0)) != NULL) {
    printf("%s", line); // Satırı ekrana yazdır (GNL zaten sonuna \n eklediği
                        // için \n koymuyoruz).

    // ÖNEMLİ: GNL içinde malloc ile ürettiğimiz satırın (line) işi bitti.
    // Memory leak (bellek sızıntısı) olmaması için onu burada free
    // etmeliyiz[cite: 4].
    free(line);
  }

  // 3. İşimizi bitirince dosyayı kapatıyoruz.

  return (0);
}