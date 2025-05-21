#include "Training.h"

void TRAINING::fitnessTest(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _grayValues, int _classification, double& _totalAccuracy, int& _totalTests) {

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
		_totalTests++;

		if (_classification == index) {
			_totalAccuracy += *maxIt;
#ifdef DEBUG_SHOW_FITNESSDETAILS_CORRECT
			std::cout << "\033[32m"
				<< std::setw(15) << _network->at(1).at(_classification).getClassificationName()
				<< " erkannt als "
				<< std::setw(15) << _network->at(1).at(index).getClassificationName()
				<< " mit "
				<< std::setw(6) << *maxIt << "%\033[0m" << std::endl;
#endif // DEBUG_SHOW_FITNESSDETAILS_CORRECT
		}

#ifdef DEBUG_SHOW_FITNESSDETAILS_WRONG
		else if (_classification != index) {
			std::cout << "\033[31m"
				<< std::setw(15) << _network->at(1).at(_classification).getClassificationName()
				<< " erkannt als "
				<< std::setw(15) << _network->at(1).at(index).getClassificationName()
				<< " mit "
				<< std::setw(6) << *maxIt << "%\033[0m" << std::endl;
		}
#endif // DEBUG_SHOW_FITNESSDETAILS_WRONG

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

void TRAINING::calculateMSE(std::vector<double>* _errors, double& totalError) {
	for (double e : *_errors) {
		totalError += e * e;
	}
	totalError /= _errors->size();
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

void TRAINING::training(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _grayValues, const int _target, double _epsilon, const double _momentumFactor) {

	for (int i = 0; i < _network->at(0).size(); ++i) {
		_network->at(0).at(i).setOutputValue(_grayValues->at(i));
	}

	for (int j = 0; j < _network->at(1).size(); ++j) {
		_network->at(1).at(j).setTargetOutputValue(0);
	}
	_network->at(1).at(_target).setTargetOutputValue(1);

	std::vector<double>errors(_network->at(1).size());

	TRAINING::forwardPassTraining(_network);
	TRAINING::calculateErrors(_network, &errors);
	
	double totalError = 0.0;
	TRAINING::calculateMSE(&errors, totalError);
	
	if (totalError > 0.0005) {
		TRAINING::weightAdaption(_network, &errors, _epsilon, _momentumFactor);
		TRAINING::biasAdaption(_network, &errors, _epsilon);
	}
}

void TRAINING::processValidationManual(std::vector<std::vector<Neuron>>* _network) {

	ERRORHANDLING::checkNetForError(10, _network);

	std::vector<double>grayValues;
	double totalAccuracy = 0.0;
	int totalTests = 0;
	
	std::cout << std::endl << "Validieren..." << std::endl;
	for (int counter = 0; counter < (NETWORKPROPERTIES::getIndividualSamples(_network) * VALIDATION_SHARE) / 100; counter++) {
		for (int classification = 0; classification < _network->at(1).size(); classification++) {
			FILEHANDLING::loadValidationIMG(_network, &grayValues, classification, counter);
			TRAINING::fitnessTest(_network, &grayValues, classification, totalAccuracy, totalTests);
			grayValues.clear();
		}
	}
	double averageAccuracy = totalAccuracy / totalTests;
	std::cout << "Durchschnittliche Erkennungsgenauigkeit: " << averageAccuracy << "%" << std::endl << std::endl;
	system("pause");
}

void TRAINING::processValidation(std::vector<std::vector<Neuron>>* _network, const int _counter, double& _totalAccuracy, int& _totalTests, int& x, std::vector<cv::Point>& _lossPoints, std::chrono::duration<double> _duration, const int _epoch, const double _epsilon) {

	std::vector<double>grayValues;

//	std::cout << std::endl << "Validieren..." << std::endl;;
	for (int counter = 0; counter < (NETWORKPROPERTIES::getIndividualSamples(_network) * VALIDATION_SHARE) / 100; counter++) {
		for (int classification = 0; classification < _network->at(1).size(); classification++) {
			FILEHANDLING::loadValidationIMG(_network, &grayValues, classification, counter);
			TRAINING::fitnessTest(_network, &grayValues, classification, _totalAccuracy, _totalTests);
			grayValues.clear();
		}
	}
	if (_totalTests > 0) {
		double averageAccuracy = _totalAccuracy / _totalTests;
	//	std::cout << "Durchschnittliche Erkennungsgenauigkeit: " << averageAccuracy << "%" << std::endl << std::endl;
		GRAPHICS::drawLoss(averageAccuracy, _counter, x, _lossPoints, _duration, _epoch, _epsilon);

		// First Neuron of the first Layer will contain the information about the average accuracy
		_network->at(0).at(0).setAverageAccuracy(averageAccuracy);
	}
}

void TRAINING::processTraining(std::vector<std::vector<Neuron>>* _network, double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs) {

	double totalAccuracy = 0.0;
	int totalTests = 0;

	std::vector<double>grayValues;

	// Variables for drawing the loss function
	std::vector<cv::Point> lossPoints;
	int x = 10;

	std::chrono::duration<double> duration = std::chrono::seconds(0);

	for (int epoch = 0; epoch < _epochs; ++epoch) {
	//	std::cout << "\033[32mTraining Epoch\033[0m " << epoch << std::endl;
		auto start = std::chrono::high_resolution_clock::now();
		if (epoch % 10 == 0 && epoch > 0) {  // After 10 epochs, reduce the learning rate every 10 epochs
			_epsilon *= _epsilonDecay;  // Reduce learning rate by epsilon decay
		}
		double epochError = 0.0;
		try {
			for (int counter = 0; counter < NETWORKPROPERTIES::getIndividualSamples(_network); counter++) {
				// Load training images and validate
				for (int classification = 0; classification < _network->at(1).size(); classification++) {
					FILEHANDLING::loadTrainingIMG(_network, &grayValues, classification, counter);	
					TRAINING::training(_network, &grayValues, classification, _epsilon, _momentumFactor);
					grayValues.clear();
				}
				if (NETWORKPROPERTIES::getValidationFlag(_network) == true) {
					TRAINING::processValidation(_network, counter, totalAccuracy, totalTests, x, lossPoints, duration, epoch, _epsilon);
				}
			}
		}
		catch (const std::string& error) {
			std::cerr << error << std::endl;
		}
		auto end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		duration = duration *_epochs/60;
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

	if (NETWORKPROPERTIES::getValidationFlag(_network) == true) {
		destroyWindow("Loss");
	}

	NETWORKPROPERTIES::enableTrainedFlag(_network);
	NETWORKPROPERTIES::disableSavedFlag(_network);
}

