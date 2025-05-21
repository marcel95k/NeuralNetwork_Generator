#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <iomanip>
#include <assert.h>
#include <opencv2/opencv.hpp>
#include "Neuron.h"
#include "NetworkProperties.h"
#include "MathFunctions.h"
#include "FileHandling.h"
#include "Defines.h"

/*-------------------------------------*/
/* Handles all training-relazted tasks */
/*-------------------------------------*/

/*Still left to implement:

*/


namespace TRAINING {

	void fitnessTest(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _grayValues, int _classification, double& _totalAccuracy, int& _totalTests); // Calculates the outputs based on validationdata.
	
	void biasAdaption(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors, const double _epsilon);
	void weightAdaption(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors, const double _epsilon, const double _momentumFactor);
	void calculateMSE(std::vector<double>* _errors, double& totalError); // Calculate the MSE for the current epoch
	void calculateErrors(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors); // Calculates all errors based on the target MINUS the calculated output
	void forwardPassTraining(std::vector<std::vector<Neuron>>* _network); // Calculates all outputs
	void training(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _grayValues, const int _target, double _epsilon, const double _momentumFactor); // Processes the forwardPass + adaptions
	
	void processValidationManual(std::vector<std::vector<Neuron>>* _network); // Loop for processing the validation iteration and by loading one image after another 
	void processValidation(std::vector<std::vector<Neuron>>* _network, const int _counter, double& _totalAccuracy, int& _totalTests, int& x, std::vector<cv::Point>& _lossPoints, std::chrono::duration<double> _duration, const int _epoch, const double _epsilon); // Loop for processing the validation iteration and by loading one image after another and drawing the loss-graph
	void processTraining(std::vector<std::vector<Neuron>>* _network, double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs); // Loop for processing the training iteration and by loading one image after another 
	
	void setTrainingParameters(std::vector<std::vector<Neuron>>* _network, double& _epsilon, double& _epsilonDecay, double& _momentumfactor, int& _epochs);	// Setup for training parameters
	void setupTraining(std::vector<std::vector<Neuron>>* _network);
}
