#include "NetworkHandler.h"

// NEWNET
void NETWORKHANDLER::NEWNET::fillLayers(Network& _network, const std::vector<int> _topology) {

	assert(_network.getNetworkSize() > 0);   // Network size has to be > 0

	// Input Layer size will always be 400
	for (int i = 0; i < INPUTSIZE; i++) {
		Neuron neuron;
		_network.atLayer(0).addNeuron(neuron);
	}

	for (int i = 1; i < _network.getNetworkSize(); i++) {	// Starting at 2nd layer because the Input Layer is already filled
		for (int t = 0; t < _topology.at(i - 1); t++) {
			Neuron neuron;
			_network.atLayer(i).addNeuron(neuron);
		}
	}
}

Network NETWORKHANDLER::NEWNET::buildNewNet(const int _amountOfHiddenLayers, const std::vector<int> _topology) {

	Network tempNetwork;
	int netWorkTotalSize = _amountOfHiddenLayers + 2;	// Add 2 because of Input Layer and Output Layer

	for (int i = 0; i < netWorkTotalSize; i++) {
		Layer layer;
		tempNetwork.addLayer(layer);
	}

	NETWORKHANDLER::NEWNET::fillLayers(tempNetwork, _topology);

	tempNetwork.connectLayers();
	tempNetwork.randomize();

	return tempNetwork;
}

// DATA MANAGEMENT
void NETWORKHANDLER::DATAMANAGEMENT::saveNewNetwork(Network& _network) {

	_network.setModifiedStatus(false);
	_network.setSavedStatus(true);
	_network.saveToFile("Networks/" + _network.getNetworkName() + "/" + _network.getNetworkName() + ".nng");
}

void NETWORKHANDLER::DATAMANAGEMENT::saveNewNetworkAs(Network& _network) {

	if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _network.getNetworkName()) == true) {
		throw NNG_Exception("Netzwerk existiert bereits!");
	}

	FILEHANDLING::createNewNetworkFolder(_network);
	FILEHANDLING::createNewTrainingdataFolder(_network);
	FILEHANDLING::createNewValidationdataFolder(_network);
	FILEHANDLING::addNetworkToList(_network);

	_network.setModifiedStatus(false);
	_network.saveToFile("Networks/" + _network.getNetworkName() + "/" + _network.getNetworkName() + ".nng");
}

void NETWORKHANDLER::DATAMANAGEMENT::loadNetwork(Network& _network, const std::string _networkName) {

	if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _networkName) == false) {
		throw NNG_Exception("Netzwerk nicht gefunden!");
	}
	_network.loadFromFile("Networks/" + _networkName + "/" + _networkName + ".nng");
}

void NETWORKHANDLER::DATAMANAGEMENT::deleteNetwork(Network& _network) {

	FILEHANDLING::removeNetworkFromList(_network);
	FILEHANDLING::deleteFolders(_network);
	_network.resizeNetwork(0);
	_network.setNetworkName("Unbenanntes Netz");
}