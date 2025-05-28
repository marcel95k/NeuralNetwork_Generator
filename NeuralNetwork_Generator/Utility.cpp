#include "Utility.h"

void awaitAnyKey() {

	std::cout << std::endl << "Zum Fortfahren beliebige Taste druecken..." << std::endl;
	_getch();
}

void clearScreen() {

	printf("\033[2J\033[H");
}