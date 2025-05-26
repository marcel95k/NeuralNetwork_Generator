#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <assert.h>

#include "ExceptionHandling.h"

#include "Layer.h"

class Network {

private:
	std::vector<Layer>network;
	std::string networkName = "";
	std::vector<std::string> outputLabels;
	bool isSaved = false;
	bool isModified = false;

public:
	void resizeNetwork(const int _size);
	void addLayer(const Layer _layer);
	void connectLayers();
	void randomize();

	void setNetworkName(const std::string _networkName);
	void setOutputLabels(const std::vector<std::string>& _labels);

	int getNetworkSize() const;
	std::string getNetworkName() const;
	const std::vector<std::string>& getOutputLabels() const;
	const std::string getOutputLabelAt(const int _index) const;
	Layer& atLayer(const int _index);
	const Layer& atLayer(const int _index) const;

	std::vector<double>forwardPass();
	void backpropagation(const std::vector<double>& _expected, double _learningRate);

	void calculateOutputLayerError(std::vector<std::vector<double>>& _deltas, const std::vector<double>& _expected);
	void propagateErrorsBackward(std::vector<std::vector<double>>& _deltas);
	void updateWeights(const std::vector<std::vector<double>>& _deltas, double _learningRate);

	void saveToFile(const std::string& _filename) const;
	void loadFromFile(const std::string& _filename);

	void setSavedStatus(bool _isSaved);
	bool getSavedStatus();

	void setModifiedStatus(bool _isModified);
	bool getModifiedStatus();

};

