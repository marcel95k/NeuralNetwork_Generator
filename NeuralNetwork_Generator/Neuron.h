#pragma once
#include <vector>
#include <cmath>
#include <string>

class Neuron {

private:
	double outputValue = 0;
	double bias;
	double momentum = 0;
	std::vector<double>weights;

public:
	void setOutputValue(const double _value);
	void setBias(const double _value);
	void resizeWeightVector(const int _size);
	void setWeightAt(const int _index, const double _value);
	void addWeightAt(const int _index, const double _value, const double _momentumFactor);

	double getOutputValue() const;
	double getBias() const;
	double getWeightAt(const int _index) const;
	int getSizeOfWeightVector() const;
	double sigmoid(const double x) const;
	double sigmoidDerivative(const double x) const;
};

