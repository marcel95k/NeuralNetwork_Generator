#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "Defines.h"
#include "ExceptionHandling.h"
#include "FileHandling.h"

#include "Network.h"

namespace NETWORKHANDLER {

	void setOutputLabels(Network& _network);
	void fillLayers(Network& _network, const std::vector<int> _topology);
	Network newNet(const int _amountOfHiddenLayers, const std::vector<int> _topology);

	void networkSaver(Network& _network);
}