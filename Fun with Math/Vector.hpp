#pragma once

namespace la {
	template<typename T>
	class Vector {
	private:
		int mDimension;
		T * mEntries;

	public:
		Vector(int);
		~Vector();
		double getLength();
		int getDimension();
		T& operator[](int);
	};

	template<typename T>
	Vector<T>::Vector(int size) 
		: mDimension(size) {
		mEntries = new T[size];
	}

	template<typename T>
	Vector<T>::~Vector() {
		delete[] mEntries;
	}

	template<typename T>
	double Vector<T>::getLength() {
		double sum = 0;
		for (int i = 0; i < mDimension; i++) { sum += mEntries[i] * mEntries[i]; }
		return sqrt(sum);
	}

	template<typename T>
	int Vector<T>::getDimension() {
		return mDimension;
	}

	template<typename T>
	T& Vector<T>::operator[](int index) {
		if (index >= mDimension) {
			throw std::out_of_range("Vector index out of range.");
		}
		return mEntries[index];
	}
}