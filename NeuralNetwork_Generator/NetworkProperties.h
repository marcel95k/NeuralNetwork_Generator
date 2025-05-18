#pragma once
#include "Neuron.h"

namespace NETWORKPROPERTIES{
	
	void enableSavedFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as saved by changing the information in the first Neuron of the first Layer
	void disableSavdFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT saved by changing the information in the first Neuron of the first Layer

	void enableTrainedFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as trained by changing the information in the first Neuron of the first Layer
	void disableTrainedFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT trained by changing the information in the first Neuron of the first Layer
	
	void enableValidationFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as having validationdata by changing the information in the first Neuron of the first Layer
	void disableValidationFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT having validationdata by changing the information in the first Neuron of the first Layer
}