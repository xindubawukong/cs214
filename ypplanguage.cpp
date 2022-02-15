#include "ypplanguage.h"

#include <string>

#include "parlay/sequence.h"
#include "scan.h"

bool YppLanguageCheck(std::string code, int granularity) {
  int n = code.length();
  parlay::sequence<int> a(n);
  parlay::parallel_for(
      0, n,
      [&](int i) {
        if (code[i] == '(')
          a[i] = 1;
        else
          a[i] = -1;
      },
      granularity);
  auto b = Scan(a, granularity);
  if (b[n - 1] != 0) return false;
  bool flag = true;
  parlay::parallel_for(
      0, n,
      [&](int i) {
        if (b[i] < 0) flag = false;
      },
      granularity);
  return flag;
}

void YppLanguageTest() {
  assert(YppLanguageCheck("((()))", 1));
  assert(!YppLanguageCheck("((())", 1));
  assert(YppLanguageCheck("(()()((())))", 1));
  assert(!YppLanguageCheck("(()))(()", 1));
  printf("Y++ Language test: pass!\n");
}