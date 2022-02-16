#ifndef PARALLELMERGING_H_
#define PARALLELMERGING_H_

#include <utility>

#include "parlay/sequence.h"
#include "parlay/slice.h"

namespace hw2 {

std::pair<int, std::pair<int, int>> GetKth(const parlay::sequence<int>& a,
                                           const parlay::sequence<int>& b,
                                           int l1, int r1, int l2, int r2,
                                           int k);

parlay::sequence<int> Merge(const parlay::sequence<int>& a,
                            const parlay::sequence<int>& b,
                            int granularity = 1);

void GetKthTest();

void MergeTest();

}  // namespace hw2

#endif  // PARALLELMERGING_H_