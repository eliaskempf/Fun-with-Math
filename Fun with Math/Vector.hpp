#pragma once

#include <stdexcept>
#include <iostream>
#include <utility>
#include <cmath>

namespace la {
	template<typename T>
	class matrix;

	template<typename T = double>
	class vector {
	private:
		size_t m_dimension;
		matrix<T> m_matrix;

		friend matrix<T>;

	public:
		// Constructors

		// Default constructor
		vector() = default;
		// Construct vector with optional default value
		explicit vector(size_t, T = T(0))
			noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>);
		// Intializer list constructor
		vector(std::initializer_list<std::initializer_list<T>>);
		// Copy constructor
		vector(const vector &)
			noexcept(std::is_nothrow_default_constructible_v<T>);
		// Move constructor
		vector(vector &&) noexcept;

		// Getter for dimensions
		size_t size() const noexcept;
		// Return vector norm
		double norm() const;
		// Return unit vector
		vector unit() const;

		// Calculate angle between vectors
		double angle(const vector &) const;

		// Overloaded operators

		// Access operators
		T operator[](size_t) const;
		T& operator[](size_t);

		// Arithmetic operators
		// All arithmetic operations throw if the operations are mathematically
		// not well-defined
		T operator*(const vector &) const;
		vector operator*(const matrix<T> &) const;
		vector operator*(const T &) const
			noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>);
		vector operator/(const T &) const
			noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>);
		vector operator+(const vector &) const;
		vector operator-(const vector &) const;

		// Assignment operators
		vector& operator*=(const T &)
			noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>);
		vector& operator/=(const T &)
			noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>);
		vector& operator+=(const vector &);
		vector& operator-=(const vector &);

		// Copy assignment
		vector& operator=(const vector &)
			noexcept(std::is_nothrow_default_constructible_v<T>);
		// Move assignment
		vector& operator=(vector &&) noexcept;

		// Comparison operators
		bool operator==(const vector &) const noexcept;
		bool operator!=(const vector &) const noexcept;
	};

	// Allow for both-sided multiplication by constant
	template<typename U, typename T>
	vector<T> operator*(const U &, const vector<T> &)
		noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>);

	// Allow vector to be printed
	template<typename T>
	std::ostream& operator<<(std::ostream &, const vector<T> &);

	// Create vector of given size and optional default value
	template<typename T>
	vector<T>::vector(size_t size, T defVal)
		noexcept(std::is_nothrow_constructible_v<matrix<T>, size_t, size_t>)
		: m_dimension(size), m_matrix(size, 1, defVal) 
	{}

	// Create vector from initializer_list
	template<typename T>
	vector<T>::vector(std::initializer_list<std::initializer_list<T>> list)
		: m_dimension(std::max(list.size(), list.begin()->size())), m_matrix(list) {
		// Make no distinction between row and column vectors
		m_matrix.m_rows = m_dimension;
		m_matrix.m_cols = 1;
	}

	// Copy constructor
	template<typename T>
	vector<T>::vector(const vector<T> &other)
		noexcept(std::is_nothrow_default_constructible_v<T>)
		: m_dimension(other.m_dimension), m_matrix(other.m_matrix)
	{}

	// Move constructor
	template<typename T>
	vector<T>::vector(vector<T> &&other) noexcept
		: m_dimension(other.m_dimension), m_matrix(std::forward<matrix<T>>(other.m_matrix))
	{}

	// Return size of vector
	template<typename T>
	size_t vector<T>::size() const noexcept {
		return m_dimension;
	}

	// Calculate the norm of the vector
	template<typename T>
	double vector<T>::norm() const {
		// Check vector is arithmetic
		if constexpr (std::is_arithmetic_v<T>) {
			return std::sqrt((*this) * (*this));
		}
		else {
			throw std::logic_error("Can not calculate length of non-arithmetic type vector.");
		}
	}

	// Return the vector divided my its norm
	template<typename T>
	vector<T> vector<T>::unit() const {
		// Check if vector can be normalized
		if constexpr (std::is_integral_v<T>) {
			throw std::logic_error("Can not normalize vector of integral type.");
		}

		double norm = this->norm();
		return *this / norm;
	}

	template<typename T>
	double vector<T>::angle(const vector<T> &other) const {
		return std::acos((*this * other) / (norm() * other.norm()));
	}

	// Access elements of vector by value
	template<typename T>
	T vector<T>::operator[](size_t index) const {
		// Check for valid argument
		if (index >= m_dimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return m_matrix(index, 0);
	}

	// Access elements of vector by reference
	template<typename T>
	T& vector<T>::operator[](size_t index) {
		// Check for valid argument
		if (index >= m_dimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return m_matrix(index, 0);
	}

	// Multiply two vectors
	template<typename T>
	T vector<T>::operator*(const vector<T> &other) const {
		// Check for valid argument
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		T scalar(0);
		for (size_t i = 0; i < m_dimension; i++) { scalar += m_matrix(i, 0) * other.m_matrix(i, 0); }
		return scalar;
	}

	// Multiply vector by a matrix
	template<typename T>
	vector<T> vector<T>::operator*(const matrix<T> &other) const {
		// Check for valid argument
		if (m_dimension != other.m_rows) {
			throw std::runtime_error("Size of vector has to match rows \
									  of the matrix.");
		}

		// Create result vector
		vector<T> v;
		v.m_size = other.m_cols;
		// Intern we only use row vectors so we temporarily transform it into a column
		// vector for the calculation
		m_matrix.m_cols = m_matrix.m_rows;
		m_matrix.m_rows = 1;
		// Now we can multiply them and adjust the dimensions accordingly
		v.matrix = m_matrix * other;
		v.matrix.m_rows = v.m_matrix.m_cols;
		v.matrix.m_cols = 1;
		// And reverse the original values
		m_matrix.m_rows = m_matrix.m_cols;
		m_matrix.m_cols = 1;
		return v;
	}

	// Multiply vector by constant
	template<typename T>
	vector<T> vector<T>::operator*(const T &other) const
		noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>) {
		vector<T> v(m_dimension);
		v.m_matrix = m_matrix * other;
		return v;
	}

	// Divide vector by constant
	template<typename T>
	vector<T> vector<T>::operator/(const T &other) const
		noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>) {
		vector<T> v(m_dimension);
		v.m_matrix = m_matrix / other;
		return v;
	}

	// Add two vectors
	template<typename T>
	vector<T> vector<T>::operator+(const vector<T> &other) const {
		// Check for valid argument
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		vector<T> v(m_dimension);
		v.m_matrix = m_matrix + other.m_matrix;
		return v;
	}

	// Subtract two vectors
	template<typename T>
	vector<T> vector<T>::operator-(const vector<T> &other) const {
		// Check for valid argument
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		vector<T> v(m_dimension);
		v.m_matrix = m_matrix - other.m_matrix;
		return v;
	}

	// Multiply by constant and assign result to *this
	template<typename T>
	vector<T>& vector<T>::operator*=(const T &other)
		noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>) {
		*this = *this * other;
		return *this;
	}

	// Divide by constant and assign result to *this
	template<typename T>
	vector<T>& vector<T>::operator/=(const T &other)
		noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>) {
		*this = *this * other;
		return *this;
	}

	// Add two vectors and assign result to *this
	template<typename T>
	vector<T>& vector<T>::operator+=(const vector<T> &other) {
		*this = *this + other;
		return *this;
	}

	// Subtract two vectors and assign result to *this
	template<typename T>
	vector<T>& vector<T>::operator-=(const vector<T> &other) {
		*this = *this - other;
		return *this;
	}

	// Copy assignment operator
	template<typename T>
	vector<T>& vector<T>::operator=(const vector<T> &other)
		noexcept(std::is_nothrow_default_constructible_v<T>) {
		if (this != &other) {
			m_dimension = other.m_dimension;
			m_matrix = other.m_matrix;
		}
		return *this;
	}

	// Move assignment operator
	template<typename T>
	vector<T>& vector<T>::operator=(vector<T> &&other) noexcept {
		if (this != &other) {
			m_dimension = other.m_dimension;
			m_matrix = std::forward<matrix<T>>(other.m_matrix);
		}
		return *this;
	}

	// Test two vectors for equality
	template<typename T>
	bool vector<T>::operator==(const vector<T> &other) const noexcept {
		if (m_dimension != other.m_dimension) { return false; }
		if (m_matrix != other.m_matrix) { return false; }
		return true;
	}

	// Test two vectors for inequality
	template<typename T>
	bool vector<T>::operator!=(const vector<T> &other) const noexcept {
		if (*this == other) { return false; }
		return true;
	}

	// Allow left handed multiplication by constant
	template<typename U, typename T>
	vector<T> operator*(const U &lhs, const vector<T> &v)
		noexcept(std::is_nothrow_constructible_v<vector<T>, size_t>) {
		return v * lhs;
	}

	// Test wise implementation
	template<typename T>
	std::ostream& operator<<(std::ostream &os, const vector<T> &v) {
		if constexpr (std::is_arithmetic<T>::value) {
			T max(0);
			for (size_t i = 0; i < v.size(); i++) {
				if (std::abs(v[i]) > max) { max = std::abs(v[i]); }
			}
			int maxLength = std::log(max) / std::log(10) + 1.000001;
			maxLength = std::max(maxLength, 1);
			for (size_t i = 0; i < v.size(); i++) {
				os << "| ";
				int length = v[i] == 0 ? 1 : std::log(std::abs(v[i])) / std::log(10) + 1.000001;
				for (int j = length; j < maxLength; j++) {
					os << " ";
				}
				os << v[i] << " |\n";
			}
		}
		else {
			for (size_t i = 0; i < v.size(); i++) {
				os << "| " << v[i] << " |\n";
			}
		}
		return os;
	}
}