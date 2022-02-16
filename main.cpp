#include <iostream>

#include "integral.h"
#include "parallelmerging.h"
#include "parlay/sequence.h"
#include "parlay/slice.h"
#include "scan.h"
#include "utils.h"
#include "ypplanguage.h"

void RunAllTests() {
  ScanTest();
  hw1::YppLanguageTest();
  hw1::IntegralTest();
  hw2::GetKthTest();
}

int main() {
  RunAllTests();
  return 0;
}
