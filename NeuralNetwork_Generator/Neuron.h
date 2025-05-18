#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Neuron {
private:
	int individualClassifications = 0;	// Amount of how many individual images of a classification exist
	double inputValue = 0;
	double outputValue = 0;
	double biasWeight = 0;
	double targetOutputValue = 0;
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
	int getIndividualClassifications() const;
	double getInputValue() const;
	double getOutputValue() const;
	double getBiasWeight() const;
	double getTargetOutputValue() const;
	double getAverageAccuracy() const;
	bool getNewWeight() const;
	bool getNewBiasWeight() const;
	bool getIsTrained() const;
	bool getIsSaved() const;
	bool getHasValidationdata() const;
	std::string getClassificationName() const;
	 std::string getNetworkName() const;
	double getWeightAt(const int _position) const;

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

