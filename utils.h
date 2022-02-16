#ifndef UTILS_H_
#define UTILS_H_

#include "parlay/sequence.h"

template <typename T, typename Allocator, bool EnableSSO>
void Debug(const parlay::sequence<T, Allocator, EnableSSO>& a) {
  for (auto x: a) {
    std::cout << x << ' ';
  }
  std::cout << std::endl;
}

int Rand(int n = -1);

#endif  // UTILS_H_