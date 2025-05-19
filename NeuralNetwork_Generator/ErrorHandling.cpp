#include "ErrorHandling.h"

std::string ERRORHANDLING::error(const int _what) {
	switch (_what) {
	case 1: return "\n\033[31mUngueltige Einagbe!\n\033[0m";
	case 2: return "\n\033[31mNetz leer! Speichern fehlgeschlagen!\n\033[0m";
	case 3: return "\n\033[31mDatei nicht gefunden!\n\033[0m";
	case 4: return "\n\033[31mNetz leer! Training fehlgeschlagen!\n\033[0m";
	case 5: return "\n\033[31mNetz leer! Test fehlgeschlagen!\n\033[0m";
	case 6: return "\n\033[31mNetz leer! Zu loeschendes Netz erst laden!\n\033[0m";
	case 7: return "\n\033[31mKeine Trainingsdaten gefunden! Training fehlgeschlagen!\n\033[0m";
	case 8: return "\n\033[31mNetz leer! Trainingsdaten erstellen fehlgeschlagen!\n\033[0m";
	case 9: return "\n\033[31mNetz leer! Initialisierung fehlgeschlagen!\n\033[0m";
	case 10: return "\n\033[31mNetz leer! Validieren fehlgeschlagen!\n\033[0m";
	default:
		break;
	}
}

void ERRORHANDLING::checkNetForError(const int _errorCode, std::vector<std::vector<Neuron>>* _network) {

	if (_network->size() == 0) {											
		throw ERRORHANDLING::error(_errorCode);
	}
}

void ERRORHANDLING::checkNetForTrainingData(std::vector<std::vector<Neuron>>* _network) {

	if (_network->at(0).at(0).getIndividualClassifications() == 0) {		
		throw ERRORHANDLING::error(7);
	}
}

void ERRORHANDLING::checkUserInputForError() {

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw ERRORHANDLING::error(1);
	}
}

void ERRORHANDLING::globalNetworkAssertion(std::vector<std::vector<Neuron>>* _network) {

	NN_ASSERT(_network->at(0).size() == 400, "Input Size");
	NN_ASSERT(_network->at(1).size() >= MIN_OUTPUTNEURONS && _network->at(1).size() <= MAX_OUTPUTNEURONS, "Output Size");
	NN_ASSERT(_network->at(0).at(0).getIndividualClassifications() >= 10 && _network->at(0).at(0).getIndividualClassifications() <= 80, "Individual classifications");
}
