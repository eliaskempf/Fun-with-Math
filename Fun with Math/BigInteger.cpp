#include "stdafx.h"
#include "BigInteger.hpp"
#include <iostream>


BigInteger::BigInteger(int64_t value) noexcept
{
	int minBits = std::ceil(std::log2(std::abs(value) + (value < 0 ? 0 : 1))) + 1;
	int neededBytes = std::ceil(minBits / 8.0);
	std::cout << "Value: " << value << " minBits: " << minBits << " neededBytes: " << neededBytes << std::endl;
}

BigInteger::BigInteger(std::string_view parseString)
{

}


BigInteger::~BigInteger()
{
	delete[] m_bits;
}
