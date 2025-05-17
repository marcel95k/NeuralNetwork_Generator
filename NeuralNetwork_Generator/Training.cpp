#include "Training.h"

void TRAINING::fitnessTest(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, int digit, double& totalAccuracy, int& totalTests) {

	std::vector<double>percentVector;

	for (int i = 0; i < _network->at(0).size(); ++i) {
		_network->at(0).at(i).setOutputValue(_grayValues->at(i));
	}

	double totalSum = 0.0; 	// Calculate total sum of outputs (only once, after calculating all outputs)
	TEST::forwardPassTest(_network, totalSum);

	// Calculate percentages of outputs and fill the percentVector
	for (int j = 0; j < _network->at(1).size(); ++j) {
		// Percentage of the current output in relation to the total amount
		double percentage = (_network->at(1).at(j).getOutputValue() / totalSum) * 100;
		percentVector.push_back(percentage);
	}

	// Find highest element
	auto maxIt = max_element(percentVector.begin(), percentVector.end());

	// Update accuracy statistics
	if (maxIt != percentVector.end()) {
		int index = distance(percentVector.begin(), maxIt);
		totalTests++;
		if (digit == index) {
			totalAccuracy += *maxIt;

#ifdef DEBUG_SHOW_FITNESSDETAILS
			std::cout << std::endl << _network->at(1).at(digit).getClassificationName()
				<< " erkannt als " << _network->at(1).at(index).getClassificationName()
				<< " mit " << *maxIt << "%";
#endif // DEBUG_SHOW_FITNESSDETAILS 
		}
	}
}

void TRAINING::biasAdaption(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors, const double _epsilon) {

	for (int j = 0; j < _network->at(1).size(); ++j) {
		double delta = _epsilon * _errors->at(j) * sigmoid_derivative(_network->at(1).at(j).getOutputValue()) * 1.0;
		double newBias = _network->at(1).at(j).getBiasWeight() + delta;
		_network->at(1).at(j).setBiasWeight(newBias);
	}
}

void TRAINING::weightAdaption(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors, const double _epsilon, const double _momentumFactor) {

	for (int i = 0; i < _network->at(0).size(); ++i) {
		for (int j = 0; j < _network->at(1).size(); ++j) {
			double delta = _epsilon * _errors->at(j) * sigmoid_derivative(_network->at(1).at(j).getOutputValue()) * _network->at(0).at(i).getOutputValue();
			_network->at(0).at(i).addWeightAt(j, delta, _momentumFactor);
		}
	}
}

void TRAINING::calculateErrors(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _errors) {

	for (int k = 0; k < _network->at(1).size(); ++k) {
		_errors->at(k) = _network->at(1).at(k).getTargetOutputValue() - _network->at(1).at(k).getOutputValue();
	}
}

void TRAINING::forwardPassTraining(std::vector<std::vector<Neuron>>* _network) {

	for (int j = 0; j < _network->at(1).size(); ++j) {
		double sum = 0.0;
		for (int i = 0; i < _network->at(0).size(); ++i) {
			sum += _network->at(0).at(i).getOutputValue() * _network->at(0).at(i).getWeightAt(j);

		}
		sum += 1.0 * _network->at(1).at(j).getBiasWeight(); // Add bias
		_network->at(1).at(j).setOutputValue(sigmoid(sum)); // Apply sigmoid to output
	}
}

void TRAINING::training(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, const int _target, double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs) {

	for (int i = 0; i < _network->at(0).size(); ++i) {
		_network->at(0).at(i).setOutputValue(_grayValues->at(i));
	}
	
	for (int j = 0; j < _network->at(1).size(); ++j) {
		_network->at(1).at(j).setTargetOutputValue(0);
	}
	_network->at(1).at(_target).setTargetOutputValue(1);
	
	for (int epoch = 0; epoch < _epochs; ++epoch) {

		std::vector<double>errors(_network->at(1).size());

		if (epoch % 50 == 0 && epoch > 0) {  // After 50 epochs, reduce the learning rate every 50 epochs
			_epsilon *= _epsilonDecay;  // Reduce learning rate by epsilon decay
		}

		TRAINING::forwardPassTraining(_network);
		TRAINING::calculateErrors(_network, &errors);
		TRAINING::weightAdaption(_network, &errors, _epsilon, _momentumFactor);
		TRAINING::biasAdaption(_network, &errors, _epsilon);

		errors.clear();
	}
}

