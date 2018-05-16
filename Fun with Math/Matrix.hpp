#pragma once

namespace la {
	template<typename T>
	class Matrix {
	private:
		T * mEntries;
		int mRows;
		int mCols;

	public:
		Matrix(int, int);
		~Matrix();

		int rows();
		int columns();
		T& entry(int, int);
		T& operator[](int);
	};

	template<typename T>
	Matrix<T>::Matrix(int rows, int cols)
		: mCols(cols), mRows(rows) {
		mEntries = new T[rows * cols];
		memset(mEntries, 0, rows * cols * sizeof(T));
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		delete[] mEntries;
	}

	template<typename T>
	int Matrix<T>::rows() {
		return mRows;
	}

	template<typename T>
	int Matrix<T>::columns() {
		return mCols;
	}

	template<typename T>
	T& Matrix<T>::entry(int i, int j) {
		if (i > mRows || i < 1 || j > mCols || j < 1) {
			throw std::out_of_range("Exceeded matrix range.");
		}

		return mEntries[(i - 1) * mCols + (j - 1)];
	}

	template<typename T>
	T& Matrix<T>::operator[](int entry) {
		return mEntries[rows * mCols + cols];
	}
}
