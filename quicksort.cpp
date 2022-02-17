#include "quicksort.h"

#include "parlay/delayed_sequence.h"
#include "scan.h"
#include "utils.h"

namespace project1 {

namespace {

void QuickSort(parlay::slice<int*, int*> a, parlay::slice<int*, int*> helper1,
               parlay::slice<int*, int*> helper2,
               parlay::slice<int*, int*> helper3, int granularity) {
  int n = a.size();
  if (n <= 1) return;
  assert(helper1.size() == n);
  assert(helper2.size() == n);
  assert(helper3.size() == n);
  if (n <= granularity) {
    std::sort(a.begin(), a.end());
    return;
  }
  int pivot_id = n / 2;
  int pivot = a[pivot_id];
  auto smaller =
      parlay::delayed_seq<int>(n, [&](int i) { return a[i] < pivot; });
  auto larger =
      parlay::delayed_seq<int>(n, [&](int i) { return a[i] >= pivot; });
  auto f = [](int a, int b) { return a + b; };
  Scan(parlay::make_slice(smaller), helper1, helper3, f, 0, granularity);
  Scan(parlay::make_slice(larger), helper2, helper3, f, 0, granularity);

  int num = helper1[n - 1];
  parlay::parallel_for(
      0, n,
      [&](int i) {
        if (smaller[i]) {
          helper3[helper1[i] - 1] = a[i];
        } else {
          helper3[num + helper2[i] - 1] = a[i];
        }
      },
      granularity);
  parlay::parallel_for(
      0, n, [&](int i) { a[i] = helper3[i]; }, granularity);

  int pivot_pos = num + helper2[pivot_id] - 1;
  std::swap(a[pivot_pos], a[num]);  // move pivot to the middle

  parlay::par_do(
      [&]() {
        QuickSort(a.cut(0, num), helper1.cut(0, num), helper2.cut(0, num),
                  helper3.cut(0, num), granularity);
      },
      [&]() {
        QuickSort(a.cut(num + 1, n), helper1.cut(num + 1, n),
                  helper2.cut(num + 1, n), helper3.cut(num + 1, n),
                  granularity);
      });
}

}  // namespace

void QuickSort(parlay::sequence<int>& a, int granularity) {
  int n = a.size();
  parlay::sequence<int> helper1(n);
  parlay::sequence<int> helper2(n);
  parlay::sequence<int> helper3(n);
  QuickSort(parlay::make_slice(a), parlay::make_slice(helper1),
            parlay::make_slice(helper2), parlay::make_slice(helper3),
            granularity);
}

void QuickSortTest() {
  int n = 1000000;
  parlay::sequence<int> a(n);
  parlay::sequence<int> b(n);
  for (int i = 0; i < n; i++) {
    a[i] = b[i] = Rand();
  }
  QuickSort(a, 100);
  std::sort(b.begin(), b.end());
  for (int i = 0; i < n; i++) {
    assert(a[i] == b[i]);
  };
  printf("QuickSortTest: pass!\n");
}

}  // namespace project1