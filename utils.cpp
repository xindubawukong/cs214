#include "utils.h"

#include <cstdlib>

int Rand(int n) {
  if (n == -1) return rand();
  return rand() % n;
}
