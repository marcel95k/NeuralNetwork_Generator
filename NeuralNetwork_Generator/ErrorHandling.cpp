#include "ErrorHandling.h"

void ERRORHANDLING::FLAGCHECK::checkNetBeforeTraining(const Network& _network) {

	if (_network.getNetworkSize() <= 0) {
		throw NNG_Exception("Netz leer! Training fehlgeschlagen!");
	}
	
	for (int i = 0; i < _network.getNetworkSize(); i++) {
		if (_network.atLayer(i).getLayerSize() <= 0) {
			throw NNG_Exception("Unvollständige Schichten! Training fehlgeschlagen!");
		}
	}
}

void ERRORHANDLING::FLAGCHECK::checkNetBeforeSaving(const Network& _network) {

	if (_network.getNetworkSize() <= 0) {
		throw NNG_Exception("Netz leer! Speichern fehlgeschlagen!");
	}
}

void ERRORHANDLING::FLAGCHECK::checkNetBeforeDeleting(const Network& _network) {

	if (_network.getNetworkSize() <= 0) {
		throw NNG_Exception("Netz leer! Loeschen fehlgeschlagen!");
	}
}

void ERRORHANDLING::FLAGCHECK::checkNetBeforeInitializing(const Network& _network) {

	if (_network.getNetworkSize() <= 0) {
		throw NNG_Exception("Netz leer! Initialisieren fehlgeschlagen!");
	}
}
