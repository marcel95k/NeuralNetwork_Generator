#include "NetworkHandler.h"

void NETWORKHANDLER::setOutputLabels(Network& _network) {

	std::cout << std::endl;
	int outputLayer = _network.getNetworkSize() - 1;
	std::vector<std::string>outputLabels;
	for (int i = 0; i < _network.atLayer(outputLayer).getLayerSize(); i++) {
		std::string label;
		std::cout << "Klassifikation von Output " << i + 1 << ": ";
		std::cin >> label;
		outputLabels.push_back(label);
	}

	_network.setOutputLabels(outputLabels);
}

void NETWORKHANDLER::fillLayers(Network& _network, const std::vector<int> _topology) {

	// Input Layer size will always be 400
	for (int i = 0; i < 400; i++) {
		Neuron neuron;
		_network.atLayer(0).addNeuron(neuron);
	}

	for (int i = 1; i < _network.getNetworkSize(); i++) {	// Starting at 2nd layer because the Input Layer is already filled
		for (int t = 0; t < _topology.at(i-1); t++) {
			Neuron neuron;
			_network.atLayer(i).addNeuron(neuron);
		}
	}
}

Network NETWORKHANDLER::newNet(const int _amountOfHiddenLayers, const std::vector<int> _topology) {

	Network tempNetwork;
	int netWorkTotalSize = _amountOfHiddenLayers + 2;	// Add 2 because of Input Layer and Output Layer

	for (int i = 0; i < netWorkTotalSize; i++) {
		Layer layer;
		tempNetwork.addLayer(layer);
	}

	NETWORKHANDLER::fillLayers(tempNetwork, _topology);
	NETWORKHANDLER::setOutputLabels(tempNetwork);

	tempNetwork.connectLayers();
	tempNetwork.randomize();

	return tempNetwork;
}


void NETWORKHANDLER::networkSaver(Network& _network) {

	if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _network.getNetworkName())) {
		_network.setNetworkName("");
		throw NNG_Exception("Netzwerk existiert bereits!");
	}
}