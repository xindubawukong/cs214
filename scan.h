#ifndef SCAN_H_
#define SCAN_H_

#include "parlay/sequence.h"

namespace {

template <typename F, typename Input, typename Output>
int Work1(const parlay::slice<Input, Input>& a,
          const parlay::slice<Output, Output>& b, int l, int r,
          typename std::iterator_traits<Output>::value_type identity, F f,
          int granularity) {
  if (r - l + 1 <= granularity) {
    int res = identity;
    for (int i = l; i <= r; i++) res = f(res, a[i]);
    return res;
  }
  int mid = (l + r) / 2;
  int left, right;
  parlay::par_do(
      [&]() { left = Work1(a, b, l, mid, identity, f, granularity); },
      [&]() { right = Work1(a, b, mid + 1, r, identity, f, granularity); });
  b[mid] = left;
  return f(left, right);
}

template <typename F, typename Input, typename Output>
void Work2(const parlay::slice<Input, Input>& a,
           const parlay::slice<Output, Output>& b,
           parlay::slice<Output, Output> c, int l, int r,
           typename std::iterator_traits<Output>::value_type offset, F f,
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

}  // namespace

template <typename F, typename Input, typename Output>
void Scan(parlay::slice<Input, Input> input,
          parlay::slice<Output, Output> output,
          parlay::slice<Output, Output> helper, F f,
          typename std::iterator_traits<Output>::value_type identity,
          int granularity = 1) {
  int n = input.size();
  assert(output.size() == n);
  assert(helper.size() == n);
  Work1(input, helper, 0, n - 1, identity, f, granularity);
  Work2(input, helper, output, 0, n - 1, identity, f, granularity);
}

// int array sum scan
parlay::sequence<int> Scan(const parlay::sequence<int>& a, int granularity);

void ScanTest();

#endif  // SCAN_H_