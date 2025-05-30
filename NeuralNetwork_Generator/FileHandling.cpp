#include "FileHandling.h"



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

bool FILEHANDLING::isEmptySpace(const std::string& _line) {
	return std::all_of(_line.begin(), _line.end(), [](char c) { return std::isspace(c); });
}
//
//void FILEHANDLING::writeGrayscaleToFile(const std::string _mainFolder, std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter, cv::Mat _centered) {
//
//	std::ofstream outFile(_mainFolder + NETWORKPROPERTIES::getNetworkName(_network) + "/" + _network->at(1).at(_classification).getClassificationName() + "/" + _network->at(1).at(_classification).getClassificationName() + "_" + std::to_string(_counter) + ".txt");
//	if (outFile.is_open()) {
//		for (int i = 0; i < _centered.rows; ++i) {
//			for (int j = 0; j < _centered.cols; ++j) {
//				double value = _centered.at<uchar>(i, j) / 255.0f;
//				outFile << value << std::endl;;
//			}
//		}
//		outFile.close();
//	}
//
//	else {
//		throw(ERRORHANDLING::error(3));
//	}
//}
//
//int FILEHANDLING::createValidationdataLoop(std::vector<std::vector<Neuron>>* _network, const int _amountOfIndividualSamples) {
//
//	int classification = 0;
//	int counter = 0;
//	while (classification < _network->at(1).size()) {
//		int status = GRAPHICS::drawValidationdata(_network, counter, classification);
//		if (GRAPHICS::processKeyPressValidation(status, counter, classification, _amountOfIndividualSamples) == 1) {
//			FILEHANDLING::deleteValidationFolders(_network);
//			NETWORKPROPERTIES::disableValidationFlag(_network);
//			destroyWindow("ZeichenfensterValidation");
//			return 1;
//		}
//	}
//	return 0;
//}
//
//
void FILEHANDLING::createNewValidationdataFolder(const Network& _network) {

	// Create the main folder and name it after the name of the network.
	std::string validationdataFolderName = "Networks/" + _network.getNetworkName() + "/Validationdata";
	fs::create_directories(validationdataFolderName);
}

void FILEHANDLING::createNewTrainingdataFolder(const Network& _network) {

	// Create the main folder and name it after the name of the network.
	std::string trainingdataFolderName = "Networks/" + _network.getNetworkName() + "/Trainingdata";
	fs::create_directories(trainingdataFolderName);
}

