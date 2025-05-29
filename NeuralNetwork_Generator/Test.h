#pragma once
#include <iostream>
#include<vector>
#include <string>

#include "MathFunctions.h"
#include "Training.h"
#include "Graphics.h"
#include "ErrorHandling.h"
//
///*------------------------------------*/
///* Handles all testing-relazted tasks */
///*------------------------------------*/
//
///*Still left to implement:
//
//*/
//
//
namespace TEST {

	void printSortedClassification(const std::vector<std::string>& labels, const std::vector<double>& outputs);
//	void forwardPassTest(std::vector<std::vector<Neuron>>* _network, double& _totalSum);
//	void test(std::vector<std::vector<Neuron>>* _network, std::vector<double>* _grayValues);
//	void checkIfTrained(std::vector<std::vector<Neuron>>* _network);	// Check if the network was trained yet						
//	void setupTest(std::vector<std::vector<Neuron>>* _network);
}