#include "FileHandling.h"

namespace fs = std::filesystem;


bool FILEHANDLING::networkExisting(const std::string& _filename, const std::string& _networkName) {

	std::ifstream file(_filename);
	std::string line;
	while (std::getline(file, line)) {
		if (cv::toLowerCase(line) == cv::toLowerCase(_networkName)) {
			return true; 
		}
	}
	return false;
}

void FILEHANDLING::writeGrayscaleToFile(const std::string _mainFolder, std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter, cv::Mat _centered) {

	std::ofstream outFile(_mainFolder + _network->at(0).at(0).getNetworkName() + "/" + _network->at(1).at(_classification).getClassificationName() + "/" + _network->at(1).at(_classification).getClassificationName() + "_" + std::to_string(_counter) + ".txt");
	if (outFile.is_open()) {
		for (int i = 0; i < _centered.rows; ++i) {
			for (int j = 0; j < _centered.cols; ++j) {
				float value = _centered.at<uchar>(i, j) / 255.0f;
				outFile << value << std::endl;;
			}
		}
		outFile.close();
	}

	else {
		std::cerr << "Konnte Datei nicht laden!" << std::endl;
	}
}

int FILEHANDLING::createValidationdataLoop(std::vector<std::vector<Neuron>>* _network, const int _amountOfIndividualClassifications) {

	int classification = 0;
	int counter = 0;
	while (classification < _network->at(1).size()) {
		int status = GRAPHICS::drawValidationdata(_network, counter, classification);
		if (GRAPHICS::processKeyPressValidation(status, counter, classification, _amountOfIndividualClassifications) == 1) {
			FILEHANDLING::deleteValidationFolders(_network);

			NETWORKPROPERTIES::disableValidationFlag(_network);

			destroyWindow("ZeichenfensterValidation");
			return 1;
		}
	}
}


void FILEHANDLING::createNewValdiationFolders(std::vector<std::vector<Neuron>>* _network) {

	// Create the main folder and name it after the name of the network.
	std::string mainFolder = VALIDATIONDATA + _network->at(0).at(0).getNetworkName();
	fs::create_directories(mainFolder);

	// Create the folders for each classification
	for (int i = 0; i < _network->at(1).size(); i++) {
		std::string classificationFolder = mainFolder + "/" + _network->at(1).at(i).getClassificationName();
		fs::create_directories(classificationFolder);
	}
}

void FILEHANDLING::createValidationdataSetup(std::vector<std::vector<Neuron>>* _network) {
	
	ERRORHANDLING::checkNetForError(8, _network);
	
	std::string userInput_s;
	std::cout << "Validierungsdaten ertsellen? (J/N)" << std::endl;
	std::cout << "Eingabe: ";
	std::cin >> userInput_s;

	if (userInput_s == "J" || userInput_s == "j") {
		
		FILEHANDLING::deleteValidationFolders(_network);
		FILEHANDLING::createNewValdiationFolders(_network);

		// 30% of individual classifications will be the amount of individual validation classifications
		int amountOfIndividualClassifications = (_network->at(0).at(0).getIndividualClassifications() * VALIDATION_SHARE) / 100;	

		if (FILEHANDLING::createValidationdataLoop(_network, amountOfIndividualClassifications) == 1) { return; }

		if (cv::getWindowProperty("ZeichenfensterValidation", cv::WND_PROP_VISIBLE) >= 0) { destroyWindow("ZeichenfensterValidation"); }

		NETWORKPROPERTIES::enableValidationFlag(_network);
	}
	else if (userInput_s == "N" || userInput_s == "n") {
		NETWORKPROPERTIES::disableValidationFlag(_network);
	}
	else { 
		NETWORKPROPERTIES::disableValidationFlag(_network);
		throw ERRORHANDLING::error(1); 
	}
}

int FILEHANDLING::createTrainingdataLoop(std::vector<std::vector<Neuron>>* _network, const int _amountOfIndividualClassifications) {

	int classification = 0;
	int counter = 0;
	while (classification < _network->at(1).size()) {
		int status = GRAPHICS::drawTrainingdata(_network, counter, classification);
		if (GRAPHICS::processKeyPressTraining(status, counter, classification, _amountOfIndividualClassifications) == 1) {
			//_network->at(0).at(0).setIndividualClassifications(0);
			FILEHANDLING::deleteTrainingFolders(_network);
			destroyWindow("Zeichenfenster");
			return 1;
		}
	}
}


