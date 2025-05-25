#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "Defines.h"
#include "UI.h"
#include "ExceptionHandling.h"
#include "FileHandling.h"

#include "Network.h"

namespace NETWORKHANDLER {

	namespace NEWNET {
		void fillLayers(Network& _network, const std::vector<int> _topology);
		Network buildNewNet(const int _amountOfHiddenLayers, const std::vector<int> _topology);
	}

	namespace DATAMANAGEMENT {
		void networkSaver(Network& _network);
	}
}