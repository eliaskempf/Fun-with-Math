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
		explicit vector(size_t, T = T(0)) noexcept;
		vector(std::initializer_list<T>);
		vector(const vector &) noexcept;
		vector(vector &&) noexcept;

		// Getter for dimensions
		double length() const;
		size_t dimension() const;

		// Return normal vector
		vector normalize() const;

		// Calculate angle between vectors
		double angle(const vector &) const;

		// Overloaded operators
		T operator[](size_t) const;
		T& operator[](size_t);
		T operator*(const vector &) const;
		vector operator*(double) const;
		vector operator*(const T &) const;
		vector operator/(double) const;
		vector operator/(const T &) const;
		vector operator+(const vector &) const;
		vector operator-(const vector &) const;
		vector& operator*=(double);
		vector& operator*=(const T &);
		vector& operator/=(double);
		vector& operator/=(const T &);
		vector& operator+=(const vector &);
		vector& operator-=(const vector &);
		vector& operator=(const vector &);
		vector& operator=(vector &&);
		bool operator==(const vector &) const;
		bool operator!=(const vector &) const;
	};


	template<typename T>
	vector<T> operator*(double, const vector<T> &);

	template<typename T>
	vector<T> operator*(const T &, const vector<T> &);

	template<typename T>
	std::ostream& operator<<(std::ostream &, const vector<T> &);

	template<typename T>
	vector<T>::vector(size_t size, T defVal) noexcept
		: m_dimension(size), m_matrix(size, 1, defVal) 
	{}

	template<typename T>
	vector<T>::vector(std::initializer_list<T> list)
		: m_dimension(list.size()), m_matrix(list, false)
	{}

	template<typename T>
	vector<T>::vector(const vector<T> &other) noexcept
		: m_dimension(other.m_dimension), m_matrix(other.m_matrix)
	{}

	template<typename T>
	vector<T>::vector(vector<T> &&other) noexcept
		: m_dimension(other.m_dimension), m_matrix(std::forward<matrix<T>>(other.m_matrix))
	{}

	template<typename T>
	double vector<T>::length() const {
		if constexpr (std::is_arithmetic<T>::value) {
			return std::sqrt((*this) * (*this));
		}
		else {
			throw std::logic_error("Can not calculate length of non-arithmetic type vector.");
		}
	}

	template<typename T>
	size_t vector<T>::dimension() const {
		return m_dimension;
	}

	template<typename T>
	vector<T> vector<T>::normalize() const {
		if constexpr (std::is_integral<T>::value) {
			throw std::logic_error("Can not normalize vector of integral type.");
		}

		double norm = length();
		return *this / norm;
	}

	template<typename T>
	double vector<T>::angle(const vector<T> &other) const {
		return std::acos((*this * other) / (length() * other.length()));
	}

	template<typename T>
	T vector<T>::operator[](size_t index) const {
		if (index >= m_dimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return m_matrix(index, 0);
	}

	template<typename T>
	T& vector<T>::operator[](size_t index) {
		if (index >= m_dimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return m_matrix(index, 0);
	}

	template<typename T>
	T vector<T>::operator*(const vector<T> &other) const {
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		T scalar = 0;
		for (size_t i = 0; i < m_dimension; i++) { scalar += m_matrix(i, 0) * other.m_matrix(i, 0); }
		return scalar;
	}

	template<typename T>
	vector<T> vector<T>::operator*(double other) const {
		vector<T> v(m_dimension);
		v.m_matrix = m_matrix * other;
		return v;
	}

	template<typename T>
	vector<T> vector<T>::operator*(const T &other) const {
		vector<T> v(m_dimension);
		v.m_matrix = m_matrix * other;
		return v;
	}

	template<typename T>
	vector<T> vector<T>::operator/(double other) const {
		vector<T> v(m_dimension);
		v.m_matrix = m_matrix / other;
		return v;
	}

	template<typename T>
	vector<T> vector<T>::operator/(const T &other) const {
		vector<T> v(m_dimension);
		v.m_matrix = m_matrix * other;
		return v;
	}

	template<typename T>
	vector<T> vector<T>::operator+(const vector<T> &other) const {
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		vector<T> v(m_dimension);
		v.m_matrix = m_matrix + other.m_matrix;
		return v;
	}

	template<typename T>
	vector<T> vector<T>::operator-(const vector<T> &other) const {
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		vector<T> v(m_dimension);
		v.m_matrix = m_matrix - other.m_matrix;
		return v;
	}

	template<typename T>
	vector<T>& vector<T>::operator*=(double other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator*=(const T &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator/=(double other) {
		*this = *this / other;
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator/=(const T &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator+=(const vector<T> &other) {
		*this = *this + other;
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator-=(const vector<T> &other) {
		*this = *this - other;
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator=(const vector<T> &other) {
		if (this != &other) {
			m_dimension = other.m_dimension;
			m_matrix = other.m_matrix;
		}
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator=(vector<T> &&other) {
		if (this != &other) {
			m_dimension = other.m_dimension;
			m_matrix = std::forward<matrix<T>>(other.m_matrix);
		}
		return *this;
	}

	template<typename T>
	bool vector<T>::operator==(const vector<T> &other) const {
		if (m_dimension != other.m_dimension) { return false; }
		if (m_matrix != other.m_matrix) { return false; }
		return true;
	}

	template<typename T>
	bool vector<T>::operator!=(const vector<T> &other) const {
		if (*this == other) { return false; }
		return true;
	}

	template<typename T>
	vector<T> operator*(double lhs, const vector<T> &v) {
		return lhs * v;
	}

	template<typename T>
	vector<T> operator*(const T &lhs, const vector<T> &v) {
		return lhs * v;
	}

	// Test wise implementation
	template<typename T>
	std::ostream& operator<<(std::ostream &os, const vector<T> &v) {
		if constexpr (std::is_arithmetic<T>::value) {
			T max = 0;
			for (size_t i = 0; i < v.dimension(); i++) {
				if (std::abs(v[i]) > max) { max = std::abs(v[i]); }
			}
			int maxLength = std::log(max) / std::log(10) + 1.000001;
			maxLength = std::max(maxLength, 1);
			for (size_t i = 0; i < v.dimension(); i++) {
				os << "| ";
				int length = v[i] == 0 ? 1 : std::log(std::abs(v[i])) / std::log(10) + 1.000001;
				for (int j = length; j < maxLength; j++) {
					os << " ";
				}
				os << v[i] << " |\n";
			}
		}
		else {
			for (size_t i = 0; i < v.dimension(); i++) {
				os << "| " << v[i] << " |\n";
			}
		}
		return os;
	}
}