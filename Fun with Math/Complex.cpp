#include "stdafx.h"
#include "Complex.hpp"

namespace la {
	double Complex::real() const {
		return m_real;
	}

	double Complex::img() const {
		return m_img;
	}

	double Complex::abs() const {
		return std::sqrt(m_real * m_real + m_img * m_img);
	}

	Complex Complex::conjugate() const {
		return Complex(m_real, -m_img);
	}

	Complex Complex::operator*(double other) const {
		return Complex(m_real * other, m_img * other);
	}

	Complex Complex::operator*(const Complex &other) const {
		return Complex((m_real * other.m_real - m_img * other.m_img), (m_real * other.m_img + m_img * other.m_real));
	}

	Complex Complex::operator/(double other) const {
		return Complex(m_real / other, m_img / other);
	}

	Complex Complex::operator/(const Complex &other) const {
		Complex conj = other.conjugate();
		return (*this * conj) / (other * conj).m_real;
	}

	Complex Complex::operator+(double other) const {
		return Complex(m_real + other, m_img);
	}

	Complex Complex::operator+(const Complex &other) const {
		return Complex(m_real + other.m_real, m_img + other.m_img);
	}

	Complex Complex::operator-(double other) const {
		return Complex(m_real - other, m_img);
	}

	Complex Complex::operator-(const Complex &other) const {
		return Complex(m_real - other.m_real, m_img - other.m_img);
	}

	Complex& Complex::operator*=(double other) {
		*this = *this * other;
		return *this;
	}

	Complex& Complex::operator*=(const Complex &other) {
		*this = *this * other;
		return *this;
	}

	Complex& Complex::operator/=(double other) {
		*this = *this / other;
		return *this;
	}

	Complex& Complex::operator/=(const Complex &other) {
		*this = *this / other;
		return *this;
	}

	Complex& Complex::operator+=(double other) {
		*this = *this + other;
		return *this;
	}

	Complex& Complex::operator+=(const Complex &other) {
		*this = *this + other;
		return *this;
	}

	Complex& Complex::operator-=(double other) {
		*this = *this - other;
		return *this;
	}

	Complex& Complex::operator-=(const Complex &other) {
		*this = *this - other;
		return *this;
	}

	Complex& Complex::operator=(const Complex &other) {
		m_real = other.m_real;
		m_img = other.m_img;
		return *this;
	}

	Complex& Complex::operator=(Complex &&other) {
		m_real = other.m_real;
		m_img = other.m_img;
		return *this;
	}

	bool Complex::operator==(double other) const {
		return m_real == other && m_img == 0;
	}

	bool Complex::operator==(const Complex &other) const {
		return m_real == other.m_real && m_img == other.m_img;
	}

	bool Complex::operator!=(double other) const {
		return m_real != other || m_img != 0;
	}

	bool Complex::operator!=(const Complex &other) const {
		return m_real != other.m_real || m_img != other.m_img;
	}

	bool Complex::operator<(double other) const {
		return abs() < other;
	}

	bool Complex::operator<(const Complex &other) const {
		return abs() < other.abs();
	}

	bool Complex::operator>(double other) const {
		return abs() > other;
	}

	bool Complex::operator>(const Complex &other) const {
		return abs() > other.abs();
	}

	bool Complex::operator<=(double other) const {
		return abs() <= other;
	}

	bool Complex::operator<=(const Complex &other) const {
		return abs() <= other.abs();
	}

	bool Complex::operator>=(double other) const {
		return abs() >= other;
	}

	bool Complex::operator>=(const Complex &other) const {
		return abs() >= other.abs();
	}

	Complex operator*(double lhs, const Complex &z) {
		return Complex(lhs * z.real(), lhs * z.img());
	}

	Complex operator/(double lhs, const Complex &z) {
		return Complex(lhs, 0) / z;
	}

	Complex operator+(double lhs, const Complex &z) {
		return Complex(lhs + z.real(), z.img());
	}

	Complex operator-(double lhs, const Complex &z) {
		return Complex(lhs - z.real(), -z.img());
	}

	std::ostream& operator<<(std::ostream &os, const Complex &z) {
		if (z.abs() == 0) { os << 0; return os; }
		if (z.real() != 0) { os << "(" << z.real() << (z.img() < 0 ? " - " : " + "); }
		if (std::abs(z.img()) != 1) { os << std::abs(z.img()); }
		os << "i)";
		return os;
	}
}

la::Complex operator""_i(long double x) {
	return la::Complex(0, x);
}

la::Complex operator""_i(unsigned long long int x) {
	return la::Complex(0, x);
}