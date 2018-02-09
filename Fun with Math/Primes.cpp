#include "stdafx.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <set>


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

std::vector<int> sieveOfEratosthenes(int n) {
	std::vector<int> primes; 
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
	return primes;
}