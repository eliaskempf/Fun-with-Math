#include "stdafx.h"
#include "EulersPhi.hpp"

uint64_t euclid(uint64_t a, uint64_t b) noexcept {
	uint64_t h = 0;
	while (b != 0) {
		h = a % b;
		a = b;
		b = h;
	}
	return a;
}

uint64_t phi(uint64_t n) noexcept {
	uint64_t phi = 0;
	for (size_t i = 1; i <= n; i++) {
		if (euclid(i, n) == 1) { phi++; }
	}
	return phi;
}