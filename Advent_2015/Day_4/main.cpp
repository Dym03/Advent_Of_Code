#include <iostream>
#include <openssl/md5.h>

int main() {
  std::string md5 = MD5(const unsigned char *d, size_t n, unsigned char *md);
  std::cout << md5 << "\n";
}
