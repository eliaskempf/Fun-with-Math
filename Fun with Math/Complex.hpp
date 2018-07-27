#pragma once

#include <iostream>

namespace la {
	class Complex {
	private:
		double m_real = 0;
		double m_img = 0;

	public:
		// Constructors
		constexpr Complex() = default;
		constexpr explicit Complex(double) noexcept;
		constexpr Complex(double, double) noexcept;
		constexpr Complex(const Complex &) = default;

		// Member functions
		double real() const;
		double img() const;
		double abs() const;
		Complex conjugate() const;

		// Overloaded operators
		Complex operator*(double) const;
		Complex operator*(const Complex &) const;
		Complex operator/(double) const;
		Complex operator/(const Complex &) const;
		Complex operator+(double) const;
		Complex operator+(const Complex &) const;
		Complex operator-(double) const;
		Complex operator-(const Complex &) const;
		Complex& operator*=(double);
		Complex& operator*=(const Complex &);
		Complex& operator/=(double);
		Complex& operator/=(const Complex &);
		Complex& operator+=(double);
		Complex& operator+=(const Complex &);
		Complex& operator-=(double);
		Complex& operator-=(const Complex &);
		Complex& operator=(const Complex &);
		Complex& operator=(Complex &&);
		bool operator==(double) const;
		bool operator==(const Complex &) const;
		bool operator!=(double) const;
		bool operator!=(const Complex &) const;
		bool operator<(double) const;
		bool operator<(const Complex &) const;
		bool operator>(double) const;
		bool operator>(const Complex &) const;
		bool operator<=(double) const;
		bool operator<=(const Complex &) const;
		bool operator>=(double) const;
		bool operator>=(const Complex &) const;
	};

	Complex operator*(double, const Complex &);
	Complex operator/(double, const Complex &);
	Complex operator+(double, const Complex &);
	Complex operator-(double, const Complex &);
	std::ostream& operator<<(std::ostream &, const Complex &);

	constexpr Complex::Complex(double re) noexcept
		: m_real(re)
	{}

	constexpr Complex::Complex(double re, double im) noexcept
		: m_real(re), m_img(im)
	{}
}

la::Complex operator""_i(long double);
la::Complex operator""_i(unsigned long long int);