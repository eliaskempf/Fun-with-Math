#pragma once

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <future>
#include <thread>
#include <cmath>

namespace la {
	template <typename T>
	class vector;

	template<typename T = double>
	class matrix {
	private:
		T * m_entries;
		size_t m_rows;
		size_t m_cols;

	public:
		// Constructors
		matrix() noexcept = default;
		matrix(size_t, size_t, T = T(0));
		matrix(std::initializer_list<T>, bool) noexcept;
		matrix(std::initializer_list<std::initializer_list<T>>);
		matrix(const matrix &) noexcept;
		matrix(matrix &&) noexcept;

		// Destructor
		virtual ~matrix();

		// getter for the dimensions
		size_t rows() const;
		size_t columns() const;
		size_t entries() const;

		// Mathematical matrix operations
		matrix transpose() const;
		matrix gauss() const;
		T det() const;

		// Static methods
		static matrix Identitymatrix(size_t);
		static matrix Elementarymatrix(size_t, size_t, size_t, T);

		// Overloaded operators
		T operator()(size_t, size_t) const;
		T& operator()(size_t, size_t);
		matrix operator*(double) const;
		matrix operator*(const matrix &) const;
		vector<T> operator*(const vector<T> &) const;
		matrix operator/(double) const;
		matrix operator+(const matrix &) const;
		matrix operator-(const matrix &) const;
		matrix& operator*=(double);
		matrix& operator*=(const matrix &);
		matrix& operator/=(double);
		matrix& operator+=(const matrix &);
		matrix& operator-=(const matrix &);
		matrix& operator=(const matrix &);
		matrix& operator=(matrix &&);
		bool operator==(const matrix &) const;
		bool operator!=(const matrix &) const;
	};

	template<typename T>
	matrix<T> operator*(double, const matrix<T> &);

	template<typename T>
	std::ostream& operator<<(std::ostream &, const matrix<T> &);

	template<typename T>
	matrix<T>::matrix(size_t rows, size_t cols, T defVal)
		: m_cols(cols), m_rows(rows) {
		if (m_cols == 0 || m_rows == 0) {
			throw std::logic_error("Matrix does not allow zero dimensions.");
		}

		m_entries = new T[rows * cols];
		std::fill(m_entries, m_entries + (rows * cols), defVal);
	}

	template<typename T>
	matrix<T>::matrix(std::initializer_list<T> list, bool mode) noexcept {
		m_entries = new T[list.size()];
		m_cols = mode ? list.size() : 1;
		m_rows = mode ? 1 : list.size();
		std::move(list.begin(), list.end(), stdext::checked_array_iterator<T*>(m_entries, list.size()));
	}

