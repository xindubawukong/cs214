#include "scan.h"

#include "parlay/sequence.h"

parlay::sequence<int> Scan(const parlay::sequence<int>& a, int granularity) {
  int n = a.size();
  parlay::sequence<int> b(n);
  parlay::sequence<int> c(n);
  auto f = [](int a, int b) { return a + b; };
  Scan(parlay::make_slice(a), parlay::make_slice(b), parlay::make_slice(c), f,
       0, granularity);
  return b;
}

void ScanTest() {
  int n = 1000000;
  parlay::sequence<int> a(n);
  for (int i = 0; i < n; i++) a[i] = i;
  parlay::sequence<int> b(n);
  b[0] = a[0];
  for (int i = 0; i < n; i++) b[i] = b[i - 1] + a[i];
  parlay::sequence<int> c = Scan(a, 5);
  bool pass = true;
  for (int i = 0; i < n; i++) {
    assert(b[i] == c[i]);
  }
  printf("ScanTest: pass!\n");
}
