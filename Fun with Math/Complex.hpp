#pragma once

#include <iostream>

namespace la {
	class complex {
	private:
		double m_real = 0;
		double m_img = 0;

	public:
		// Constructors
		constexpr complex() = default;
		constexpr explicit complex(double) noexcept;
		constexpr complex(double, double) noexcept;
		constexpr complex(const complex &) = default;
		constexpr complex(complex &&) = default;

		// Member functions
		double real() const;
		double img() const;
		double abs() const;
		complex conjugate() const;

		// Overloaded operators

		// Arithmetic operators
		complex operator*(double) const;
		complex operator*(const complex &) const;
		complex operator/(double) const;
		complex operator/(const complex &) const;
		complex operator+(double) const;
		complex operator+(const complex &) const;
		complex operator-(double) const;
		complex operator-(const complex &) const;
		complex operator-() const;

		// Assignment operators
		complex& operator*=(double);
		complex& operator*=(const complex &);
		complex& operator/=(double);
		complex& operator/=(const complex &);
		complex& operator+=(double);
		complex& operator+=(const complex &);
		complex& operator-=(double);
		complex& operator-=(const complex &);

		// Copy assignment
		complex& operator=(const complex &) = default;
		// Move assignment
		complex& operator=(complex &&) = default;

		// Comparison operators
		bool operator==(double) const;
		bool operator==(const complex &) const;
		bool operator!=(double) const;
		bool operator!=(const complex &) const;
		bool operator<(double) const;
		bool operator<(const complex &) const;
		bool operator>(double) const;
		bool operator>(const complex &) const;
		bool operator<=(double) const;
		bool operator<=(const complex &) const;
		bool operator>=(double) const;
		bool operator>=(const complex &) const;
	};

	// Allow for operations for both side
	complex operator*(double, const complex &);
	complex operator/(double, const complex &);
	complex operator+(double, const complex &);
	complex operator-(double, const complex &);
	std::ostream& operator<<(std::ostream &, const complex &);

	// constexpr constructor for complex numbers with real part only
	constexpr complex::complex(double re) noexcept
		: m_real(re)
	{}

	// constexpr constructor for complex numbers with real and imaginary part
	constexpr complex::complex(double re, double im) noexcept
		: m_real(re), m_img(im)
	{}
}

// User-defined literals for better readability
la::complex operator""_i(long double);

// User-defined literals for better readability
la::complex operator""_i(uint64_t);