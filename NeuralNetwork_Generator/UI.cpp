#include "UI.h"

std::string currentInput;
bool inputConfirmed = false;
bool inputActive = false;
cv::Rect inputBoxRect(50, 100, 200, 40);

// BUTTONS
std::vector<Button> BUTTONS::mainMenuButtons = {
	{{50, 50, 300, 40}, "Neues Netz erstellen", MenuState::NEWNET},
	{{50, 100, 300, 40}, "Trainingsdaten erstellen", MenuState::CREATE_TRAININGDATA},
	{{50, 150, 300, 40}, "Netz speichern", MenuState::SAVE},
	{{50, 200, 300, 40}, "Netz laden", MenuState::LOAD},
	{{50, 250, 300, 40}, "Netz trainieren", MenuState::TRAINING},
	{{50, 300, 300, 40}, "Netz testen", MenuState::TEST},
	{{50, 350, 300, 40}, "Initialisieren", MenuState::INITIALIZE},
	{{50, 400, 300, 40}, "Netz Info anzeigen", MenuState::SHOW_NETWORKINFO},
	{{50, 450, 300, 40}, "Netz loeschen", MenuState::DELETE},
	{{50, 500, 300, 40}, "Beenden", MenuState::EXIT}
};

std::vector<Button> BUTTONS::createTrainingdataMenuButtons = {
	{{50, 50, 300, 40}, "Trainingsdaten erstellen", MenuState::SUB_CREATE_TRAININGDATA},
	{{50, 100, 300,	40}, "Validierungsdaten erstellen", MenuState::SUB_CREATE_VALIDATIONDATA},
	{{50, 150, 300,	40}, "Zurueck", MenuState::MAIN},
};

std::vector<Button> BUTTONS::saveMenuButtons = {
	{{50, 50, 300, 40}, "Speichern", MenuState::SUB_SAVE},
	{{50, 100, 300,	40}, "Speichern als...", MenuState::SUB_SAVE_AS},
	{{50, 150, 300,	40}, "Zurueck", MenuState::MAIN},
};

std::vector<Button> BUTTONS::initializeMenuButtons = {
	{{50, 50, 300, 40}, "Abbrechen", MenuState::MAIN},
	{{50, 100, 300,	40}, "Weiter (Initialisieren)", MenuState::SUB_INITIALIZE},
};

std::vector<Button> BUTTONS::notificationMenuButtons = {
	{{50, 100, 300,	40}, "OK", MenuState::MAIN},
};

MenuState selectedAction = MenuState::MAIN;

std::vector<Button> *BUTTONS::activeButtons = nullptr;

// UI
void UI::onMouse(int event, int x, int y, int, void*) {

	if (event != cv::EVENT_LBUTTONDOWN || BUTTONS::activeButtons == nullptr) { return; }

	cv::Point clickPoint(x, y);
	for (const auto& btn : *BUTTONS::activeButtons) {
		if (btn.rect.contains(clickPoint)) {
			selectedAction = btn.action;
			break;
		}
	}
}

void UI::onTextboxMouse(int event, int x, int y, int, void*) {

	if (event == cv::EVENT_LBUTTONDOWN) {
		inputActive = inputBoxRect.contains(cv::Point(x, y));
	}
}

//U UI::MISC
void UI::MISC::makeTextboxBlink(const int& _framkecounter, bool& _blinkstate, cv::Scalar& _boxColor) {

	if (_framkecounter % 15 == 0) { // Switch color every 15 frames
		_blinkstate = !_blinkstate;
	}

	if (inputActive) {
		_boxColor = _blinkstate ? cv::Scalar(150, 150, 150) : cv::Scalar(100, 100, 100); 
	}
	else {
		_boxColor = cv::Scalar(100, 100, 100); 
	}
}

// UI::PROCESSING
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

