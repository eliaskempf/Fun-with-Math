#include <cstdint>
#include "stdafx.h"
#include "Trigonometry.hpp"
/*
double sine(double x) noexcept {
	uint32_t f = 1;
	double res = 0;
	double diff = 0;

	for (size_t n = 0; diff >= 0.000001; n++) {
		if (n != 0) { f *= n; }
		// diff = pow(-1.0, n) * pow(x, 2 * n + 1) / f;
		res += diff;
	}
	return res;
}*/