void TRAINING::processValidation(std::vector<std::vector<Neuron>>* _network, double& totalAccuracy, int& totalTests, int& x, std::vector<cv::Point>& _lossPoints, std::chrono::duration<double> _duration) {

	std::vector<float>grayValues;

	std::cout << std::endl << "Validieren...";
	for (int counter = 0; counter < (_network->at(0).at(0).getIndividualClassifications() * VALIDATION_SHARE) / 100; counter++) {
		for (int classification = 0; classification < _network->at(1).size(); classification++) {
			FILEHANDLING::loadValidationIMG(_network, &grayValues, classification, counter, totalAccuracy, totalTests);
			TRAINING::fitnessTest(_network, &grayValues, classification, totalAccuracy, totalTests);
			grayValues.clear();
		}
	}
	if (totalTests > 0) {
		double averageAccuracy = totalAccuracy / totalTests;
		std::cout << std::endl << "Durchschnittliche Erkennungsgenauigkeit: " << averageAccuracy << "%" << std::endl << std::endl;
		GRAPHICS::drawLoss(averageAccuracy, x, _lossPoints, _duration);

		// First Neuron of the first Layer will contain the information about the average accuracy
		_network->at(0).at(0).setAverageAccuracy(averageAccuracy);
	}
}

void TRAINING::processTraining(std::vector<std::vector<Neuron>>* _network, const double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs) {

	double totalAccuracy = 0.0;
	int totalTests = 0;

	std::vector<float>grayValues;

	// Variables for drawing the loss function
	std::vector<cv::Point> lossPoints;
	int x = 10;

	// Saves the duration of the training loop
	std::chrono::duration<double> duration = std::chrono::seconds(0);

	try {
		// Load training images and validate
		for (int counter = 0; counter < _network->at(0).at(0).getIndividualClassifications(); counter++) {
			auto start = std::chrono::high_resolution_clock::now();

			std::cout << "Training..." << std::endl;
			for (int classification = 0; classification < _network->at(1).size(); classification++) {
				FILEHANDLING::loadTrainingIMG(_network, &grayValues, classification, counter, _epsilon, _epsilonDecay, _momentumFactor, _epochs);
				TRAINING::training(_network, &grayValues, classification, _epsilon, _epsilonDecay, _momentumFactor, _epochs);
				grayValues.clear();
			}
			if (_network->at(0).at(0).getHasValidationdata()) {
				TRAINING::processValidation(_network, totalAccuracy, totalTests, x, lossPoints, duration);
			}

			auto end = std::chrono::high_resolution_clock::now();
			duration = end - start;
			duration = duration * _network->at(0).at(0).getIndividualClassifications() / 60;
		}
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
	}

	x = 0;
	lossPoints.clear();
}

void TRAINING::setTrainingParameters(std::vector<std::vector<Neuron>>* _network, double& _epsilon, double& _epsilonDecay, double& _momentumFactor, int& _epochs) {

	std::cout << std::endl << "Lernrate: ";
	std::cin >> _epsilon;
	ERRORHANDLING::checkUserInputForError();

	std::cout << "Lernraten-Reduktion (max 1): ";
	std::cin >> _epsilonDecay;
	ERRORHANDLING::checkUserInputForError();

	if (_epsilonDecay > 1 || _epsilonDecay < 0) { throw ERRORHANDLING::error(1); }

	std::cout << "Momentum Faktor (max 0.9): ";
	std::cin >> _momentumFactor;
	ERRORHANDLING::checkUserInputForError();
	if (_momentumFactor > 0.9 || _momentumFactor < 0) { throw ERRORHANDLING::error(1); }

	std::cout << "Trainingsepochen (min. 60): ";
	std::cin >> _epochs;
	ERRORHANDLING::checkUserInputForError();
	if (_epochs < 60) { throw ERRORHANDLING::error(1); }
}

void TRAINING::setupTraining(std::vector<std::vector<Neuron>>* _network) {

	ERRORHANDLING::checkNetForError(4, _network);
	ERRORHANDLING::checkNetForTrainingData(_network);

	std::string userInput_s;
	double epsilon;
	double epsilonDecay;
	double momentumFactor;
	int epochs;

	std::cout << "Standard Training? (J/N)";
	std::cout << std::endl << "Eingabe: ";
	std::cin >> userInput_s;

	if (userInput_s == "j" || userInput_s == "J") {
		epsilon =			DEFAULT_EPSILON;
		epsilonDecay =		DEFAULT_EPSILON_DECAY;
		momentumFactor =	DEFAULT_MOMENTUMFACTOR;
		epochs =			DEFAULT_EPOCHS;
	}
	else if (userInput_s == "n" || userInput_s == "N") {
		TRAINING::setTrainingParameters(_network, epsilon, epsilonDecay, momentumFactor, epochs);
	}
	else { throw ERRORHANDLING::error(1); }

	TRAINING::processTraining(_network, epsilon, epsilonDecay, momentumFactor, epochs);

	std::cout << std::endl << "\033[32mTraining abgeschlossen.\033[0m" << std::endl << std::endl;
	system("pause");

	if (_network->at(0).at(0).getHasValidationdata()) {
		destroyWindow("Loss");
	}

	NETWORKPROPERTIES::setNetworkAsTrained(_network);
	NETWORKPROPERTIES::setNetworkAsUnsaved(_network);
}

