#pragma once

#include <stdexcept>
#include <iostream>
#include <utility>
#include <cmath>

namespace la {
	template<typename T>
	class Matrix;

	template<typename T = double>
	class Vector {
	private:
		size_t m_dimension;
		Matrix<T> m_matrix;

		friend Matrix<T>;

	public:
		// Constructors
		explicit Vector(size_t, T = 0) noexcept;
		Vector(std::initializer_list<T>);
		Vector(const Vector &) noexcept;
		Vector(Vector &&) noexcept;

		// Getter for dimensions
		double length() const;
		size_t dimension() const;

		// Return normal vector
		Vector normalize() const;

		// Calculate angle between vectors
		double angle(const Vector &) const;

		// Overloaded operators
		T operator[](size_t) const;
		T& operator[](size_t);
		T operator*(const Vector &) const;
		Vector operator*(double) const;
		Vector operator*(const T &) const;
		Vector operator/(double) const;
		Vector operator/(const T &) const;
		Vector operator+(const Vector &) const;
		Vector operator-(const Vector &) const;
		Vector& operator*=(double);
		Vector& operator*=(const T &);
		Vector& operator/=(double);
		Vector& operator/=(const T &);
		Vector& operator+=(const Vector &);
		Vector& operator-=(const Vector &);
		Vector& operator=(const Vector &);
		Vector& operator=(Vector &&);
		bool operator==(const Vector &) const;
		bool operator!=(const Vector &) const;
	};


	template<typename T>
	Vector<T> operator*(double, const Vector<T> &);

	template<typename T>
	Vector<T> operator*(const T &, const Vector<T> &);

	template<typename T>
	std::ostream& operator<<(std::ostream &, const Vector<T> &);

	template<typename T>
	Vector<T>::Vector(size_t size, T defVal) noexcept
		: m_dimension(size), m_matrix(size, 1, defVal) 
	{}

	template<typename T>
	Vector<T>::Vector(std::initializer_list<T> list)
		: m_dimension(list.size()), m_matrix(list, false)
	{}

	template<typename T>
	Vector<T>::Vector(const Vector<T> &other) noexcept
		: m_dimension(other.m_dimension), m_matrix(other.m_matrix)
	{}

	template<typename T>
	Vector<T>::Vector(Vector<T> &&other) noexcept
		: m_dimension(other.m_dimension), m_matrix(std::forward<Matrix<T>>(other.m_matrix))
	{}

	template<typename T>
	double Vector<T>::length() const {
		if constexpr (std::is_arithmetic<T>::value) {
			return std::sqrt((*this) * (*this));
		}
		else {
			throw std::logic_error("Can not calculate length of non-arithmetic type vector.");
		}
	}

	template<typename T>
	size_t Vector<T>::dimension() const {
		return m_dimension;
	}

	template<typename T>
	Vector<T> Vector<T>::normalize() const {
		if constexpr (std::is_integral<T>::value) {
			throw std::logic_error("Can not normalize vector of integral type.");
		}

		double norm = length();
		return *this / norm;
	}

	template<typename T>
	double Vector<T>::angle(const Vector<T> &other) const {
		return std::acos((*this * other) / (length() * other.length()));
	}

	template<typename T>
	T Vector<T>::operator[](size_t index) const {
		if (index >= m_dimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return m_matrix(index, 0);
	}

	template<typename T>
	T& Vector<T>::operator[](size_t index) {
		if (index >= m_dimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return m_matrix(index, 0);
	}

	template<typename T>
	T Vector<T>::operator*(const Vector<T> &other) const {
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		T scalar = 0;
		for (size_t i = 0; i < m_dimension; i++) { scalar += m_matrix(i, 0) * other.m_matrix(i, 0); }
		return scalar;
	}

	template<typename T>
	Vector<T> Vector<T>::operator*(double other) const {
		Vector<T> v(m_dimension);
		v.m_matrix = m_matrix * other;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator*(const T &other) const {
		Vector<T> v(m_dimension);
		v.m_matrix = m_matrix * other;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator/(double other) const {
		Vector<T> v(m_dimension);
		v.m_matrix = m_matrix / other;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator/(const T &other) const {
		Vector<T> v(m_dimension);
		v.m_matrix = m_matrix * other;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator+(const Vector<T> &other) const {
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		Vector<T> v(m_dimension);
		v.m_matrix = m_matrix + other.m_matrix;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator-(const Vector<T> &other) const {
		if (m_dimension != other.m_dimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		Vector<T> v(m_dimension);
		v.m_matrix = m_matrix - other.m_matrix;
		return v;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator*=(double other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator*=(const T &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator/=(double other) {
		*this = *this / other;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator/=(const T &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator+=(const Vector<T> &other) {
		*this = *this + other;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator-=(const Vector<T> &other) {
		*this = *this - other;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T> &other) {
		if (this != &other) {
			m_dimension = other.m_dimension;
			m_matrix = other.m_matrix;
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector<T> &&other) {
		if (this != &other) {
			m_dimension = other.m_dimension;
			m_matrix = std::forward<Matrix<T>>(other.m_matrix);
		}
		return *this;
	}

	template<typename T>
	bool Vector<T>::operator==(const Vector<T> &other) const {
		if (m_dimension != other.m_dimension) { return false; }
		if (m_matrix != other.m_matrix) { return false; }
		return true;
	}

	template<typename T>
	bool Vector<T>::operator!=(const Vector<T> &other) const {
		if (*this == other) { return false; }
		return true;
	}

	template<typename T>
	Vector<T> operator*(double lhs, const Vector<T> &v) {
		return lhs * v;
	}

	template<typename T>
	Vector<T> operator*(const T &lhs, const Vector<T> &rv) {
		return lhs * v;
	}

	// Test wise implementation
	template<typename T>
	std::ostream& operator<<(std::ostream &os, const Vector<T> &v) {
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