// UI::QUERY
int UI::QUERY::promptUserForInteger_OpenCV(const std::string& _question) {

	currentInput = "";
	inputConfirmed = false;
	inputActive = false;

	cv::Mat inputWindow(200, 400, CV_8UC3, cv::Scalar(230, 230, 230));
	cv::namedWindow("Eingabe");
	cv::setMouseCallback("Eingabe", UI::onTextboxMouse);

	int frameCounter = 0;
	bool blinkState = false;
	cv::Scalar boxColor;

	while (!inputConfirmed) {
		cv::Mat display = inputWindow.clone();
		frameCounter++;

		UI::DISPLAY::displayPromptInWindow(display, _question);

		UI::MISC::makeTextboxBlink(frameCounter, blinkState, boxColor);

		cv::rectangle(display, inputBoxRect, boxColor, -1);
		cv::putText(display, currentInput, cv::Point(inputBoxRect.x + 5, inputBoxRect.y + 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);

		if (cv::getWindowProperty("Eingabe", cv::WND_PROP_VISIBLE) == 1) {
			cv::moveWindow("Eingabe", 700, 400);
			cv::imshow("Eingabe", display);
		}
		else if (cv::getWindowProperty("Eingabe", cv::WND_PROP_VISIBLE) < 1) {
			return -1;	// Return -1 by closing the window
		}
		
		int key = cv::waitKey(30);

		if (inputActive && key != -1) {
			if (key == 13 || key == 10) {  // Enter
				inputConfirmed = true;
			}
			else if (key == 8 && !currentInput.empty()) {  // Backspace
				currentInput.pop_back();
			}
			else if (key >= '0' && key <= '9') {
				currentInput += static_cast<char>(key);
			}
		}
	}

	if (cv::getWindowProperty("Eingabe", cv::WND_PROP_VISIBLE) == 1) {
		cv::destroyWindow("Eingabe");
	}
	if (currentInput == "") {
		currentInput = "0"; // currentInput will be changed to "0" when ENTER was pressed while the textbox was empty
	}

	return std::stoi(currentInput);
}

std::string UI::QUERY::promptUserForString_OpenCV(const std::string& _question) {

	currentInput = "";
	inputConfirmed = false;
	inputActive = false;

	cv::Mat inputWindow(200, 400, CV_8UC3, cv::Scalar(230, 230, 230));
	cv::namedWindow("Eingabe");
	cv::setMouseCallback("Eingabe", UI::onTextboxMouse);

	int frameCounter = 0;
	bool blinkState = false;
	cv::Scalar boxColor;

	while (!inputConfirmed) {
		cv::Mat display = inputWindow.clone();
		frameCounter++;

		UI::DISPLAY::displayPromptInWindow(display, _question);

		UI::MISC::makeTextboxBlink(frameCounter, blinkState, boxColor);

		cv::rectangle(display, inputBoxRect, boxColor, -1);
		cv::putText(display, currentInput, cv::Point(inputBoxRect.x + 5, inputBoxRect.y + 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);

		if (cv::getWindowProperty("Eingabe", cv::WND_PROP_VISIBLE) == 1) {
			cv::moveWindow("Eingabe", 700, 400);
			cv::imshow("Eingabe", display);
		}
		else if (cv::getWindowProperty("Eingabe", cv::WND_PROP_VISIBLE) < 1) {
			return "";	// By closing the window any empty string will be returned
		}

		int key = cv::waitKey(30);

		if (inputActive && key != -1) {
			if (key == 13 || key == 10) {  // Enter
				inputConfirmed = true;
			}
			
			else if (key == 8 && !currentInput.empty()) {  // Backspace
				currentInput.pop_back();
			}
			else if(key != 8) {
				currentInput += static_cast<char>(key);
			}
		}
	}

	if (cv::getWindowProperty("Eingabe", cv::WND_PROP_VISIBLE) == 1) {
		cv::destroyWindow("Eingabe");
	}
	if (currentInput == "") {
		currentInput = "Undefiniert"; // currentInput will be changed to "Undefiniert" when ENTER was pressed while the textbox was empty
	}
	return currentInput;
}

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
		std::string msg = exception.what();
		UI::DISPLAY::displayNotificationOpenCV(msg);
		//awaitAnyKey();
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
		throw NNG_Exception(exception);
	}

	if (tempLearningrate > _max) {
		tempLearningrate = -1;
		throw NNG_Exception("Lernrate zu hoch!");
	}

	else if (tempLearningrate <= 0) {
		tempLearningrate = -1;
		throw NNG_Exception("Lernrate zu niedrig!");
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
		throw NNG_Exception(exception);
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
		throw NNG_Exception(exception);
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
		throw NNG_Exception(exception);
	}

	return tempEpochs;
}

// UI::DISPLAY
void UI::DISPLAY::displayPromptInWindow(cv::Mat& _display, const std::string& _question) {

	cv::putText(_display, _question, cv::Point(50, 50),
		cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1);
}

