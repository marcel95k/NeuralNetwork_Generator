#include "UI.h"

// PROCESSING
int UI::PROCESSING::processUserInputINT() {

	int userInput = -1;
	std::cin >> userInput;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw NNG_Exception("Ungueltige Eingabe!");
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

MenuState UI::PROCESSING::menuLoop(const std::vector<std::string>& _menuList, const std::map<int, MenuState>& _menuActions) {

	while (true) {
		UI::DISPLAY::displayMenuOptions(_menuList);

		try {
			int chosenOption = UI::PROCESSING::processUserInputOnMenuOptions(_menuList.size());
			if (_menuActions.contains(chosenOption)) {
				return _menuActions.at(chosenOption); 
			}
		}
		catch (const NNG_Exception& exception) {
			std::cerr << std::endl << exception.what() << std::endl;
			system("pause");
		}
	}
}

// DISPLAY
void UI::DISPLAY::displayMenuOptions(const std::vector<std::string> _menuList) {

	system("cls");
	for (int menuIndex = 0; menuIndex < _menuList.size(); menuIndex++) {
		std::cout << _menuList[menuIndex] << std::endl;
	}
	std::cout << std::endl << "Eingabe: ";
}

// QUERY
std::string UI::QUERY::userSetNetworkName() {

	std::string tempNetworkName;
	std::cout << "Speichern als: ";
	std::cin >> tempNetworkName;
	return tempNetworkName;
}

int UI::QUERY::userSetAmountOfHiddenLayers() {

	int tempAmountOfHiddenLayers = -1;
	std::cout << "Anzahl verteckter Schichten: ";
	try {
		tempAmountOfHiddenLayers = UI::PROCESSING::processUserInputINT();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
	}
	
	return tempAmountOfHiddenLayers;
}

int UI::QUERY::userSetAmountOfHiddenNeurons(const int _layerIndex) {

	int tempAmountOfHiddenNeurons = -1;
	std::cout << "Anzahl Neuronen in Hidden Layer " << _layerIndex + 1 << ": ";
	try {
		tempAmountOfHiddenNeurons = UI::PROCESSING::processUserInputINT();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
	}

	return tempAmountOfHiddenNeurons;
}

int UI::QUERY::userSetAmountOfOutputNeurons() {

	int tempAmountOfOutputNeurons = -1;
	std::cout << "Anzahl Neuronen in Output Layer: ";
	try {
		tempAmountOfOutputNeurons = UI::PROCESSING::processUserInputINT();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
	}

	return tempAmountOfOutputNeurons;
}

std::string UI::QUERY::userSetOutputLabels(const int _layerIndex) {

	std::string tempLabel;
	std::cout << "Klassifikation von Output " << _layerIndex + 1 << ": ";
	std::cin >> tempLabel;
	return tempLabel;
}

// MENU
MenuState UI::MENU::mainMenu(Network& _network) {;

	std::vector<std::string> menuList = {
		"(1) Neues Netz erstellen", "(2) Trainingsdaten erstellen",
		"(3) Netz speichern", "(4) Netz laden",
		"(5) Netz trainieren", "(6) Netz testen",
		"(7) Netz auf durschnittliche Genauigkeit testen", "(8) Netz neu initialisieren",
		"(9) Netz loeschen", "(0) Beenden"
	};

	std::map<int, MenuState> menuActions = {
		{1, MenuState::NEWNET}, {2, MenuState::CREATE_TRAININGDATA},
		{3, MenuState::SAVE}, {4, MenuState::LOAD},
		{5, MenuState::TRAINING}, {6, MenuState::TEST},
		{7, MenuState::VALIDATE}, {8, MenuState::INITIALIZE},
		{9, MenuState::DELETE}, {0, MenuState::EXIT}
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions);
}

MenuState UI::MENU::newNetMenu(Network& _network) {
	
	system("cls");
	int amountOfHiddenLayers = 0;

	if (amountOfHiddenLayers = UI::QUERY::userSetAmountOfHiddenLayers() == -1) {
		return MenuState::MAIN;
	}

	std::cout << std::endl;
	std::vector<int>topology;
	try {
		for (int i = 0; i < amountOfHiddenLayers; i++) {
			topology.push_back(UI::QUERY::userSetAmountOfHiddenNeurons(i));
		}
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
		return MenuState::MAIN;
	}
	
	std::cout << std::endl;
	try {
		topology.push_back(UI::QUERY::userSetAmountOfOutputNeurons());
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
		return MenuState::MAIN;
	}

	_network = NETWORKHANDLER::NEWNET::newNet(amountOfHiddenLayers, topology);

	return MenuState::MAIN;
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

	return UI::PROCESSING::menuLoop(menuList, menuActions);
}

MenuState UI::MENU::saveMenu(Network& _network) {

	if (_network.getNetworkSize() == 0) {
		std::cout << std::endl << "Netz leer! Speichern fehlgeschlagen!" << std::endl;
		system("pause");
		return MenuState::MAIN;
	}
	
	system("cls");
	FILEHANDLING::displaySavedNetworks();

	std::string networkName = UI::QUERY::userSetNetworkName();
	_network.setNetworkName(networkName);

	try {
		NETWORKHANDLER::DATAMANAGEMENT::networkSaver(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
	}
	
	return MenuState::MAIN;
}

MenuState UI::MENU::loadMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::trainingMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN; 
}

MenuState UI::MENU::testMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN; 
}

MenuState UI::MENU::validationCheckMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::initializeMenu(Network& _network){
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::deleteMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::exitMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
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
