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
		T * m_entries;
		size_t m_rows;
		size_t m_cols;

	public:
		// Constructors
		Matrix(size_t, size_t, T = 0);
		Matrix(std::initializer_list<T>, bool) noexcept;
		Matrix(std::initializer_list<std::initializer_list<T>>);
		Matrix(const Matrix &) noexcept;
		Matrix(Matrix &&) noexcept;

		// Destructor
		virtual ~Matrix();

		// getter for the dimensions
		size_t rows() const;
		size_t columns() const;
		size_t entries() const;

		// Calculate determinant
		double det() const;

		// Static methods
		static Matrix IdentityMatrix(size_t);
		static Matrix ElementaryMatrix(size_t, size_t, size_t, T);

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
	};

	template<typename T>
	Matrix<T> operator*(double, const Matrix<T> &);

	template<typename T>
	std::ostream& operator<<(std::ostream &, const Matrix<T> &);

	template<typename T>
	Matrix<T>::Matrix(size_t rows, size_t cols, T defVal)
		: m_cols(cols), m_rows(rows) {
		if (m_cols == 0 || m_rows == 0) {
			throw std::logic_error("Matrix does not allow zero dimensions.");
		}

		m_entries = new T[rows * cols];
		std::fill(m_entries, m_entries + (rows * cols), defVal);
	}

	template<typename T>
	Matrix<T>::Matrix(std::initializer_list<T> list, bool mode) noexcept {
		m_entries = new T[list.size()];
		m_cols = mode ? list.size() : 1;
		m_rows = mode ? 1 : list.size();
		std::copy(list.begin(), list.end(), stdext::checked_array_iterator<T*>(m_entries, m_cols));
	}

	template<typename T>
	Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> list) {
		for (const auto i : list) {
			if (i.size() != list.begin()->size()) {
				throw std::runtime_error("All rows have to be equal in length.");
			}
		}

		m_rows = list.size();
		m_cols = list.begin()->size();
		m_entries = new T[m_rows * m_cols];

		uint32_t index = 0;
		for (const auto i : list) {
			std::copy(i.begin(), i.end(), stdext::checked_array_iterator<T*>(m_entries + m_cols * index, m_cols));
			index++;
		}
	}

	template<typename T>
	Matrix<T>::Matrix(const Matrix<T> &other) noexcept {
		m_rows = other.m_rows;
		m_cols = other.m_cols;
		m_entries = new T[other.entries()];
		std::copy(other.m_entries, other.m_entries + other.entries(), stdext::checked_array_iterator<T*>(m_entries, entries()));
	}

	template<typename T>
	Matrix<T>::Matrix(Matrix<T> &&other) noexcept {
		m_rows = other.m_rows;
		m_cols = other.m_cols;
		m_entries = other.m_entries;
		other.m_entries = nullptr;
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		delete[] m_entries;
	}

	template<typename T>
	size_t Matrix<T>::rows() const {
		return m_rows;
	}

	template<typename T>
	size_t Matrix<T>::columns() const {
		return m_cols;
	}

	template<typename T>
	size_t Matrix<T>::entries() const {
		return m_rows * m_cols;
	}

	template<typename T>
	double Matrix<T>::det() const {
		if (m_cols != m_rows) {
			throw std::logic_error("Matrix has to be quadratic.");
		}

		if (m_cols <= 2) {
			if (m_cols == 2) {
				return m_entries[0] * m_entries[3] - m_entries[1] * m_entries[2];
			}
			else {
				return m_entries[0];
			}
		}

		double d = 0;
		for (size_t j = 0; j < m_cols; j++) {
			Matrix<T> sm(m_rows - 1, m_cols - 1);
			int s = 0;
			for (size_t i = 0; i < sm.entries(); i++) {
				if ((i + s) % m_cols == j) { s++; }
				sm.m_entries[i] = m_entries[i + m_cols + s];
			}
			d += m_entries[j] * std::pow(-1, j) * sm.det();
		}
		return d;
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
		if (i >= m_rows || j >= m_cols) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return m_entries[i * m_cols + j];
	}

	template<typename T>
	T& Matrix<T>::operator()(size_t i, size_t j) {
		if (i >= m_rows || j >= m_cols) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return m_entries[i * m_cols + j];
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(double other) const {
		Matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] * other;
		}
		return m;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const {
		if (m_cols != other.m_rows) {
			throw std::runtime_error("Can not multiply by a matrix which rows does not \
				                      match the columns of the original matrix.");
		}

		/*
		int supportedThreads = std::thread::hardware_concurrency();
		supportedThreads = supportedThreads == 0 ? 2 : supportedThreads;

		const int minColsPerThread = 2;

		int cols = m_rows > other.m_cols ? m_rows : other.m_cols;
		int rows = m_rows > other.m_cols ? other.m_cols : m_rows;

		Matrix<T> m(rows, cols);

		int maxAmountOfThreads = (cols + minColsPerThread - 1) / minColsPerThread;
		int amountOfThreads = supportedThreads > maxAmountOfThreads ? maxAmountOfThreads : supportedThreads;

		int colsPerThread = cols / amountOfThreads;

		std::thread* threads = new std::thread[amountOfThreads - 1];

		for (int c = 0; c < amountOfThreads - 1; c++) {
			threads[c] = std::thread([c, rows, m_cols, colsPerThread, this, other, &m]() {
				for (size_t i = 0; i < rows; i++) {
					for (size_t k = c * colsPerThread; k < colsPerThread; k++) {
						T c_ik = 0;
						for (size_t j = 0; j < m_cols; j++) {
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
				for (size_t j = 0; j < m_cols; j++) {
					c_ik += (*this)(i, j) * other(j, k);
				}
				m(i, k) = c_ik;
			}
		}

		for (int c = 0; c < amountOfThreads - 1; c++) {
			threads[c].join();
		}

		if (rows != m_rows) {
			m.m_rows = rows;
			m.m_cols = cols;
		}
		*/

		Matrix<T> m(m_rows, other.m_cols);

		for (size_t i = 0; i < m.m_rows; i++) {
			for (size_t k = 0; k < m.m_cols; k++) {
				T c_ik = 0;
				for (size_t j = 0; j < m_cols; j++) {
					c_ik += (*this)(i, j) * other(j, k);
				}
				m(i, k) = c_ik;
			}
		}
		
		return m;
	}

	template<typename T>
	Vector<T> Matrix<T>::operator*(const Vector<T> &other) const {
		if (m_cols != other.mDimension) {
			throw std::runtime_error("Can not multiply by a vector which dimension does \
									  not match the columns of the matrix.");
		}

		Vector<T> v(m_rows);
		v.mMatrix = *this * other.mMatrix;
		return v;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator/(double other) const {
		Matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] / other;
		}
		return m;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		Matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] + other.m_entries[i];
		}
		return m;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		Matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] - other.m_entries[i];
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
				delete[] m_entries;
				m_entries = new T[other.entries()];
			}
			m_rows = other.m_rows;
			m_cols = other.m_cols;
			std::copy(other.m_entries, other.m_entries + other.entries(), stdext::checked_array_iterator<T*>(m_entries, entries()));
		}
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(Matrix<T> &&other) {
		if (this != &other) {
			delete[] m_entries;
			m_entries = other.m_entries;
			other.m_entries = nullptr;
			m_rows = other.m_rows;
			m_cols = other.m_cols;
		}
		return *this;
	}

	template<typename T>
	bool Matrix<T>::operator==(const Matrix &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) { return false; }
		for (size_t i = 0; i < other.entries(); i++) {
			if (m_entries[i] != other.m_entries[i]) {
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
	Matrix<T> operator*(double lhs, const Matrix<T> &m) {
		return lhs * m;
	}

	// Test wise implementation
	template<typename T>
	std::ostream& operator<<(std::ostream &os, const Matrix<T> &m) {
		if constexpr (std::is_arithmetic<T>::value) {
			T max = 0;
			T fMax = 0;
			bool firstCol = false;
			for (size_t i = 0; i < m.rows(); i++) {
				for (size_t j = 0; j < m.columns(); j++) {
					if (m(i, j) > max) {
						max = m(i, j);
					}

					if (j == 0 && m(i, j) > fMax) {
						fMax = m(i ,j);
					}
				}
			}
			int maxLength = std::log(max) / std::log(10) + 1.000001;
			int fMaxLength = std::log(fMax) / std::log(10) + 1.000001;
			maxLength = std::max(maxLength, 1);
			fMaxLength = std::max(fMaxLength, 1);

			for (size_t i = 0; i < m.rows(); i++) {
				os << "| ";
				for (size_t j = 0; j < m.columns(); j++) {
					int length = m(i, j) == 0 ? 1 : std::log(m(i, j)) / std::log(10) + 1.000001;
					for (int k = 0; k < (j == 0 ? fMaxLength : maxLength) - length; k++) { os << " "; }
					os << m(i, j) << " ";
				}
				os << "|\n";
			}
		}
		else {
			for (size_t i = 0; i < m.rows(); i++) {
				os << "| ";
				for (size_t j = 0; j < m.columns(); j++) {
					os << m(i, j) << " ";
				}
				os << "|\n";
			}
		}
		return os;
	}
}