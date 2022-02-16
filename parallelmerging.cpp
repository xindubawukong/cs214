#include "parallelmerging.h"

#include <algorithm>

#include "parlay/sequence.h"
#include "utils.h"

namespace hw2 {

int GetKth(const parlay::slice<int*, int*>& a,
           const parlay::slice<int*, int*>& b, int k) {
  int m = a.size(), n = b.size();
  if (m == 0) return b[k - 1];
  if (n == 0) return a[k - 1];
  if (k == m + n) return std::max(a[m - 1], b[n - 1]);
  if (m == 1) {
    if (b[k - 1] < a[0]) return b[k - 1];
    return std::max(b[k - 2], a[0]);
  }
  if (n == 1) {
    if (a[k - 1] < b[0]) return a[k - 1];
    return std::max(a[k - 2], b[0]);
  }
  int p = m / 2, q = n / 2;
  // a[0..p), a[p..m)
  // b[0..q), b[q..m)
  if (a[p] < b[q]) {
    if (k <= p + q) {
      return GetKth(a, b.cut(0, q), k);
    } else {
      return GetKth(a.cut(p, m), b, k - p);
    }
  } else {
    if (k <= p + q) {
      return GetKth(a.cut(0, p), b, k);
    } else {
      return GetKth(a, b.cut(q, n), k - q);
    }
  }
}

void GetKthTest() {
  int m = 100000, n = 100000;
  parlay::sequence<int> a(m);
  parlay::sequence<int> b(n);
  for (int i = 0; i < m; i++) a[i] = Rand();
  for (int i = 0; i < n; i++) b[i] = Rand();
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  std::vector<int> c;
  for (int i = 0; i < m; i++) c.push_back(a[i]);
  for (int i = 0; i < n; i++) c.push_back(b[i]);
  std::sort(c.begin(), c.end());
  for (int i = 0; i < m + n; i++) {
    auto aa = parlay::make_slice(a.begin(), a.end());
    auto bb = parlay::make_slice(b.begin(), b.end());
    int res = GetKth(aa, bb, i + 1);
    assert(res == c[i]);
  }
  printf("GetKthTest: pass!\n");
}

}  // namespace hw2
