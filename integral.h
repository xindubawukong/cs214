#ifndef INTEGRAL_H_
#define INTEGRAL_H_

namespace hw1 {

template <typename Func>
double Integral(Func& f, int n, double low, double high, int granularity = 1);

void IntegralTest();

}  // namespace hw1

#endif  // INTEGRAL_H_