#ifndef SCAN_H_
#define SCAN_H_

#include "parlay/sequence.h"

namespace dxy {

parlay::sequence<int> Scan(const parlay::sequence<int>& a, int granuality = 10000);

void TestScan();

}  // namespace dxy

#endif  // SCAN_H_