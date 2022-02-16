#include "scan.h"

#include "parlay/parallel.h"
#include "parlay/sequence.h"

template <typename F>
int Work1(const parlay::sequence<int>& a, parlay::sequence<int>& b, int l,
          int r, F f, int granularity) {
  if (r - l + 1 <= granularity) {
    int res = 0;
    for (int i = l; i <= r; i++) res = f(res, a[i]);
    return res;
  }
  int mid = (l + r) / 2;
  int left, right;
  parlay::par_do([&]() { left = Work1(a, b, l, mid, f, granularity); },
                 [&]() { right = Work1(a, b, mid + 1, r, f, granularity); });
  b[mid] = left;
  return f(left, right);
}

template <typename F>
void Work2(const parlay::sequence<int>& a, const parlay::sequence<int>& b,
           parlay::sequence<int>& c, int l, int r, int offset, F f,
           int granularity) {
  if (r - l + 1 <= granularity) {
    c[l] = f(offset, a[l]);
    for (int i = l + 1; i <= r; i++) {
      c[i] = f(c[i - 1], a[i]);
    }
    return;
  }
  int mid = (l + r) / 2;
  parlay::par_do(
      [&]() { Work2(a, b, c, l, mid, offset, f, granularity); },
      [&]() { Work2(a, b, c, mid + 1, r, offset + b[mid], f, granularity); });
}

parlay::sequence<int> Scan(const parlay::sequence<int>& a, int granularity) {
  auto f = [](int a, int b) { return a + b; };
  int n = a.size();
  parlay::sequence<int> b(n);
  Work1(a, b, 0, n - 1, f, granularity);
  parlay::sequence<int> c(n);
  Work2(a, b, c, 0, n - 1, 0, f, granularity);
  return c;
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
