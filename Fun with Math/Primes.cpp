#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>


bool isPrime(int n) {
	if (n < 2 || n == 4) {
		return false;
	}

	for (int i = 2; i < n / 2; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

void fillPrimes(std::vector<int>& primes, int n) {
	for (int i = 0; i <= n; i++) {
		if (isPrime(i)) {
			primes.push_back(i);
		}
	}
}

void sieveOfEratosthenes(std::vector<int>& primes, int n) {
	bool* arr = new bool [n];

	for (int i = 0; i < n; i++) {
		arr[i] = true;
	}

	int sqrtN = sqrt(n);

	for (int i = 2; i <= sqrtN; i++) {
		if (arr[i]) {
			for (int j = i * i; j < n; j += (i > 2 ? i * 2 : i)) {
				arr[j] = false;
			}
		}
	}

	
	for (int i = 0; i < n; i++) {
		if (arr[i]) {
			primes.push_back(i);
		}
	}
	
	primes.erase(primes.begin());
	primes.erase(primes.begin());

	delete[] arr;
}

void primeFactorisation(int n) {
	int num = n;
	std::map<int, int> primeFactors;
	std::map<int, int>::iterator factor;
	std::vector<int> primes;

	sieveOfEratosthenes(primes, num / 2);

	for (auto i = primes.end(); i >= primes.begin(); i--) {
		while (num % *i == 0) {
			num /= *i;
			factor = primeFactors.find(*i);
			if (factor != primeFactors.end()) {
				factor->second++;
			}
			else {
				primeFactors.insert(std::pair<int, int>(*i, 1));
			}
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