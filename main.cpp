#include <iostream>

#include "integral.h"
#include "scan.h"
#include "ypplanguage.h"

#include "parlay/delayed_sequence.h"

int main() {
  ScanTest();
  hw1::YppLanguageTest();
  hw1::IntegralTest();
  return 0;
}
