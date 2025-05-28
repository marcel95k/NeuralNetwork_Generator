#include "UI.h"

// PROCESSING
int UI::PROCESSING::processUserInputINT() {

	int userInput = -1;
	std::cin >> userInput;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		//return -1;
		throw NNG_Exception("Ungueltige Eingabe! Nur Zahlen eingeben!");
	}
	if (userInput < 0) {
		throw NNG_Exception("Ungueltige Eingabe! Wert darf nicht kleiner 0 sein!");
	}

	return userInput;
}

double UI::PROCESSING::processUserInputDOUBLE() {

	double userInput = -1;
	std::cin >> userInput;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		//return -1;
		throw NNG_Exception("Ungueltige Eingabe! Nur Zahlen eingeben!");
	}
	if (userInput < 0) {
		throw NNG_Exception("Ungueltige Eingabe! Wert darf nicht kleiner 0 sein!");
	}

	return userInput;
}

int UI::PROCESSING::processUserInputOnMenuOptions(const int _menuSize) {

	int userInput = -1;
	std::cin >> userInput;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw NNG_Exception("Ungueltige Eingabe!");
	}

	if (userInput < 0 || userInput > _menuSize) {
		throw NNG_Exception("Ungueltige Eingabe!");
	}
	return userInput;
}

MenuState UI::PROCESSING::menuLoop(const std::vector<std::string>& _menuList, const std::map<int, MenuState>& _menuActions, const Network& _network) {

	while (true) {
		UI::DISPLAY::displayMenuOptions(_menuList, _network);

		try {
			int chosenOption = UI::PROCESSING::processUserInputOnMenuOptions(_menuList.size());
			if (_menuActions.contains(chosenOption)) {
				return _menuActions.at(chosenOption);
			}
		}
		catch (const NNG_Exception& exception) {
			std::cerr << std::endl << exception.what() << std::endl;
			awaitAnyKey();
		}
	}
}

// DISPLAY
void UI::DISPLAY::displayMenuOptions(const std::vector<std::string> _menuList, const Network& _network) {

	clearScreen();
	std::cout << _network.getNetworkName() << std::endl << std::endl;
	for (int menuIndex = 0; menuIndex < _menuList.size(); menuIndex++) {
		std::cout << _menuList[menuIndex] << std::endl;
	}
	std::cout << std::endl << "Eingabe: ";
}

void UI::DISPLAY::displayNetworkInfo(const Network& _network) {

	if (_network.getNetworkSize() == 0) {
		throw NNG_Exception("Netz leer! Nichts zum anzeigen!");
	}

	Layer inpuLayer = _network.atLayer(0);
	Layer outputLayer = _network.atLayer(_network.getNetworkSize() - 1);
	std::vector<std::string> outputLabels = _network.getOutputLabels();

	clearScreen();
	std::cout << "Name: " << _network.getNetworkName() << std::endl << std::endl;
	std::cout << "Anzahl Schichten gesamt: " << _network.getNetworkSize() << std::endl << std::endl;

	std::cout << "Neuronen in Input Schicht: " << inpuLayer.getLayerSize() << std::endl;
	for (int i = 1; i < _network.getNetworkSize() - 1; i++){
		std::cout << "Neuronen in versteckter Schicht " << i << ": " << _network.atLayer(i).getLayerSize() << std::endl;
	}
	std::cout << "Neuronen in Output Schicht: " << outputLayer.getLayerSize() << std::endl << std::endl;
	
	for (int i = 0; i < outputLabels.size(); i++) {
		std::cout << "Klassifikation von Output " << i + 1 << ": " << outputLabels[i] << std::endl;
	}
	std::cout << std::endl;
	awaitAnyKey();
}

// QUERY
std::string UI::QUERY::userSetNetworkNameSave() {

	std::string tempNetworkName;
	std::cout << "Speichern als: ";
	std::cin >> tempNetworkName;
	return tempNetworkName;
}

std::string UI::QUERY::userSetNetworkNameLoad() {

	std::string tempNetworkName;
	std::cout << "Laden: ";
	std::cin >> tempNetworkName;
	return tempNetworkName;
}

