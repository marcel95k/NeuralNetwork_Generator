#include <iostream>

#include "ExceptionHandling.h"
#include "Controller.h"


int main() {

    Controller controller;

	try {
		controller.run();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return 1;
	}
   
    return 0;
}