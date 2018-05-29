#pragma once
#include <stdexcept>

namespace la {
	template<typename T = double>
	class Matrix;

	template<typename T = double>
	class Vector {
	private:
		size_t mDimension;
		Matrix<T> mMatrix;

	public:
		// Constructors
		explicit Vector(size_t, T = 0);
		Vector(const Vector &);
		Vector(Vector &&);

		// getter for dimensions
		double getLength() const;
		size_t getDimension() const;

		// Transform into norm
		void normalize();

		// Overloaded operators
		T operator[](int) const;
		T& operator[](int);
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
	Vector<T>::Vector(size_t size, T defVal) 
		: mDimension(size), mMatrix(size, 1, defVal) 
	{}

	template<typename T>
	Vector<T>::Vector(const Vector<T> &other) 
		: mDimension(other.mDimension), mMatrix(other.mMatrix)
	{}

	template<typename T>
	Vector<T>::Vector(Vector<T> &&other)
		: mDimension(other.mDimension), mMatrix(other.mMatrix)
	{}

	template<typename T>
	double Vector<T>::getLength() const {
		return sqrt((*this) * (*this));
	}

	template<typename T>
	size_t Vector<T>::getDimension() const {
		return mDimension;
	}

	// TODO
	template<typename T>
	void Vector<T>::normalize() {
		double norm = this->getLength();
		for (size_t i = 0; i < mDimension; i++) {
			mMatrix(i, 0) /= norm;
		}
	}

	template<>
	void Vector<int>::normalize() {
		throw std::logic_error("Can not normalize an integer vector.");
	}

	template<typename T>
	T Vector<T>::operator[](int index) const {
		if (index >= mDimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return mMatrix(index, 0);
	}

	template<typename T>
	T& Vector<T>::operator[](int index) {
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
}