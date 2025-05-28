#include "Network.h"

void Network::resizeNetwork(const int _size) {
    network.resize(_size);
}

void Network::addLayer(const Layer _layer) {
    network.push_back(_layer);
}

void Network::connectLayers() {

    assert(getNetworkSize() > 0);   // Network size has to be > 0

    for (int i = 0; i < getNetworkSize() - 1; ++i) {
        for (int t = 0; t < atLayer(i).getLayerSize(); ++t) {
            atLayer(i).atNeuron(t).resizeWeightVector(atLayer(i + 1).getLayerSize());
        }
    }
}

void Network::randomize() {

    assert(getNetworkSize() > 0);   // Network size has to be > 0

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    for (int i = 0; i < getNetworkSize() - 1; i++) {
        for (int u = 0; u < atLayer(i).getLayerSize(); u++) {
            Neuron& neuron = atLayer(i).atNeuron(u);

            neuron.setBias(dist(gen));

            for (int n = 0; n < neuron.getSizeOfWeightVector(); n++) {
                neuron.setWeightAt(n, dist(gen));
            }
        }
    }
}


void Network::setNetworkName(const std::string _networkName) {
    networkName = _networkName;
}

void Network::setOutputLabels(const std::vector<std::string>& _labels) {
    outputLabels = _labels;
}


int Network::getNetworkSize() const {
    return network.size();
}

std::string Network::getNetworkName() const {
    return networkName;
}

const std::vector<std::string>& Network::getOutputLabels() const {
    return outputLabels;
}

const std::string Network::getOutputLabelAt(const int _index) const {
    return outputLabels[_index];
}

Layer& Network::atLayer(const int _index) {
    return network.at(_index);
}

const Layer& Network::atLayer(const int _index) const {
    return network.at(_index);
}


void Network::fillInputLayer(const std::vector<double> _inputValues) {

    Layer& inputLayer = atLayer(0);
    for (int n = 0; n < inputLayer.getLayerSize(); n++) {
        Neuron& currentNeuron = inputLayer.atNeuron(n);
        currentNeuron.setOutputValue(currentNeuron.sigmoid(_inputValues[n]));
    }
}


std::vector<double> Network::forwardPass() {

    assert(getNetworkSize() > 0);   // Network size has to be > 0

    for (int l = 1; l < getNetworkSize(); ++l) {
        Layer& prevLayer = network[l - 1];
        Layer& currentLayer = network[l];


        for (int j = 0; j < currentLayer.getLayerSize(); ++j) {
            Neuron& currentNeuron = currentLayer.atNeuron(j);

            double sum = currentNeuron.getBias();

            for (int i = 0; i < prevLayer.getLayerSize(); ++i) {
                double output_i = prevLayer.atNeuron(i).getOutputValue();
                double weight_ij = prevLayer.atNeuron(i).getWeightAt(j);
                sum += output_i * weight_ij;
            }

            double activated = currentNeuron.sigmoid(sum);
            currentNeuron.setOutputValue(activated);
        }
    }

    std::vector<double> newOutputs;
    for (int i = 0; i < network.back().getLayerSize(); i++) {
        newOutputs.push_back(network.back().atNeuron(i).getOutputValue());
    }
    return newOutputs;
}

void Network::backpropagation(const std::vector<double>& _expected, double _learningRate) {

    assert(getNetworkSize() > 0);   // Network size has to be > 0

    std::vector<std::vector<double>> deltas(getNetworkSize());

    calculateOutputLayerError(deltas, _expected);
    propagateErrorsBackward(deltas);
    updateWeights(deltas, _learningRate);
}

void Network::calculateOutputLayerError(std::vector<std::vector<double>>& _deltas, const std::vector<double>& _expected) {

    int outputLayerIndex = getNetworkSize() - 1;
    Layer& outputLayer = atLayer(outputLayerIndex);
    _deltas[outputLayerIndex] = std::vector<double>(outputLayer.getLayerSize());

    for (int j = 0; j < outputLayer.getLayerSize(); ++j) {
        Neuron& neuron = outputLayer.atNeuron(j);
        double output = neuron.getOutputValue();
        double error = output - _expected[j];
        _deltas[outputLayerIndex][j] = error * neuron.sigmoidDerivative(output);
    }
}

void Network::propagateErrorsBackward(std::vector<std::vector<double>>& _deltas) {

    for (int l = getNetworkSize() - 2; l > 0; --l) {
        Layer& layer = atLayer(l);
        Layer& nextLayer = atLayer(l + 1);
        _deltas[l] = std::vector<double>(layer.getLayerSize());

        for (int j = 0; j < layer.getLayerSize(); ++j) {
            double sum = 0.0;
            for (int k = 0; k < nextLayer.getLayerSize(); ++k) {
                sum += layer.atNeuron(j).getWeightAt(k) * _deltas[l + 1][k];
            }

            double output = layer.atNeuron(j).getOutputValue();
            _deltas[l][j] = sum * layer.atNeuron(j).sigmoidDerivative(output);
        }
    }
}

