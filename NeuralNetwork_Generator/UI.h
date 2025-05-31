#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <opencv2/opencv.hpp>

#include "Utility.h"
#include "Graphics.h"
#include "NetworkHandler.h"
#include "Defines.h"
#include "ExceptionHandling.h"
#include "FileHandling.h"

#include "Trainer.h"
#include "Network.h"

enum class  MenuState {

	MAIN,
	NEWNET,
	CREATE_TRAININGDATA,
	SAVE,
	LOAD,
	TRAINING,
	TEST,
	INITIALIZE,
	SHOW_NETWORKINFO,
	DELETE,
	EXIT,

	SUB_NEWNET,
	SUB_CREATE_TRAININGDATA,
	SUB_CREATE_VALIDATIONDATA,
	SUB_SAVE,
	SUB_SAVE_AS,
	SUB_LOAD,
	SUB_TRAIN_DEFAULT,
	SUB_TRAIN_CUSTOM,
	SUB_TEST,
	SUB_INITIALIZE,
	SUB_DELETE,

	ERROR,
};

inline bool operator<(MenuState lhs, MenuState rhs) {
	return static_cast<int>(lhs) < static_cast<int>(rhs);
}

struct Button {
	cv::Rect rect;
	std::string label;
	MenuState action;
};

namespace BUTTONS {

	extern std::vector<Button> mainMenuButtons;
	extern std::vector<Button> newNetMenuButtons;
	extern std::vector<Button> createTrainingdataMenuButtons;
	extern std::vector<Button> saveMenuButtons;
	extern std::vector<Button> loadMenuButtons;
	extern std::vector<Button> initializeMenuButtons;
	extern std::vector<Button> deleteMenuButtons;

	extern std::vector<Button> notificationMenuButtons;

	extern MenuState selectedAction;
	extern std::vector<Button>* activeButtons;
}

namespace UI {

	void onMouse(int event, int x, int y, int, void*);
	void onTextboxMouse(int event, int x, int y, int, void*);
	void putButtonsOnWindow(Mat& _img, const std::vector<Button>& _buttons);

	namespace MISC {

		void makeTextboxBlink(const int& _framkecounter, bool& _blinkstate, cv::Scalar& _boxColor);
	}
	 
	namespace PROCESSING {

		int processUserInputINT();
		double processUserInputDOUBLE();
		int processUserInputOnMenuOptions(const int _menuSize);
		MenuState menuLoop(const std::vector<std::string>& _menuList, const std::map<int, MenuState>& _menuActions, const Network& _network);
	}

	namespace QUERY {

		int promptUserForInteger_OpenCV(const std::string& _prompt);
		std::string promptUserForString_OpenCV(const std::string& _prompt);
		std::string userSetNetworkNameSave();
		std::string userSetNetworkNameLoad(const std::string& _prompt);
		int userSetAmountOfHiddenLayers();
		std::vector<int> userSetAmountOfHiddenNeurons(const int _amountOfHiddenLayers);
		int userSetAmountOfOutputNeurons();
		void userSetOutputLabels(Network& _network);
		double userSetLearningrate(const double _max);
		double userSetEpsilonDecay(const double _max);
		double userSetMomentumFactor(const double _max);
		int userSetEpochs();
	}

	namespace DISPLAY {

		void displayPromptInWindow(cv::Mat& _display, const std::string& _prompt);
		void displayMenuOptions(const std::vector<std::string>& _menuList, const Network& _network);
		void displayNetworkInfo(const Network& _network);
		MenuState displayNotificationOpenCV(const std::string& _message); // <== TODO: Text formatting, not centered yet

		namespace MENU {

			MenuState displayMainMenuOpenCV(Network& _network);
			MenuState displayNewNetMenuOpenCV(Network& _network);
			MenuState displayCreateTrainingdataMenuOpenCV(Network& _network);
			MenuState displaySaveMenuOpenCV(Network& _network);
			MenuState displayLoadMenuOpenCV(Network& _network);
			MenuState displayInitializeMenuOpenCV(Network& _network);
			MenuState displayDeleteMenuOpenCV(Network& _network);

			MenuState mainMenu(const Network& _network);
			MenuState newNetMenu(const Network& _network);
			MenuState createTrainingdataMenu(Network& _network);
			MenuState saveMenu(const Network& _network);
			MenuState loadMenu(const Network& _network);
			MenuState trainingMenu(Network& _network);
			MenuState testMenu(Network& _network);
			MenuState initializeMenu(const Network& _network);
			MenuState showNetworkInfoMenu(const Network& _network);
			MenuState deleteMenu(const Network& _network);
			MenuState exitMenu(Network& _network);
		}

		namespace SUBMENU {

			MenuState SUBNewNetMenu(Network& _network);
			MenuState SUBcreateTrainingdataMenu(Network& _network);
			MenuState SUBcreateValidationdataMenu(Network& _network);
			MenuState SUBSaveNetworkMenu(Network& _network);
			MenuState SUBSaveNetworkAsMenu(Network& _network);
			MenuState SUBLoadNetworkMenu(Network& _network);
			MenuState SUBDefaultTrainingMenu(Network& _network);
			MenuState SUBCustomTrainingMenu(Network& _network);
			MenuState SUBTestMenu(Network& _network);
			MenuState SUBInitializeMenu(Network& _network);
			MenuState SUBDeleteMenu(Network& _network);
		}
	}
}
