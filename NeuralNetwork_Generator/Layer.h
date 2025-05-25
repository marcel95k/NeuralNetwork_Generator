#pragma once
#include<vector>
#include "Neuron.h"

class Layer {

private:
	std::vector<Neuron>layer;

public:
	void resizeLayer(const int _size);
	void addNeuron(const Neuron _neuron);

	int getLayerSize() const;
	Neuron& atNeuron(const int _index);
	const Neuron& atNeuron(const int _index) const;
};

