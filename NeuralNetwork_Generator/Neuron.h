#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Neuron {
private:
	int individualClassifications;	// Amount of how many individual images of a classification exist
	double inputValue;
	double outputValue;
	double biasWeight;
	double targetOutputValue;
	double averageAccuracy = 0;
	double momentum = 0;
	bool newWeight = false;
	bool newBiasWeight = false;
	bool isTrained = false;
	bool isSaved = false;
	bool hasValidationdata = false;
	std::string classificationName = "";
	std::string networkName = "";
	std::vector<double>weights;

public:
	int getIndividualClassifications();
	double getInputValue();
	double getOutputValue();
	double getBiasWeight();
	double getTargetOutputValue();
	double getAverageAccuracy();
	bool getNewWeight();
	bool getNewBiasWeight();
	bool getIsTrained();
	bool getIsSaved();
	bool getHasValidationdata();
	std::string getClassificationName();
	std::string getNetworkName();
	double getWeightAt(const int _position);

	void setIndividualClassifications(const int _individualClassifications);
	void setInputValue(const double _inputValue);
	void setOutputValue(const double _outoutValue);
	void setBiasWeight(const double _biasWeight);
	void setTargetOutputValue(const double _targetOutputValue);
	void setAverageAccuracy(const double _averageAccuracy);
	void setNewWeight(bool _newWeight);
	void setNewBiasWeight(bool _newBiasWeight);
	void setIsTrained(bool _isTrained);
	void setIsSaved(bool _isSaved);
	void setHasValidationdata(bool _hasValidationdata);
	void setClassificationName(const std::string _classificationName);
	void setNetworkName(const std::string _classificationName);
	void setWeightAt(const int _position, const double _value);
	void addWeightAt(const int _position, const double _value, const double _momentumFactor);
	void resizeWeightVector(const int _size);

	void save(std::ofstream& out) const;
	void load(std::ifstream& in);
};

