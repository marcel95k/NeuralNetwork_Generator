#pragma once
#include "Neuron.h"

namespace NETWORKPROPERTIES{
	
	void setNetworkAsSaved(std::vector<std::vector<Neuron>>* _network);
	void setNetworkAsUnsaved(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT saved by changing the information in the first Neuron of the first Layer

	void setNetworkAsTrained(std::vector<std::vector<Neuron>>* _network);
	void setNetworkAsUntrained(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT trained by changing the information in the first Neuron of the first Layer
	
	void enableValidationFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as having validationdata by changing the information in the first Neuron of the first Layer
	void clearValidationFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT having validationdata by changing the information in the first Neuron of the first Layer
}