	template<typename T>
	matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list) {
		for (const auto i : list) {
			if (i.size() != list.begin()->size()) {
				throw std::runtime_error("All rows have to be equal in length.");
			}
		}

		m_rows = list.size();
		m_cols = list.begin()->size();
		m_entries = new T[m_rows * m_cols];

		uint32_t index = 0;
		for (const auto &i : list) {
			std::move(i.begin(), i.end(), stdext::checked_array_iterator<T*>(m_entries + m_cols * index, m_cols));
			index++;
		}
	}

	template<typename T>
	matrix<T>::matrix(const matrix<T> &other) noexcept {
		m_rows = other.m_rows;
		m_cols = other.m_cols;
		m_entries = new T[other.entries()];
		std::copy(other.m_entries, other.m_entries + other.entries(), stdext::checked_array_iterator<T*>(m_entries, entries()));
	}

	template<typename T>
	matrix<T>::matrix(matrix<T> &&other) noexcept {
		m_rows = other.m_rows;
		m_cols = other.m_cols;
		m_entries = other.m_entries;
		other.m_entries = nullptr;
	}

	template<typename T>
	matrix<T>::~matrix() {
		delete[] m_entries;
	}

	template<typename T>
	size_t matrix<T>::rows() const {
		return m_rows;
	}

	template<typename T>
	size_t matrix<T>::columns() const {
		return m_cols;
	}

	template<typename T>
	size_t matrix<T>::entries() const {
		return m_rows * m_cols;
	}

	template<typename T>
	matrix<T> matrix<T>::transpose() const {
		matrix<T> m(m_cols, m_rows);
		
		for (size_t i = 0; i < m_cols; i++) {
			for (size_t j = 0; j < m_rows; j++) {
				m.m_entries[i * m_rows + j] = m_entries[j * m_cols + i];
			}
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::gauss() const {
		matrix<T> m(*this);

		for (size_t j = 0; j < m_cols; j++) {
			T t1 = m.m_entries[j * (m_cols + 1)];
			if (t1 == T(0)) {
				for (size_t i = j + 1; i < m_rows; i++) {
					if (m.m_entries[j * (m_cols + 1) + (i - j) * m_cols] != T(0)) {
						std::swap_ranges(m.m_entries + j * m_cols, m.m_entries + (j + 1) * m_cols,
							stdext::checked_array_iterator<T*>(m.m_entries + i * m_cols, m_cols));
						t1 = m.m_entries[j * (m_cols + 1)];
						break;
					}
				}
			}
			for (size_t i = j + 1; i < m_rows; i++) {
				T t2 = m.m_entries[i * m_cols + j];
				if (t2 == T(0)) { continue; }
				std::transform(m.m_entries + j * m_cols, m.m_entries + (j + 1) * m_cols,
					           stdext::checked_array_iterator<T*>(m.m_entries + i * m_cols, m_cols),
					           stdext::checked_array_iterator<T*>(m.m_entries + i * m_cols, m_cols),
					[t1, t2](T first, T second) { return second - first * (t2 / t1); });
			}
		}
		return m;
	}

	template<typename T>
	T matrix<T>::det() const {
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

		T d = 0;
		for (size_t j = 0; j < m_cols; j++) {
			matrix<T> sm(m_rows - 1, m_cols - 1);
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
	matrix<T> matrix<T>::Identitymatrix(size_t size) {
		matrix<T> m(size, size);
		for (int i = 0; i < size; i++) {
			m(i, i) = 1;
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::Elementarymatrix(size_t size, size_t row, size_t col, T val) {
		matrix<T> m(size, size);
		for (int i = 0; i < size; i++) {
			m(i, i) = 1;
		}
		m(row, col) = val;
		return m;
	}

	template<typename T>
	T matrix<T>::operator()(size_t i, size_t j) const {
		if (i >= m_rows || j >= m_cols) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return m_entries[i * m_cols + j];
	}

	template<typename T>
	T& matrix<T>::operator()(size_t i, size_t j) {
		if (i >= m_rows || j >= m_cols) {
			throw std::out_of_range("Exceeded matrix range.");
		}
		return m_entries[i * m_cols + j];
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(double other) const {
		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] * other;
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const matrix<T> &other) const {
		if (m_cols != other.m_rows) {
			throw std::runtime_error("Can not multiply by a matrix which rows does not \
				                      match the columns of the original matrix.");
		}

		const size_t minColsPerThread = 50;
		size_t supportedThreads = std::max<size_t>(std::thread::hardware_concurrency(), 2);

		size_t rows = m_rows; // std::min(m_rows, other.m_cols);
		size_t cols = other.m_cols; // std::max(m_rows, other.m_cols);

		matrix<T> m(rows, cols);

		size_t maxAmountOfThreads = (cols + minColsPerThread - 1) / minColsPerThread;
		size_t amountOfThreads = supportedThreads > maxAmountOfThreads ? maxAmountOfThreads : supportedThreads;

		size_t colsPerThread = cols / amountOfThreads;

		std::future<void>* futures = new std::future<void>[amountOfThreads - 1];

		for (int c = 0; c < amountOfThreads - 1; c++) {
			futures[c] = std::async([c, rows, cols, colsPerThread, this, other, &m]() {
				for (size_t i = 0; i < rows; i++) {
					for (size_t k = c * colsPerThread; k < (c + 1) * colsPerThread; k++) {
						T c_ik = 0;
						for (size_t j = 0; j < m_cols; j++) {
							c_ik += m_entries[i * m_cols + j] * other.m_entries[j * other.m_cols + k];
						}
						m.m_entries[i * cols + k] = c_ik;
					}
				}
			});
		}

		for (size_t i = 0; i < rows; i++) {
			for (size_t k = (amountOfThreads - 1) * colsPerThread; k < cols; k++) {
				T c_ik = 0;
				for (size_t j = 0; j < m_cols; j++) {
					c_ik += m_entries[i * m_cols + j] * other.m_entries[j * other.m_cols + k];
				}
				m.m_entries[i * cols + k] = c_ik;
			}
		}

		for (int c = 0; c < amountOfThreads - 1; c++) {
			futures[c].get();
		}

		delete[] futures;

		if (rows != m_rows) {
			m.m_rows = cols;
			m.m_cols = rows;
		}
		
		return m;
	}

	template<typename T>
	vector<T> matrix<T>::operator*(const vector<T> &other) const {
		if (m_cols != other.mDimension) {
			throw std::runtime_error("Can not multiply by a vector which dimension does \
									  not match the columns of the matrix.");
		}

		vector<T> v(m_rows);
		v.mmatrix = *this * other.mmatrix;
		return v;
	}

	template<typename T>
	matrix<T> matrix<T>::operator/(double other) const {
		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] / other;
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::operator+(const matrix<T> &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] + other.m_entries[i];
		}
		return m;
	}

	template<typename T>
	matrix<T> matrix<T>::operator-(const matrix<T> &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) {
			throw std::runtime_error("Dimensions of matrices can not differ from each other.");
		}

		matrix<T> m(m_rows, m_cols);
		for (size_t i = 0; i < entries(); i++) {
			m.m_entries[i] = m_entries[i] - other.m_entries[i];
		}
		return m;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator*=(double other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator*=(const matrix<T> &other) {
		*this = *this * other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator/=(double other) {
		*this = *this / other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator+=(const matrix<T> &other) {
		*this = *this + other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator-=(const matrix<T> &other) {
		*this = *this - other;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator=(const matrix<T> &other) {
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
	matrix<T>& matrix<T>::operator=(matrix<T> &&other) {
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
	bool matrix<T>::operator==(const matrix &other) const {
		if (m_rows != other.m_rows || m_cols != other.m_cols) { return false; }
		for (size_t i = 0; i < other.entries(); i++) {
			if (m_entries[i] != other.m_entries[i]) {
				return false;
			}
		}
		return true;
	}

	template<typename T>
	bool matrix<T>::operator!=(const matrix &other) const {
		if (*this == other) { return false; }
		return true;
	}


	template<typename T>
	matrix<T> operator*(double lhs, const matrix<T> &m) {
		return lhs * m;
	}

	// Test wise implementation
	template<typename T>
	std::ostream& operator<<(std::ostream &os, const matrix<T> &m) {
		if constexpr (std::is_arithmetic<T>::value) {
			T max = 0;
			T fMax = 0;
			bool firstCol = false;
			for (size_t i = 0; i < m.rows(); i++) {
				for (size_t j = 0; j < m.columns(); j++) {
					if (std::abs(m(i, j)) > max) {
						max = std::abs(m(i, j));
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
					int length = m(i, j) == 0 ? 1 : std::log(std::abs(m(i, j))) / std::log(10) + 1.000001;
					for (int k = length; k < (j == 0 ? fMaxLength : maxLength); k++) { os << " "; }
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