void FILEHANDLING::createNewNetworkFolder(const Network& _network) {

	// Create the main folder and name it after the name of the network.
	std::string mainFolder = "Networks/" + _network.getNetworkName();
	fs::create_directories(mainFolder);
}
//
//void FILEHANDLING::createValidationdataSetup(std::vector<std::vector<Neuron>>* _network) {
//	
//	ERRORHANDLING::checkNetForError(8, _network);
//	
//	std::string userInput_s;
//	std::cout << "Validierungsdaten ertsellen? (J/N)" << std::endl;
//	std::cout << "Eingabe: ";
//	std::cin >> userInput_s;
//
//	if (userInput_s == "J" || userInput_s == "j") {
//		
//		FILEHANDLING::deleteValidationFolders(_network);
//		FILEHANDLING::createNewValdiationFolders(_network);
//
//		// 30% of individual classifications will be the amount of individual validation classifications
//		int amountOfIndividualSamples = (NETWORKPROPERTIES::getIndividualSamples(_network) * VALIDATION_SHARE) / 100;	
//
//		if (FILEHANDLING::createValidationdataLoop(_network, amountOfIndividualSamples) == 1) { return; }
//
//		if (cv::getWindowProperty("ZeichenfensterValidation", cv::WND_PROP_VISIBLE) >= 0) { destroyWindow("ZeichenfensterValidation"); }
//
//		NETWORKPROPERTIES::enableValidationFlag(_network);
//	}
//	else if (userInput_s == "N" || userInput_s == "n") {
//		NETWORKPROPERTIES::disableValidationFlag(_network);
//	}
//	else { 
//		NETWORKPROPERTIES::disableValidationFlag(_network);
//		throw ERRORHANDLING::error(1); 
//	}
//}
//
//int FILEHANDLING::createTrainingdataLoop(std::vector<std::vector<Neuron>>* _network, const int _amountOfIndividualSamples) {
//
//	int classification = 0;
//	int counter = 0;
//	while (classification < _network->at(1).size()) {
//		int status = GRAPHICS::drawTrainingdata(_network, counter, classification);
//		if (GRAPHICS::processKeyPressTraining(status, counter, classification, _amountOfIndividualSamples) == 1) {
//			NETWORKPROPERTIES::setIndividualSamples(_network, 0);
//			FILEHANDLING::deleteTrainingFolders(_network);
//			destroyWindow("Zeichenfenster");
//			return 1;
//		}
//	}
//	return 0;
//}
//
//
//void FILEHANDLING::createNewTrainingFolders(std::vector<std::vector<Neuron>>* _network) {
//
//	// Create the main folder and name it after the name of the network.
//	std::string mainFolder = TRAININGDATA + NETWORKPROPERTIES::getNetworkName(_network);
//	fs::create_directories(mainFolder);
//
//	// Create the folders for each classification
//	for (int i = 0; i < _network->at(1).size(); i++) {
//		std::string classificationFolder = mainFolder + "/" + _network->at(1).at(i).getClassificationName();
//		fs::create_directories(classificationFolder);
//	}
//}
//
//void FILEHANDLING::createTrainingdataSetup(std::vector<std::vector<Neuron>>* _network) {
//
//	ERRORHANDLING::checkNetForError(8, _network);
//
//	int amountOfIndividualSamples;
//	std::cout << std::endl << "Wieviele individuelle Einheiten pro Klassifikation? (min. 10, max. 80)" << std::endl;
//	std::cout << std::endl << "Eingabe: ";
//	std::cin >> amountOfIndividualSamples;
//
//	ERRORHANDLING::checkUserInputForError();
//
//	if (amountOfIndividualSamples > 80 || amountOfIndividualSamples < 10) { throw ERRORHANDLING::error(1); }
//
//	FILEHANDLING::deleteTrainingFolders(_network);
//	FILEHANDLING::createNewTrainingFolders(_network);
//
//	// First Neuron of the first Layer will contain the information about how many individual entities of a classification exist
//	NETWORKPROPERTIES::setIndividualSamples(_network, amountOfIndividualSamples);
//
//	if (FILEHANDLING::createTrainingdataLoop(_network, amountOfIndividualSamples) == 1) { return; }
//	if (cv::getWindowProperty("Zeichenfenster", cv::WND_PROP_VISIBLE) >= 0) { destroyWindow("Zeichenfenster"); }
//	NETWORKPROPERTIES::disableTrainedFlag(_network);
//
//	//// First Neuron of the first Layer will contain the information about how many individual entities of a classification exist --> AGAIN if createTrainingdataLoop resets it
//	//_network->at(0).at(0).setIndividualClassifications(amountOfIndividualClassifications);
//	
//	clearScreen();
//	try {
//		FILEHANDLING::createValidationdataSetup(_network);
//	}
//	catch (const std::string& error) {
//		std::cerr << error << std::endl;
//		awaitAnyKey();
//	}
//	NETWORKPROPERTIES::disableSavedFlag(_network);
//}
//
//void FILEHANDLING::saveNeuronLayer(const std::vector<Neuron>& _layer, const std::string& _filename) {
//
//	std::ofstream out(_filename, std::ios::binary);
//	if (!out) {
//		throw ERRORHANDLING::error(3);
//	}
//
//	size_t size = _layer.size();
//	out.write(reinterpret_cast<const char*>(&size), sizeof(size));
//	for (const Neuron& n : _layer) {
//		n.save(out);
//	}
//}
//
//std::vector<Neuron> FILEHANDLING::loadNeuronLayer(const std::string& _filename) {
//
//	std::vector<Neuron> layer;
//	std::ifstream in(_filename, std::ios::binary);
//	if (!in) {
//		throw ERRORHANDLING::error(3);
//	}
//
//	size_t size;
//	in.read(reinterpret_cast<char*>(&size), sizeof(size));
//	for (size_t i = 0; i < size; ++i) {
//		Neuron n;
//		n.load(in);
//		layer.push_back(n);
//	}
//	return layer;
//}
//
void FILEHANDLING::removeEmptySpacesfromNetworkList() {

	std::ifstream inputFile(SAVED_NETWORKS);
	std::ofstream outputFIle(SAVED_NETWORKS);

	if (!inputFile || !outputFIle) {
		throw NNG_Exception("Datei nicht gefunden: " SAVED_NETWORKS);
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		if (!FILEHANDLING::isEmptySpace(line)) {
			outputFIle << line << '\n';
		}
	}

	inputFile.close();
	outputFIle.close();
}

