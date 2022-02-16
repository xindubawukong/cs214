#include "integral.h"

#include <cmath>

#include "parlay/parallel.h"

namespace hw1 {

template <typename Func>
double Integral(Func& f, int n, double low, double high, int granularity) {
  double dx = (high - low) / n;
  if (n <= granularity) {
    double ans = 0;
    for (int i = 0; i < n; i++) {
      double x = low + dx * i;
      ans += f(x) * dx;
    }
    return ans;
  }
  int a = n / 2;
  int b = n - a;
  double mid = low + dx * a;
  double left, right;
  parlay::par_do(
    [&]() {
      left = Integral(f, a, low, mid, granularity);
    },
    [&]() {
      right = Integral(f, b, mid, high, granularity);
    }
  );
  return left + right;
}

void IntegralTest() {
  auto f = [](double x) {
    return sin(x) * log(x);
  };
  double res = Integral(f, 100000000, 1, 2, 100000);
  double ans = 0.374027912;
  assert(abs(res - ans) < 1e-6);
  printf("IntegralTest: pass!\n");
}

}  // namespace hw1