void FILEHANDLING::createNewTrainingFolders(std::vector<std::vector<Neuron>>* _network) {

	// Create the main folder and name it after the name of the network.
	std::string mainFolder = TRAININGDATA + _network->at(0).at(0).getNetworkName();
	fs::create_directories(mainFolder);

	// Create the folders for each classification
	for (int i = 0; i < _network->at(1).size(); i++) {
		std::string classificationFolder = mainFolder + "/" + _network->at(1).at(i).getClassificationName();
		fs::create_directories(classificationFolder);
	}
}

void FILEHANDLING::createTrainingdataSetup(std::vector<std::vector<Neuron>>* _network) {

	ERRORHANDLING::checkNetForError(8, _network);

	int amountOfIndividualClassifications;
	std::cout << std::endl << "Wieviele individuelle Einheiten pro Klassifikation? (min. 10, max. 80)" << std::endl;
	std::cout << std::endl << "Eingabe: ";
	std::cin >> amountOfIndividualClassifications;

	ERRORHANDLING::checkUserInputForError();

	if (amountOfIndividualClassifications > 80 || amountOfIndividualClassifications < 10) { throw ERRORHANDLING::error(1); }

	FILEHANDLING::deleteTrainingFolders(_network);
	FILEHANDLING::createNewTrainingFolders(_network);

	// First Neuron of the first Layer will contain the information about how many individual entities of a classification exist
	_network->at(0).at(0).setIndividualClassifications(amountOfIndividualClassifications);

	if (FILEHANDLING::createTrainingdataLoop(_network, amountOfIndividualClassifications) == 1) { return; }

	if (cv::getWindowProperty("Zeichenfenster", cv::WND_PROP_VISIBLE) >= 0) { destroyWindow("Zeichenfenster"); }

	NETWORKPROPERTIES::disableTrainedFlag(_network);

	//// First Neuron of the first Layer will contain the information about how many individual entities of a classification exist --> AGAIN if createTrainingdataLoop resets it
	//_network->at(0).at(0).setIndividualClassifications(amountOfIndividualClassifications);
	
	system("cls");
	try {
		FILEHANDLING::createValidationdataSetup(_network);
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
		system("pause");
	}
	NETWORKPROPERTIES::disableSavdFlag(_network);
}

void FILEHANDLING::saveNeuronLayer(const std::vector<Neuron>& _layer, const std::string& _filename) {

	std::ofstream out(_filename, std::ios::binary);
	if (!out) {
		throw ERRORHANDLING::error(3);
	}

	size_t size = _layer.size();
	out.write(reinterpret_cast<const char*>(&size), sizeof(size));
	for (const Neuron& n : _layer) {
		n.save(out);
	}
}

std::vector<Neuron> FILEHANDLING::loadNeuronLayer(const std::string& _filename) {

	std::vector<Neuron> layer;
	std::ifstream in(_filename, std::ios::binary);
	if (!in) {
		throw ERRORHANDLING::error(3);
	}

	size_t size;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (size_t i = 0; i < size; ++i) {
		Neuron n;
		n.load(in);
		layer.push_back(n);
	}
	return layer;
}

void FILEHANDLING::displaySavedNetworks() {

	std::ifstream savedNetworks(SAVED_NETWORKS);

	if (!savedNetworks) {
		throw ERRORHANDLING::error(3);
	}

	std::string content;
	while (std::getline(savedNetworks, content)) {
		std::cout << content << std::endl;
	}

	savedNetworks.close();
}

int FILEHANDLING::checkIfSaved(std::vector<std::vector<Neuron>>* _network) {

	std::string userInput_s;

	if (_network->size() > 0) {

		if (_network->at(0).at(0).getIsSaved() == false) {
			std::cout << std::endl << "Netz zuerst speichern?(J/N)";
			std::cout << std::endl << "Eingabe: ";
			std::cin >> userInput_s;
			if (userInput_s == "J" || userInput_s == "j") {
				try {
					FILEHANDLING::setupSave(_network);
				}
				catch (const std::string& error) {
					std::cerr << error << std::endl;
					system("pause");
					return 1;
				}
				return 0;
			}
			else if(userInput_s == "N" || userInput_s == "n") {
				return 0;
			}
			else {
				throw ERRORHANDLING::error(1);
			}
		}
	}
}

