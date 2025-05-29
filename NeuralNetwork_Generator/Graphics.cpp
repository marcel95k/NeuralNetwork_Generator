#include "Graphics.h"
//
const int WINDOW_HEIGHT = 400;
const int WINDOW_WIDTH = 600;
const int CANVAS_SIZE = 300;
int DRAW_RADIUS = 7;
//
Mat window(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC1, Scalar(255));  // White background
Mat canvas(CANVAS_SIZE, CANVAS_SIZE, CV_8UC1, Scalar(0));       // Black drawing area
//
void GRAPHICS::draw(int x, int y) {
    circle(canvas, Point(x, y), DRAW_RADIUS, Scalar(255), FILLED);  // Draw white
}
//
void GRAPHICS::erase(int x, int y) {
    circle(canvas, Point(x, y), DRAW_RADIUS, Scalar(0), FILLED);    // Draw black (erase)
}
//
//int GRAPHICS::processKeyPressValidation(const int _status, int& _counter, int& _classification, const int _amountOfIndividualSamples) {
//
//	if (_status == 1) { // ESC was pressed
//		return 1;
//	}
//	else if (_status == -1) {	// 'b' was pressed: back to previous individual or previous classification	
//		if (_counter > 0) { _counter--; }
//		else if (_classification > 0) {
//			_classification--;
//			_counter = _amountOfIndividualSamples - 1;
//		}
//	}
//	else {	// Continue to next individual
//		_counter++;
//		if (_counter == _amountOfIndividualSamples) {	// Continue to next classifiction when _amountOfIndividualClassifications is reached
//			_classification++;
//			_counter = 0;
//		}
//	}
//	return 0;
//}
//
//int GRAPHICS::processKeyPressTraining(const int _status, int& _counter, int& _classification, const int _amountOfIndividualSamples) {
//
//	if (_status == 1) { // ESC was pressed
//		return 1;
//	}
//	else if (_status == -1) {	// 'b' was pressed: back to previous individual or previous classification	
//		if (_counter > 0) { _counter--; }
//		else if (_classification > 0) {
//			_classification--;
//			_counter = _amountOfIndividualSamples -1;
//		}
//	}
//	else {	// Continue to next individual
//		_counter++;
//		if (_counter == _amountOfIndividualSamples) {	// Continue to next classifiction when _amountOfIndividualClassifications is reached
//			_classification++;
//			_counter = 0;
//		}
//	}
//	return 0;
//}
//
void GRAPHICS::mouseHandler(int event, int x, int y, int flags, void*) {
	
	static bool drawing = false;
	static bool erasing = false;

	// Convert coordinates to drawing area
	int localX = x - 20;
	int localY = y - 80;

	if (event == EVENT_MOUSEWHEEL) {	// Changing the DRAW_RADIUS
		int direction = getMouseWheelDelta(flags);
		DRAW_RADIUS += (direction > 0 ? 1 : -1);
		DRAW_RADIUS = max(DRAWRADIUS_MIN, min(DRAW_RADIUS, DRAWRADIUS_MAX));
	}

	if (localX >= 0 && localY >= 0 && localX < CANVAS_SIZE && localY < CANVAS_SIZE) {	// Drawing and erasing
		if (event == EVENT_LBUTTONDOWN) drawing = true;
		else if (event == EVENT_RBUTTONDOWN) erasing = true;
		else if (event == EVENT_LBUTTONUP) drawing = false;
		else if (event == EVENT_RBUTTONUP) erasing = false;

		if (drawing) GRAPHICS::draw(localX, localY);
		else if (erasing) GRAPHICS::erase(localX, localY);
	}
}	

