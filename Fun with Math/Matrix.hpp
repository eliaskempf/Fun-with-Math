#pragma once

namespace la {
	template<typename T = double>
	class Matrix {
	private:
		T * mEntries;
		size_t mRows;
		size_t mCols;

	public:
		// Constructors
		Matrix(int, int, T = 0);
		Matrix(const Matrix &);
		Matrix(Matrix &&);

		// Destructor
		~Matrix();

		// getter for the dimensions
		size_t rows() const;
		size_t columns() const;
		size_t entries() const;

		// Overloaded operators
		T operator()(int, int) const;
		T& operator()(int, int);
		Matrix operator+(const Matrix &) const;
		Matrix& operator=(const Matrix &);
		Matrix& operator=(Matrix &&);
	};

	template<typename T>
	Matrix<T>::Matrix(int rows, int cols, T defVal)
		: mCols(cols), mRows(rows) {
		mEntries = new T[rows * cols];
		memset(mEntries, defVal, rows * cols * sizeof(T));
	}

	template<typename T>
	Matrix<T>::Matrix(const Matrix<T> &other) {
		mRows = other.mRows;
		mCols = other.mCols;
		mEntries = new T[other.entries()];
		memcpy(mEntries, other.mEntries, other.entries() * sizeof(T));
	}

	template<typename T>
	Matrix<T>::Matrix(Matrix<T> &&other) {
		mRows = other.mRows;
		mCols = other.mCols;
		mEntries = other.mEntries;
		other.mEntries = nullptr;
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		delete[] mEntries;
	}

	template<typename T>
	size_t Matrix<T>::rows() const {
		return mRows;
	}

	template<typename T>
	size_t Matrix<T>::columns() const {
		return mCols;
	}

	template<typename T>
	size_t Matrix<T>::entries() const {
		return mRows * mCols;
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

	template<typename T>
	Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const {
		if (mRows != other.mRows || mCols != other.mCols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		Matrix<T> m(mRows, mCols);
		for (size_t i = 0; i < this->entries(); i++) {
			m.mEntries[i] = this->mEntries[i] + other.mEntries[i];
		}
		return m;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(const Matrix<T> &other) {
		if (this != &other) {
			if (this->entries() != other.entries()) {
				delete[] mEntries;
				mEntries = new T[other.entries()];
			}
			mRows = other.mRows;
			mCols = other.mCols;
			memcpy(mEntries, other.mEntries, other.entries() * sizeof(T));
		}
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(Matrix<T> &&other) {
		if (this != &other) {
			delete[] mEntries;
			mEntries = other.mEntries;
			other.mEntries = nullptr;
			mRows = other.mRows;
			mCols = other.mCols;
		}
		return *this;
	}
}
