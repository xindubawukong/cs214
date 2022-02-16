#ifndef PARALLELMERGING_H_
#define PARALLELMERGING_H_

#include "parlay/slice.h"

namespace hw2 {

int GetKth(const parlay::slice<int*, int*>& a,
           const parlay::slice<int*, int*>& b, int k);

void GetKthTest();

}  // namespace hw2

#endif  // PARALLELMERGING_H_