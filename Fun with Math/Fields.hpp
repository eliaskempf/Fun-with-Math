#pragma once

#include "Complex.hpp"
#include "ModuleRing.hpp"

namespace la {
	using complex = Complex;

	template<uint32_t m>
	using module_ring = ModuleRing<m>;
}