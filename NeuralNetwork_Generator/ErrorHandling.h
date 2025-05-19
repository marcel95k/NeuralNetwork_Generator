#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Neuron.h"
#include "Defines.h"

/*---------------------------------------*/
/* Helping-procedures for error handling */
/*---------------------------------------*/

/*Still left to implement:

*/

namespace ERRORHANDLING {

	std::string error(const int _what);															// Returns an error message based one the caught error (_what)
	void checkNetForError(const int _errorCode, std::vector<std::vector<Neuron>>* _network);	// Check if a network exists and if NOT, throw an exception based on the given errorCode
	void checkNetForTrainingData(std::vector<std::vector<Neuron>>* _network);					// Check if training data exist and throw an exception if not
	void checkUserInputForError();																// Checks if the user entered a char instead of an int
	void globalNetworkAssertion(std::vector<std::vector<Neuron>>* _network);
}

