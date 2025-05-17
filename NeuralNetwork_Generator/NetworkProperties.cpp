#include "NetworkProperties.h"

void NETWORKPROPERTIES::setNetworkAsSaved(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsSaved(true);
}
void NETWORKPROPERTIES::setNetworkAsUnsaved(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsSaved(false);
}


void NETWORKPROPERTIES::setNetworkAsTrained(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsTrained(true);
}
void NETWORKPROPERTIES::setNetworkAsUntrained(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setIsTrained(false);
}


void NETWORKPROPERTIES::enableValidationFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setHasValidationdata(true);
}
void NETWORKPROPERTIES::clearValidationFlag(std::vector<std::vector<Neuron>>* _network) {
	_network->at(0).at(0).setHasValidationdata(false);
}