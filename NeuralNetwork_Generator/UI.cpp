#include "UI.h"

// PROCESSING
int UI::PROCESSING::processUserInputINT() {

	int userInput = -1;
	std::cin >> userInput;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		//return -1;
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
			system("pause");
		}
	}
}

// DISPLAY
void UI::DISPLAY::displayMenuOptions(const std::vector<std::string> _menuList, const Network& _network) {

	system("cls");
	std::cout << _network.getNetworkName() << std::endl << std::endl;
	for (int menuIndex = 0; menuIndex < _menuList.size(); menuIndex++) {
		std::cout << _menuList[menuIndex] << std::endl;
	}
	std::cout << std::endl << "Eingabe: ";
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
	std::cout << "Anzahl verteckter Schichten: ";
	try {
		tempAmountOfHiddenLayers = UI::PROCESSING::processUserInputINT();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
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
			system("pause");
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
		system("pause");
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

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
}

MenuState UI::MENU::newNetMenu(Network& _network) {
	
	system("cls");	
	int amountOfHiddenLayers = UI::QUERY::userSetAmountOfHiddenLayers();
	if (amountOfHiddenLayers == -1) {
		return MenuState::MAIN;
	}

	std::vector<int>topology = UI::QUERY::userSetAmountOfHiddenNeurons(amountOfHiddenLayers);
	if (topology.size() == 0) {
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
		system("pause");
		return MenuState::MAIN;
	}
	UI::QUERY::userSetOutputLabels(_network);

	_network.setModifiedStatus(true);

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

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);
}

MenuState UI::MENU::saveMenu(Network& _network) {

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

MenuState UI::MENU::loadMenu(Network& _network) {

	system("cls");
	FILEHANDLING::displaySavedNetworks();

	std::string networkName = UI::QUERY::userSetNetworkNameLoad();

	try {
		_network = NETWORKHANDLER::DATAMANAGEMENT::loadNetwork(networkName);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
	}
	
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

// SUB MENUS

MenuState UI::MENU::CREATETRAININGDATA::SUBcreateTrainingdataMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::CREATETRAININGDATA::SUBcreateValidationdataMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::MENU::SAVE::SUBSaveNetwork(Network& _network) {

	if (_network.getNetworkSize() == 0) {
		std::cout << std::endl << "Netz leer! Speichern fehlgeschlagen!" << std::endl;
		system("pause");
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
		system("pause");
	}

	return MenuState::MAIN;
}

MenuState UI::MENU::SAVE::SUBSaveNetworkAs(Network& _network) {

	if (_network.getNetworkSize() == 0) {
		std::cout << std::endl << "Netz leer! Speichern fehlgeschlagen!" << std::endl;
		system("pause");
		return MenuState::MAIN;
	}

	system("cls");
	FILEHANDLING::displaySavedNetworks();

	std::string networkName = UI::QUERY::userSetNetworkNameSave();
	_network.setNetworkName(networkName);

	try {
		NETWORKHANDLER::DATAMANAGEMENT::saveNewNetworkAs(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		system("pause");
	}

	return MenuState::MAIN;
}

MenuState UI::MENU::LOAD::SUBLoadNetwork(Network& _network) {

	std::vector<std::string> menuList = {
	"(1) Zuerst speichern",
	"(2) Fortfahren zum Laden",
	"(0) Zurueck"
	};

	std::map<int, MenuState> menuActions = {
	{1, MenuState::SAVE},
	{2, MenuState::LOAD},
	{0, MenuState::MAIN}
	};

	return UI::PROCESSING::menuLoop(menuList, menuActions, _network);

}
