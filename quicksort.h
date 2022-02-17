#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include "parlay/sequence.h"

namespace project1 {

void QuickSort(parlay::sequence<int>& a, int granularity);

void QuickSortTest();

}  // namespace project1

#endif  // QUICKSORT_H_