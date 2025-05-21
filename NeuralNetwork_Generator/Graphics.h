#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "Neuron.h"
#include "Test.h"
#include "Defines.h"

/*---------------------------------------------*/
/* Manages all graphical aspecs of the program */
/*---------------------------------------------*/

/*Still left to implement:

*/

using namespace cv;

namespace GRAPHICS {

	void draw(int x, int y);
	void erase(int x, int y);
	int processKeyPressValidation(const int _status, int& _counter, int& _classification, const int _amountOfIndividualSamples);
	int processKeyPressTraining(const int _status, int& _counter, int& _classification, const int _amountOfIndividualSamples);
	void mouseHandler(int event, int x, int y, int flags, void*);
	Mat centerImage(const Mat& input, const Size& targetSize);
	void drawTestImage(std::vector<std::vector<Neuron>>* _network);
	int drawValidationdata(std::vector<std::vector<Neuron>>* _network, const int _counter, const int _classification);
	int drawTrainingdata(std::vector<std::vector<Neuron>>* _network, const  int _counter, const int _classification);
	void displaySidebarTextTest(Mat& window);
	void displaySidebarText(Mat& window);
	void drawBarGraph(std::vector<Neuron>* outputNeurons, int width = 600, int height = 400);
	void drawLoss(const double _accuracy, const int _counter, int& x, std::vector<cv::Point>& _lossPoints, std::chrono::duration<double> _duration, const int _epoch);
	void drawLossT(const double _accuracy, int& x, std::vector<cv::Point>* _lossPoints);
}