std::vector<std::string> FILEHANDLING::getNetworkList() {

	try {
	//	FILEHANDLING::removeEmptySpacesfromNetworkList();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		UI::DISPLAY::displayNotificationOpenCV(msg);
		//awaitAnyKey();
	}

	std::vector<std::string>tempNetworkList;
	std::ifstream savedNetworks(SAVED_NETWORKS);

	if (!savedNetworks) {
		throw NNG_Exception("Datei nicht gefunden: " SAVED_NETWORKS);
	}

	std::string content;
	while (std::getline(savedNetworks, content)) {
		tempNetworkList.push_back(content);
	}
	savedNetworks.close();

	return tempNetworkList;
}

void FILEHANDLING::displaySavedNetworks() {

	std::ifstream savedNetworks(SAVED_NETWORKS);

	if (!savedNetworks) {
		throw NNG_Exception("Datei nicht gefunden: " SAVED_NETWORKS);
	}

	std::string content;
	while (std::getline(savedNetworks, content)) {
		std::cout << content << std::endl;
	}
	savedNetworks.close();
	std::cout << std::endl;
}
//
//int FILEHANDLING::checkIfSaved(std::vector<std::vector<Neuron>>* _network) {
//
//	std::string userInput_s;
//
//	if (_network->size() > 0) {
//
//		if (NETWORKPROPERTIES::getSavedFlag(_network) == false) {
//			std::cout << std::endl << "Netz zuerst speichern?(J/N)";
//			std::cout << std::endl << "Eingabe: ";
//			std::cin >> userInput_s;
//			if (userInput_s == "J" || userInput_s == "j") {
//				try {
//					FILEHANDLING::setupSave(_network);
//				}
//				catch (const std::string& error) {
//					std::cerr << error << std::endl;
//					awaitAnyKey();
//					return 1;
//				}
//				return 0;
//			}
//			else if (userInput_s == "N" || userInput_s == "n") {
//				return 0;
//			}
//			else {
//				throw ERRORHANDLING::error(1);
//			}
//		}
//	}
//	return 0;
//}
//
//void FILEHANDLING::saveNet(std::vector<std::vector<Neuron>>* _network) {
//
//	NETWORKPROPERTIES::enableSavedFlag(_network);
//
//	for (int i = 0; i < _network->size(); i++) {
//		try {
//			FILEHANDLING::saveNeuronLayer(_network->at(i), NETWORKS + NETWORKPROPERTIES::getNetworkName(_network) + "_" + std::to_string(i) + ".dat");
//		}
//		catch (const std::string& error) {
//			std::cerr << error << std::endl;
//			awaitAnyKey();
//		}
//	}
//
//	std::ofstream savedNetworks(SAVED_NETWORKS, std::ios::app);
//
//	if (!savedNetworks) {
//		throw ERRORHANDLING::error(3);
//	}