int UI::QUERY::userSetAmountOfHiddenLayers() {

	int tempAmountOfHiddenLayers = -1;
	std::cout << "Anzahl versteckter Schichten: ";
	try {
		tempAmountOfHiddenLayers = UI::PROCESSING::processUserInputINT();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return -1;
	}

	return tempAmountOfHiddenLayers;
}

std::vector<int> UI::QUERY::userSetAmountOfHiddenNeurons(const int _amountOfHiddenLayers) {

	std::vector<int>tempTopology;
	int tempAmountOfHiddenNeurons = -1;

	std::cout << std::endl;
	for (int i = 0; i < _amountOfHiddenLayers; i++) {
		std::cout << "Anzahl Neuronen in Hidden Layer " << i + 1 << ": ";
		try {
			tempAmountOfHiddenNeurons = UI::PROCESSING::processUserInputINT();
			tempTopology.push_back(tempAmountOfHiddenNeurons);
		}
		catch (const NNG_Exception& exception) {
			tempTopology.clear();
			std::cerr << std::endl << exception.what() << std::endl;
			awaitAnyKey();
			return tempTopology;
		}
	}

	return tempTopology;
}

int UI::QUERY::userSetAmountOfOutputNeurons() {

	int tempAmountOfOutputNeurons = -1;
	std::cout << std::endl << "Anzahl Neuronen in Output Layer: ";
	try {
		tempAmountOfOutputNeurons = UI::PROCESSING::processUserInputINT();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return -1;
	}

	return tempAmountOfOutputNeurons;
}

void UI::QUERY::userSetOutputLabels(Network& _network) {

	std::cout << std::endl;
	int outputLayer = _network.getNetworkSize() - 1;
	std::vector<std::string>outputLabels;
	for (int i = 0; i < _network.atLayer(outputLayer).getLayerSize(); i++) {
		std::string tempOutputLabel;
		std::cout << "Klassifikation von Output " << i + 1 << ": ";
		std::cin >> tempOutputLabel;
		outputLabels.push_back(tempOutputLabel);
	}

	_network.setOutputLabels(outputLabels);
}

double UI::QUERY::userSetLearningrate(const double _max) {

	double tempLearningrate = -1;
	std::cout << "Lernrate (max. 0.9): ";
	try {
		tempLearningrate = UI::PROCESSING::processUserInputDOUBLE();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return -1;
	}

	if (tempLearningrate > _max) {
		tempLearningrate = -1;
		throw NNG_Exception("Lernrate zu hoch!");
	}

	return tempLearningrate;
}

double UI::QUERY::userSetEpsilonDecay(const double _max) {

	double tempEpsilonDecay = -1;
	std::cout << "Lernraten-Redutkion (max. 1): ";
	try {
		tempEpsilonDecay = UI::PROCESSING::processUserInputDOUBLE();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return -1;
	}

	if (tempEpsilonDecay > _max) {
		tempEpsilonDecay = -1;
		throw NNG_Exception("Lernraten-Redutkion zu hoch!");
	}

	return tempEpsilonDecay;
}

double UI::QUERY::userSetMomentumFactor(const double _max) {

	double tempMomentumFactor = -1;
	std::cout << "Momentum Faktor (max. 0.9): ";
	try {
		tempMomentumFactor = UI::PROCESSING::processUserInputDOUBLE();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return -1;
	}

	if (tempMomentumFactor > _max) {
		tempMomentumFactor = -1;
		throw NNG_Exception("Momentum Faktor zu hoch!");
	}

	return tempMomentumFactor;
}

int UI::QUERY::userSetEpochs() {

	int tempEpochs = -1;
	std::cout << "Epochen: ";
	try {
		tempEpochs = UI::PROCESSING::processUserInputINT();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return -1;
	}

	return tempEpochs;
}

