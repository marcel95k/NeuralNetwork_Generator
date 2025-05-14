#pragma once
#include <string>
#include <vector>
#include "Neuron.h"

std::string error(const int _what);		// Returns an error message based one the caught error (_what)
void checkNetForError(const int _errorCode, std::vector<std::vector<Neuron>>* _network);
void checkNetForError(std::vector<std::vector<Neuron>>* _network);
void checkUserInputForError();
