#include "Neuron.h"

void Neuron::setOutputValue(const double _value) {
	outputValue = _value;
}

void Neuron::setBias(const double _value) {
	bias = _value;
}

void Neuron::resizeWeightVector(const int _size) {
	weights.resize(_size);
}

void Neuron::setWeightAt(const int _index, const double _value) {
	weights[_index] = _value;
}

void Neuron::addWeightAt(const int _index, const double _value, const double _momentumFactor) {
	momentum += _value;
	momentum *= _momentumFactor;
	weights[_index] += _value;
}


double Neuron::getOutputValue() const {
	return outputValue;
}

double Neuron::getBias() const {
	return bias;
}

double Neuron::getWeightAt(const int _index) const {
	return weights[_index];
}

int Neuron::getSizeOfWeightVector() const {
	return weights.size();
}

double Neuron::sigmoid(const double x) const {
	return 1.0 / (1.0 + std::exp(-x));
}

double Neuron::sigmoidDerivative(const double x) const {
	return x * (1 - x);
}
