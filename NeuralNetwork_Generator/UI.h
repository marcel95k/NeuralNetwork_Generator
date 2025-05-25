#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "NetworkHandler.h"
#include "Defines.h"
#include "ExceptionHandling.h"
#include "FileHandling.h"

#include "Network.h"


enum class MenuState {

	MAIN,
	NEWNET,
	CREATE_TRAININGDATA,
	SAVE,
	LOAD,
	TRAINING,
	TEST,
	VALIDATE,
	INITIALIZE,
	DELETE,
	EXIT,

	SUB_CREATE_TRAININGDATA,
	SUB_CREATE_VALIDATIONDATA,
	
};

namespace UI {

	namespace PROCESSING {
		int processUserInputOnMenuOptions(const int _menuSize);
		MenuState menuLoop(const std::vector<std::string>& _menuList, const std::map<int, MenuState>& _menuActions);
	}

	namespace DISPLAY {
		void displayMenuOptions(const std::vector<std::string> _menuList);
	}

	namespace MENU {
		MenuState mainMenu(Network& _network);
		MenuState newNetMenu(Network& _network);
		MenuState createTrainingdataMenu(Network& _network);
		MenuState saveMenu(Network& _network);
		MenuState loadMenu(Network& _network);
		MenuState trainingMenu(Network& _network);
		MenuState testMenu(Network& _network);
		MenuState validationCheckMenu(Network& _network);
		MenuState initializeMenu(Network& _network);
		MenuState deleteMenu(Network& _network);
		MenuState exitMenu(Network& _network);

		namespace CREATETRAININGDATA {
			MenuState SUBcreateTrainingdataMenu(Network& _network);
			MenuState SUBcreateValidationdataMenu(Network& _network);
		}
	}
}
