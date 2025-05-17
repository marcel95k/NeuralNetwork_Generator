#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Neuron.h"

namespace ERRORHANDLING {

	std::string error(const int _what);		// Returns an error message based one the caught error (_what)
	void checkNetForError(const int _errorCode, std::vector<std::vector<Neuron>>* _network);
	void checkNetForTrainingData(std::vector<std::vector<Neuron>>* _network);
	void checkUserInputForError();	// Checks if the user entered a char instead of an int
}