void FILEHANDLING::saveNet(std::vector<std::vector<Neuron>>* _network) {

	NETWORKPROPERTIES::enableSavedFlag(_network);

	for (int i = 0; i < _network->size(); i++) {
		try {
			FILEHANDLING::saveNeuronLayer(_network->at(i), NETWORKS + _network->at(0).at(0).getNetworkName() + "_" + std::to_string(i) + ".dat");
		}
		catch (const std::string& error) {
			std::cerr << error << std::endl;
			system("pause");
		}
	}

	std::ofstream savedNetworks(SAVED_NETWORKS, std::ios::app);

	if (!savedNetworks) {
		throw ERRORHANDLING::error(3);
	}

	// Check if the network already exists in the "saved_networks.txt" file and write it if not
	if (!networkExisting(SAVED_NETWORKS, _network->at(0).at(0).getNetworkName())) {
		savedNetworks << _network->at(0).at(0).getNetworkName() << std::endl;
		savedNetworks.close();
	}
}
void FILEHANDLING::setupSave(std::vector<std::vector<Neuron>>* _network) {
	
	ERRORHANDLING::checkNetForError(2, _network);

	try {
		saveNet(_network);
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
		system("pause");
	}
}

void FILEHANDLING::loadNet(std::vector<std::vector<Neuron>>* _network) {

	system("cls");

	std::vector<Neuron>inputLayer;
	std::vector<Neuron>outputLayer;

	std::string userInput_s;
	
	try {
		FILEHANDLING::displaySavedNetworks();
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
		system("pause");
	}

	std::cout << std::endl << "Laden: ";
	std::cin >> userInput_s;
	
	// Check if the network exists in the "saved_networks.txt" file and throw and exception if not
	if (!FILEHANDLING::networkExisting(SAVED_NETWORKS, userInput_s)) {
		throw ERRORHANDLING::error(3);
	}

	_network->clear(); // Resetting the network

	try {
		inputLayer = FILEHANDLING::loadNeuronLayer(NETWORKS + userInput_s + "_0.dat");
		outputLayer = FILEHANDLING::loadNeuronLayer(NETWORKS + userInput_s + "_1.dat");
		_network->push_back(inputLayer);
		_network->push_back(outputLayer);
	}
	catch (const std::string& error)
	{
		std::cerr << error << std::endl;
		system("pause");
	}
}

void FILEHANDLING::setupLoad(std::vector<std::vector<Neuron>>* _network) {

	FILEHANDLING::checkIfSaved(_network);
	
	try {
		FILEHANDLING::loadNet(_network);
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
		system("pause");
	}
}

void FILEHANDLING::deleteValidationFolders(std::vector<std::vector<Neuron>>* _network) {

	std::string folderNameValidation = VALIDATIONDATA + _network->at(0).at(0).getNetworkName();

	std::error_code ecValidation;
	std::uintmax_t amountValidation = std::filesystem::remove_all(folderNameValidation, ecValidation);
}

void FILEHANDLING::deleteTrainingFolders(std::vector<std::vector<Neuron>>* _network) {

	std::string folderNameTraining = TRAININGDATA + _network->at(0).at(0).getNetworkName();

	std::error_code ecTraining;
	std::uintmax_t amountTraining = std::filesystem::remove_all(folderNameTraining, ecTraining);
}

void FILEHANDLING::deleteFolders(std::vector<std::vector<Neuron>>* _network) {

	std::string folderNameTraining = TRAININGDATA + _network->at(0).at(0).getNetworkName();

	std::error_code ecTraining;
	std::uintmax_t amountTraining = std::filesystem::remove_all(folderNameTraining, ecTraining);

	std::string folderNameValidation = VALIDATIONDATA + _network->at(0).at(0).getNetworkName();

	std::error_code ecValidation;
	std::uintmax_t amountValidation = std::filesystem::remove_all(folderNameValidation, ecValidation);
}

