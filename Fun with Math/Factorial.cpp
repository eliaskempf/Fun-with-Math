#include "stdafx.h"
#include "Factorial.hpp"
#include <cmath>

unsigned long long factorial(int n) {
	return n == 0 ? 1 : n * factorial(n - 1);
}

unsigned long long fact_it(int n) {
	unsigned long long factorial = 1;

	for (int i = 2; i <= n; i++) {
		factorial *= i;
	}
	return factorial;
}

unsigned long long fact(int n) {
	return std::tgamma(n + 1) + 0.001;
}