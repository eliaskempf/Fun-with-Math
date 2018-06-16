#pragma once

#include <iostream>

namespace la::fields {
	template<unsigned int m>
	class module_ring {
	private:
		unsigned int m_value = 0;

	public:
		// Constructors
		module_ring() = default;
		module_ring(unsigned int) noexcept;

		// Member functions
		unsigned int value() const;

		// Overloaded operators
		module_ring operator*(const unsigned int) const;
		module_ring operator*(const module_ring &) const;
		module_ring operator+(const unsigned int) const;
		module_ring operator+(const module_ring &) const;
		module_ring operator-(const unsigned int) const;
		module_ring operator-(const module_ring &) const;
		module_ring& operator*=(const unsigned int);
		module_ring& operator*=(const module_ring &);
		module_ring& operator+=(const unsigned int);
		module_ring& operator+=(const module_ring &);
		module_ring& operator-=(const unsigned int);
		module_ring& operator-=(const module_ring &);
		module_ring& operator=(const unsigned int);
		module_ring& operator=(const module_ring &);
		bool operator==(const unsigned int) const;
		bool operator==(const module_ring &) const;
		bool operator!=(const unsigned int) const;
		bool operator!=(const module_ring &) const;
	};

	template<unsigned int m>
	module_ring<m>::module_ring(unsigned int value) noexcept
		: m_value(value % m)
	{}

	template<unsigned int m>
	unsigned int module_ring<m>::value() const {
		return m_value;
	}

	template<unsigned int m>
	module_ring<m> module_ring<m>::operator*(const unsigned int other) const {
		return module_ring<m>(m_value * other);
	}

	template<unsigned int m>
	module_ring<m> module_ring<m>::operator*(const module_ring<m> &other) const {
		return module_ring<m>(m_value * other.m_value);
	}

	template<unsigned int m>
	module_ring<m> module_ring<m>::operator+(const unsigned int other) const {
		return module_ring<m>(m_value + other);
	}

	template<unsigned int m>
	module_ring<m> module_ring<m>::operator+(const module_ring<m> &other) const {
		return module_ring<m>(m_value + other.m_value);
	}

	template<unsigned int m>
	module_ring<m> module_ring<m>::operator-(const unsigned int other) const {
		return module_ring<m>(m_value - other);
	}

	template<unsigned int m>
	module_ring<m> module_ring<m>::operator-(const module_ring<m> &other) const {
		return module_ring<m>(m_value - other.m_value);
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator*=(const unsigned int other) {
		*this = *this * other;
		return *this;
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator*=(const module_ring<m> &other) {
		*this = *this * other;
		return *this;
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator+=(const unsigned int other) {
		*this = *this + other;
		return *this;
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator+=(const module_ring<m> &other) {
		*this = *this + other;
		return *this;
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator-=(const unsigned int other) {
		*this = *this - other;
		return *this;
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator-=(const module_ring<m> &other) {
		*this = *this - other;
		return *this;
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator=(const unsigned int other) {
		m_value = other % m;
		return *this;
	}

	template<unsigned int m>
	module_ring<m>& module_ring<m>::operator=(const module_ring<m> &other) {
		m_value = other.m_value;
		return *this;
	}

	template<unsigned int m>
	bool module_ring<m>::operator==(const unsigned int other) const {
		return m_value == (other % m);
	}

	template<unsigned int m>
	bool module_ring<m>::operator==(const module_ring<m> &other) const {
		return m_value == other.m_value;
	}

	template<unsigned int m>
	bool module_ring<m>::operator!=(const unsigned int other) const {
		return m_value != (other % m);
	}

	template<unsigned int m>
	bool module_ring<m>::operator!=(const module_ring<m> &other) const {
		return m_value != other.m_value;
	}

	template<unsigned m>
	std::ostream& operator<<(std::ostream &os, const module_ring<m> &f) {
		os << f.value();
		return os;
	}
}