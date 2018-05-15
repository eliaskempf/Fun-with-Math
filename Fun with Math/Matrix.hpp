#pragma once

namespace la {
	template<typename T>
	class Matrix {
	private:
		Vector<T> * mEntries;
		int mRows;
		int mCols;

	public:
		Matrix(int, int);
		~Matrix();

		int rows();
		int columns();
		Vector<T>& operator[](int);
	};

	template<typename T>
	Matrix<T>::Matrix(int rows, int cols)
		: mCols(cols), mRows(rows) {
		mEntries = new Vector<T>[rows](rows);
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
	Vector<T>& Matrix<T>::operator[](int rows) {
		if (rows >= mRows) {
			throw std::out_ouf_range("Exceeded Matrix range.");
		}

		return mEntries[rows];
	}
}
