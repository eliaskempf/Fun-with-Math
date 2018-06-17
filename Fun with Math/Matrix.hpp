#pragma once

#include <stdexcept>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <thread>

namespace la {
	template <typename T>
	class Vector;

	template<typename T = double>
	class Matrix {
	private:
		T * mEntries;
		size_t mRows;
		size_t mCols;

	public:
		// Constructors
		Matrix(size_t, size_t, T = 0) noexcept;
		Matrix(const Matrix &) noexcept;
		Matrix(Matrix &&) noexcept;

		// Destructor
		virtual ~Matrix();

		// getter for the dimensions
		size_t rows() const;
		size_t columns() const;
		size_t entries() const;

		// Static methods
		static Matrix IdentityMatrix(size_t);
		static Matrix ElementaryMatrix(size_t, size_t, size_t, T);

		// Calculate determinant
		double det() const;

		// Overloaded operators
		T operator()(size_t, size_t) const;
		T& operator()(size_t, size_t);
		Matrix operator*(double) const;
		Matrix operator*(const Matrix &) const;
		Vector<T> operator*(const Vector<T> &) const;
		Matrix operator/(double) const;
		Matrix operator+(const Matrix &) const;
		Matrix operator-(const Matrix &) const;
		Matrix& operator*=(double);
		Matrix& operator*=(const Matrix &);
		Matrix& operator/=(double);
		Matrix& operator+=(const Matrix &);
		Matrix& operator-=(const Matrix &);
		Matrix& operator=(const Matrix &);
		Matrix& operator=(Matrix &&);
		bool operator==(const Matrix &) const;
		bool operator!=(const Matrix &) const;
		friend std::ostream& operator<<(std::ostream &, const Matrix &);
	};

	template<typename T>
	Matrix<T> operator*(double, const Matrix<T> &);

	template<typename T>
	Matrix<T>::Matrix(size_t rows, size_t cols, T defVal) noexcept
		: mCols(cols), mRows(rows) {
		if (mCols == 0 || mRows == 0) {
			throw std::logic_error("Matrix does not allow zero dimensions.");
		}

		mEntries = new T[rows * cols];
		std::fill(mEntries, mEntries + (rows * cols), defVal);
	}

	template<typename T>
	Matrix<T>::Matrix(const Matrix<T> &other) noexcept {
		mRows = other.mRows;
		mCols = other.mCols;
		mEntries = new T[other.entries()];
		std::copy(other.mEntries, other.mEntries + other.entries(), stdext::checked_array_iterator<T*>(mEntries, entries()));
	}

