#ifndef SCAN_H_
#define SCAN_H_

#include "parlay/sequence.h"

parlay::sequence<int> Scan(const parlay::sequence<int>& a,
                           int granularity = 1);

void ScanTest();

#endif  // SCAN_H_