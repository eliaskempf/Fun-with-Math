#pragma once

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <future>
#include <thread>
#include <cmath>

namespace la {
	template <typename T>
	class vector;

	template<typename T = double>
	class matrix {
	private:
		// Field to hold values of the matrix
		T* m_entries;

		// Dimensions for the matrix
		size_t m_rows, m_cols;

		// This is an intern version for the gauss algorithm that also yields
		// information for the determinant calculation
		std::pair<matrix, T> gauss_intern() const
			/* noexcept(std::is_nothrow_copy_constructible<matrix<T>>::value) */;

	public:
		// Constructors

		// Default constructor
		matrix() noexcept = default;
		// Construct matrix with dimensions and optional default value
		matrix(size_t, size_t, T = T(0))
			noexcept(std::is_nothrow_default_constructible_v<T>);
		// Constructor for vector intialization
		matrix(std::initializer_list<T>, bool) noexcept;
		// Constructor for std::initializer_list
		// Throws std::invalid_argument if inner lists differ in size
		matrix(std::initializer_list<std::initializer_list<T>>);
		// Copy constructor
		matrix(const matrix &) 
			noexcept(std::is_nothrow_default_constructible_v<T>);
		// Move constructor
		matrix(matrix &&) noexcept;

		// Destructor
		virtual ~matrix();

		// Getter for the dimensions
		size_t rows() const;
		size_t columns() const;
		size_t entries() const;

		// Matrix algorithms
		T determinant() const;
		matrix transpose() const;
		matrix gauss() const
			noexcept(noexcept(gauss_intern)/*std::is_nothrow_copy_constructible_v<matrix<T>>*/);
		matrix gauss_jordan() const
			/* noexcept(std::is_nothrow_copy_constructible_v<matrix<T>>) */;
		matrix invert() const;

		// Static methods
		static matrix identity(size_t)
			noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>);
		static matrix elementary(size_t, size_t, size_t, T);

		// Overloaded operators

		// Access operators
		T operator()(size_t, size_t) const;
		T& operator()(size_t, size_t);

