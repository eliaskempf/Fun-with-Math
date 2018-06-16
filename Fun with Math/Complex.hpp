#pragma once

#include <iostream>

namespace la::fields {
	class complex {
	private:
		double m_real = 0;
		double m_img = 0;

	public:
		// Constructor
		complex() noexcept = default;
		complex(double) noexcept;
		complex(double, double) noexcept;
		complex(const complex &) noexcept = default;

		// Member functions
		double real() const;
		double img() const;
		complex conjugate() const;

		// Overloaded operators
		complex operator*(const double) const;
		complex operator*(const complex &) const;
		complex operator/(const double) const;
		complex operator/(const complex &) const;
		complex operator+(const double) const;
		complex operator+(const complex &) const;
		complex operator-(const double) const;
		complex operator-(const complex &) const;
		complex& operator*=(const double);
		complex& operator*=(const complex &);
		complex& operator/=(const double);
		complex& operator/=(const complex &);
		complex& operator+=(const double);
		complex& operator+=(const complex &);
		complex& operator-=(const double);
		complex& operator-=(const complex &);
		complex& operator=(const complex &);
		complex& operator=(complex &&);
		bool operator==(const double) const;
		bool operator==(const complex &) const;
		bool operator!=(const double) const;
		bool operator!=(const complex &) const;
	};

	complex operator*(const double, const complex &);
	complex operator/(const double, const complex &);
	complex operator+(const double, const complex &);
	complex operator-(const double, const complex &);
	std::ostream& operator<<(std::ostream &, const complex &);
}