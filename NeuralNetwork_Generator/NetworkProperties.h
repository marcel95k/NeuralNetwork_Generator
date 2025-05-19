#pragma once
#include "Neuron.h"

/*-------------------------------------------------------------------------------*/
/* The first neuron in the first layer of a network contains all of its metadata */
/*-------------------------------------------------------------------------------*/

/*Still left to implement:
	- Network name
	- Individual classifications
*/

namespace NETWORKPROPERTIES{
	
	void enableSavedFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as saved by changing the information in the first Neuron of the first Layer
	void disableSavdFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT saved by changing the information in the first Neuron of the first Layer

	void enableTrainedFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as trained by changing the information in the first Neuron of the first Layer
	void disableTrainedFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT trained by changing the information in the first Neuron of the first Layer
	
	void enableValidationFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as having validationdata by changing the information in the first Neuron of the first Layer
	void disableValidationFlag(std::vector<std::vector<Neuron>>* _network); // Marking the network as NOT having validationdata by changing the information in the first Neuron of the first Layer

	bool getSavedFlag(std::vector<std::vector<Neuron>>* _network);	// Returns whether the network is saved or NOT
	bool getTrainedFlag(std::vector<std::vector<Neuron>>* _network);	// Returns whether the network is trained or NOT
	bool getValidationFlag(std::vector<std::vector<Neuron>>* _network);	// Returns whether the network has validationdata or NOT
}