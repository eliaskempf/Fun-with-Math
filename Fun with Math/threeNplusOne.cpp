#include "stdafx.h"
#include "threeNplusOne.hpp"
#include <iostream>

unsigned long long tnpo(unsigned long long n) {
	// std::cout << n << std::endl;
	if (n == 1) {
		return n;
	}
	if (n % 2 == 0) {
		return tnpo(n / 2);
	}
	else {
		return tnpo(3 * n + 1);
	}
}