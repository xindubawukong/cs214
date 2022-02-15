#ifndef SCAN_H_
#define SCAN_H_

#include "parlay/sequence.h"

parlay::sequence<int> Scan(const parlay::sequence<int>& a,
                           int granuality = 10000);

void ScanTest();

#endif  // SCAN_H_