void FILEHANDLING::deleteNet(std::vector<std::vector<Neuron>>* _network) {
	
	for (int i = 0; i < _network->size(); i++) {
		std::string filenameDAT = NETWORKS+ _network->at(0).at(0).getNetworkName() + "_" + std::to_string(i) + ".dat";
		std::remove(filenameDAT.c_str());
	}

	std::string filenameTXT = SAVED_NETWORKS;
	std::ifstream infile(filenameTXT);
	std::ofstream tempFile("temp.txt");
	std::string line;

	if (!infile || !tempFile) {
		throw ERRORHANDLING::error(3);
	}

	while (std::getline(infile, line)) {
		if (line != _network->at(0).at(0).getNetworkName()) {
			tempFile << line << '\n';
		}
	}

	infile.close();
	tempFile.close();

	if (std::remove(filenameTXT.c_str()) != 0) {
		throw ERRORHANDLING::error(3);
	}
	if (std::rename("temp.txt", filenameTXT.c_str()) != 0) {
		throw ERRORHANDLING::error(3);
	}

	FILEHANDLING::deleteFolders(_network);

	_network->resize(0);
}

void FILEHANDLING::setupDelete(std::vector<std::vector<Neuron>>* _network) {

	ERRORHANDLING::checkNetForError(6, _network);

	system("cls");
	std::string userInput_s;
	std::cout << std::endl << "Netz sicher loeschen?(J/N)";
	std::cout << std::endl << "Eingabe: ";
	std::cin >> userInput_s;

	if (userInput_s == "J" || userInput_s == "j") {
		try {
			FILEHANDLING::deleteNet(_network);
		}
		catch (const std::string& error) {
			std::cerr << error << std::endl;
			system("pause");
		}
	}
}

int FILEHANDLING::setupExit(std::vector<std::vector<Neuron>>* _network) {
	
	// Check if a network exists
	if (_network->size() > 0) {
		if (FILEHANDLING::checkIfSaved(_network) == 1) {
			return 1;
		}
		else {
			if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _network->at(0).at(0).getNetworkName()) == true) {
				return 0;
			}
			else if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _network->at(0).at(0).getNetworkName()) == false) {
				FILEHANDLING::deleteFolders(_network);
				return 0;
			}
		}
	}
}

std::string FILEHANDLING::getImageFilePath(const std::string _mainFolder, std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter) {

	return _mainFolder +
		_network->at(0).at(0).getNetworkName() + "/" +
		_network->at(1).at(_classification).getClassificationName() + "/" +
		_network->at(1).at(_classification).getClassificationName() + "_" +
		std::to_string(_counter) + ".txt";
}

void FILEHANDLING::fillGrayValues(std::ifstream& _inFile, std::vector<float>* _grayValues) {

	std::string line;
	while (getline(_inFile, line)) {
		std::stringstream ss(line);
		double val;
		while (ss >> val) {
			_grayValues->push_back(val);
		}
	}
}

void FILEHANDLING::loadValidationIMG(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, const int _classification, const int _counter, double& totalAccuracy, int& totalTests) {

	std::string filename = FILEHANDLING::getImageFilePath(VALIDATIONDATA, _network, _classification, _counter);
	std::ifstream inFile(filename);

	if (!inFile.is_open()) {
		throw ERRORHANDLING::error(3);
	}

	FILEHANDLING::fillGrayValues(inFile, _grayValues);

#ifdef DEBUG_SHOW_VALIDATIONFILENAME
	std::cout << "Validating " << filename << std::endl;
#endif // DEBUG_SHOW_VALIDATIONFILENAME

	inFile.close();
}

void FILEHANDLING::loadTrainingIMG(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, const int _classification, const int _counter, const double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs) {

	std::string filename = FILEHANDLING::getImageFilePath(TRAININGDATA, _network, _classification, _counter);
	std::ifstream inFile(filename);

	if (!inFile.is_open()) {
		throw ERRORHANDLING::error(3);
	}

	FILEHANDLING::fillGrayValues(inFile, _grayValues);

#ifdef DEBUG_SHOW_TRAININGFILENAME
	std::cout << "Training " << filename << std::endl;
#endif // DEBUG_SHOW_TRAININGFILENAME

	inFile.close();
}





