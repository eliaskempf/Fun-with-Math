#pragma once
#include <stdexcept>
#include "Matrix.hpp"

namespace la {
	template<typename T = double>
	class Vector {
	private:
		size_t mDimension;
		Matrix<T> mEntries;

	public:
		// Constructors
		Vector(int, T = 0);
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
		Vector operator+(const Vector &) const;
		Vector& operator=(const Vector &);
		Vector& operator=(Vector &&);

	};

	template<typename T>
	Vector<T>::Vector(int size, T defVal) 
		: mDimension(size), mEntries(size, 1, defVal) 
	{}

	template<typename T>
	Vector<T>::Vector(const Vector<T> &other) 
		: mDimension(other.mDimension), mEntries(other.mEntries) 
	{}

	template<typename T>
	Vector<T>::Vector(Vector<T> &&other)
		: mDimension(other.mDimension), mEntries(other.mEntries) 
	{}

	template<typename T>
	double Vector<T>::getLength() const {
		double sum = 0;
		for (int i = 0; i < mDimension; i++) { sum += mEntries(i, 0) * mEntries(i, 0); }
		return sqrt(sum);
	}

	template<typename T>
	size_t Vector<T>::getDimension() const {
		return mDimension;
	}

	template<typename T>
	void Vector<T>::normalize() {
		double norm = this->getLength();
		for (size_t i = 0; i < mDimension; i++) {
			mEntries(i, 0) /= norm;
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
		return mEntries(index, 0);
	}

	template<typename T>
	T& Vector<T>::operator[](int index) {
		if (index >= mDimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return mEntries(index, 0);
	}

	template<typename T>
	Vector<T> Vector<T>::operator+(const Vector<T> &other) const {
		if (mDimension != other.mDimension) {
			throw std::runtime_error("Vectors can not differ in dimension.");
		}

		Vector<T> v(mDimension);
		v.mEntries = mEntries + other.mEntries;
		return v;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T> &other) {
		if (this != &other) {
			mDimension = other.mDimension;
			mEntries = other.mEntries;
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector<T> &&other) {
		if (this != &other) {
			mDimension = other.mDimension;
			mEntries = other.mEntries;
		}
		return *this;
	}
}