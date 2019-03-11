#pragma once

#include <iostream>

namespace la {
	template<uint32_t m>
	class module_ring {
	private:
		uint32_t m_value = 0;

	public:
		// Constructors
		constexpr module_ring() = default;
		constexpr module_ring(uint32_t) noexcept;

		// Member functions
		uint32_t value() const;

		// Overloaded operators
		module_ring operator*(uint32_t) const;
		module_ring operator*(const module_ring &) const;
		module_ring operator+(uint32_t) const;
		module_ring operator+(const module_ring &) const;
		module_ring operator-(uint32_t) const;
		module_ring operator-(const module_ring &) const;
		module_ring& operator*=(uint32_t);
		module_ring& operator*=(const module_ring &);
		module_ring& operator+=(uint32_t);
		module_ring& operator+=(const module_ring &);
		module_ring& operator-=(uint32_t);
		module_ring& operator-=(const module_ring &);
		module_ring& operator=(uint32_t);
		module_ring& operator=(const module_ring &);
		bool operator==(uint32_t) const;
		bool operator==(const module_ring &) const;
		bool operator!=(uint32_t) const;
		bool operator!=(const module_ring &) const;
		bool operator<(uint32_t) const;
		bool operator<(const module_ring &) const;
		bool operator>(uint32_t) const;
		bool operator>(const module_ring &) const;
		bool operator<=(uint32_t) const;
		bool operator<=(const module_ring &) const;
		bool operator>=(uint32_t) const;
		bool operator>=(const module_ring &) const;
	};

	template<uint32_t m>
	module_ring<m> operator*(uint32_t, const module_ring<m> &);

	template<uint32_t m>
	module_ring<m> operator+(uint32_t, const module_ring<m> &);

	template<uint32_t m>
	module_ring<m> operator-(uint32_t, const module_ring<m> &);

	template<uint32_t m>
	std::ostream& operator<<(std::ostream &, const module_ring<m> &);

	template<uint32_t m>
	constexpr module_ring<m>::module_ring(uint32_t value) noexcept
		: m_value(value % m)
	{}

	template<uint32_t m>
	uint32_t module_ring<m>::value() const {
		return m_value;
	}

	template<uint32_t m>
	module_ring<m> module_ring<m>::operator*(uint32_t other) const {
		return module_ring<m>(m_value * other);
	}

	template<uint32_t m>
	module_ring<m> module_ring<m>::operator*(const module_ring<m> &other) const {
		return module_ring<m>(m_value * other.m_value);
	}

	template<uint32_t m>
	module_ring<m> module_ring<m>::operator+(uint32_t other) const {
		return module_ring<m>(m_value + other);
	}

	template<uint32_t m>
	module_ring<m> module_ring<m>::operator+(const module_ring<m> &other) const {
		return module_ring<m>(m_value + other.m_value);
	}

	template<uint32_t m>
	module_ring<m> module_ring<m>::operator-(uint32_t other) const {
		return module_ring<m>(m_value - other);
	}

	template<uint32_t m>
	module_ring<m> module_ring<m>::operator-(const module_ring<m> &other) const {
		return module_ring<m>(m_value - other.m_value);
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator*=(uint32_t other) {
		*this = *this * other;
		return *this;
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator*=(const module_ring<m> &other) {
		*this = *this * other;
		return *this;
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator+=(uint32_t other) {
		*this = *this + other;
		return *this;
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator+=(const module_ring<m> &other) {
		*this = *this + other;
		return *this;
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator-=(uint32_t other) {
		*this = *this - other;
		return *this;
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator-=(const module_ring<m> &other) {
		*this = *this - other;
		return *this;
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator=(uint32_t other) {
		m_value = other % m;
		return *this;
	}

	template<uint32_t m>
	module_ring<m>& module_ring<m>::operator=(const module_ring<m> &other) {
		m_value = other.m_value;
		return *this;
	}

	template<uint32_t m>
	bool module_ring<m>::operator==(uint32_t other) const {
		return m_value == (other % m);
	}

	template<uint32_t m>
	bool module_ring<m>::operator==(const module_ring<m> &other) const {
		return m_value == other.m_value;
	}

	template<uint32_t m>
	bool module_ring<m>::operator!=(uint32_t other) const {
		return m_value != (other % m);
	}

	template<uint32_t m>
	bool module_ring<m>::operator!=(const module_ring<m> &other) const {
		return m_value != other.m_value;
	}

	template<uint32_t m>
	bool module_ring<m>::operator<(uint32_t other) const {
		return m_value < (other % m);
	}

	template<uint32_t m>
	bool module_ring<m>::operator<(const module_ring<m> &other) const {
		return m_value < other.m_value;
	}

	template<uint32_t m>
	bool module_ring<m>::operator>(uint32_t other) const {
		return m_value > (other % m);
	}

	template<uint32_t m>
	bool module_ring<m>::operator>(const module_ring<m> &other) const {
		return m_value > other.m_value;
	}

	template<uint32_t m>
	bool module_ring<m>::operator<=(uint32_t other) const {
		return m_value <= (other % m);
	}

	template<uint32_t m>
	bool module_ring<m>::operator<=(const module_ring<m> &other) const {
		return m_value <= other.m_value;
	}

	template<uint32_t m>
	bool module_ring<m>::operator>=(uint32_t other) const {
		return m_value >= (other % m);
	}

	template<uint32_t m>
	bool module_ring<m>::operator>=(const module_ring<m> &other) const {
		return m_value >= other.m_value;
	}

	template<uint32_t m>
	module_ring<m> operator*(uint32_t lhs, const module_ring<m> &rhs) {
		return rhs * lhs;
	}

	template<uint32_t m>
	module_ring<m> operator+(uint32_t lhs, const module_ring<m> &rhs) {
		return rhs + lhs;
	}

	template<uint32_t m>
	module_ring<m> operator-(uint32_t lhs, const module_ring<m> &rhs) {
		return rhs - lhs;
	}

	template<unsigned m>
	std::ostream& operator<<(std::ostream &os, const module_ring<m> &f) {
		os << f.value();
		return os;
	}
}