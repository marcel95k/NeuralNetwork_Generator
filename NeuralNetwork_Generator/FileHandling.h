#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <chrono>
#include "Neuron.h"
#include "NetworkProperties.h"
#include "Training.h"
#include "ErrorHandling.h"
#include "Graphics.h"

/*-----------------------------------------------------------*/
/* Manages all network- and image-related fielhandling tasks */
/*-----------------------------------------------------------*/

/*Still left to implement:

*/


namespace fs = std::filesystem;


namespace FILEHANDLING {

	bool networkExisting(const std::string& _filename, const std::string& _networkName);	// Checks if a network with the same name is already existing

	void writeGrayscaleToFile(const std::string _mainFolder, std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter, cv::Mat _centered);	// Writes the grayscale values into a .txt file

	int createValidationdataLoop(std::vector<std::vector<Neuron>>* _network, const int _amountOfIndividualClassifications);
	void createNewValdiationFolders(std::vector<std::vector<Neuron>>* _network);			// Creates validationdata folders for a created network - called from createValidationdataSetup()
	void createValidationdataSetup(std::vector<std::vector<Neuron>>* _network);				// Setup for creating new validationdata

	int createTrainingdataLoop(std::vector<std::vector<Neuron>>* _network, const int _amountOfIndividualClassifications);
	void createNewTrainingFolders(std::vector<std::vector<Neuron>>* _network);				// Creates trainingdata folders for a created network - called from createTrainingdataSetup()
	void createTrainingdataSetup(std::vector<std::vector<Neuron>>* _network);				// Setup for creating new trainingdata

	void saveNeuronLayer(const std::vector<Neuron>& _layer, const std::string& _filename);	// Save a layer of a network - called from saveNet()
	std::vector<Neuron> loadNeuronLayer(const std::string& _filename);						// Load a layer of a network - called from loadNet()
	void displaySavedNetworks();															// Display the list of all saved networks
	int checkIfSaved(std::vector<std::vector<Neuron>>* _network);							// Check if a network is existing and not saved yet
	void saveNet(std::vector<std::vector<Neuron>>* _network);								// Save a network - called from setupSave()
	void setupSave(std::vector<std::vector<Neuron>>* _network);								// Setup for saving a network 
	void loadNet(std::vector<std::vector<Neuron>>* _network);								// Load a network - called from setupLoad()
	void setupLoad(std::vector<std::vector<Neuron>>* _network);								// Setup for loading a network 

	void deleteValidationFolders(std::vector<std::vector<Neuron>>* _network);				// Delete the network related Validationfolders
	void deleteTrainingFolders(std::vector<std::vector<Neuron>>* _network);					// Delete the network related Trainingfolders
	void deleteFolders(std::vector<std::vector<Neuron>>* _network);							// Delete all of the network related folders
	void deleteNet(std::vector<std::vector<Neuron>>* _network);								// Delete a network - called from setupDelete()
	void setupDelete(std::vector<std::vector<Neuron>>* _network);							// Setup for deleting a network 

	int setupExit(std::vector<std::vector<Neuron>>* _network);								// Setup exiting the program
	std::string getImageFilePath(const std::string _mainFolder, std::vector<std::vector<Neuron>>* _network, const int _classification, const int _counter);

	void fillGrayValues(std::ifstream& _inFile, std::vector<float>* _grayValues);
	void loadValidationIMG(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, const int _classification, const int _counter, double& totalAccuracy, int& totalTests);	// Load the validation image and call fitnessTest() 
	void loadTrainingIMG(std::vector<std::vector<Neuron>>* _network, std::vector<float>* _grayValues, const int _classification, const int _counter, const double _epsilon, const double _epsilonDecay, const double _momentumFactor, const int _epochs);	// Load the training image and call training() 
}