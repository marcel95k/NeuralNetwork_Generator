#include "ErrorHandling.h"

std::string error(const int _what) {
	switch (_what) {
	case 1: return "\nUngueltige Einagbe!\n";
	case 2: return "\nNetz leer! Speichern fehlgeschlagen!\n";
	case 3: return "\nDatei nicht gefunden!\n";
	case 4: return "\nNetz leer! Training fehlgeschlagen!\n";
	case 5: return "\nNetz leer! Test fehlgeschlagen!\n";
	case 6: return "\nNetz leer! Zu loeschendes Netz erst laden!\n";
	case 7: return "\nKeine Trainingsdaten gefunden! Training fehlgeschlagen!\n";
	case 8: return "\nNetz leer! Trainingsdaten erstellen fehlgeschlagen!\n";
	case 9: return "\nNetz leer! Initialisierung fehlgeschlagen!\n";
	default:
		break;
	}
}

void checkNetForError(const int _errorCode, std::vector<std::vector<Neuron>>* _network) {

	if (_network->size() == 0) {											// Check if a network exists and if NOT, throw an exception based on the given errorCode
		throw error(_errorCode);
	}
}

void checkNetForError(std::vector<std::vector<Neuron>>* _network) {

	if (_network->at(0).at(0).getIndividualClassifications() == 0) {		// Check if training data exist and throw an exception if not
		throw error(7);
	}
}

void checkUserInputForError() {

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw error(1);
	}
}
