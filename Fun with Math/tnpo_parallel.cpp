#include "stdafx.h"
#include "tnpo_parallel.hpp"
#include "threeNplusOne.hpp"
#include <iostream>
#include <thread>

void tnpo_parallel(unsigned long long n_max) {
	int supportedThreads = std::thread::hardware_concurrency();
	int numberThreads = supportedThreads == 0 ? 2 : supportedThreads;

	std::thread threads[3];

	for (int i = 0; i < numberThreads - 1; i++) {
		threads[i] = std::thread(tnpo_threads, i + 1, n_max);
	}

	tnpo_threads(4, n_max);

	for (int i = 0; i < numberThreads - 1; i++) {
		threads[i].join();
	}

	std::cout << "Holy shit!" << std::endl;
}

void tnpo_threads(int j, unsigned long long n) {
	for (int i = j; i < n; i += 4) {
		tnpo(i);
		std::cout << "Terminates for i = " << i << "\n";
	}
}
