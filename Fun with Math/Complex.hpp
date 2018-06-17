#pragma once

#include <iostream>

namespace la::fields {
	class Complex {
	private:
		double m_real = 0;
		double m_img = 0;

	public:
		// Constructor
		Complex() = default;
		Complex(double) noexcept;
		Complex(double, double) noexcept;
		Complex(const Complex &) = default;

		// Member functions
		double real() const;
		double img() const;
		Complex conjugate() const;

		// Overloaded operators
		Complex operator*(const double) const;
		Complex operator*(const Complex &) const;
		Complex operator/(const double) const;
		Complex operator/(const Complex &) const;
		Complex operator+(const double) const;
		Complex operator+(const Complex &) const;
		Complex operator-(const double) const;
		Complex operator-(const Complex &) const;
		Complex& operator*=(const double);
		Complex& operator*=(const Complex &);
		Complex& operator/=(const double);
		Complex& operator/=(const Complex &);
		Complex& operator+=(const double);
		Complex& operator+=(const Complex &);
		Complex& operator-=(const double);
		Complex& operator-=(const Complex &);
		Complex& operator=(const Complex &);
		Complex& operator=(Complex &&);
		bool operator==(const double) const;
		bool operator==(const Complex &) const;
		bool operator!=(const double) const;
		bool operator!=(const Complex &) const;
	};

	Complex operator*(const double, const Complex &);
	Complex operator/(const double, const Complex &);
	Complex operator+(const double, const Complex &);
	Complex operator-(const double, const Complex &);
	std::ostream& operator<<(std::ostream &, const Complex &);
}