#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>


bool isPrime(uint64_t n) {
	if (n < 2 || n == 4) {
		return false;
	}

	for (size_t i = 2; i < n / 2; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

void fillPrimes(std::vector<uint64_t>& primes, uint64_t n) {
	for (size_t i = 0; i <= n; i++) {
		if (isPrime(i)) {
			primes.push_back(i);
		}
	}
}

void sieveOfEratosthenes(std::vector<uint64_t>& primes, uint64_t n) {
	bool* arr = new bool [n];

	for (size_t i = 0; i < n; i++) {
		arr[i] = true;
	}

	uint64_t sqrtN = static_cast<uint64_t>(std::sqrt(n));

	for (size_t i = 2; i < sqrtN + 1; i++) {
		if (arr[i]) {
			for (size_t j = i * i; j < n; j += (i > 2 ? i * 2 : i)) {
				arr[j] = false;
			}
		}
	}

	
	for (size_t i = 0; i < n; i++) {
		if (arr[i]) {
			primes.push_back(i);
		}
	}
	
	primes.erase(primes.begin(), primes.begin() + 2);

	delete[] arr;
}

void primeFactorisation(uint64_t n) {
	uint64_t num = n;
	std::map<uint64_t, uint64_t> primeFactors;
	std::vector<uint64_t> primes;

	sieveOfEratosthenes(primes, num / 2);

	for (auto i = primes.end() - 1; i > primes.begin(); i--) {
		while (num % *i == 0) {
			num /= *i;
			auto factor = primeFactors.find(*i);
			if (factor != primeFactors.end()) {
				factor->second++;
			}
			else {
				primeFactors.insert(std::make_pair(*i, 1));
			}
		}
	}

	while (num != 1) {
		num /= 2;
		auto factor = primeFactors.find(2);
		if (factor != primeFactors.end()) {
			factor->second++;
		}
		else {
			primeFactors.insert(std::make_pair(2, 1));
		}
	}

	std::cout << "Prime factorisation of " << n << " := " << std::flush;

	for (auto i = primeFactors.begin(); i != primeFactors.end(); i++) {
		if (i != primeFactors.begin()) {
			std::cout << " * " << std::flush;
		}
		std::cout << i->first << "^" << i->second << std::flush;
	}

	if (primeFactors.begin() == primeFactors.end()) {
		std::cout << n << "^1" << std::endl;
	}
	
	std::cout << std::endl;
}