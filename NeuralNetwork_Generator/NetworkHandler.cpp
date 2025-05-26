#include "NetworkHandler.h"


void NETWORKHANDLER::NEWNET::fillLayers(Network& _network, const std::vector<int> _topology) {

	// Input Layer size will always be 400
	for (int i = 0; i < 400; i++) {
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


void NETWORKHANDLER::DATAMANAGEMENT::saveNewNetwork(Network& _network) {

	_network.setModifiedStatus(false);
	_network.setSavedStatus(true);
	_network.saveToFile("Networks/" + _network.getNetworkName() + "/" + _network.getNetworkName() + ".nng");
}

void NETWORKHANDLER::DATAMANAGEMENT::saveNewNetworkAs(Network& _network) {

	if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _network.getNetworkName())) {
		throw NNG_Exception("Netzwerk existiert bereits!");
	}

	FILEHANDLING::createNewNetworkFolder(_network);
	FILEHANDLING::createNewTrainingdataFolder(_network);
	FILEHANDLING::createNewValidationdataFolder(_network);
	FILEHANDLING::addNetworkToSavedNetworksList(_network);

	_network.setModifiedStatus(false);
	_network.setSavedStatus(true);
	_network.saveToFile("Networks/" + _network.getNetworkName() + "/" + _network.getNetworkName() + ".nng");
}

void NETWORKHANDLER::DATAMANAGEMENT::loadNetwork(Network& _network, const std::string _networkName) {

	Network tempNetwork;

	if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _networkName) == false) {
		throw NNG_Exception("Netzwerk nicht gefunden!");
	}

	tempNetwork.loadFromFile("Networks/" + _networkName + "/" + _networkName + ".nng");

	std::cout << tempNetwork.getNetworkSize() << std::endl; system("pause");
}