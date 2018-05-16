#pragma once
#include <iostream>

namespace la {
	template<typename T = double>
	class Vector {
	private:
		T * mEntries;
		size_t mDimension;

	public:
		Vector(int, T = 0);
		~Vector();

		double getLength() const;
		size_t getDimension() const;
		void normalize();

		T operator[](int) const;
		T& operator[](int);
	};

	template<typename T>
	Vector<T>::Vector(int size, T defVal) 
		: mDimension(size) {
		mEntries = new T[size];
		memset(mEntries, defVal, size * sizeof(T));
	}

	template<typename T>
	Vector<T>::~Vector() {
		delete[] mEntries;
	}

	template<typename T>
	double Vector<T>::getLength() const {
		double sum = 0;
		for (int i = 0; i < mDimension; i++) { sum += mEntries[i] * mEntries[i]; }
		return sqrt(sum);
	}

	template<typename T>
	size_t Vector<T>::getDimension() const {
		return mDimension;
	}

	template<typename T>
	void Vector<T>::normalize() {
		double norm = this->getLength();
		std::cout << norm << std::endl;
		for (size_t i = 0; i < mDimension; i++) {
			mEntries[i] /= norm;
		}
	}

	/*
	void Vector<int>::normalize() {
		Vector<double> v(mDimension);
		double norm = this->getLength();
		for (size_t i = 0; i < mDimension; i++) {
			v[i] = mEntries[i] / norm;
		}
	}
	*/
	template<typename T>
	T Vector<T>::operator[](int index) const {
		if (index >= mDimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return mEntries[index];
	}

	template<typename T>
	T& Vector<T>::operator[](int index) {
		if (index >= mDimension) {
			throw std::out_of_range("Exceeded vector range.");
		}
		return mEntries[index];
	}
}