//Mat GRAPHICS::centerImage(const Mat& input, const Size& targetSize) {
//
//	// Resize to target size
//	Mat resized;
//	resize(input, resized, targetSize, 0, 0, INTER_AREA);
//
//	// Calculate center of mass
//	Moments m = moments(resized, true);
//	int cx = int(m.m10 / (m.m00 + 1e-5));
//	int cy = int(m.m01 / (m.m00 + 1e-5));
//
//	// Center the digit
//	int dx = resized.cols / 2 - cx;
//	int dy = resized.rows / 2 - cy;
//	Mat translationMat = Mat::eye(2, 3, CV_64F);
//	translationMat.at<double>(0, 2) = dx;
//	translationMat.at<double>(1, 2) = dy;
//
//	Mat centered;
//	warpAffine(resized, centered, translationMat, resized.size());
//
//	return centered;
//}
//
void GRAPHICS::drawTestImage(Network& _network) {
	
	cv::Mat image(400, 600, CV_8UC3, cv::Scalar(255, 255, 255)); // White background
	cv::imshow("Balkendiagramm", image);

	cv::waitKey(1);
	std::vector<double>grayValues;
	namedWindow("ZeichenfensterTest");
	setMouseCallback("ZeichenfensterTest", GRAPHICS::mouseHandler);

	while (true) {

		// Insert drawing area into main window
		window = Scalar(150);
		Mat roi = window(Rect(20, 80, CANVAS_SIZE, CANVAS_SIZE));
		canvas.copyTo(roi);

		// Text above drawing area
		std::string infoText = _network.getNetworkName();
		putText(window, infoText, Point(20, 60), FONT_ITALIC, 0.8, Scalar(0), 2);

		GRAPHICS::displaySidebarTextTest(window);

		imshow("ZeichenfensterTest", window);
		char key = (char)waitKey(1);
		if (key == 'c') {
			canvas = Scalar(0);  // Reset image to black
		}

		else if (key == 't' || key == 'T') {

			Mat resized;
			resize(canvas, resized, Size(20, 20), 0, 0, INTER_AREA);

			Moments m = moments(resized, true);
			int cx = int(m.m10 / (m.m00 + 1e-5));
			int cy = int(m.m01 / (m.m00 + 1e-5));

			int dx = resized.cols / 2 - cx;
			int dy = resized.rows / 2 - cy;
			Mat translationMat = Mat::eye(2, 3, CV_64F);
			translationMat.at<double>(0, 2) = dx;
			translationMat.at<double>(1, 2) = dy;

			Mat centered;
			warpAffine(resized, centered, translationMat, resized.size());

			for (int i = 0; i < resized.rows; ++i) {
				for (int j = 0; j < resized.cols; ++j) {
					grayValues.push_back(centered.at<uchar>(i, j) / 255.0f);
				}
			}
			_network.fillInputLayer(grayValues);
			std::vector<double>output = _network.forwardPass();
			TEST::printSortedClassification(_network.getOutputLabels(), output);
			/*Layer& outputLayer = _network.atLayer(_network.getNetworkSize() - 1);
			for (int o = 0; o < output.size(); o++) {
				std::cout << _network.getOutputLabelAt(o) << ": " << outputLayer.atNeuron(o).getOutputValue() << std::endl;
			}*/
			grayValues.clear();
		}
		else if (key == 27) break; // ESC to exit
	}
	destroyWindow("ZeichenfensterTest");
	destroyWindow("Balkendiagramm");
}
//
//int GRAPHICS::drawValidationdata(std::vector<std::vector<Neuron>>* _network, const int _counter, const int _classification) {
//
//	namedWindow("ZeichenfensterValidation");
//	setMouseCallback("ZeichenfensterValidation", GRAPHICS::mouseHandler);
//
//	while (true) {
//		// Insert drawing area into main window
//		window = Scalar(150);
//		Mat roi = window(Rect(20, 80, CANVAS_SIZE, CANVAS_SIZE));
//		canvas.copyTo(roi);
//
//		// Text above drawing area
//		std::string infoText = _network->at(1).at(_classification).getClassificationName() + " " + std::to_string(_counter + 1) + "/" +
//		std::to_string((NETWORKPROPERTIES::getIndividualSamples(_network)*30)/100);
//
//		putText(window, infoText, Point(20, 60), FONT_ITALIC, 0.8, Scalar(0), 2);
//
//		GRAPHICS::displaySidebarText(window);
//
//		imshow("ZeichenfensterValidation", window);
//		char key = (char)waitKey(1);
//
//		if (key == 's' || key == 'S') {
//
//			Mat centered = GRAPHICS::centerImage(canvas, Size(20, 20));
//
//			FILEHANDLING::writeGrayscaleToFile("Validationdata/", _network, _classification, _counter, centered);
//
//			canvas = Scalar(0);  // Reset image to black
//			break;
//
//		}
//		else if (key == 'c' || key == 'C') {
//			canvas = Scalar(0);  // Reset image to black
//		}
//		else if (key == 'b' || key == 'B') { // Go back one step in drawing loop
//			return -1; 
//		}
//		else if (key == 27) return 1;  // Exit drawing when ESC was pressed
//	}
//	return 0;
//}
//
//int GRAPHICS::drawTrainingdata(std::vector<std::vector<Neuron>>* _network, const int _counter, const int _classification) {
//
//	namedWindow("Zeichenfenster");
//	setMouseCallback("Zeichenfenster", GRAPHICS::mouseHandler);
//
//	while (true) {
//		// Insert drawing area into main window
//		window = Scalar(150);
//		Mat roi = window(Rect(20, 80, CANVAS_SIZE, CANVAS_SIZE));
//		canvas.copyTo(roi);
//
//		// Text above drawing area
//		std::string infoText = _network->at(1).at(_classification).getClassificationName() + " " + std::to_string(_counter + 1) + "/" + std::to_string(NETWORKPROPERTIES::getIndividualSamples(_network));
//		putText(window, infoText, Point(20, 60), FONT_ITALIC, 0.8, Scalar(0), 2);
//
//		GRAPHICS::displaySidebarText(window);
//
//		imshow("Zeichenfenster", window);
//		char key = (char)waitKey(1);
//
//		if (key == 's' || key == 'S') {
//
//			Mat centered = GRAPHICS::centerImage(canvas, Size(20, 20));
//			FILEHANDLING::writeGrayscaleToFile(TRAININGDATA, _network, _classification, _counter, centered);
//
//			canvas = Scalar(0);  // Reset image to black
//			break;
//
//		}
//		else if (key == 'c' || key == 'C') {
//			canvas = Scalar(0);  // Reset image to black
//		}
//		else if (key == 'b' || key == 'B') { // Go back one step in drawing loop
//			return -1;
//		}
//		else if (key == 27) return 1;  // Exit drawing when ESC was pressed
//	}
//	return 0;
//}
//
void GRAPHICS::displaySidebarTextTest(Mat& window) {

	// Text right next to drawing area
	putText(window, "T = TEST", Point(340, 100), FONT_ITALIC, 0.6, Scalar(0), 2);
	putText(window, "C = CLEAR", Point(340, 140), FONT_ITALIC, 0.6, Scalar(0), 2);
	putText(window, "RADIUS = " + std::to_string(DRAW_RADIUS), Point(340, 180), FONT_ITALIC, 0.6, Scalar(0), 2);

	cv::circle(window, Point(490, 175), DRAW_RADIUS, Scalar(0, 0, 255), FILLED);
}

