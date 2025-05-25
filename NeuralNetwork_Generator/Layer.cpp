#include "Layer.h"

void Layer::resizeLayer(const int _size) {
	layer.resize(_size);
}

void Layer::addNeuron(const Neuron _neuron) {
	layer.push_back(_neuron);
}


int Layer::getLayerSize() const {
	return layer.size();
}

Neuron& Layer::atNeuron(const int _index) {
	return layer.at(_index);
}

const Neuron& Layer::atNeuron(const int _index) const {
	return layer.at(_index);
}

