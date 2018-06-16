#pragma once

#include <iostream>

namespace la::fields {
	template<unsigned int m>
	class ModuleRing {
	private:
		unsigned int m_value = 0;

	public:
		// Constructors
		ModuleRing() = default;
		ModuleRing(unsigned int) noexcept;

		// Member functions
		unsigned int value() const;

		// Overloaded operators
		ModuleRing operator*(const unsigned int) const;
		ModuleRing operator*(const ModuleRing &) const;
		ModuleRing operator+(const unsigned int) const;
		ModuleRing operator+(const ModuleRing &) const;
		ModuleRing operator-(const unsigned int) const;
		ModuleRing operator-(const ModuleRing &) const;
		ModuleRing& operator*=(const unsigned int);
		ModuleRing& operator*=(const ModuleRing &);
		ModuleRing& operator+=(const unsigned int);
		ModuleRing& operator+=(const ModuleRing &);
		ModuleRing& operator-=(const unsigned int);
		ModuleRing& operator-=(const ModuleRing &);
		ModuleRing& operator=(const unsigned int);
		ModuleRing& operator=(const ModuleRing &);
		bool operator==(const unsigned int) const;
		bool operator==(const ModuleRing &) const;
		bool operator!=(const unsigned int) const;
		bool operator!=(const ModuleRing &) const;
	};

	template<unsigned int m>
	ModuleRing<m>::ModuleRing(unsigned int value) noexcept
		: m_value(value % m)
	{}

	template<unsigned int m>
	unsigned int ModuleRing<m>::value() const {
		return m_value;
	}

	template<unsigned int m>
	ModuleRing<m> ModuleRing<m>::operator*(const unsigned int other) const {
		return ModuleRing<m>(m_value * other);
	}

	template<unsigned int m>
	ModuleRing<m> ModuleRing<m>::operator*(const ModuleRing<m> &other) const {
		return ModuleRing<m>(m_value * other.m_value);
	}

	template<unsigned int m>
	ModuleRing<m> ModuleRing<m>::operator+(const unsigned int other) const {
		return ModuleRing<m>(m_value + other);
	}

	template<unsigned int m>
	ModuleRing<m> ModuleRing<m>::operator+(const ModuleRing<m> &other) const {
		return ModuleRing<m>(m_value + other.m_value);
	}

	template<unsigned int m>
	ModuleRing<m> ModuleRing<m>::operator-(const unsigned int other) const {
		return ModuleRing<m>(m_value - other);
	}

	template<unsigned int m>
	ModuleRing<m> ModuleRing<m>::operator-(const ModuleRing<m> &other) const {
		return ModuleRing<m>(m_value - other.m_value);
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator*=(const unsigned int other) {
		*this = *this * other;
		return *this;
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator*=(const ModuleRing<m> &other) {
		*this = *this * other;
		return *this;
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator+=(const unsigned int other) {
		*this = *this + other;
		return *this;
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator+=(const ModuleRing<m> &other) {
		*this = *this + other;
		return *this;
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator-=(const unsigned int other) {
		*this = *this - other;
		return *this;
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator-=(const ModuleRing<m> &other) {
		*this = *this - other;
		return *this;
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator=(const unsigned int other) {
		m_value = other % m;
		return *this;
	}

	template<unsigned int m>
	ModuleRing<m>& ModuleRing<m>::operator=(const ModuleRing<m> &other) {
		m_value = other.m_value;
		return *this;
	}

	template<unsigned int m>
	bool ModuleRing<m>::operator==(const unsigned int other) const {
		return m_value == (other % m);
	}

	template<unsigned int m>
	bool ModuleRing<m>::operator==(const ModuleRing<m> &other) const {
		return m_value == other.m_value;
	}

	template<unsigned int m>
	bool ModuleRing<m>::operator!=(const unsigned int other) const {
		return m_value != (other % m);
	}

	template<unsigned int m>
	bool ModuleRing<m>::operator!=(const ModuleRing<m> &other) const {
		return m_value != other.m_value;
	}

	template<unsigned m>
	std::ostream& operator<<(std::ostream &os, const ModuleRing<m> &f) {
		os << f.value();
		return os;
	}
}