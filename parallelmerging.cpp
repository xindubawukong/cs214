#include "parallelmerging.h"

#include <algorithm>
#include <utility>

#include "parlay/sequence.h"
#include "utils.h"

namespace hw2 {

std::pair<int, std::pair<int, int>> GetKth(const parlay::sequence<int>& a,
                                           const parlay::sequence<int>& b,
                                           int l1, int r1, int l2, int r2,
                                           int k) {
  if (l1 == r1) {
    return std::make_pair(b[l2 + k - 1], std::make_pair(l1, l2 + k - 1));
  }
  if (l2 == r2) {
    return std::make_pair(a[l1 + k - 1], std::make_pair(l1 + k - 1, l2));
  }
  if (k == r1 - l1 + r2 - l2) {
    if (a[r1 - 1] < b[r2 - 1]) {
      return std::make_pair(b[r2 - 1], std::make_pair(r1 - 1, r2 - 1));
    } else {
      return std::make_pair(a[r1 - 1], std::make_pair(r1 - 1, r2 - 1));
    }
  }
  if (r1 - l1 == 1) {
    if (b[l2 + k - 1] < a[l1]) {
      return std::make_pair(b[l2 + k - 1], std::make_pair(l1 - 1, l2 + k - 1));
    }
    if (b[l2 + k - 2] < a[l1]) {
      return std::make_pair(a[l1], std::make_pair(l1, l2 + k - 2));
    } else {
      return std::make_pair(b[l2 + k - 2], std::make_pair(l1, l2 + k - 2));
    }
  }
  if (r2 - l2 == 1) {
    if (a[l1 + k - 1] < b[l2]) {
      return std::make_pair(a[l1 + k - 1], std::make_pair(l1 + k - 1, l2 - 1));
    }
    if (a[l1 + k - 2] < b[l2]) {
      return std::make_pair(b[l2], std::make_pair(l1 + k - 2, l2));
    } else {
      return std::make_pair(a[l1 + k - 2], std::make_pair(l1 + k - 2, l2));
    }
  }
  int p = (l1 + r1) / 2, q = (l2 + r2) / 2;
  // a[l1..p), a[p..r1)
  // b[l2..q), b[q..r2)
  if (a[p] < b[q]) {
    if (k <= p - l1 + q - l2) {
      return GetKth(a, b, l1, r1, l2, q, k);
    } else {
      return GetKth(a, b, p, r1, l2, r2, k - (p - l1));
    }
  } else {
    if (k <= p - l1 + q - l2) {
      return GetKth(a, b, l1, p, l2, r2, k);
    } else {
      return GetKth(a, b, l1, r1, q, r2, k - (q - l2));
    }
  }
}

template <typename T, typename S>
void SequentialMerge(const parlay::slice<T, T>& a, const parlay::slice<T, T>& b,
                     parlay::slice<S, S> c) {
  int m = a.size(), n = b.size(), tot = c.size();
  assert(m + n == tot);
  int i = 0, j = 0;
  for (int k = 0; k < tot; k++) {
    if (i == m)
      c[k] = b[j++];
    else if (j == n)
      c[k] = a[i++];
    else if (a[i] < b[j])
      c[k] = a[i++];
    else
      c[k] = b[j++];
  }
}

parlay::sequence<int> Merge(const parlay::sequence<int>& a,
                            const parlay::sequence<int>& b, int granularity) {
  int m = a.size(), n = b.size();
  int tot = m + n;
  int logn = 1;
  for (int t = tot; t > 1; t >>= 1) logn++;
  int num = tot / logn;
  if (tot % logn > 0) num++;
  parlay::sequence<int> pos_a(num - 1);
  parlay::sequence<int> pos_b(num - 1);
  parlay::parallel_for(
      0, num - 1,
      [&](int i) {
        int k = (i + 1) * logn;
        auto res = GetKth(a, b, 0, m, 0, n, k);
        pos_a[i] = res.second.first;
        pos_b[i] = res.second.second;
      },
      granularity);
  parlay::sequence<int> c(tot);
  parlay::parallel_for(
      0, num,
      [&](int i) {
        int l1 = i == 0 ? 0 : pos_a[i - 1] + 1;
        int r1 = i == (num - 1) ? m : pos_a[i] + 1;
        int l2 = i == 0 ? 0 : pos_b[i - 1] + 1;
        int r2 = i == (num - 1) ? m : pos_b[i] + 1;
        int l = i * logn;
        int r = i == (num - 1) ? tot : (i + 1) * logn;
        SequentialMerge(parlay::make_slice(a).cut(l1, r1),
                        parlay::make_slice(b).cut(l2, r2),
                        parlay::make_slice(c).cut(l, r));
      },
      granularity);
  return c;
}

void GetKthTest() {
  int m = 50000, n = 50000;
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
    std::pair<int, std::pair<int, int>> res = GetKth(a, b, 0, m, 0, n, i + 1);
    assert(res.first == c[i]);
    int p1 = res.second.first;
    int p2 = res.second.second;
    if (p1 >= 0) assert(a[p1] <= c[i]);
    if (p1 < m - 1) assert(a[p1 + 1] >= c[i]);
    if (p2 >= 0) assert(b[p2] <= c[i]);
    if (p2 < n - 1) assert(b[p2 + 1] >= c[i]);
  }
  printf("GetKthTest: pass!\n");
}

void MergeTest() {
  int m = 1000000, n = 1000000;
  parlay::sequence<int> a(m);
  parlay::sequence<int> b(n);
  for (int i = 0; i < m; i++) a[i] = Rand(m * 5);
  for (int i = 0; i < n; i++) b[i] = Rand(m * 5);
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  auto c = Merge(a, b, 100);
  parlay::sequence<int> res;
  for (int i = 0; i < m; i++) res.push_back(a[i]);
  for (int i = 0; i < n; i++) res.push_back(b[i]);
  std::sort(res.begin(), res.end());
  for (int i = 0; i < m + n; i++) {
    assert(res[i] == c[i]);
  }
  printf("MergeTest: pass!\n");
}

}  // namespace hw2