void UI::DISPLAY::displayMenuOptions(const std::vector<std::string>& _menuList, const Network& _network) {

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
	for (int i = 1; i < _network.getNetworkSize() - 1; i++) {
		std::cout << "Neuronen in versteckter Schicht " << i << ": " << _network.atLayer(i).getLayerSize() << std::endl;
	}
	std::cout << "Neuronen in Output Schicht: " << outputLayer.getLayerSize() << std::endl << std::endl;

	for (int i = 0; i < outputLabels.size(); i++) {
		std::cout << "Klassifikation von Output " << i + 1 << ": " << outputLabels[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Individual Samplesize: " << _network.getIndividualSampleSize();
	awaitAnyKey();
}

MenuState UI::DISPLAY::displayNotificationOpenCV(const std::string& _message) {

	selectedAction = MenuState::ERROR;
	BUTTONS::activeButtons = &BUTTONS::notificationMenuButtons;

	cv::Mat errorDisplay(200, 500, CV_8UC3, cv::Scalar(240, 240, 240));

	for (const auto& btn : BUTTONS::notificationMenuButtons) {
		cv::rectangle(errorDisplay, btn.rect, cv::Scalar(70, 70, 70), -1);
		cv::putText(errorDisplay, btn.label,
			cv::Point(btn.rect.x + 10, btn.rect.y + 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);
	}

	cv::putText(errorDisplay, _message, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);

	cv::namedWindow("Meldung");
	cv::moveWindow("Meldung", 700, 400);
	cv::setMouseCallback("Meldung", UI::onMouse);

	while (selectedAction == MenuState::ERROR) {
		cv::imshow("Meldung", errorDisplay);
		if (cv::waitKey(30) == 27)  // ESC zum Beenden
			break;
	}

	cv::destroyWindow("Meldung");
	return selectedAction;

}

// UI::DISPLAY::MENU
MenuState UI::DISPLAY::MENU::displayMainMenuOpenCV(Network& _network) {

	selectedAction = MenuState::MAIN;
	BUTTONS::activeButtons = &BUTTONS::mainMenuButtons;

	cv::Mat menuImg(600, 400, CV_8UC3, cv::Scalar(240, 240, 240));
	cv::putText(menuImg,_network.getNetworkName(),
		cv::Point(20, 30),
		cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 2);

	for (const auto& btn : BUTTONS::mainMenuButtons) {
		cv::rectangle(menuImg, btn.rect, cv::Scalar(70, 70, 70), -1);
		cv::putText(menuImg, btn.label,
			cv::Point(btn.rect.x + 10, btn.rect.y + 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);
	}

	cv::namedWindow("Menü");
	cv::moveWindow("Menü", 700, 100);
	cv::setMouseCallback("Menü", UI::onMouse);

	while (selectedAction == MenuState::MAIN) {
		cv::imshow("Menü", menuImg);
		if (cv::waitKey(30) == 27)  // ESC zum Beenden
			break;
	}

	cv::destroyWindow("Menü");
	return selectedAction;
}

MenuState UI::DISPLAY::MENU::displayCreateTrainingdataMenuOpenCV(Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeCreatingTrainingdata(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		displayNotificationOpenCV(msg);
		//awaitAnyKey();
		return MenuState::MAIN;
	}

	selectedAction = MenuState::CREATE_TRAININGDATA;
	BUTTONS::activeButtons = &BUTTONS::createTrainingdataMenuButtons;

	cv::Mat createTrainingdataMenuImg(250, 400, CV_8UC3, cv::Scalar(240, 240, 240));

	for (const auto& btn : BUTTONS::createTrainingdataMenuButtons) {
		cv::rectangle(createTrainingdataMenuImg, btn.rect, cv::Scalar(70, 70, 70), -1);
		cv::putText(createTrainingdataMenuImg, btn.label,
			cv::Point(btn.rect.x + 10, btn.rect.y + 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);
	}

	cv::namedWindow("Trainingsdaten");
	cv::moveWindow("Trainingsdaten", 700, 100);
	cv::setMouseCallback("Trainingsdaten", UI::onMouse);

	while (selectedAction == MenuState::CREATE_TRAININGDATA) {
		cv::imshow("Trainingsdaten", createTrainingdataMenuImg);
		if (cv::waitKey(30) == 27)  // ESC zum Beenden
			break;
	}

	cv::destroyWindow("Trainingsdaten");
	return selectedAction;
}

MenuState UI::DISPLAY::MENU::displaySaveMenuOpenCV(Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeSaving(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		displayNotificationOpenCV(msg);
		//awaitAnyKey();
		return MenuState::MAIN;
	}

	selectedAction = MenuState::SAVE;
	BUTTONS::activeButtons = &BUTTONS::saveMenuButtons;

	cv::Mat saveMenuImg(250, 400, CV_8UC3, cv::Scalar(240, 240, 240));

	for (const auto& btn : BUTTONS::saveMenuButtons) {
		cv::rectangle(saveMenuImg, btn.rect, cv::Scalar(70, 70, 70), -1);
		cv::putText(saveMenuImg, btn.label,
			cv::Point(btn.rect.x + 10, btn.rect.y + 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);
	}

	cv::namedWindow("Speichern");
	cv::moveWindow("Speichern", 700, 100);
	cv::setMouseCallback("Speichern", UI::onMouse);

	while (selectedAction == MenuState::SAVE) {
		cv::imshow("Speichern", saveMenuImg);
		if (cv::waitKey(30) == 27)  // ESC zum Beenden
			break;
	}

	cv::destroyWindow("Speichern");
	return selectedAction;
}

MenuState UI::DISPLAY::MENU::displayInitializeMenuOpenCV(Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeInitializing(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		displayNotificationOpenCV(msg);
		//awaitAnyKey();
		return MenuState::MAIN;
	}

	selectedAction = MenuState::INITIALIZE;
	BUTTONS::activeButtons = &BUTTONS::initializeMenuButtons;

	cv::Mat initializeMenuImg(250, 400, CV_8UC3, cv::Scalar(240, 240, 240));

	for (const auto& btn : BUTTONS::initializeMenuButtons) {
		cv::rectangle(initializeMenuImg, btn.rect, cv::Scalar(70, 70, 70), -1);
		cv::putText(initializeMenuImg, btn.label,
			cv::Point(btn.rect.x + 10, btn.rect.y + 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);
	}

	cv::namedWindow("Netz initialisieren");
	cv::moveWindow("Netz initialisieren", 700, 100);
	cv::setMouseCallback("Netz initialisieren", UI::onMouse);

	while (selectedAction == MenuState::INITIALIZE) {
		cv::imshow("Netz initialisieren", initializeMenuImg);
		if (cv::waitKey(30) == 27)  // ESC zum Beenden
			break;
	}

	cv::destroyWindow("Netz initialisieren");
	return selectedAction;
}


MenuState UI::DISPLAY::MENU::mainMenu(const Network& _network) {;

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

MenuState UI::DISPLAY::MENU::newNetMenu(const Network& _network) {

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

MenuState UI::DISPLAY::MENU::createTrainingdataMenu(Network& _network) {

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

MenuState UI::DISPLAY::MENU::saveMenu(const Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeSaving(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		UI::DISPLAY::displayNotificationOpenCV(msg);
		//awaitAnyKey();
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

MenuState UI::DISPLAY::MENU::loadMenu(const Network& _network) {

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

MenuState UI::DISPLAY::MENU::trainingMenu(Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeTraining(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		UI::DISPLAY::displayNotificationOpenCV(msg);
		//awaitAnyKey();
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

MenuState UI::DISPLAY::MENU::testMenu(Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeTesting(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		UI::DISPLAY::displayNotificationOpenCV(msg);
		//awaitAnyKey();
		return MenuState::MAIN;
	}

	GRAPHICS::drawBarGraph(_network);
	GRAPHICS::drawTestImage(_network);
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN; 
}

MenuState UI::DISPLAY::MENU::initializeMenu(const Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeInitializing(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		UI::DISPLAY::displayNotificationOpenCV(msg);
		//awaitAnyKey();
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

MenuState UI::DISPLAY::MENU::showNetworkInfoMenu(const Network& _network){
	
	try {
		UI::DISPLAY::displayNetworkInfo(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		std::string msg = exception.what();
		UI::DISPLAY::displayNotificationOpenCV(msg);
		//awaitAnyKey();
		return MenuState::MAIN;
	}

	return MenuState::MAIN;
}

MenuState UI::DISPLAY::MENU::deleteMenu(const Network& _network) {

	try {
		ERRORHANDLING::FLAGCHECK::checkNetBeforeDeleting(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
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

MenuState UI::DISPLAY::MENU::exitMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

// UI::DISPLAY::SUBMENU
MenuState UI::DISPLAY::SUBMENU::SUBNewNetMenu(Network& _network) {

	int amountOfHiddenLayers = UI::QUERY::promptUserForInteger_OpenCV("Anzahl versteckter Schichten:");

	if (amountOfHiddenLayers == -1) {
		UI::DISPLAY::displayNotificationOpenCV("Netzwerk erstellen abgebrochen!");
		_network.resizeNetwork(0);
		return MenuState::MAIN;
	}

	std::vector<int>topology;
	for (int i = 0; i < amountOfHiddenLayers; i++) {
		int amountOfHiddenNeurons = UI::QUERY::promptUserForInteger_OpenCV("Anzahl Neuronen in Hidden Layer " + std::to_string(i+1) + ":");

		if (amountOfHiddenNeurons == -1) {
			_network.resizeNetwork(0);
			UI::DISPLAY::displayNotificationOpenCV("Netzwerk erstellen abgebrochen!");
			return MenuState::MAIN;
		}

		topology.push_back(amountOfHiddenNeurons);
	}

	if (topology.size() < 0) {
		_network.resizeNetwork(0);
		UI::DISPLAY::displayNotificationOpenCV("Netzwerk erstellen abgebrochen!");
		return MenuState::MAIN;
	}

	int amountOfOutputNeurons = UI::QUERY::promptUserForInteger_OpenCV("Anzahl Neuronen in Output Layer:");

	if (amountOfOutputNeurons <= 0) { // Also checking if amountOfOutputNeurons equals "0" because a network without output neurons won't make any sense
		_network.resizeNetwork(0);
		UI::DISPLAY::displayNotificationOpenCV("Netzwerk erstellen abgebrochen! Kein Output!");
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

	std::vector<std::string>outputLabels;
	for (int i = 0; i < amountOfOutputNeurons; i++) {
		std::string outputLabel = UI::QUERY::promptUserForString_OpenCV("Klassifikation von Output " + std::to_string(i+1) + ":");

		if (outputLabel == "") {
			_network.resizeNetwork(0);
			UI::DISPLAY::displayNotificationOpenCV("Netzwerk erstellen abgebrochen!");
			return MenuState::MAIN;
		}

		outputLabels.push_back(outputLabel);
	}
	_network.setOutputLabels(outputLabels);

	_network.setModifiedStatus(true);

	return MenuState::MAIN;
}

MenuState UI::DISPLAY::SUBMENU::SUBcreateTrainingdataMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::DISPLAY::SUBMENU::SUBcreateValidationdataMenu(Network& _network) {
	std::cout << "Bin in " << __func__ << std::endl;
	return MenuState::MAIN;
}

MenuState UI::DISPLAY::SUBMENU::SUBSaveNetworkMenu(Network& _network) {

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

MenuState UI::DISPLAY::SUBMENU::SUBSaveNetworkAsMenu(Network& _network) {

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

MenuState UI::DISPLAY::SUBMENU::SUBLoadNetworkMenu(Network& _network) {

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

MenuState UI::DISPLAY::SUBMENU::SUBDefaultTrainingMenu(Network& _network) {

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

MenuState UI::DISPLAY::SUBMENU::SUBCustomTrainingMenu(Network& _network) {

	double learningRate;
	double epsilonDecay;
	double momentumFactor;
	int epochs;

	clearScreen();
	try {
		learningRate = UI::QUERY::userSetLearningrate(MAX_LEARNINGRATE);
		epsilonDecay = UI::QUERY::userSetEpsilonDecay(MAX_EPSILON_DECAY);
		momentumFactor = UI::QUERY::userSetMomentumFactor(MAX_MOMENTUMFACTOR);
		epochs = UI::QUERY::userSetEpochs();
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
		return MenuState::MAIN;
	}
	if (learningRate == -1) {
		return MenuState::MAIN;
	}

	Trainer trainer(learningRate, epsilonDecay, momentumFactor, epochs);

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

MenuState UI::DISPLAY::SUBMENU::SUBTestMenu(Network& _network) {
	return MenuState::MAIN;
}

MenuState UI::DISPLAY::SUBMENU::SUBInitializeMenu(Network& _network) {

	_network.randomize();
	std::string info = "Netzwerk neu initialisiert!";
	UI::DISPLAY::displayNotificationOpenCV(info);
	return MenuState::MAIN;
}

MenuState UI::DISPLAY::SUBMENU::SUBDeleteMenu(Network& _network) {

	try {
		NETWORKHANDLER::DATAMANAGEMENT::deleteNetwork(_network);
	}
	catch (const NNG_Exception& exception) {
		std::cerr << std::endl << exception.what() << std::endl;
		awaitAnyKey();
	}

	return MenuState::MAIN;
}

