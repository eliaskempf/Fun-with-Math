#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Fields.hpp"

namespace la {
	template<typename T = double>
	using matrix = Matrix<T>;

	template<typename T = double>
	using vector = Vector<T>;
}