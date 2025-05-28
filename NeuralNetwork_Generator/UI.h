#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "Utility.h"
#include "Graphics.h"
#include "NetworkHandler.h"
#include "Defines.h"
#include "ExceptionHandling.h"
#include "FileHandling.h"

#include "Trainer.h"
#include "Network.h"


enum class MenuState {

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
};

namespace UI {

	namespace PROCESSING {

		int processUserInputINT();
		double processUserInputDOUBLE();
		int processUserInputOnMenuOptions(const int _menuSize);
		MenuState menuLoop(const std::vector<std::string>& _menuList, const std::map<int, MenuState>& _menuActions, const Network& _network);
	}

	namespace DISPLAY {
		void displayMenuOptions(const std::vector<std::string> _menuList, const Network& _network);
		void displayNetworkInfo(const Network& _network);
	}

	namespace QUERY {
		std::string userSetNetworkNameSave();
		std::string userSetNetworkNameLoad();
		int userSetAmountOfHiddenLayers();
		std::vector<int> userSetAmountOfHiddenNeurons(const int _amountOfHiddenLayers);
		int userSetAmountOfOutputNeurons();
		void userSetOutputLabels(Network& _network);
		double userSetLearningrate(const double _max);
		double userSetEpsilonDecay(const double _max);
		double userSetMomentumFactor(const double _max);
		int userSetEpochs();
	}

	namespace MENU {
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

		namespace NEWNET {
			MenuState SUBNewNetMenu(Network& _network);
		}

		namespace CREATETRAININGDATA {
			MenuState SUBcreateTrainingdataMenu(Network& _network);
			MenuState SUBcreateValidationdataMenu(Network& _network);
		}

		namespace SAVE {
			MenuState SUBSaveNetworkMenu(Network& _network);
			MenuState SUBSaveNetworkAsMenu(Network& _network);
		}

		namespace LOAD {
			MenuState SUBLoadNetworkMenu(Network& _network);
		}

		namespace TRAIN {
			MenuState SUBDefaultTrainingMenu(Network& _network);
			MenuState SUBCustomTrainingMenu(Network& _network);
		}

		namespace TEST {
			MenuState SUBTestMenu(Network& _network);
		}

		namespace INITIALIZE {
			MenuState SUBInitializeMenu(Network& _network);
		}

		namespace DELETE {
			MenuState SUBDeleteMenu(Network& _network);
		}


	}
}
