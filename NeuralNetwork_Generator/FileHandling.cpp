#include "FileHandling.h"

namespace fs = std::filesystem;

bool networkExisting(const std::string& _filename, const std::string& _networkName) {

	std::ifstream file(_filename);
	std::string line;
	while (std::getline(file, line)) {
		if (line == _networkName) {
			return true; 
		}
	}
	return false;
}

void writeGrayscaleToFile(const std::string _mainFolder, std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter, cv::Mat _centered) {

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

void createNewValdiationFolders(std::vector<std::vector<Neuron>>* _network) {

	// Create the main folder and name it after the name of the network.
	std::string mainFolder = "Validationdata/" + _network->at(0).at(0).getNetworkName();
	fs::create_directories(mainFolder);

	// Create the folders for each classification
	for (int i = 0; i < _network->at(1).size(); i++) {
		std::string classificationFolder = mainFolder + "/" + _network->at(1).at(i).getClassificationName();
		fs::create_directories(classificationFolder);
	}
}

void createValidationdata(std::vector<std::vector<Neuron>>* _network) {
	
	checkNetForError(8, _network);
	
	std::string userInput_s;
	std::cout << "Validierungsdaten ertsellen? (J/N)" << std::endl;
	std::cout << "Eingabe: ";
	std::cin >> userInput_s;

	if (userInput_s == "J" || userInput_s == "j") {
		std::string folderName = "Validationdata/" + _network->at(0).at(0).getNetworkName();

		std::error_code ec;
		std::uintmax_t amount = std::filesystem::remove_all(folderName, ec);

		createNewValdiationFolders(_network);

		int u = 0;
		int i = 0;
		while (u < _network->at(1).size()) {
			int status = drawValidationdata(_network, i, u);

			if (status == 1) { // ESC was pressed
				destroyWindow("ZeichenfensterValidation");
				return;
			}
			else if (status == -1) {	// 'b' was pressed: back to previous individual or previous classification	
				if (i > 0) { i--; }
				else if (u > 0) {
					u--;
					i = 19;
				}
			}
			else {	// Continue to next individual
				i++;
				if (i == 20) {	// Continue to next classifiction when 20 individuals are reached
					u++;
					i = 0;
				}
			}
		}

		destroyWindow("ZeichenfensterValidation");

		// Marking the network as having validationdata by changing the information in the first Neuron of the first Layer
		_network->at(0).at(0).setHasValidationdata(true);
	}
}

void createNewTrainingFolders(std::vector<std::vector<Neuron>>* _network) {

	// Create the main folder and name it after the name of the network.
	std::string mainFolder = "Trainingdata/" + _network->at(0).at(0).getNetworkName();
	fs::create_directories(mainFolder);

	// Create the folders for each classification
	for (int i = 0; i < _network->at(1).size(); i++) {
		std::string classificationFolder = mainFolder + "/" + _network->at(1).at(i).getClassificationName();
		fs::create_directories(classificationFolder);
	}
}

void createTrainingdata(std::vector<std::vector<Neuron>>* _network) {

	checkNetForError(8, _network);

	std::string folderName = "Trainingdata/" + _network->at(0).at(0).getNetworkName();

	std::error_code ec;
	std::uintmax_t amount = std::filesystem::remove_all(folderName, ec);

	createNewTrainingFolders(_network);

	int amountOfIndividualClassifications;
	std::cout << std::endl << "Wieviele individuelle Einheiten pro Klassifikation? (min. 10, max. 80)" << std::endl;
	std::cout << std::endl << "Eingabe: ";
	std::cin >> amountOfIndividualClassifications;

	checkUserInputForError();

	if (amountOfIndividualClassifications > 80 || amountOfIndividualClassifications < 10) { throw error(1); }

	// First Neuron of the first Layer will contain the information about how many individual entities of a classification exist
	_network->at(0).at(0).setIndividualClassifications(amountOfIndividualClassifications);

	int u = 0;
	int i = 0;
	while (u < _network->at(1).size()) {
		int status = drawTrainingdata(_network, i, u);

		if (status == 1) { // ESC was pressed
			destroyWindow("Zeichenfenster");
			return;
		}
		else if (status == -1) {	// 'b' was pressed: back to previous individual or previous classification	
			if (i > 0) { i--; }
			else if (u > 0) {
				u--;
				i = amountOfIndividualClassifications - 1;
			}
		}
		else {	// Continue to next individual
			i++;
			if (i == amountOfIndividualClassifications) {	// Continue to next classifiction when amountOfIndividualClassifications is reached
				u++;
				i = 0;
			}
		}
	}

	destroyWindow("Zeichenfenster");

	// Marking the network as NOT trained by changing the information in the first Neuron of the first Layer
	_network->at(0).at(0).setIsTrained(false);
}

void saveNeuronLayer(const std::vector<Neuron>& _layer, const std::string& _filename) {

	std::ofstream out(_filename, std::ios::binary);
	if (!out) {
		throw error(3);
	}

	size_t size = _layer.size();
	out.write(reinterpret_cast<const char*>(&size), sizeof(size));
	for (const Neuron& n : _layer) {
		n.save(out);
	}
}

std::vector<Neuron> loadNeuronLayer(const std::string& _filename) {

	std::vector<Neuron> layer;
	std::ifstream in(_filename, std::ios::binary);
	if (!in) {
		throw error(3);
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

void checkIfSaved(std::vector<std::vector<Neuron>>* _network) {

	std::string userInput_s;

	if (_network->size() > 0) {

		if (_network->at(0).at(0).getIsSaved() == false) {
			std::cout << std::endl << "Netz zuerst speichern?(J/N)";
			std::cout << std::endl << "Eingabe: ";
			std::cin >> userInput_s;
			if (userInput_s == "J" || userInput_s == "j") {
				try {
					setupSave(_network);
				}
				catch (const std::string& error) {
					std::cerr << error << std::endl;
					system("pause");
				}
			}
			else {
				std::cin.clear();
				std::cin.ignore();
			}
		}
	}
}

void saveNet(std::vector<std::vector<Neuron>>* _network) {

	// Marking the network as saved by changing the information in the first Neuron of the first Layer
	_network->at(0).at(0).setIsSaved(true);

	for (int i = 0; i < _network->size(); i++) {
		try {
			saveNeuronLayer(_network->at(i), "Networks/" + _network->at(0).at(0).getNetworkName() + "_" + std::to_string(i) + ".dat");
		}
		catch (const std::string& error) {
			std::cerr << error << std::endl;
			system("pause");
		}
	}

	std::ofstream savedNetworks("Networks/saved_networks.txt", std::ios::app);

	if (!savedNetworks) {
		throw error(3);
	}

	// Check if the network already exists in the "saved_networks.txt" file and write it if not
	if (!networkExisting("Networks/saved_networks.txt", _network->at(0).at(0).getNetworkName())) {
		savedNetworks << _network->at(0).at(0).getNetworkName() << std::endl;
		savedNetworks.close();
	}
}
void setupSave(std::vector<std::vector<Neuron>>* _network) {
	
	checkNetForError(2, _network);

	try {
		saveNet(_network);
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
		system("pause");
	}
}

void loadNet(std::vector<std::vector<Neuron>>* _network) {

	system("cls");

	std::vector<Neuron>inputLayer;
	std::vector<Neuron>outputLayer;

	std::string userInput_s;
	std::ifstream savedNetworks("Networks/saved_networks.txt");

	if (!savedNetworks) {
		throw error(3);
	}

	std::string content;
	while (std::getline(savedNetworks, content)) {
		std::cout << content << std::endl;
	}

	savedNetworks.close();

	std::cout << std::endl << "Laden: ";
	std::cin >> userInput_s;
	
	// Check if the network exists in the "saved_networks.txt" file and throw and exception if not
	if (!networkExisting("Networks/saved_networks.txt", userInput_s)) {
		throw error(3);
	}

	_network->clear(); // Resetting the network

	try {
		inputLayer = loadNeuronLayer("Networks/" + userInput_s + "_0.dat");
		outputLayer = loadNeuronLayer("Networks/" + userInput_s + "_1.dat");
		_network->push_back(inputLayer);
		_network->push_back(outputLayer);
	}
	catch (const std::string& error)
	{
		std::cerr << error << std::endl;
		system("pause");
	}
}

void setupLoad(std::vector<std::vector<Neuron>>* _network) {

	checkIfSaved(_network);
	
	try {
		loadNet(_network);
	}
	catch (const std::string& error) {
		std::cerr << error << std::endl;
		system("pause");
	}
}

void deleteNet(std::vector<std::vector<Neuron>>* _network) {
	
	for (int i = 0; i < _network->size(); i++) {
		std::string filenameDAT = "Networks/" + _network->at(0).at(0).getNetworkName() + "_" + std::to_string(i) + ".dat";
		std::remove(filenameDAT.c_str());
	}

	std::string filenameTXT = "Networks/saved_networks.txt";
	std::ifstream infile(filenameTXT);
	std::ofstream tempFile("temp.txt");
	std::string line;

	if (!infile || !tempFile) {
		throw error(3);
	}

	while (std::getline(infile, line)) {
		if (line != _network->at(0).at(0).getNetworkName()) {
			tempFile << line << '\n';
		}
	}

	infile.close();
	tempFile.close();

	// Replace original
	std::remove(filenameTXT.c_str());
	std::rename("temp.txt", filenameTXT.c_str());

	std::string folderNameTraining = "Trainingdata/" + _network->at(0).at(0).getNetworkName();

	std::error_code ecTraining;
	std::uintmax_t amountTraining = std::filesystem::remove_all(folderNameTraining, ecTraining);

	std::string folderNameValidation = "Validationdata/" + _network->at(0).at(0).getNetworkName();

	std::error_code ecValidation;
	std::uintmax_t amountVaidation = std::filesystem::remove_all(folderNameValidation, ecValidation);

	_network->resize(0);
}

void setupDelete(std::vector<std::vector<Neuron>>* _network) {

	checkNetForError(6, _network);

	system("cls");
	std::string userInput_s;
	std::cout << std::endl << "Netz sicher loeschen?(J/N)";
	std::cout << std::endl << "Eingabe: ";
	std::cin >> userInput_s;
	if (userInput_s == "J" || userInput_s == "j") {
		try {
			deleteNet(_network);
		}
		catch (const std::string& error) {
			std::cerr << error << std::endl;
			system("pause");
		}
	}
}

void loadValidationIMG(std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter, double& totalAccuracy, int& totalTests) {

	std::vector<float> values;
	std::string filename = "Validationdata/" + _network->at(0).at(0).getNetworkName() + "/" + _network->at(1).at(_classification).getClassificationName() + "/" + _network->at(1).at(_classification).getClassificationName() + "_" + std::to_string(_counter) + ".txt";
	std::ifstream inFile(filename);

	if (!inFile.is_open()) {
		throw error(3);
	}

	std::string line;
	while (getline(inFile, line)) {
		std::stringstream ss(line);
		double val;
		while (ss >> val) {
			values.push_back(val);
		}
	}

	fitnessTest(_network, &values, _classification, totalAccuracy, totalTests);

	inFile.close();
	values.clear();
}

void loadTrainingIMG(std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter, const double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs) {

	std::vector<float> values;
	std::string filename = "Trainingdata/" + _network->at(0).at(0).getNetworkName() + "/" + _network->at(1).at(_classification).getClassificationName() + "/" + _network->at(1).at(_classification).getClassificationName() + "_" + std::to_string(_counter) + ".txt";

#ifdef DEBUG_SHOW_TRAININGFILE
	std::cout << "Training " << filename << std::endl;
#endif // DEBUG_SHOW_TRAININGFILE

	std::ifstream inFile(filename);

	if (!inFile.is_open()) {
		throw error(3);
	}

	std::string line;
	while (getline(inFile, line)) {
		std::stringstream ss(line);
		double val;
		while (ss >> val) {
			values.push_back(val);
		}
	}

	training(_network, &values, _classification, _epsilon, _epsilonDecay, _momentumFactor, _epochs);

	inFile.close();
	values.clear();
}





