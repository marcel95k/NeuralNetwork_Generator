#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "Defines.h"
#include "ErrorHandling.h"
#include "FileHandling.h"
#include "MathFunctions.h"
#include "Neuron.h"
#include "NetworkProperties.h"
#include "Test.h"
#include "Training.h"
#include "Graphics.h"


using namespace std;

static void initializeNet(std::vector<std::vector<Neuron>>* _network) {

	ERRORHANDLING::checkNetForError(9, _network);

	for (int i = 0; i < _network->at(0).size(); ++i) {
		_network->at(0).at(i).resizeWeightVector(_network->at(1).size());
	}

	// Init weights and bias weights once
	for (int j = 0; j < _network->at(1).size(); ++j) {
		double biasInit = ((rand() % 100) / 100.0) - 0.5; // [-0.5, 0.5]
		_network->at(1).at(j).setBiasWeight(biasInit);

		for (int i = 0; i < _network->at(0).size(); ++i) {
			double w = ((rand() % 100) / 100.0) - 0.5; // [-0.5, 0.5]
			_network->at(0).at(i).setWeightAt(j, w);
			_network->at(0).at(i).setNewWeight(false);
		}
	}
	NETWORKPROPERTIES::disableTrainedFlag(_network);
}

static vector<vector<Neuron>> newNet() {

	vector<vector<Neuron>> network;

	string networkName;

	FILEHANDLING::displaySavedNetworks();

	cout << endl << "Name des Netzes: ";
	cin >> networkName;

	// Check if the network already exists in the "saved_networks.txt" file
	if (FILEHANDLING::networkExisting(SAVED_NETWORKS, networkName)) {
		cout << endl << "\033[31mNetztwerk existiert bereits!\033[0m" << endl; 
		system("pause");
		return network;
	}

	vector<Neuron>inputLayer;
	vector<Neuron>outputLayer;
	network.push_back(inputLayer);
	network.push_back(outputLayer);

	network.at(0).resize(INPUTSIZE);

	int amountOfOutpus;
	cout << endl << "Wieviele Ausgaben soll das Netz haben? (min. 2, max. 30)" << endl;
	cout << endl << "Eingabe: "; 
	cin >> amountOfOutpus;

	ERRORHANDLING::checkUserInputForError();

	if (amountOfOutpus < MIN_OUTPUTNEURONS || amountOfOutpus > MAX_OUTPUTNEURONS) { throw ERRORHANDLING::error(1); }
	network.at(1).resize(amountOfOutpus);

	cout << endl;
	for (int i = 0; i < network.at(1).size(); i++) {
		string classification;
		cout << "Klassifiaktion von Output " << i << ": ";
		cin >> classification;
		network.at(1).at(i).setClassificationName(classification);
	}

	// First Neuron of the first Layer will contain the Network name
	network.at(0).at(0).setNetworkName(networkName);

	initializeNet(&network);
	NETWORKPROPERTIES::disableSavdFlag(&network);
	
	return network;
}

static void printNetworkInfo(vector<vector<Neuron>>* _network) {

	if (_network->size() > 0) {
		if (_network->at(0).at(0).getIsSaved() == true) {
			//cout << network.at(0).at(0).getNetworkName() << " - SAVED\t" << network.at(0).at(0).getAverageAccuracy() << "%" << endl << endl;
			cout << _network->at(0).at(0).getNetworkName() << " - SAVED" << endl << endl;
		}
		else if (_network->at(0).at(0).getIsSaved() == false) {
			//cout << network.at(0).at(0).getNetworkName() << " - NOT SAVED\t" << network.at(0).at(0).getAverageAccuracy() << "%" << endl << endl;
			cout << _network->at(0).at(0).getNetworkName() << " - NOT SAVED" << endl << endl;
		}
	}
}

int main() {

	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	vector<vector<Neuron>> network;

	int userInput;

	while (true) {
		system("cls");

		printNetworkInfo(&network);
		
		cout << "(1) Neues Netz erstellen" << endl;
		cout << "(2) Trainingsdaten erstellen" << endl;
		cout << "(3) Netz speichern" << endl;
		cout << "(4) Netz laden" << endl;
		cout << "(5) Netz trainieren" << endl;
		cout << "(6) Netz testen" << endl;
		cout << "(7) Netz auf alle Validierungsdaten testen" << endl;
		cout << "(8) Netz neu initialisieren" << endl;
		cout << "(9) Netz loeschen" << endl;
		cout << endl <<"(0) Ende" << endl;
		cout << endl << "Eingabe: "; 
		cin >> userInput;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		if (userInput == 1) {
			system("cls");
			try {
				FILEHANDLING::checkIfSaved(&network);
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
			
			try {
				system("cls");
				network = newNet();
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 2) {
			system("cls");
			try {
				FILEHANDLING::createTrainingdataSetup(&network);
				system("cls");
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 3) {
			try {
				FILEHANDLING::setupSave(&network);
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 4) {
			system("cls");
			try {
				FILEHANDLING::setupLoad(&network);
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 5) {
			system("cls");
			try {
				TRAINING::setupTraining(&network);
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 6) {
			system("cls");
			try {
				TEST::setupTest(&network);
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 7) {
			system("cls");
			try {
				TRAINING::processValidationManaul(&network);
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 8) {
			system("cls");
			string userInput_s;

			cout << endl << "Netz sicher neu initialisieren?(J/N)";
			cout << endl << "Eingabe: ";
			cin >> userInput_s;
			if (userInput_s == "J" || userInput_s == "j") {
				try {
					initializeNet(&network);
				}
				catch (const string& error) {
					cerr << error << endl;
					system("pause");
				}
			}
		}

		else if (userInput == 9) {
			system("cls");
			try {
				FILEHANDLING::setupDelete(&network);
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}

		else if (userInput == 0) {
			system("cls");
			try {
				if (FILEHANDLING::setupExit(&network) == 0) {
					return 0;
				}
			}
			catch (const string& error) {
				cerr << error << endl;
				system("pause");
			}
		}
	}
}