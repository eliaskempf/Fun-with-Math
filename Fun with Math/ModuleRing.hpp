#pragma once

#include <iostream>

namespace la::fields {
	template<uint32_t m>
	class ModuleRing {
	private:
		uint32_t m_value = 0;

	public:
		// Constructors
		ModuleRing() = default;
		ModuleRing(uint32_t) noexcept;

		// Member functions
		uint32_t value() const;

		// Overloaded operators
		ModuleRing operator*(uint32_t) const;
		ModuleRing operator*(const ModuleRing &) const;
		ModuleRing operator+(uint32_t) const;
		ModuleRing operator+(const ModuleRing &) const;
		ModuleRing operator-(uint32_t) const;
		ModuleRing operator-(const ModuleRing &) const;
		ModuleRing& operator*=(uint32_t);
		ModuleRing& operator*=(const ModuleRing &);
		ModuleRing& operator+=(uint32_t);
		ModuleRing& operator+=(const ModuleRing &);
		ModuleRing& operator-=(uint32_t);
		ModuleRing& operator-=(const ModuleRing &);
		ModuleRing& operator=(uint32_t);
		ModuleRing& operator=(const ModuleRing &);
		bool operator==(uint32_t) const;
		bool operator==(const ModuleRing &) const;
		bool operator!=(uint32_t) const;
		bool operator!=(const ModuleRing &) const;
		bool operator<(uint32_t) const;
		bool operator<(const ModuleRing &) const;
		bool operator>(uint32_t) const;
		bool operator>(const ModuleRing &) const;
		bool operator<=(uint32_t) const;
		bool operator<=(const ModuleRing &) const;
		bool operator>=(uint32_t) const;
		bool operator>=(const ModuleRing &) const;
	};

	template<uint32_t m>
	std::ostream& operator<<(std::ostream &, const ModuleRing<m> &);

	template<uint32_t m>
	ModuleRing<m>::ModuleRing(uint32_t value) noexcept
		: m_value(value % m)
	{}

	template<uint32_t m>
	uint32_t ModuleRing<m>::value() const {
		return m_value;
	}

	template<uint32_t m>
	ModuleRing<m> ModuleRing<m>::operator*(uint32_t other) const {
		return ModuleRing<m>(m_value * other);
	}

	template<uint32_t m>
	ModuleRing<m> ModuleRing<m>::operator*(const ModuleRing<m> &other) const {
		return ModuleRing<m>(m_value * other.m_value);
	}

	template<uint32_t m>
	ModuleRing<m> ModuleRing<m>::operator+(uint32_t other) const {
		return ModuleRing<m>(m_value + other);
	}

	template<uint32_t m>
	ModuleRing<m> ModuleRing<m>::operator+(const ModuleRing<m> &other) const {
		return ModuleRing<m>(m_value + other.m_value);
	}

	template<uint32_t m>
	ModuleRing<m> ModuleRing<m>::operator-(uint32_t other) const {
		return ModuleRing<m>(m_value - other);
	}

	template<uint32_t m>
	ModuleRing<m> ModuleRing<m>::operator-(const ModuleRing<m> &other) const {
		return ModuleRing<m>(m_value - other.m_value);
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator*=(uint32_t other) {
		*this = *this * other;
		return *this;
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator*=(const ModuleRing<m> &other) {
		*this = *this * other;
		return *this;
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator+=(uint32_t other) {
		*this = *this + other;
		return *this;
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator+=(const ModuleRing<m> &other) {
		*this = *this + other;
		return *this;
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator-=(uint32_t other) {
		*this = *this - other;
		return *this;
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator-=(const ModuleRing<m> &other) {
		*this = *this - other;
		return *this;
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator=(uint32_t other) {
		m_value = other % m;
		return *this;
	}

	template<uint32_t m>
	ModuleRing<m>& ModuleRing<m>::operator=(const ModuleRing<m> &other) {
		m_value = other.m_value;
		return *this;
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator==(uint32_t other) const {
		return m_value == (other % m);
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator==(const ModuleRing<m> &other) const {
		return m_value == other.m_value;
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator!=(uint32_t other) const {
		return m_value != (other % m);
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator!=(const ModuleRing<m> &other) const {
		return m_value != other.m_value;
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator<(uint32_t other) const {
		return m_value < (other % m);
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator<(const ModuleRing<m> &other) const {
		return m_value < other.m_value;
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator>(uint32_t other) const {
		return m_value > (other % m);
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator>(const ModuleRing<m> &other) const {
		return m_value > other.m_value;
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator<=(uint32_t other) const {
		return m_value <= (other % m);
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator<=(const ModuleRing<m> &other) const {
		return m_value <= other.m_value;
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator>=(uint32_t other) const {
		return m_value >= (other % m);
	}

	template<uint32_t m>
	bool ModuleRing<m>::operator>=(const ModuleRing<m> &other) const {
		return m_value >= other.m_value;
	}

	template<unsigned m>
	std::ostream& operator<<(std::ostream &os, const ModuleRing<m> &f) {
		os << f.value();
		return os;
	}
}