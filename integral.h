#ifndef INTEGRAL_H_
#define INTEGRAL_H_

template <typename Func>
double Integral(Func& f, int n, double low, double high, int granularity = 1);

void IntegralTest();

#endif  // INTEGRAL_H_