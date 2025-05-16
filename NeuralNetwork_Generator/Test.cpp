#include "Test.h"

void TEST::forwardPassTest(std::vector<std::vector<Neuron>>* _network, double& _totalSum) {

	for (int j = 0; j < _network->at(1).size(); ++j) {
		double sum = 0.0;
		for (int i = 0; i < _network->at(0).size(); ++i) {
			sum += _network->at(0).at(i).getOutputValue() * _network->at(0).at(i).getWeightAt(j);
		}
		sum += 1.0 * _network->at(1).at(j).getBiasWeight(); // Add bias
		_network->at(1).at(j).setOutputValue(sigmoid(sum));  // Apply sigmoid to output

		// Update total sum of outputs
		_totalSum += _network->at(1).at(j).getOutputValue();
	}
}

void TEST::test(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues) {

	system("cls");
	for (int i = 0; i < _network->at(0).size(); ++i) {
		_network->at(0).at(i).setOutputValue(_grayValues->at(i));
	}

	double totalSum = 0.0; 	// Calculate total sum of outputs (only once, after calculating all outputs)
	TEST::forwardPassTest(_network, totalSum);

	// Calculate and show percentages of outputs
	for (int j = 0; j < _network->at(1).size(); ++j) {
		// Percentage of the current output in relation to the total amount
		double percentage = (_network->at(1).at(j).getOutputValue() / totalSum) * 100;

		// Display percentage
		std::cout << _network->at(1).at(j).getClassificationName() << ": " << percentage << "%" << std::endl;
	}
	GRAPHICS::drawBarGraph(&_network->at(1));
}

void TEST::checkIfTrained(std::vector<std::vector<Neuron>>* _network) {

	std::string userInput_s;

	if (_network->at(0).at(0).getIsTrained() == false) {
		std::cout << "Netzwerk zuerst trainieren? (J/N)";
		std::cout << std::endl << "Eingabe: ";
		std::cin >> userInput_s;
		if (userInput_s == "J" || userInput_s == "j") {
			ERRORHANDLING::checkNetForError(_network);
			try {
				TRAINING::setupTraining(_network);
			}
			catch (const std::string& error) {
				std::cerr << error << std::endl;
				system("pause");
			}
		}
	}
}

void TEST::setupTest(std::vector<std::vector<Neuron>>* _network) {

	ERRORHANDLING::checkNetForError(5, _network);
	checkIfTrained(_network);

	std::vector<float>dummyValues;
	dummyValues.resize(_network->at(0).size());
	TEST::test(_network, &dummyValues);

	try {
		GRAPHICS::drawTestImage(_network);
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
		system("pause");
	}
}