		// Arithmetic operators
		matrix operator*(const matrix &) const;
		vector<T> operator*(const vector<T> &) const;
		matrix operator*(const T &) const
			noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>);
		matrix operator/(const T &) const
			noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>);
		matrix operator+(const matrix &) const;
		matrix operator-(const matrix &) const;

		// Assignment operators
		matrix& operator*=(const matrix &);
		matrix& operator*=(const T &)
			noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>);
		matrix& operator/=(const T &)
			noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>);
		matrix& operator+=(const matrix &);
		matrix& operator-=(const matrix &);

		// Copy assignment
		matrix& operator=(const matrix &)
			noexcept(std::is_nothrow_default_constructible_v<T>);
		// Move assignment
		matrix& operator=(matrix &&) noexcept;

		// Comparison operators
		bool operator==(const matrix &) const noexcept;
		bool operator!=(const matrix &) const noexcept;
	};

	template<typename U, typename T>
	matrix<T> operator*(const U &, const matrix<T> &)
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>);

	template<typename T>
	std::ostream& operator<<(std::ostream &, const matrix<T> &);

	template<typename T>
	matrix<T>::matrix(size_t rows, size_t cols, T defVal)
		noexcept(std::is_nothrow_default_constructible_v<T>)
		: m_cols(cols), m_rows(rows) {
		if (m_cols == 0 || m_rows == 0) {
			throw std::logic_error("Matrix does not allow zero dimensions.");
		}

		m_entries = new T[rows * cols];
		std::fill(m_entries, m_entries + (rows * cols), defVal);
	}

	template<typename T>
	matrix<T>::matrix(std::initializer_list<T> list, bool mode) noexcept {
		m_entries = new T[list.size()];
		m_cols = mode ? list.size() : 1;
		m_rows = mode ? 1 : list.size();
		std::move(list.begin(), list.end(),
			stdext::checked_array_iterator<T*>(m_entries, list.size()));
	}

	template<typename T>
	matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list) {
		for (const auto i : list) {
			if (i.size() != list.begin()->size()) {
				throw std::runtime_error("All rows have to be equal in length.");
			}
		}

		m_rows = list.size();
		m_cols = list.begin()->size();
		m_entries = new T[m_rows * m_cols];

		uint32_t index = 0;
		for (const auto &i : list) {
			std::move(i.begin(), i.end(),
				stdext::checked_array_iterator<T*>(m_entries + m_cols * index, m_cols));
			index++;
		}
	}

	template<typename T>
	matrix<T>::matrix(const matrix<T> &other)
		noexcept(std::is_nothrow_default_constructible_v<T>) {
		m_rows = other.m_rows;
		m_cols = other.m_cols;
		m_entries = new T[other.entries()];
		std::copy(other.m_entries, other.m_entries + other.entries(),
			stdext::checked_array_iterator<T*>(m_entries, entries()));
	}

	template<typename T>
	matrix<T>::matrix(matrix<T> &&other) noexcept {
		m_rows = other.m_rows;
		m_cols = other.m_cols;
		m_entries = other.m_entries;
		other.m_entries = nullptr;
	}

	template<typename T>
	matrix<T>::~matrix() {
		delete[] m_entries;
	}

	template<typename T>
	size_t matrix<T>::rows() const {
		return m_rows;
	}

	template<typename T>
	size_t matrix<T>::columns() const {
		return m_cols;
	}

	template<typename T>
	size_t matrix<T>::entries() const {
		return m_rows * m_cols;
	}

	// Calculate determinant using gauss algorithm
	template<typename T>
	T matrix<T>::determinant() const {
		// Check for valid argument
		if (this->m_rows != this->m_cols) {
			throw std::invalid_argument("Matrix has to be quadratic.");
		}

		// Do gauss transformation and obtain additional correction factor
		auto gPair = gauss_intern();

		// Calculate result variable and multiply by correction factor
		T det = T(1) * gPair.second;

		// Multiply result variable by every entry of the main diagonal of the
		// transformed matrix
		for (size_t i = 0; i < this->m_rows; i++) {
			det *= gPair.first(i, i);
		}

		return det == 0 ? 0 : det;
	}

	// Return the transposed matrix
	template<typename T>
	matrix<T> matrix<T>::transpose() const {
		matrix<T> m(m_cols, m_rows);
		
		for (size_t i = 0; i < m_cols; i++) {
			for (size_t j = 0; j < m_rows; j++) {
				m.m_entries[i * m_rows + j] = m_entries[j * m_cols + i];
			}
		}
		return m;
	}

	// Return the gauss transformed matrix
	template<typename T>
	matrix<T> matrix<T>::gauss() const
		noexcept(noexcept(gauss_intern)/*std::is_nothrow_copy_constructible_v<matrix<T>>*/) {
		// Just a public wrapper for the intern gauss algorithm that also return
		// addtional information for determinant calculation
		return gauss_intern().first;
	}

	// The actual gauss transformations takes place and also yields a correction
	// factor for determinant calculation
	template<typename T>
	std::pair<matrix<T>, T> matrix<T>::gauss_intern() const
		/* noexcept(std::is_nothrow_copy_constructible_v<matrix<T>>) */ {
		// Create a working copy of the matrix
		matrix<T> m(*this);
		// Correction factor
		T cf(1);

		for (size_t j = 0; j < std::min(m.m_rows, m.m_cols); j++) {
			// Reference value we use to make all the entries below in this column 0
			T t1 = m.m_entries[j * (m.m_cols + 1)];
			// If our reference value is 0 we need to swap some rows
			if (t1 == T(0)) {
				for (size_t i = j + 1; i < m.m_rows; i++) {
					// If an entry below is unequal to 0 we swap the rows and adjust cf
					if (m.m_entries[j * (m.m_cols + 1) + (i - j) * m.m_cols] != T(0)) {
						std::swap_ranges(m.m_entries + j * m.m_cols,
							m.m_entries + (j + 1) * m.m_cols,
							m.m_entries + i * m.m_cols);
						// This value has now changed
						t1 = m.m_entries[j * (m.m_cols + 1)];
						cf *= T(-1);
						break;
					}
				}
			}

			// If t1 is unequal to 0 the real transformation part begins
			for (size_t i = j + 1; i < m.m_rows; i++) {
				// Second reference value to calculate factor we multiply the above row by
				// to make this entry 0
				T t2 = m.m_entries[i * m.m_cols + j];
				// If it already is 0 we don't need to doing something here
				if (t2 == T(0)) { continue; }
				// This might look very confusing but in the end it simply subtracts the
				// above row from all the above rows multiplied by a factor of our
				// reference values t2 / t1 so all the entries below the first reference
				// become 0
				std::transform(m.m_entries + j * m.m_cols,
					m.m_entries + (j + 1) * m.m_cols,
					m.m_entries + i * m.m_cols, m.m_entries + i * m.m_cols,
					[t1, t2](T first, T second) { return second - first * (t2 / t1); });
			}
		}
		// The madness is over we make a pair from matrix and factor and return it
		return std::make_pair(m, cf);
	}

	// Return gauss-jordan transformed matrix
	template<typename T>
	matrix<T> matrix<T>::gauss_jordan() const
		/* noexcept(std::is_nothrow_copy_constructible_v<matrix<T>>) */ {
		// Do normal gauss first
		matrix<T> m = gauss_intern().first;

		// We don't use size_t here because j and i have to become negative to
		// terminate the loops
		for (int32_t j = std::min(m.m_rows, m.m_cols) - 1; j >= 0; j--) {
			// Make entry m(j, j) 1 by diving the row by m(j, j)
			std::transform(m.m_entries + j * (m.m_cols + 1),
				m.m_entries + (j + 1) * m.m_cols,
				m.m_entries + j * (m.m_cols + 1),
				[m, j](T first) { return first / m(j, j); });
			// Now we want to make every entry above m(j, j) 0, so m(j, j) remains as
			// the only 1 in its column
			for (int32_t i = j - 1; i >= 0; i--) {
				std::transform(m.m_entries + j * (m.m_cols + 1),
					m.m_entries + (j + 1) * m.m_cols,
					m.m_entries + j * (m.m_cols + 1) - m.m_cols * (j - i),
					m.m_entries + j * (m.m_cols + 1) - m.m_cols * (j - i),
					[m, i, j](T first, T second) { return second - first * m(i, j); });
			}
		}
		return m;
	}

	// Calculated inverted matrix
	template<typename T>
	matrix<T> matrix<T>::invert() const {
		// Check for valid argument
		if (this->m_rows != this->m_cols) {
			throw std::invalid_argument("Matrix has to be quadratic.");
		}

		// Check for invertibility
		if (this->determinant() == 0) {
			throw::std::invalid_argument("Matrix is not invertible.");
		}

		// Construct helper matrix
		matrix<T> h(this->m_rows, 2 * this->m_cols);

		// Copy entries of this in the left section of h
		for (size_t i = 0; i < h.m_rows; i++) {
			std::copy(m_entries + i * this->m_cols, m_entries + (i + 1) * this->m_cols,
				h.m_entries + i * h.m_cols);
		}
		// Make right section of h and identity matrix
		for (size_t i = 0; i < h.m_rows; i++) {
			h(i, i + h.m_rows) = 1;
		}

		// Do gauss-jordan on h so the identity matrix will be on the left side now
		h = h.gauss_jordan();

		// Create result matrix
		matrix<T> m(this->m_rows, this->m_cols);

		// Copy entries of the right side of h into m
		for (size_t i = 0; i < m.m_rows; i++) {
			std::copy(h.m_entries + 2 * i * m.m_cols + m.m_cols,
				h.m_entries + (2 * i + 1) * m.m_cols + m.m_cols,
				m.m_entries + i * m.m_cols);
		}
		return m;
	}

	// Create an identity matrix
	template<typename T>
	matrix<T> matrix<T>::identity(size_t size)
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>) {
		matrix<T> m(size, size);
		for (int i = 0; i < size; i++) {
			m(i, i) = 1;
		}
		return m;
	}

	// Create an elementary matrix
	template<typename T>
	matrix<T> matrix<T>::elementary(size_t size, size_t row, size_t col, T val) {
		matrix<T> m(size, size);
		for (int i = 0; i < size; i++) {
			m(i, i) = 1;
		}
		m(row, col) = val;
		return m;
	}

	template<typename T>
	T matrix<T>::operator()(size_t i, size_t j) const {
		if (i >= m_rows || j >= m_cols) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return m_entries[i * m_cols + j];
	}

	template<typename T>
	T& matrix<T>::operator()(size_t i, size_t j) {
		if (i >= m_rows || j >= m_cols) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return m_entries[i * m_cols + j];
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const matrix<T> &other) const {
		if (m_cols != other.m_rows) {
			throw std::runtime_error("Can not multiply by a matrix which rows does not \
				                      match the columns of the original matrix.");
		}

		const size_t minColsPerThread = 50;
		size_t supportedThreads = std::max<size_t>(std::thread::hardware_concurrency(), 2);

		size_t rows = m_rows; // std::min(m_rows, other.m_cols);
		size_t cols = other.m_cols; // std::max(m_rows, other.m_cols);

		matrix<T> m(rows, cols);

		size_t maxAmountOfThreads = (cols + minColsPerThread - 1) / minColsPerThread;
		size_t amountOfThreads = supportedThreads > maxAmountOfThreads ? maxAmountOfThreads : supportedThreads;

		size_t colsPerThread = cols / amountOfThreads;

		std::future<void>* futures = new std::future<void>[amountOfThreads - 1];

		for (int c = 0; c < amountOfThreads - 1; c++) {
			futures[c] = std::async([c, rows, cols, colsPerThread, this, other, &m]() {
				for (size_t i = 0; i < rows; i++) {
					for (size_t k = c * colsPerThread; k < (c + 1) * colsPerThread; k++) {
						T c_ik = 0;
						for (size_t j = 0; j < m_cols; j++) {
							c_ik += m_entries[i * m_cols + j] * other.m_entries[j * other.m_cols + k];
						}
						m.m_entries[i * cols + k] = c_ik;
					}
				}
				});
		}

		for (size_t i = 0; i < rows; i++) {
			for (size_t k = (amountOfThreads - 1) * colsPerThread; k < cols; k++) {
				T c_ik = 0;
				for (size_t j = 0; j < m_cols; j++) {
					c_ik += m_entries[i * m_cols + j] * other.m_entries[j * other.m_cols + k];
				}
				m.m_entries[i * cols + k] = c_ik;
			}
		}

		for (int c = 0; c < amountOfThreads - 1; c++) {
			futures[c].get();
		}

		delete[] futures;

		if (rows != m_rows) {
			m.m_rows = cols;
			m.m_cols = rows;
		}

		return m;
	}

	template<typename T>
	vector<T> matrix<T>::operator*(const vector<T> &other) const {
		if (m_cols != other.mDimension) {
			throw std::runtime_error("Can not multiply by a vector which dimension does \
									  not match the columns of the matrix.");
		}

		vector<T> v(m_rows);
		v.mmatrix = *this * other.mmatrix;
		return v;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const T &other) const
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>) {
		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] * other;
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::operator/(const T &other) const
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>) {
		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] / other;
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::operator+(const matrix<T> &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] + other.m_entries[i];
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::operator-(const matrix<T> &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] - other.m_entries[i];
		}
		return m;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator*=(const matrix<T> &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator*=(const T &other)
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator/=(const T &other)
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>) {
		*this = *this / other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator+=(const matrix<T> &other) {
		*this = *this + other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator-=(const matrix<T> &other) {
		*this = *this - other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator=(const matrix<T> &other)
		noexcept(std::is_nothrow_default_constructible_v<T>) {
		if (this != &other) {
			if (this->entries() != other.entries()) {
				delete[] m_entries;
				m_entries = new T[other.entries()];
			}
			m_rows = other.m_rows;
			m_cols = other.m_cols;
			std::copy(other.m_entries, other.m_entries + other.entries(), 
				stdext::checked_array_iterator<T*>(m_entries, entries()));
		}
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator=(matrix<T> &&other) noexcept {
		if (this != &other) {
			delete[] m_entries;
			m_entries = other.m_entries;
			other.m_entries = nullptr;
			m_rows = other.m_rows;
			m_cols = other.m_cols;
		}
		return *this;
	}

	template<typename T>
	bool matrix<T>::operator==(const matrix &other) const noexcept {
		if (m_rows != other.m_rows || m_cols != other.m_cols) { return false; }
		for (size_t i = 0; i < other.entries(); i++) {
			if (m_entries[i] != other.m_entries[i]) {
				return false;
			}
		}
		return true;
	}

	template<typename T>
	bool matrix<T>::operator!=(const matrix &other) const noexcept {
		if (*this == other) { return false; }
		return true;
	}


	template<typename U, typename T>
	matrix<T> operator*(const U &lhs, const matrix<T> &m) 
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>) {
		return m * lhs;
	}

	// Test wise implementation
	template<typename T>
	std::ostream& operator<<(std::ostream &os, const matrix<T> &m) {
		if constexpr (std::is_arithmetic_v<T>) {
			T max = 0;
			T fMax = 0;
			bool firstCol = false;
			for (size_t i = 0; i < m.rows(); i++) {
				for (size_t j = 0; j < m.columns(); j++) {
					if (std::abs(m(i, j)) > max) {
						max = std::abs(m(i, j));
					}

					if (j == 0 && m(i, j) > fMax) {
						fMax = m(i ,j);
					}
				}
			}
			int32_t maxLength = std::log(max) / std::log(10) + 1.000001;
			int32_t fMaxLength = std::log(fMax) / std::log(10) + 1.000001;
			maxLength = std::max(maxLength, 1);
			fMaxLength = std::max(fMaxLength, 1);

			for (size_t i = 0; i < m.rows(); i++) {
				os << "| ";
				for (size_t j = 0; j < m.columns(); j++) {
					int32_t length = m(i, j) == 0 ? 1 : 
						std::log(std::abs(m(i, j))) / std::log(10) + 1.000001;
					for (size_t k = length; k < (j == 0 ? fMaxLength : maxLength); k++) { os << " "; }
					os << m(i, j) << " ";
				}
				os << "|\n";
			}
		}
		else {
			for (size_t i = 0; i < m.rows(); i++) {
				os << "| ";
				for (size_t j = 0; j < m.columns(); j++) {
					os << m(i, j) << " ";
				}
				os << "|\n";
			}
		}
		return os;
	}
}