void FILEHANDLING::addNetworkToList(const Network& _network) {

	std::ofstream savedNetworks(SAVED_NETWORKS, std::ios::app);
	
	if (!savedNetworks) {
		throw NNG_Exception("Datei nicht gefunden: " SAVED_NETWORKS);
	}

	savedNetworks << _network.getNetworkName() << std::endl;
	savedNetworks.close();
}
//	// Check if the network already exists in the "saved_networks.txt" file and write it if not
//	if (!networkExisting(SAVED_NETWORKS, NETWORKPROPERTIES::getNetworkName(_network))) {
//		savedNetworks << NETWORKPROPERTIES::getNetworkName(_network) << std::endl;
//		savedNetworks.close();
//	}
//}
//void FILEHANDLING::setupSave(std::vector<std::vector<Neuron>>* _network) {
//	
//	ERRORHANDLING::checkNetForError(2, _network);
//
//	try {
//		saveNet(_network);
//	}
//	catch (const std::string& error) {
//		std::cerr << error << std::endl;
//		awaitAnyKey();
//	}
//}
//
//void FILEHANDLING::loadNet(std::vector<std::vector<Neuron>>* _network) {
//
//	clearScreen();
//
//	std::vector<Neuron>inputLayer;
//	std::vector<Neuron>outputLayer;
//
//	std::string userInput_s;
//	
//	try {
//		FILEHANDLING::displaySavedNetworks();
//	}
//	catch (const std::string& error) {
//		std::cerr << error << std::endl;
//		awaitAnyKey();
//	}
//
//	std::cout << std::endl << "Laden: ";
//	std::cin >> userInput_s;
//	
//	// Check if the network exists in the "saved_networks.txt" file and throw and exception if not
//	if (!FILEHANDLING::networkExisting(SAVED_NETWORKS, userInput_s)) {
//		throw ERRORHANDLING::error(3);
//	}
//
//	_network->clear(); // Resetting the network
//
//	try {
//		inputLayer = FILEHANDLING::loadNeuronLayer(NETWORKS + userInput_s + "_0.dat");
//		outputLayer = FILEHANDLING::loadNeuronLayer(NETWORKS + userInput_s + "_1.dat");
//		_network->push_back(inputLayer);
//		_network->push_back(outputLayer);
//	}
//	catch (const std::string& error)
//	{
//		std::cerr << error << std::endl;
//		awaitAnyKey();
//	}
//}
//
//void FILEHANDLING::setupLoad(std::vector<std::vector<Neuron>>* _network) {
//
//	FILEHANDLING::checkIfSaved(_network);
//	
//	try {
//		FILEHANDLING::loadNet(_network);
//	}
//	catch (const std::string& error) {
//		std::cerr << error << std::endl;
//		awaitAnyKey();
//	}
//}
//
//void FILEHANDLING::deleteValidationFolders(std::vector<std::vector<Neuron>>* _network) {
//
//	std::string folderNameValidation = VALIDATIONDATA + NETWORKPROPERTIES::getNetworkName(_network);
//
//	std::error_code ecValidation;
//	std::uintmax_t amountValidation = std::filesystem::remove_all(folderNameValidation, ecValidation);
//}
//
//void FILEHANDLING::deleteTrainingFolders(std::vector<std::vector<Neuron>>* _network) {
//
//	std::string folderNameTraining = TRAININGDATA + NETWORKPROPERTIES::getNetworkName(_network);
//
//	std::error_code ecTraining;
//	std::uintmax_t amountTraining = std::filesystem::remove_all(folderNameTraining, ecTraining);
//}
//
void FILEHANDLING::removeNetworkFromList(Network& _network) {

	std::string filenameTXT = SAVED_NETWORKS;
	std::ifstream infile(filenameTXT);
	std::ofstream tempFile("temp.txt");
	std::string line;

	if (!infile || !tempFile) {
		throw NNG_Exception("Datei nicht gefunden!");
	}

	if (FILEHANDLING::networkExisting(SAVED_NETWORKS, _network.getNetworkName()) == false) {
		infile.close();
		tempFile.close();
		throw NNG_Exception("Netzwerk nicht in "  + filenameTXT + " gefunden!");
	}

	while (std::getline(infile, line)) {
		if (line != _network.getNetworkName()) {
			tempFile << line << '\n';
		}
	}

	infile.close();
	tempFile.close();

	if (std::remove(filenameTXT.c_str()) != 0) {
		throw NNG_Exception("Netzwerk nicht gefunden!");
	}
	if (std::rename("temp.txt", filenameTXT.c_str()) != 0) {
		throw NNG_Exception("Netzwerk nicht gefunden!");
	}
}

