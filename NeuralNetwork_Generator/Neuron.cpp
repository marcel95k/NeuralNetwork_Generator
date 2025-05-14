#include "Neuron.h"

int Neuron::getIndividualClassifications() { return individualClassifications; }
double Neuron::getInputValue() { return inputValue; }
double Neuron::getOutputValue() { return outputValue; }
double Neuron::getBiasWeight() { return biasWeight; }
double Neuron::getTargetOutputValue() { return targetOutputValue; }
double Neuron::getAverageAccuracy() { return averageAccuracy; }
bool Neuron::getNewWeight() { return newWeight; }
bool Neuron::getNewBiasWeight() { return newBiasWeight; }
bool Neuron::getIsTrained() { return isTrained; }
bool Neuron::getIsSaved() { return isSaved; }
bool Neuron::getHasValidationdata() { return hasValidationdata; }
std::string Neuron::getClassificationName() { return classificationName; }
std::string Neuron::getNetworkName() { return networkName; }
double Neuron::getWeightAt(const int _position) { return weights.at(_position); }

void Neuron::setIndividualClassifications(const int _individualClassifications) { individualClassifications = _individualClassifications; }
void Neuron::setInputValue(const double _inputValue) { inputValue = _inputValue; }
void Neuron::setOutputValue(const double _outoutValue) { outputValue = _outoutValue; }
void Neuron::setBiasWeight(const double _biasWeight) { biasWeight = _biasWeight; }
void Neuron::setTargetOutputValue(const double _targetOutputValue) { targetOutputValue = _targetOutputValue; }
void Neuron::setAverageAccuracy(const double _averageAccuracy) { averageAccuracy = _averageAccuracy; }
void Neuron::setNewWeight(bool _newWeight) { newWeight = _newWeight; }
void Neuron::setNewBiasWeight(bool _newBiasWeight) { newBiasWeight = _newBiasWeight; }
void Neuron::setIsTrained(bool _isTrained) { isTrained = _isTrained; }
void Neuron::setIsSaved(bool _isSaved) { isSaved = _isSaved; }
void Neuron::setHasValidationdata(bool _hasValidationdata) { hasValidationdata = _hasValidationdata; }
void Neuron::setClassificationName(const std::string _classificationName) { classificationName = _classificationName; }
void Neuron::setNetworkName(const std::string _networkName) { networkName = _networkName; }
void Neuron::setWeightAt(const int _position, const double _value) { weights.at(_position) = _value; }
void Neuron::addWeightAt(const int _position, const double _value, const double _momentumFactor) { 
    momentum += _value;
    momentum *= _momentumFactor;
    weights.at(_position) += _value + momentum; 
}
void Neuron::resizeWeightVector(const int _size) { weights.resize(_size); }

void Neuron::save(std::ofstream& out) const {
    out.write(reinterpret_cast<const char*>(&individualClassifications), sizeof(individualClassifications));
    out.write(reinterpret_cast<const char*>(&inputValue), sizeof(inputValue));
    out.write(reinterpret_cast<const char*>(&outputValue), sizeof(outputValue));
    out.write(reinterpret_cast<const char*>(&biasWeight), sizeof(biasWeight));
    out.write(reinterpret_cast<const char*>(&targetOutputValue), sizeof(targetOutputValue));
   // out.write(reinterpret_cast<const char*>(&averageAccuracy), sizeof(averageAccuracy));
    out.write(reinterpret_cast<const char*>(&newWeight), sizeof(newWeight));
    out.write(reinterpret_cast<const char*>(&newBiasWeight), sizeof(newBiasWeight));
    out.write(reinterpret_cast<const char*>(&isTrained), sizeof(isTrained));
    out.write(reinterpret_cast<const char*>(&isSaved), sizeof(isSaved));
    out.write(reinterpret_cast<const char*>(&hasValidationdata), sizeof(hasValidationdata));

    // Save classificationName
    size_t nameLen = classificationName.size();
    out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    out.write(classificationName.c_str(), nameLen);

    // Save networkName
    size_t nameLen_net = networkName.size();
    out.write(reinterpret_cast<const char*>(&nameLen_net), sizeof(nameLen_net));
    out.write(networkName.c_str(), nameLen_net);

    // Save weights
    size_t weightSize = weights.size();
    out.write(reinterpret_cast<const char*>(&weightSize), sizeof(weightSize));
    for (double w : weights) {
        out.write(reinterpret_cast<const char*>(&w), sizeof(w));
    }
}

void Neuron::load(std::ifstream& in) {
    in.read(reinterpret_cast<char*>(&individualClassifications), sizeof(individualClassifications));
    in.read(reinterpret_cast<char*>(&inputValue), sizeof(inputValue));
    in.read(reinterpret_cast<char*>(&outputValue), sizeof(outputValue));
    in.read(reinterpret_cast<char*>(&biasWeight), sizeof(biasWeight));
    in.read(reinterpret_cast<char*>(&targetOutputValue), sizeof(targetOutputValue));
  //  in.read(reinterpret_cast<char*>(&averageAccuracy), sizeof(averageAccuracy));
    in.read(reinterpret_cast<char*>(&newWeight), sizeof(newWeight));
    in.read(reinterpret_cast<char*>(&newBiasWeight), sizeof(newBiasWeight));
    in.read(reinterpret_cast<char*>(&isTrained), sizeof(isTrained));
    in.read(reinterpret_cast<char*>(&isSaved), sizeof(isSaved));
    in.read(reinterpret_cast<char*>(&hasValidationdata), sizeof(hasValidationdata));

    // Load classificationName
    size_t nameLen;
    in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    classificationName.resize(nameLen);
    in.read(&classificationName[0], nameLen);

    // Load networkName
    size_t nameLen_net;
    in.read(reinterpret_cast<char*>(&nameLen_net), sizeof(nameLen_net));
    networkName.resize(nameLen_net);
    in.read(&networkName[0], nameLen_net);

    // Load weights
    size_t weightSize;
    in.read(reinterpret_cast<char*>(&weightSize), sizeof(weightSize));
    weights.resize(weightSize);
    for (size_t i = 0; i < weightSize; ++i) {
        in.read(reinterpret_cast<char*>(&weights[i]), sizeof(weights[i]));
    }
}


