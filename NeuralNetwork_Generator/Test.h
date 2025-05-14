#pragma once
#include <iostream>
#include<vector>
#include <string>
#include "Neuron.h"
#include "MathFunctions.h"
#include "Training.h"
#include "Graphics.h"
#include "ErrorHandling.h"

void forwardPassTest(std::vector<std::vector<Neuron>>* _network, double& _totalSum);
void test(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues);
void setupTest(std::vector<std::vector<Neuron>>* _network);