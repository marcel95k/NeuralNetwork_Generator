#include "NetworkProperties.h"

void NETWORKPROPERTIES::enableSavedFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsSaved(true);
}
void NETWORKPROPERTIES::disableSavdFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsSaved(false);
}


void NETWORKPROPERTIES::enableTrainedFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsTrained(true);
}
void NETWORKPROPERTIES::disableTrainedFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsTrained(false);
}


void NETWORKPROPERTIES::enableValidationFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setHasValidationdata(true);
}
void NETWORKPROPERTIES::disableValidationFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setHasValidationdata(false);
}