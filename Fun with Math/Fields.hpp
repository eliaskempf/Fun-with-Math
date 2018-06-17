#pragma once

#include "Complex.hpp"
#include "ModuleRing.hpp"

namespace la::fields {
	using complex = Complex;

	template<unsigned int m>
	using module_ring = ModuleRing<m>;
}