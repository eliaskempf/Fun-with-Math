#pragma once

namespace la {
	template<typename T = double>
	class Matrix {
	private:
		T * mEntries;
		size_t mRows;
		size_t mCols;

	public:
		Matrix(int, int, T = 0);
		~Matrix();

		int rows() const;
		int columns() const;

		T operator()(int, int) const;
		T& operator()(int, int);
	};

	template<typename T>
	Matrix<T>::Matrix(int rows, int cols, T defVal)
		: mCols(cols), mRows(rows) {
		mEntries = new T[rows * cols];
		memset(mEntries, defVal, rows * cols * sizeof(T));
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		delete[] mEntries;
	}

	template<typename T>
	int Matrix<T>::rows() const {
		return mRows;
	}

	template<typename T>
	int Matrix<T>::columns() const {
		return mCols;
	}

	template<typename T>
	T Matrix<T>::operator()(int i, int j) const {
		if (i > mRows || i < 1 || j > mCols || j < 1) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return mEntries[(i - 1) * mCols + (j - 1)];
	}

	template<typename T>
	T& Matrix<T>::operator()(int i, int j) {
		if (i > mRows || i < 1 || j > mCols || j < 1) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return mEntries[(i - 1) * mCols + (j - 1)];
	}
}
