#include "stdafx.h"
#include "complex.hpp"

namespace la {
	double complex::real() const {
		return m_real;
	}

	double complex::img() const {
		return m_img;
	}

	double complex::abs() const {
		return std::sqrt(m_real * m_real + m_img * m_img);
	}

	complex complex::conjugate() const {
		return complex(m_real, -m_img);
	}

	complex complex::operator*(double other) const {
		return complex(m_real * other, m_img * other);
	}

	complex complex::operator*(const complex &other) const {
		return complex((m_real * other.m_real - m_img * other.m_img), (m_real * other.m_img + m_img * other.m_real));
	}

	complex complex::operator/(double other) const {
		return complex(m_real / other, m_img / other);
	}

	complex complex::operator/(const complex &other) const {
		complex conj = other.conjugate();
		return (*this * conj) / (other * conj).m_real;
	}

	complex complex::operator+(double other) const {
		return complex(m_real + other, m_img);
	}

	complex complex::operator+(const complex &other) const {
		return complex(m_real + other.m_real, m_img + other.m_img);
	}

	complex complex::operator-(double other) const {
		return complex(m_real - other, m_img);
	}

	complex complex::operator-(const complex &other) const {
		return complex(m_real - other.m_real, m_img - other.m_img);
	}

	complex complex::operator-() const {
		return complex(-m_real, -m_img);
	}

	complex& complex::operator*=(double other) {
		*this = *this * other;
		return *this;
	}

	complex& complex::operator*=(const complex &other) {
		*this = *this * other;
		return *this;
	}

	complex& complex::operator/=(double other) {
		*this = *this / other;
		return *this;
	}

	complex& complex::operator/=(const complex &other) {
		*this = *this / other;
		return *this;
	}

	complex& complex::operator+=(double other) {
		*this = *this + other;
		return *this;
	}

	complex& complex::operator+=(const complex &other) {
		*this = *this + other;
		return *this;
	}

	complex& complex::operator-=(double other) {
		*this = *this - other;
		return *this;
	}

	complex& complex::operator-=(const complex &other) {
		*this = *this - other;
		return *this;
	}

	bool complex::operator==(double other) const {
		return m_real == other && m_img == 0;
	}

	bool complex::operator==(const complex &other) const {
		return m_real == other.m_real && m_img == other.m_img;
	}

	bool complex::operator!=(double other) const {
		return m_real != other || m_img != 0;
	}

	bool complex::operator!=(const complex &other) const {
		return m_real != other.m_real || m_img != other.m_img;
	}

	bool complex::operator<(double other) const {
		return abs() < other;
	}

	bool complex::operator<(const complex &other) const {
		return abs() < other.abs();
	}

	bool complex::operator>(double other) const {
		return abs() > other;
	}

	bool complex::operator>(const complex &other) const {
		return abs() > other.abs();
	}

	bool complex::operator<=(double other) const {
		return abs() <= other;
	}

	bool complex::operator<=(const complex &other) const {
		return abs() <= other.abs();
	}

	bool complex::operator>=(double other) const {
		return abs() >= other;
	}

	bool complex::operator>=(const complex &other) const {
		return abs() >= other.abs();
	}

	complex operator*(double lhs, const complex &z) {
		return complex(lhs * z.real(), lhs * z.img());
	}

	complex operator/(double lhs, const complex &z) {
		return complex(lhs, 0) / z;
	}

	complex operator+(double lhs, const complex &z) {
		return complex(lhs + z.real(), z.img());
	}

	complex operator-(double lhs, const complex &z) {
		return complex(lhs - z.real(), -z.img());
	}

	std::ostream& operator<<(std::ostream &os, const complex &z) {
		if (z.abs() == 0) { os << 0; return os; }
		if (z.real() != 0) { os << "(" << z.real() << (z.img() < 0 ? " - " : " + "); }
		if (std::abs(z.img()) != 1) { os << std::abs(z.img()); }
		os << "i)";
		return os;
	}
}

la::complex operator""_i(long double x) {
	return la::complex(0, x);
}

la::complex operator""_i(uint64_t x) {
	return la::complex(0, x);
}