// MENU
MenuState UI::MENU::mainMenu(const Network& _network) {;

	std::vector<std::string> menuList = {
		"(1) Neues Netz erstellen", "(2) Trainingsdaten erstellen",
		"(3) Netz speichern", "(4) Netz laden",
		"(5) Netz trainieren", "(6) Netz testen",
		"(7) Netz neu initialisieren", "(8) Netz Informationen anzeigen",
		"(9) Netz loeschen", "(0) Beenden"
	};

	std::map<int, MenuState> menuActions = {
		{1, MenuState::NEWNET}, {2, MenuState::CREATE_TRAININGDATA},
		{3, MenuState::SAVE}, {4, MenuState::LOAD},
		{5, MenuState::TRAINING}, {6, MenuState::TEST},
		{7, MenuState::INITIALIZE}, {8, MenuState::SHOW_NETWORKINFO},
		{9, MenuState::DELETE}, {0, MenuState::EXIT}
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
}

MenuState UI::MENU::newNetMenu(const Network& _network) {

	if (_network.getModifiedStatus() == true && _network.getSavedStatus() == false) {
		std::vector<std::string> menuList = {
		"(1) Zuerst speichern",
		"(2) Fortfahren (Neues Netz erstellen)",
		"(0) Zurueck"
		};

		std::map<int, MenuState> menuActions = {
		{1, MenuState::SAVE},
		{2, MenuState::SUB_NEWNET},
		{0, MenuState::MAIN}
		};

		return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
	}

	return MenuState::SUB_NEWNET;
}

MenuState UI::MENU::createTrainingdataMenu(Network& _network) {

	std::vector<std::string> menuList = { 
		"(1) Trainingsdaten", 
		"(2) Validierungsdaten", 
		"(0) Zurueck"
	};

	std::map<int, MenuState> menuActions = {
	{1, MenuState::SUB_CREATE_TRAININGDATA}, 
	{2, MenuState::SUB_CREATE_VALIDATIONDATA},
	{0, MenuState::MAIN}
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
}

MenuState UI::MENU::saveMenu(const Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeSaving(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}
	
	std::vector<std::string> menuList = {
	"(1) Speichern",
	"(2) Speichern als",
	"(0) Zurueck"
	};

	std::map<int, MenuState> menuActions = {
	{1, MenuState::SUB_SAVE},
	{2, MenuState::SUB_SAVE_AS},
	{0, MenuState::MAIN}
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
}

MenuState UI::MENU::loadMenu(const Network& _network) {

	if (_network.getModifiedStatus() == true) {
		std::vector<std::string> menuList = {
		"(1) Zuerst speichern",
		"(2) Fortfahren zum Laden",
		"(0) Zurueck"
		};

		std::map<int, MenuState> menuActions = {
		{1, MenuState::SAVE},
		{2, MenuState::SUB_LOAD},
		{0, MenuState::MAIN}
		};

		return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
	}

	return MenuState::SUB_LOAD;
}

MenuState UI::MENU::trainingMenu(Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeTraining(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}

	std::vector<std::string> menuList = {
	"(1) Standard Training",
	"(2) Trainings Parameter selbst bestimmen",
	"(0) Zurueck"
	};

	std::map<int, MenuState> menuActions = {
	{1, MenuState::SUB_TRAIN_DEFAULT},
	{2, MenuState::SUB_TRAIN_CUSTOM},
	{0, MenuState::MAIN}
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network); 
}

MenuState UI::MENU::testMenu(Network& _network) {
	GRAPHICS::drawBarGraph(_network);
	GRAPHICS::drawTestImage(_network);
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN; 
}

MenuState UI::MENU::initializeMenu(const Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeInitializing(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}

	std::vector<std::string> menuList = {
	"(1) Abbruch",
	"(2) Weiter (Initialisieren)",
	};

	std::map<int, MenuState> menuActions = {
	{1, MenuState::MAIN},
	{2, MenuState::SUB_INITIALIZE},
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
}

MenuState UI::MENU::showNetworkInfoMenu(const Network& _network){
	
	try {
		UI::DISPLAY::displayNetworkInfo(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}

	return MenuState::MAIN;
}

MenuState UI::MENU::deleteMenu(const Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeDeleting(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		_getch();
		return MenuState::MAIN;
	}

	std::vector<std::string> menuList = {
	"(1) Abbruch",
	"(2) Weiter (Loeschen)",
	};

	std::map<int, MenuState> menuActions = {
	{1, MenuState::MAIN},
	{2, MenuState::SUB_DELETE},
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
}

MenuState UI::MENU::exitMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

// SUB MENUS
MenuState UI::MENU::NEWNET::SUBNewNetMenu(Network& _network) {

	clearScreen();
	int amountOfHiddenLayers = UI::QUERY::userSetAmountOfHiddenLayers();
	if (amountOfHiddenLayers == -1) {
		return MenuState::MAIN;
	}

	std::vector<int>topology = UI::QUERY::userSetAmountOfHiddenNeurons(amountOfHiddenLayers);
	if (topology.size() < 0) {
		return MenuState::MAIN;
	}

	int amountOfOutputNeurons = UI::QUERY::userSetAmountOfOutputNeurons();
	if (amountOfOutputNeurons == -1) {
		return MenuState::MAIN;
	}
	topology.push_back(amountOfOutputNeurons);

	try {
		_network = NETWORKHANDLER::NEWNET::buildNewNet(amountOfHiddenLayers, topology);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}
	UI::QUERY::userSetOutputLabels(_network);

	_network.setModifiedStatus(true);

	return MenuState::MAIN;
}

MenuState UI::MENU::CREATETRAININGDATA::SUBcreateTrainingdataMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::CREATETRAININGDATA::SUBcreateValidationdataMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::SAVE::SUBSaveNetworkMenu(Network& _network) {

	if (_network.getNetworkSize() == 0) {
		std::cout << std::endl << "Netz leer! Speichern fehlgeschlagen!" << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}

	if (_network.getSavedStatus() == false) {
		return MenuState::SUB_SAVE_AS;
	}

	try {
		NETWORKHANDLER::DATAMANAGEMENT::saveNewNetwork(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
	}

	return MenuState::MAIN;
}

MenuState UI::MENU::SAVE::SUBSaveNetworkAsMenu(Network& _network) {

	if (_network.getNetworkSize() == 0) {
		std::cout << std::endl << "Netz leer! Speichern fehlgeschlagen!" << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}

	clearScreen();
	FILEHANDLING::displaySavedNetworks();

	std::string networkName = UI::QUERY::userSetNetworkNameSave();
	_network.setNetworkName(networkName);

	try {
		NETWORKHANDLER::DATAMANAGEMENT::saveNewNetworkAs(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
	}

	return MenuState::MAIN;
}

MenuState UI::MENU::LOAD::SUBLoadNetworkMenu(Network& _network) {

	clearScreen();
	FILEHANDLING::displaySavedNetworks();

	std::string networkName = UI::QUERY::userSetNetworkNameLoad();

	try {
		NETWORKHANDLER::DATAMANAGEMENT::loadNetwork(_network, networkName);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
	}

	return MenuState::MAIN;
}

MenuState UI::MENU::TRAIN::SUBDefaultTrainingMenu(Network& _network) {

	Trainer trainer(DEFAULT_EPSILON, DEFAULT_EPSILON_DECAY, DEFAULT_MOMENTUMFACTOR, DEFAULT_EPOCHS);

	try {
		trainer.trainNetwork(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}
	return MenuState::MAIN;
}

MenuState UI::MENU::TRAIN::SUBCustomTrainingMenu(Network& _network) {

	double learningRate;
	double epsilonDecay;
	double momentumFactor;

	clearScreen();
	try {
		learningRate = UI::QUERY::userSetLearningrate(MAX_LEARNINGRATE);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}
	if (learningRate == -1) {
		return MenuState::MAIN;
	}

	try {
		epsilonDecay = UI::QUERY::userSetEpsilonDecay(MAX_EPSILON_DECAY);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}
	if (epsilonDecay == -1) {
		return MenuState::MAIN;
	}

	try {
		momentumFactor = UI::QUERY::userSetMomentumFactor(MAX_MOMENTUMFACTOR);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}
	if (momentumFactor == -1) {
		return MenuState::MAIN;
	}

	int epochs = UI::QUERY::userSetEpochs();

	Trainer trainer(learningRate, epsilonDecay, momentumFactor, epochs);

	trainer.trainNetwork(_network);
}

MenuState UI::MENU::TEST::SUBTestMenu(Network& _network) {
	return MenuState::MAIN;
}

MenuState UI::MENU::INITIALIZE::SUBInitializeMenu(Network& _network) {

	_network.randomize();

	std::cout << std::endl << "Netzwerk neu initialisiert!" << std::endl;
	awaitAnyKey();

	return MenuState::MAIN;
}

MenuState UI::MENU::DELETE::SUBDeleteMenu(Network& _network) {

	try {
		NETWORKHANDLER::DATAMANAGEMENT::deleteNetwork(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
	}

	return MenuState::MAIN;
}