void Network::updateWeights(const std::vector<std::vector<double>>& _deltas, double _learningRate) {

    for (int l = 0; l < getNetworkSize() - 1; ++l) {
        Layer& current = atLayer(l);
        Layer& next = atLayer(l + 1);

        for (int j = 0; j < next.getLayerSize(); ++j) {
            Neuron& targetNeuron = next.atNeuron(j);
            double delta = _deltas[l + 1][j];

            for (int i = 0; i < current.getLayerSize(); ++i) {
                Neuron& sourceNeuron = current.atNeuron(i);
                double deltaWeight = -_learningRate * delta * sourceNeuron.getOutputValue();
                sourceNeuron.addWeightAt(j, deltaWeight, 0.9);  // <== Get the momentumFactor from Trainer here
            }

            targetNeuron.setBias(targetNeuron.getBias() - _learningRate * delta);
        }
    }
}



void Network::saveToFile(const std::string& _filename) const {

    std::ofstream out(_filename, std::ios::binary);
    if (!out) throw NNG_Exception("Fehler beim Oeffnen der Datei: " + _filename);

    int numLayers = getNetworkSize();
    out.write(reinterpret_cast<const char*>(&numLayers), sizeof(int));

    for (const auto& layer : network) {
        int numNeurons = layer.getLayerSize();
        out.write(reinterpret_cast<const char*>(&numNeurons), sizeof(int));

        for (int i = 0; i < numNeurons; ++i) {
            const Neuron& neuron = layer.atNeuron(i);
            double bias = neuron.getBias();
            out.write(reinterpret_cast<const char*>(&bias), sizeof(double));

            int weightCount = neuron.getSizeOfWeightVector();
            out.write(reinterpret_cast<const char*>(&weightCount), sizeof(int));

            for (int w = 0; w < weightCount; ++w) {
                double weight = neuron.getWeightAt(w);
                out.write(reinterpret_cast<const char*>(&weight), sizeof(double));
            }
        }
    }

    size_t nameLen_net = networkName.size();
    out.write(reinterpret_cast<const char*>(&nameLen_net), sizeof(nameLen_net));
    out.write(networkName.c_str(), nameLen_net);

    int labelCount = outputLabels.size();
    out.write(reinterpret_cast<const char*>(&labelCount), sizeof(int));
    for (const std::string& label : outputLabels) {
        int length = label.size();
        out.write(reinterpret_cast<const char*>(&length), sizeof(int));
        out.write(label.c_str(), length);
    }
    out.write(reinterpret_cast<const char*>(&isSaved), sizeof(isSaved));
    out.write(reinterpret_cast<const char*>(&isModified), sizeof(isModified));
    out.write(reinterpret_cast<const char*>(&individualSampleSize), sizeof(individualSampleSize));

    out.close();
}

void Network::loadFromFile(const std::string& _filename) {

    std::ifstream in(_filename, std::ios::binary);
    if (!in) throw std::runtime_error("Fehler beim Laden der Datei: " + _filename);

    network.clear();

    int numLayers;
    in.read(reinterpret_cast<char*>(&numLayers), sizeof(int));

    for (int l = 0; l < numLayers; ++l) {
        int numNeurons;
        in.read(reinterpret_cast<char*>(&numNeurons), sizeof(int));

        Layer layer;
        for (int n = 0; n < numNeurons; ++n) {
            Neuron neuron;
            double bias;
            in.read(reinterpret_cast<char*>(&bias), sizeof(double));
            neuron.setBias(bias);

            int weightCount;
            in.read(reinterpret_cast<char*>(&weightCount), sizeof(int));
            neuron.resizeWeightVector(weightCount);

            for (int w = 0; w < weightCount; ++w) {
                double weight;
                in.read(reinterpret_cast<char*>(&weight), sizeof(double));
                neuron.setWeightAt(w, weight);
            }

            layer.addNeuron(neuron);
        }
        addLayer(layer);
    }

    size_t nameLen_net;
    in.read(reinterpret_cast<char*>(&nameLen_net), sizeof(nameLen_net));
    networkName.resize(nameLen_net);
    in.read(&networkName[0], nameLen_net);

    int labelCount;
    in.read(reinterpret_cast<char*>(&labelCount), sizeof(int));
    outputLabels.resize(labelCount);
    for (int i = 0; i < labelCount; ++i) {
        int length;
        in.read(reinterpret_cast<char*>(&length), sizeof(int));
        std::string label(length, ' ');
        in.read(&label[0], length);
        outputLabels[i] = label;
    }
    in.read(reinterpret_cast<char*>(&isSaved), sizeof(isSaved));
    in.read(reinterpret_cast<char*>(&isModified), sizeof(isModified));
    in.read(reinterpret_cast<char*>(&individualSampleSize), sizeof(individualSampleSize));

    in.close();
}


void Network::setSavedStatus(const bool _isSaved) {
    isSaved = _isSaved;
}

bool Network::getSavedStatus() const {
    return isSaved;
}

void Network::setModifiedStatus(const bool _isModified) {
    isModified = _isModified;
}

bool Network::getModifiedStatus() const {
    return isModified;
}

void Network::setIndividualSampleSize(const int _individualSampleSize) {
    individualSampleSize = _individualSampleSize;
}

int Network::getIndividualSampleSize() {
    return individualSampleSize;
}




