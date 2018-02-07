#include "stdafx.h"
#include "Fibonacci.hpp"

unsigned long long fib(unsigned int n) {
	if (n <= 2) {
		if (n == 1 || n == 2) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return fib(n - 1) + fib(n - 2);
	}
}

unsigned long long fib_it(unsigned int n) {
	if (n == 0) {
		return 0;
	}

	unsigned long long fib1 = 1;
	unsigned long long fib2 = 1;

	for (int i = 2; i < n; i++) {
		unsigned long long temp = fib1 + fib2;
		fib1 = fib2;
		fib2 = temp;
	}

	return fib2;
}