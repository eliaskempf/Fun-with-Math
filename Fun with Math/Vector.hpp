#pragma once
#include <stdexcept>
#include <cmath>

namespace la {
	template<typename T>
	class Matrix;

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
		friend std::ostream& operator<<(std::ostream &, const Vector &);
	};

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
		: mDimension(other.mDimension), mMatrix(other.mMatrix)
	{}

	template<typename T>
	double Vector<T>::length() const {
		return sqrt((*this) * (*this));
	}

	template<typename T>
	size_t Vector<T>::dimension() const {
		return mDimension;
	}

	template<typename T>
	void Vector<T>::normalize() {
		double norm = this->length();
		for (size_t i = 0; i < mDimension; i++) {
			mMatrix(i, 0) /= norm;
		}
	}

	template<>
	void Vector<int>::normalize() {
		throw std::logic_error("Can not normalize an integer vector.");
	}

	template<typename T>
	double Vector<T>::angle(const Vector<T> &other) const {
		return acos((*this * other) / (this->length() * other.length()));
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
			mMatrix = other.mMatrix;
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
	std::ostream& operator<<(std::ostream &os, const Vector<double> &v) {
		os << v.mMatrix;
		return os;
	}
}