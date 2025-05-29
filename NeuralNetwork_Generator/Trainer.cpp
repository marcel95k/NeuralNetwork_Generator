#include "Trainer.h"

double Trainer::getEpsilon() {
	return epsilon;
}

double Trainer::getEpsilonDecay() {
	return epsilonDecay;
}

double Trainer::getMomentumFactor() {
	return momentumFactor;
}

double Trainer::getEpochs() {
	return epochs;
}


void Trainer::reduceLearningrate() {
	epsilon *= epsilonDecay;
}


void Trainer::setExpectedOutputs(std::vector<double>& _expectedOutputs, const int _classification) {

	for (int e = 0; e < _expectedOutputs.size(); e++) {
		if (e == _classification) {
			_expectedOutputs[e] = 1;
		}
		else if (e != _classification) {
			_expectedOutputs[e] = 0;
		}
	}
}


void Trainer::trainNetwork(Network& _network) {

	Layer& outputLayer = _network.atLayer(_network.getNetworkSize() - 1);


	auto total_start = std::chrono::high_resolution_clock::now();

	for (int epoch = 0; epoch < getEpochs(); epoch++) {

		auto epoch_start = std::chrono::high_resolution_clock::now();

		if (epoch % 10 == 0 && epoch > 0) {  // After 10 epochs, reduce the learning rate every 10 epochs
			reduceLearningrate();
		}

		try {
			for (int sampleCounter = 0; sampleCounter < _network.getIndividualSampleSize(); sampleCounter++) {

				// Load training images and validate
				for (int classification = 0; classification < outputLayer.getLayerSize(); classification++) {
					std::vector<double>inputValues;

					FILEHANDLING::loadTrainingIMG(_network, inputValues, classification, sampleCounter);

					_network.fillInputLayer(inputValues);
					
					std::vector<double>expectedOutputs;
					expectedOutputs.resize(outputLayer.getLayerSize());

					setExpectedOutputs(expectedOutputs, classification);

					std::vector<double>output = _network.forwardPass();
					_network.backpropagation(expectedOutputs, epsilon, momentumFactor);
				}
				/*if (NETWORKPROPERTIES::getValidationFlag(_network) == true) {
					TRAINING::processValidation(_network, counter, totalAccuracy, totalTests, x, lossPoints, duration, epoch, _epsilon);
				}*/
			}
		}
		catch (const NNG_Exception& exception) {
			std::cerr << std::endl << exception.what() << std::endl;
			throw NNG_Exception("Training abgebrochen!");
		}

		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_total = now - total_start;

		// Durchschnittszeit pro Epoche
		double avg_epoch_time = elapsed_total.count() / epoch;

		// Geschätzte verbleibende Zeit
		double time_left = avg_epoch_time * (epochs - epoch);
		if (epoch % 10 == 0 && epoch > 0) {  // After 10 epochs, reduce the learning rate every 10 epochs
			std::cout << "Geschaetzte verbleibende Zeit: " << time_left << " s\n\n";
		}
	}

	std::cout << "Training erfolgreich!" << std::endl;
	awaitAnyKey();
}