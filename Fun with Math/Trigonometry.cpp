#include "stdafx.h"
#include <cstdint>
#include <cmath>
#include "Trigonometry.hpp"

double sine(double x) noexcept {
	uint32_t f = 1;
	double res = 0;
	double diff = 0;

	for (size_t n = 0; diff >= 0.000001; n++) {
		if (n != 0) { f *= n; }
		diff = std::pow(-1.0, n) * std::pow(x, 2 * n + 1) / f;
		res += diff;
	}
	return res;
}