#include "stdafx.h"
#include "Change.hpp"
#include <utility>
#include <iostream>

std::pair<uint8_t, double> change(uint16_t money) {
	uint16_t bills[15] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };
	uint8_t maxAmountOfChange = 0;
	uint16_t maxPrice = 0;

	for (uint16_t i = 1; i < money; i++) {
		uint16_t price = i;
		uint16_t amountOfChange = 0;

		uint8_t index = 15;
		while (price != 0) {
			if (price < bills[index]) {
				index--;
			}
			else {
				amountOfChange++;
				price -= bills[index];
			}
		}
		if (amountOfChange > maxAmountOfChange) {
			maxAmountOfChange = amountOfChange;
			maxPrice = money - i;
		}
	}

	return { maxAmountOfChange, maxPrice / 100.0 };
}

void changes() {
	uint16_t bills[9] = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

	for (uint8_t i = 0; i < 9; i++) {
		auto pair = change(bills[i]);
		std::cout << "Given: " << bills[i] / 100 << ", worst-case price: " << pair.second << ", amount of change needed: " << static_cast<int>(pair.first) << std::endl;
	}
}