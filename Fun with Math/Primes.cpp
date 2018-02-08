#include "stdafx.h"
#include <vector>

bool isPrime(int n) {
	if (n < 2 || n == 4) {
		return false;
	}

	for (int i = 2; i < n / 2; i++) {
		if (n % i == 0) {
			return false;
		}
	}
}

void fillPrimes(std::vector<int>& primes, int n) {
	for (int i = 0; i <= n; i++) {
		if (isPrime(i)) {
			primes.push_back(i);
		}
	}
}

std::vector<int> sieveOfEratosthenes(int n) {
	std::vector<int> primes;
	std::vector<int> numbers;

	for (int i = 2; i <= n; i++) {
		numbers.push_back(i);
	}

	while (numbers.size() > 0) {
		int num = numbers.front();
		primes.push_back(num);
		for (int i = 0; i < numbers.size(); i++) {
			if (numbers[i] % num == 0) {
				numbers.erase(numbers.begin() + i);
			}
		}
	}

	return primes;
}