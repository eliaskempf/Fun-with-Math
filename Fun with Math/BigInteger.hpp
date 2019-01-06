#pragma once
#include <cstdint>
#include <string_view>

using byte = unsigned char;


class BigInteger
{
private:
	byte* m_bits = new byte[1];

public:
	// Constructors
	BigInteger() noexcept = default;
	BigInteger(int64_t) noexcept;
	BigInteger(std::string_view);


	virtual ~BigInteger();
};

