#pragma once
#include <vector>

#include "Defines.h"
#include "Utility.h"
#include "FileHandling.h"

#include "Network.h"

class Trainer {

private:
	double epsilon;
	double epsilonDecay;
	double momentumFactor;
	double epochs;

public:
	Trainer(const double _epsilon, const double _epsilonDecay, const double _momentumFactor, const double _epochs):
		epsilon(_epsilon), 
		epsilonDecay(_epsilonDecay), 
		momentumFactor(_momentumFactor), 
		epochs(_epochs)
	{}

	double getEpsilon();
	double getEpsilonDecay();
	double getMomentumFactor();
	double getEpochs();

	void reduceLearningrate();

	void setExpectedOutputs(std::vector<double>& _expectedOutputs, const int _classification);

	void trainNetwork(Network& _network);
};

