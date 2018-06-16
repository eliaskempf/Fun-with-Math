#include "stdafx.h"
#include "Complex.hpp"

namespace la::fields {
	complex::complex(double re) noexcept
		: m_real(re)
	{}

	complex::complex(double re, double im) noexcept
		: m_real(re), m_img(im)
	{}

	double complex::real() const {
		return m_real;
	}

	double complex::img() const {
		return m_img;
	}

	complex complex::conjugate() const {
		return complex(m_real, -m_img);
	}

	complex complex::operator*(const double other) const {
		return complex(m_real * other, m_img * other);
	}

	complex complex::operator*(const complex &other) const {
		return complex((m_real * other.m_real - m_img * other.m_img), (m_real * other.m_img + m_img * other.m_real));
	}

	complex complex::operator/(const double other) const {
		return complex(m_real / other, m_img / other);
	}

	complex complex::operator/(const complex &other) const {
		complex conj = other.conjugate();
		return (*this * conj) / (other * conj).m_real;
	}

	complex complex::operator+(const double other) const {
		return complex(m_real + other, m_img);
	}

	complex complex::operator+(const complex &other) const {
		return complex(m_real + other.m_real, m_img + other.m_img);
	}

	complex complex::operator-(const double other) const {
		return complex(m_real - other, m_img);
	}

	complex complex::operator-(const complex &other) const {
		return complex(m_real - other.m_real, m_img - other.m_img);
	}

	complex& complex::operator*=(const double other) {
		*this = *this * other;
		return *this;
	}

	complex& complex::operator*=(const complex &other) {
		*this = *this * other;
		return *this;
	}

	complex& complex::operator/=(const double other) {
		*this = *this / other;
		return *this;
	}

	complex& complex::operator/=(const complex &other) {
		*this = *this / other;
		return *this;
	}

	complex& complex::operator+=(const double other) {
		*this = *this + other;
		return *this;
	}

	complex& complex::operator+=(const complex &other) {
		*this = *this + other;
		return *this;
	}

	complex& complex::operator-=(const double other) {
		*this = *this - other;
		return *this;
	}

	complex& complex::operator-=(const complex &other) {
		*this = *this - other;
		return *this;
	}

	complex& complex::operator=(const complex &other) {
		m_real = other.m_real;
		m_img = other.m_img;
		return *this;
	}

	complex& complex::operator=(complex &&other) {
		m_real = other.m_real;
		m_img = other.m_img;
		return *this;
	}

	bool complex::operator==(const double other) const {
		return m_real == other && m_img == 0;
	}

	bool complex::operator==(const complex &other) const {
		return m_real == other.m_real && m_img == other.m_img;
	}

	bool complex::operator!=(const double other) const {
		return m_real != other || m_img != 0;
	}

	bool complex::operator!=(const complex &other) const {
		return m_real != other.m_real || m_img != other.m_img;
	}

	complex operator*(const double x, const complex &z) {
		return complex(x * z.real(), x * z.img());
	}

	complex operator/(const double x, const complex &z) {
		return complex(x, 0) / z;
	}

	complex operator+(const double x, const complex &z) {
		return complex(x + z.real(), z.img());
	}

	complex operator-(const double x, const complex &z) {
		return complex(x - z.real(), -z.img());
	}

	std::ostream & operator<<(std::ostream &os, const complex &z) {
		os << z.real() << (z.img() < 0 ? " - " : " + ") << abs(z.img()) << "i";
		return os;
	}
}