void FILEHANDLING::deleteFolders(Network& _network) {

	std::string folderName = NETWORKS + _network.getNetworkName();
	std::error_code ec;

	std::uintmax_t amountDeleted = std::filesystem::remove_all(folderName, ec);

	if (ec) {
		throw NNG_Exception("Fehler beim Loeschen des Ordners: " + ec.message());
	}
}

//
//void FILEHANDLING::setupDelete(std::vector<std::vector<Neuron>>* _network) {
//
//	ERRORHANDLING::checkNetForError(6, _network);
//
//	clearScreen();
//	std::string userInput_s;
//	std::cout << std::endl << "Netz sicher loeschen?(J/N)";
//	std::cout << std::endl << "Eingabe: ";
//	std::cin >> userInput_s;
//
//	if (userInput_s == "J" || userInput_s == "j") {
//		try {
//			FILEHANDLING::deleteNet(_network);
//		}
//		catch (const std::string& error) {
//			std::cerr << error << std::endl;
//			awaitAnyKey();
//		}
//	}
//}
//
//int FILEHANDLING::setupExit(std::vector<std::vector<Neuron>>* _network) {
//	
//	// Check if a network exists
//	if (_network->size() > 0) {
//		if (FILEHANDLING::checkIfSaved(_network) == 1) {
//			return 1;
//		}
//		else {
//			if (FILEHANDLING::networkExisting(SAVED_NETWORKS, NETWORKPROPERTIES::getNetworkName(_network)) == true) {
//				return 0;
//			}
//			else if (FILEHANDLING::networkExisting(SAVED_NETWORKS, NETWORKPROPERTIES::getNetworkName(_network)) == false) {
//				FILEHANDLING::deleteFolders(_network);
//				return 0;
//			}
//		}
//	}
//	return 1;
//}
//
std::string FILEHANDLING::getImageFilePath(const std::string _mainFolder, const Network& _network, const int _classification, const int _sampleCounter) {

	return _mainFolder +
		_network.getNetworkName() + "/" + TRAININGDATA + "/" +
		_network.getOutputLabelAt(_classification) + "/" +
		_network.getOutputLabelAt(_classification) + "_" + 
		std::to_string(_sampleCounter) + ".txt";
}

void FILEHANDLING::fillGrayValues(std::ifstream& _inFile, std::vector<double>& _inputValues) {

	std::string line;
	while (getline(_inFile, line)) {
		std::stringstream ss(line);
		double val;
		while (ss >> val) {
			_inputValues.push_back(val);
		}
	}
}

//void FILEHANDLING::loadValidationIMG(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _grayValues, const int _classification, const int _counter) {
//
//	std::string filename = FILEHANDLING::getImageFilePath(VALIDATIONDATA, _network, _classification, _counter);
//	std::ifstream inFile(filename);
//
//	if (!inFile.is_open()) {
//		throw ERRORHANDLING::error(3);
//	}
//
//	FILEHANDLING::fillGrayValues(inFile, _grayValues);
//
//#ifdef DEBUG_SHOW_VALIDATIONFILENAME
//	std::cout << "Validating " << filename << std::endl;
//#endif // DEBUG_SHOW_VALIDATIONFILENAME
//
//	inFile.close();
//}
//
void FILEHANDLING::loadTrainingIMG(const Network& _network, std::vector<double>& _inputValues, const int _classification, const int _sampleCounter) {

	std::string filename = FILEHANDLING::getImageFilePath(NETWORKS, _network, _classification, _sampleCounter);
	std::ifstream inFile(filename);

	if (!inFile.is_open()) {
		throw NNG_Exception("Datei nicht gefunden: " + filename);
	}

	FILEHANDLING::fillGrayValues(inFile, _inputValues);

#ifdef DEBUG_SHOW_TRAININGFILENAME
	std::cout << "Training " << filename << std::endl;
#endif // DEBUG_SHOW_TRAININGFILENAME

	inFile.close();
}
//
//
//
//
//
