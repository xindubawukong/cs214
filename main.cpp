#include <iostream>

#include "integral.h"
#include "scan.h"
#include "ypplanguage.h"

#include "parlay/delayed_sequence.h"

int main() {
  ScanTest();
  YppLanguageTest();
  IntegralTest();
  return 0;
}