	template<typename T>
	Matrix<T>::Matrix(Matrix<T> &&other) noexcept {
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
	Matrix<T> Matrix<T>::IdentityMatrix(size_t size) {
		Matrix<T> m(size, size);
		for (int i = 0; i < size; i++) {
			m(i, i) = 1;
		}
		return m;
	}

	template<typename T>
	Matrix<T> Matrix<T>::ElementaryMatrix(size_t size, size_t row, size_t col, T val) {
		Matrix<T> m(size, size);
		for (int i = 0; i < size; i++) {
			m(i, i) = 1;
		}
		m(row, col) = val;
		return m;
	}

	template<typename T>
	T Matrix<T>::operator()(size_t i, size_t j) const {
		if (i >= mRows || i < 0 || j >= mCols || j < 0) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return mEntries[i * mCols + j];
	}

	template<typename T>
	T& Matrix<T>::operator()(size_t i, size_t j) {
		if (i >= mRows || j >= mCols) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return mEntries[i * mCols + j];
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(double other) const {
		Matrix<T> m(mRows, mCols);
		for (size_t i = 0; i < entries(); i++) {
			m.mEntries[i] = mEntries[i] * other;
		}
		return m;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const {
		if (mCols != other.mRows) {
			throw std::runtime_error("Can not multiply by a matrix which rows does not \
				                      match the columns of the original matrix.");
		}

		/*
		int supportedThreads = std::thread::hardware_concurrency();
		supportedThreads = supportedThreads == 0 ? 2 : supportedThreads;

		const int minColsPerThread = 2;

		int cols = mRows > other.mCols ? mRows : other.mCols;
		int rows = mRows > other.mCols ? other.mCols : mRows;

		Matrix<T> m(rows, cols);

		int maxAmountOfThreads = (cols + minColsPerThread - 1) / minColsPerThread;
		int amountOfThreads = supportedThreads > maxAmountOfThreads ? maxAmountOfThreads : supportedThreads;

		int colsPerThread = cols / amountOfThreads;

		std::thread* threads = new std::thread[amountOfThreads - 1];

		for (int c = 0; c < amountOfThreads - 1; c++) {
			threads[c] = std::thread([c, rows, mCols, colsPerThread, this, other, &m]() {
				for (size_t i = 0; i < rows; i++) {
					for (size_t k = c * colsPerThread; k < colsPerThread; k++) {
						T c_ik = 0;
						for (size_t j = 0; j < mCols; j++) {
							c_ik += (*this)(i, j) * other(j, k);
						}
						m(i, k) = c_ik;
					}
				}
			}, std::ref(m));
		}

		for (size_t i = 0; i < rows; i++) {
			for (size_t k = (amountOfThreads - 1) * colsPerThread; k < cols; k++) {
				T c_ik = 0;
				for (size_t j = 0; j < mCols; j++) {
					c_ik += (*this)(i, j) * other(j, k);
				}
				m(i, k) = c_ik;
			}
		}

		for (int c = 0; c < amountOfThreads - 1; c++) {
			threads[c].join();
		}

		if (rows != mRows) {
			m.mRows = rows;
			m.mCols = cols;
		}
		*/

		Matrix<T> m(mRows, other.mCols);

		for (size_t i = 0; i < m.mRows; i++) {
			for (size_t k = 0; k < m.mCols; k++) {
				T c_ik = 0;
				for (size_t j = 0; j < mCols; j++) {
					c_ik += (*this)(i, j) * other(j, k);
				}
				m(i, k) = c_ik;
			}
		}
		
		return m;
	}

	template<typename T>
	Vector<T> Matrix<T>::operator*(const Vector<T> &other) const {
		if (mCols != other.mDimension) {
			throw std::runtime_error("Can not multiply by a vector which dimension does \
									  not match the columns of the matrix.");
		}

		Vector<T> v(mRows);
		v.mMatrix = *this * other.mMatrix;
		return v;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator/(double other) const {
		Matrix<T> m(mRows, mCols);
		for (size_t i = 0; i < entries(); i++) {
			m.mEntries[i] = mEntries[i] / other;
		}
		return m;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const {
		if (mRows != other.mRows || mCols != other.mCols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		Matrix<T> m(mRows, mCols);
		for (size_t i = 0; i < entries(); i++) {
			m.mEntries[i] = mEntries[i] + other.mEntries[i];
		}
		return m;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) const {
		if (mRows != other.mRows || mCols != other.mCols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		Matrix<T> m(mRows, mCols);
		for (size_t i = 0; i < entries(); i++) {
			m.mEntries[i] = mEntries[i] - other.mEntries[i];
		}
		return m;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator*=(double other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator*=(const Matrix<T> &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator/=(double other) {
		*this = *this / other;
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator+=(const Matrix<T> &other) {
		*this = *this + other;
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator-=(const Matrix<T> &other) {
		*this = *this - other;
		return *this;
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
			std::copy(other.mEntries, other.mEntries + other.entries(), stdext::checked_array_iterator<T*>(mEntries, entries()));
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

	template<typename T>
	bool Matrix<T>::operator==(const Matrix &other) const {
		if (mRows != other.mRows || mCols != other.mCols) { return false; }
		for (size_t i = 0; i < other.entries(); i++) {
			if (mEntries[i] != other.mEntries[i]) {
				return false;
			}
		}
		return true;
	}

	template<typename T>
	bool Matrix<T>::operator!=(const Matrix &other) const {
		if (*this == other) { return false; }
		return true;
	}

	template<typename T>
	double Matrix<T>::det() const {
		if (mCols != mRows) {
			throw std::logic_error("Matrix has to be quadratic.");
		}

		if (mCols <= 2) {
			if (mCols == 2) {
				return mEntries[0] * mEntries[3] - mEntries[1] * mEntries[2];
			}
			else {
				return mEntries[0];
			}
		}

		double d = 0;
		for (size_t j = 0; j < mCols; j++) {
			Matrix<T> sm(mRows - 1, mCols - 1);
			int s = 0;
			for (size_t i = 0; i < sm.entries(); i++) {
				if ((i + s) % mCols == j) { s++; }
				sm.mEntries[i] = mEntries[i + mCols + s];
			}
			d += mEntries[j] * std::pow(-1, j) * sm.det();
		}
		return d;
	}

	// Test wise implementation
	std::ostream& operator<<(std::ostream &os, const Matrix<double> &m) {
		double max = 0;
		double fMax = 0;
		bool firstCol = false;
		for (size_t i = 0; i < m.entries(); i++) {
			if (m.mEntries[i] > max) {
				max = m.mEntries[i];
			}

			if (i % m.mCols == 0 && m.mEntries[i] > fMax) {
				fMax = m.mEntries[i];
			}
		}
		int maxLength = std::log(max) / std::log(10) + 1.000001;
		int fMaxLength = std::log(fMax) / std::log(10) + 1.000001;
		maxLength = std::max(maxLength, 1);
		fMaxLength = std::max(fMaxLength, 1);

		for (size_t i = 0; i < m.mRows; i++) {
			os << "| ";
			for (size_t j = 0; j < m.mCols; j++) {
				int length = m(i, j) == 0 ? 1 : std::log(m(i, j)) / std::log(10) + 1.000001;
				for (int k = 0; k < (j == 0 ? fMaxLength : maxLength) - length; k++) { os << " "; }
				os << m(i, j) << " ";
			}
			os << "|\n";
		}
		return os;
	}

	template<typename T>
	Matrix<T> operator*(double x, const Matrix<T> &m) {
		return m * x;
	}
}