//void GRAPHICS::displaySidebarText(Mat& window) {
//
//	// Draw instruction text
//	putText(window, "S = SAVE", Point(340, 100), FONT_ITALIC, 0.6, Scalar(0), 2);
//	putText(window, "C = CLEAR", Point(340, 140), FONT_ITALIC, 0.6, Scalar(0), 2);
//	putText(window, "B = ZURUECK", Point(340, 180), FONT_ITALIC, 0.6, Scalar(0), 2);
//	putText(window, "RADIUS = " + std::to_string(DRAW_RADIUS), Point(340, 220), FONT_ITALIC, 0.6, Scalar(0), 2);
//
//	// Draw brush radius indicator
//	cv::circle(window, Point(490, 215), DRAW_RADIUS, Scalar(0, 0, 255), FILLED);
//}
//
//void GRAPHICS::displayTextLoss(Mat& window, const double _accuracy, const int _counter, double _estimatedDuration, const int _epoch, const double _epsilon) {
//
//	std::ostringstream stream;
//	if (_estimatedDuration < 1) {
//		_estimatedDuration *= 60;
//		stream << std::round(_estimatedDuration) << " sec.";
//	}
//	else {
//		stream << std::round(_estimatedDuration) << " min.";
//	}
//	std::string estimatedDurationText = "Geschaetze Dauer: " + stream.str();
//
//
//	std::ostringstream oss;
//	oss << std::fixed << std::setprecision(2) << _accuracy;
//
//	cv::putText(window, "Durchschnittl. Genauigkeit: " + oss.str() + " %", cv::Point(10, 18), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//	cv::putText(window, estimatedDurationText, cv::Point(400, 18), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//
//	cv::putText(window, "Batch: " + std::to_string(_counter + 1), cv::Point(680, 220), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//
//	cv::putText(window, "Epoche:", cv::Point(10, 220), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//	cv::putText(window, std::to_string(_epoch + 1), cv::Point(100, 220), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//
//	cv::putText(window, "Lernrate:", cv::Point(10, 240), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//	cv::putText(window, std::to_string(_epsilon), cv::Point(100, 240), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//}
//
//void GRAPHICS::drawLossDiagramFrame(Mat& window, const int _graphWidth, const int _graphOffset) {
//
//	// Draw axes (y-Koordinaten + 10)
//	cv::line(window, cv::Point(_graphOffset, 30), cv::Point(_graphOffset, 190), cv::Scalar(255, 255, 255), 2); // Y-axis
//	cv::line(window, cv::Point(_graphOffset, 190), cv::Point(_graphOffset + _graphWidth, 190), cv::Scalar(255, 255, 255), 2); // X-axis
//
//	// Draw Y-axis ticks (yPos + 10)
//	for (int y = 0; y <= 100; y += 20) {
//		int yPos = 180 - y * 1.6;
//		yPos += 10;
//		cv::putText(window, std::to_string(y), cv::Point(10, yPos + 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
//		cv::line(window, cv::Point(45, yPos), cv::Point(55, yPos), cv::Scalar(255, 255, 255), 2);
//	}
//
//	// Draw y-axis lines (yPos + 10)
//	for (int y = 0; y <= 100; y += 10) {
//		int yPos = 180 - y * 1.6;
//		yPos += 10;
//		cv::line(window, cv::Point(45, yPos), cv::Point(750, yPos), cv::Scalar(100, 100, 100), 1);
//	}
//}
//
void GRAPHICS::drawBarGraph(const Network& _network) {

	cv::Mat image(400, 600, CV_8UC3, cv::Scalar(255, 255, 255)); // White backgroud
	Layer outputLayer = _network.atLayer(_network.getNetworkSize() - 1);
	int numBars = outputLayer.getLayerSize();
	int barWidth = 600 / (numBars * 2);  // Space for distance
	int spacing = barWidth; // Distance between bars
	int xOffset = (600 - (barWidth * numBars + spacing * (numBars - 1))) / 2;

	// Find index of the highest value
	int maxIndex = 0;
	double maxVal = 0.0;

	for (int i = 0; i < numBars; ++i) {
		Neuron n = outputLayer.atNeuron(i);
		if (n.getOutputValue() > maxVal) {
			maxVal = n.getOutputValue();
			maxIndex = i;
		}
	}

	// Draw bars
	for (int i = 0; i < numBars; ++i) {
		Neuron& n = outputLayer.atNeuron(i);
		double val = n.getOutputValue();
		int barHeight = static_cast<int>((val / maxVal) * (400 - 100));  // Space for labels
		int x = xOffset + i * (barWidth + spacing);
		int y = 400 - barHeight - 40;

		cv::Scalar color = (i == maxIndex) ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255); // Green or red
		cv::rectangle(image, cv::Point(x, y), cv::Point(x + barWidth, 400 - 40), color, cv::FILLED);

		// Write classificationName() below
		std::string label = _network.getOutputLabelAt(i);
		cv::putText(image, label, cv::Point(x, 400 - 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
	}

	cv::imshow("Balkendiagramm", image);
	cv::waitKey(1);
}

//void GRAPHICS::drawLoss(const double _accuracy, const int _counter, int& x, std::vector<cv::Point>& _lossPoints, std::chrono::duration<double> _duration, const int _epoch, const double _epsilon) {
//
//	cv::Mat lossGraph(300, 800, CV_8UC3, cv::Scalar(0, 0, 0));
//	const int graphWidth = 700;
//	const int graphOffset = 50;
//
//	// Format the estimated duration
//	double estimatedDuration = _duration.count();
//	
//	GRAPHICS::drawLossDiagramFrame(lossGraph, graphWidth, graphOffset);
//	GRAPHICS::displayTextLoss(lossGraph, _accuracy, _counter, estimatedDuration, _epoch, _epsilon);
//
//	// Add new point (y-Koordinate + 10)
//	_lossPoints.push_back(cv::Point(graphOffset + x, 190 - static_cast<int>(_accuracy * 1.6)));  // 180 + 10 = 190
//
//	// Scroll the graph if it reaches the end
//	if (x >= graphWidth) {
//		// Shift all points left
//		for (auto& point : _lossPoints) {
//			point.x -= 3;
//		}
//
//		// Remove points that move out of the visible area
//		if (!_lossPoints.empty() && _lossPoints.front().x < graphOffset) {
//			_lossPoints.erase(_lossPoints.begin());
//		}
//	}
//
//	// Draw loss line
//	for (size_t i = 1; i < _lossPoints.size(); ++i) {
//		cv::line(lossGraph, _lossPoints[i - 1], _lossPoints[i], cv::Scalar(0, 0, 255), 1);
//	}
//
//	// Show the graph
//	cv::imshow("Loss", lossGraph);
//	cv::waitKey(1);
//
//	// Increment X for the next point
//	if (x < graphWidth) {
//		x += 3;
//	}
//}
//
//
