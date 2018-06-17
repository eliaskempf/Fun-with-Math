#pragma once

#include <stdexcept>
#include <cmath>
#include <iostream>
#include <utility>

namespace la {
	template<typename T>
	class Matrix;

	namespace fields {
		class Complex;
	}

	template<typename T = double>
	class Vector {
	private:
		size_t mDimension;
		Matrix<T> mMatrix;

		friend Matrix<T>;

	public:
		// Constructors
		explicit Vector(size_t, T = 0) noexcept;
		Vector(const Vector &) noexcept;
		Vector(Vector &&) noexcept;

		// Getter for dimensions
		double length() const;
		size_t dimension() const;

		// Transform into norm
		void normalize();

		// Calculate angle between vectors
		double angle(const Vector &) const;

		// Overloaded operators
		T operator[](size_t) const;
		T& operator[](size_t);
		T operator*(const Vector &) const;
		Vector operator*(const double &) const;
		Vector operator/(const double &) const;
		Vector operator+(const Vector &) const;
		Vector operator-(const Vector &) const;
		Vector& operator*=(const double &);
		Vector& operator/=(const double &);
		Vector& operator+=(const Vector &);
		Vector& operator-=(const Vector &);
		Vector& operator=(const Vector &);
		Vector& operator=(Vector &&);
		bool operator==(const Vector &) const;
		bool operator!=(const Vector &) const;
	};

	template<typename T>
	std::ostream& operator<<(std::ostream &, const Vector<T> &);

	template<typename T>
	Vector<T> operator*(const double, const Vector<T> &);

	template<typename T>
	Vector<T>::Vector(size_t size, T defVal) noexcept
		: mDimension(size), mMatrix(size, 1, defVal) 
	{}

	template<typename T>
	Vector<T>::Vector(const Vector<T> &other) noexcept
		: mDimension(other.mDimension), mMatrix(other.mMatrix)
	{}

	template<typename T>
	Vector<T>::Vector(Vector<T> &&other) noexcept
		: mDimension(other.mDimension), mMatrix(std::forward<Matrix<T>>(other.mMatrix))
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
		return mDimension;
	}

	template<typename T>
	void Vector<T>::normalize() {
		if (std::is_integral<T>::value) {
			throw std::logic_error("Can not normalize vector of integral type.");
		}

		double norm = length();
		for (size_t i = 0; i < mDimension; i++) {
			mMatrix(i, 0) /= norm;
		}
	}

	template<typename T>
	double Vector<T>::angle(const Vector<T> &other) const {
		return std::acos((*this * other) / (length() * other.length()));
	}

	template<typename T>
	T Vector<T>::operator[](size_t index) const {
		if (index >= mDimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return mMatrix(index, 0);
	}

	template<typename T>
	T& Vector<T>::operator[](size_t index) {
		if (index >= mDimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return mMatrix(index, 0);
	}

	template<typename T>
	T Vector<T>::operator*(const Vector<T> &other) const {
		if (mDimension != other.mDimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		T scalar = 0;
		for (size_t i = 0; i < mDimension; i++) { scalar += mMatrix(i, 0) * other.mMatrix(i, 0); }
		return scalar;
	}

	template<typename T>
	Vector<T> Vector<T>::operator*(const double &other) const {
		Vector<T> v(mDimension);
		v.mMatrix = mMatrix * other;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator/(const double &other) const {
		Vector<T> v(mDimension);
		v.mMatrix = mMatrix / other;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator+(const Vector<T> &other) const {
		if (mDimension != other.mDimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		Vector<T> v(mDimension);
		v.mMatrix = mMatrix + other.mMatrix;
		return v;
	}

	template<typename T>
	Vector<T> Vector<T>::operator-(const Vector<T> &other) const {
		if (mDimension != other.mDimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		Vector<T> v(mDimension);
		v.mMatrix = mMatrix - other.mMatrix;
		return v;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator*=(const double &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator/=(const double &other) {
		*this = *this / other;
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
			mDimension = other.mDimension;
			mMatrix = other.mMatrix;
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector<T> &&other) {
		if (this != &other) {
			mDimension = other.mDimension;
			mMatrix = std::forward<Matrix<T>>(other.mMatrix);
		}
		return *this;
	}

	template<typename T>
	bool Vector<T>::operator==(const Vector<T> &other) const {
		if (mDimension != other.mDimension) { return false; }
		if (mMatrix != other.mMatrix) { return false; }
		return true;
	}

	template<typename T>
	bool Vector<T>::operator!=(const Vector<T> &other) const {
		if (*this == other) { return false; }
		return true;
	}

	// Test wise implementation
	template<typename T>
	std::ostream& operator<<(std::ostream &os, const Vector<T> &v) {
		if constexpr (std::is_arithmetic<T>::value) {
			T max = 0;
			for (size_t i = 0; i < v.dimension(); i++) {
				if (v[i] > max) { max = v[i]; }
			}
			int maxLength = std::log(max) / std::log(10) + 1.000001;
			maxLength = std::max(maxLength, 1);
			for (size_t i = 0; i < v.dimension(); i++) {
				os << "| ";
				int length = v[i] == 0 ? 1 : std::log(v[i]) / std::log(10) + 1.000001;
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

	template<typename T>
	Vector<T> operator*(const double x, const Vector<T> &v) {
		return v * x;
	}
}