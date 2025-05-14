#pragma once
#include <string>
#include <vector>
#include "Neuron.h"
#include "MathFunctions.h"
#include "FileHandling.h"
#include "Defines.h"

void fitnessTest(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, int digit, double& totalAccuracy, int& totalTests); // Calculates the outputs based on validationdata.
void biasAdaption(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors, const double _epsilon);
void weightAdaption(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors, const double _epsilon, const double _momentumFactor);
void calculateErrors(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors); // Calculates all errors based on the target MINUS the calculated output
void forwardPassTraining(std::vector<std::vector<Neuron>>* _network); // Calculates all outputs
void training(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, const int _target, double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs); // Initializes the net and processes the forwardPass + adaptions
void processTraining(std::vector<std::vector<Neuron>>* _network, const double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs); // Loop for processing the training iteration and by loading one image after another 
void setTrainingParameters(std::vector<std::vector<Neuron>>* _network, double& _epsilon, double& _epsilonDecay, double& _momentumfactor, int& _epochs);	// Setup for training parameters
void setupTraining(std::vector<std::vector<Neuron>>* _network);														

