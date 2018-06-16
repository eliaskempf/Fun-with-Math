#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Fields.hpp"

namespace la {
	using matrix = Matrix<>;
	using vector = Vector<>;
}

namespace la::fields {
	using complex = Complex;

	template<unsigned int m>
	using module_ring = ModuleRing<m>;
}