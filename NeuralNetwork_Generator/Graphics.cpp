#include "Graphics.h"

const int WINDOW_HEIGHT = 400;
const int WINDOW_WIDTH = 600;
const int CANVAS_SIZE = 300;
int DRAW_RADIUS = 7;

Mat window(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC1, Scalar(255));  // White background
Mat canvas(CANVAS_SIZE, CANVAS_SIZE, CV_8UC1, Scalar(0));       // Black drawing area

void GRAPHICS::draw(int x, int y) {
    circle(canvas, Point(x, y), DRAW_RADIUS, Scalar(255), FILLED);  // Draw white
}

void GRAPHICS::erase(int x, int y) {
    circle(canvas, Point(x, y), DRAW_RADIUS, Scalar(0), FILLED);    // Draw black (erase)
}

int GRAPHICS::processKeyPressValidation(const int _status, int& _counter, int& _classification, const int _amountOfIndividualClassifications) {

	if (_status == 1) { // ESC was pressed
		return 1;
	}
	else if (_status == -1) {	// 'b' was pressed: back to previous individual or previous classification	
		if (_counter > 0) { _counter--; }
		else if (_classification > 0) {
			_classification--;
			_counter = _amountOfIndividualClassifications - 1;
		}
	}
	else {	// Continue to next individual
		_counter++;
		if (_counter == _amountOfIndividualClassifications) {	// Continue to next classifiction when _amountOfIndividualClassifications is reached
			_classification++;
			_counter = 0;
		}
	}
}

int GRAPHICS::processKeyPressTraining(const int _status, int& _counter, int& _classification, const int _amountOfIndividualClassifications) {

	if (_status == 1) { // ESC was pressed
		return 1;
	}
	else if (_status == -1) {	// 'b' was pressed: back to previous individual or previous classification	
		if (_counter > 0) { _counter--; }
		else if (_classification > 0) {
			_classification--;
			_counter = _amountOfIndividualClassifications -1;
		}
	}
	else {	// Continue to next individual
		_counter++;
		if (_counter == _amountOfIndividualClassifications) {	// Continue to next classifiction when _amountOfIndividualClassifications is reached
			_classification++;
			_counter = 0;
		}
	}
}

void GRAPHICS::mouseHandler(int event, int x, int y, int flags, void*) {
	
	static bool drawing = false;
	static bool erasing = false;

	// Convert coordinates to drawing area
	int localX = x - 20;
	int localY = y - 80;

	if (event == EVENT_MOUSEWHEEL) {	// Changing the DRAW_RADIUS
		int direction = getMouseWheelDelta(flags);
		DRAW_RADIUS += (direction > 0 ? 2 : -2);
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

Mat GRAPHICS::centerImage(const Mat& input, const Size& targetSize) {

	// Resize to target size
	Mat resized;
	resize(input, resized, targetSize, 0, 0, INTER_AREA);

	// Calculate center of mass
	Moments m = moments(resized, true);
	int cx = int(m.m10 / (m.m00 + 1e-5));
	int cy = int(m.m01 / (m.m00 + 1e-5));

	// Center the digit
	int dx = resized.cols / 2 - cx;
	int dy = resized.rows / 2 - cy;
	Mat translationMat = Mat::eye(2, 3, CV_32F);
	translationMat.at<float>(0, 2) = dx;
	translationMat.at<float>(1, 2) = dy;

	Mat centered;
	warpAffine(resized, centered, translationMat, resized.size());

	return centered;
}

void GRAPHICS::drawTestImage(std::vector<std::vector<Neuron>>* _network) {
	
	cv::Mat image(400, 600, CV_8UC3, cv::Scalar(255, 255, 255)); // White background
	cv::imshow("Balkendiagramm", image);
	
	cv::waitKey(1);
	std::vector<float>grayValues;
	namedWindow("ZeichenfensterTest");
	setMouseCallback("ZeichenfensterTest", GRAPHICS::mouseHandler);

	while (true) {

		// Insert drawing area into main window
		window = Scalar(150);
		Mat roi = window(Rect(20, 80, CANVAS_SIZE, CANVAS_SIZE));
		canvas.copyTo(roi);

		// Text above drawing area
		std::string infoText = _network->at(0).at(0).getNetworkName();
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
			Mat translationMat = Mat::eye(2, 3, CV_32F);
			translationMat.at<float>(0, 2) = dx;
			translationMat.at<float>(1, 2) = dy;

			Mat centered;
			warpAffine(resized, centered, translationMat, resized.size());

			for (int i = 0; i < resized.rows; ++i) {
				for (int j = 0; j < resized.cols; ++j) {
					grayValues.push_back(centered.at<uchar>(i, j) / 255.0f);
				}
			}

			TEST::test(_network, &grayValues);
			grayValues.clear();
		}
		else if (key == 27) break; // ESC to exit
	}
	destroyWindow("ZeichenfensterTest");
	destroyWindow("Balkendiagramm");
}

int GRAPHICS::drawValidationdata(std::vector<std::vector<Neuron>>* _network, const int _counter, const int _classification) {

	namedWindow("ZeichenfensterValidation");
	setMouseCallback("ZeichenfensterValidation", GRAPHICS::mouseHandler);

	while (true) {
		// Insert drawing area into main window
		window = Scalar(150);
		Mat roi = window(Rect(20, 80, CANVAS_SIZE, CANVAS_SIZE));
		canvas.copyTo(roi);

		// Text above drawing area
		std::string infoText = _network->at(1).at(_classification).getClassificationName() + " " + std::to_string(_counter + 1) + "/" +
		std::to_string((_network->at(0).at(0).getIndividualClassifications()*30)/100);

		putText(window, infoText, Point(20, 60), FONT_ITALIC, 0.8, Scalar(0), 2);

		GRAPHICS::displaySidebarText(window);

		imshow("ZeichenfensterValidation", window);
		char key = (char)waitKey(1);

		if (key == 's' || key == 'S') {

			Mat centered = GRAPHICS::centerImage(canvas, Size(20, 20));

			FILEHANDLING::writeGrayscaleToFile("Validationdata/", _network, _classification, _counter, centered);

			canvas = Scalar(0);  // Reset image to black
			break;

		}
		else if (key == 'c' || key == 'C') {
			canvas = Scalar(0);  // Reset image to black
		}
		else if (key == 'b' || key == 'B') { // Go back one step in drawing loop
			return -1; 
		}
		else if (key == 27) return 1;  // Exit drawing when ESC was pressed
	}
}

int GRAPHICS::drawTrainingdata(std::vector<std::vector<Neuron>>* _network, const int _counter, const int _classification) {

	namedWindow("Zeichenfenster");
	setMouseCallback("Zeichenfenster", GRAPHICS::mouseHandler);

	while (true) {
		// Insert drawing area into main window
		window = Scalar(150);
		Mat roi = window(Rect(20, 80, CANVAS_SIZE, CANVAS_SIZE));
		canvas.copyTo(roi);

		// Text above drawing area
		std::string infoText = _network->at(1).at(_classification).getClassificationName() + " " + std::to_string(_counter + 1) + "/" + std::to_string(_network->at(0).at(0).getIndividualClassifications());
		putText(window, infoText, Point(20, 60), FONT_ITALIC, 0.8, Scalar(0), 2);

		GRAPHICS::displaySidebarText(window);

		imshow("Zeichenfenster", window);
		char key = (char)waitKey(1);

		if (key == 's' || key == 'S') {

			Mat centered = GRAPHICS::centerImage(canvas, Size(20, 20));
			FILEHANDLING::writeGrayscaleToFile(TRAININGDATA, _network, _classification, _counter, centered);

			canvas = Scalar(0);  // Reset image to black
			break;

		}
		else if (key == 'c' || key == 'C') {
			canvas = Scalar(0);  // Reset image to black
		}
		else if (key == 'b' || key == 'B') { // Go back one step in drawing loop
			return -1;
		}
		else if (key == 27) return 1;  // Exit drawing when ESC was pressed
	}
}

void GRAPHICS::displaySidebarTextTest(Mat& window) {

	// Text right next to drawing area
	putText(window, "T = TEST", Point(340, 100), FONT_ITALIC, 0.6, Scalar(0), 2);
	putText(window, "C = CLEAR", Point(340, 140), FONT_ITALIC, 0.6, Scalar(0), 2);
	putText(window, "RADIUS = " + std::to_string(DRAW_RADIUS), Point(340, 180), FONT_ITALIC, 0.6, Scalar(0), 2);

	cv::circle(window, Point(490, 175), DRAW_RADIUS, Scalar(0, 0, 255), FILLED);
}

void GRAPHICS::displaySidebarText(Mat& window) {

	// Draw instruction text
	putText(window, "S = SAVE", Point(340, 100), FONT_ITALIC, 0.6, Scalar(0), 2);
	putText(window, "C = CLEAR", Point(340, 140), FONT_ITALIC, 0.6, Scalar(0), 2);
	putText(window, "B = ZURUECK", Point(340, 180), FONT_ITALIC, 0.6, Scalar(0), 2);
	putText(window, "RADIUS = " + std::to_string(DRAW_RADIUS), Point(340, 220), FONT_ITALIC, 0.6, Scalar(0), 2);

	// Draw brush radius indicator
	cv::circle(window, Point(490, 215), DRAW_RADIUS, Scalar(0, 0, 255), FILLED);
}

void GRAPHICS::drawBarGraph(std::vector<Neuron>* outputNeurons, int width, int height) {
	cv::Mat image(height, width, CV_8UC3, cv::Scalar(255, 255, 255)); // White backgroud

	int numBars = outputNeurons->size();
	int barWidth = width / (numBars * 2);  // Space for distance
	int spacing = barWidth; // Distance between bars
	int xOffset = (width - (barWidth * numBars + spacing * (numBars - 1))) / 2;

	// Find index of the highest value
	int maxIndex = 0;
	double maxVal = 0.0;

	for (int i = 0; i < numBars; ++i) {
		if (outputNeurons->at(i).getOutputValue() > maxVal) {
			maxVal = outputNeurons->at(i).getOutputValue();
			maxIndex = i;
		}
	}

	// Draw bars
	for (int i = 0; i < numBars; ++i) {
		double val = outputNeurons->at(i).getOutputValue();
		int barHeight = static_cast<int>((val / maxVal) * (height - 100));  // Space for labels
		int x = xOffset + i * (barWidth + spacing);
		int y = height - barHeight - 40;

		cv::Scalar color = (i == maxIndex) ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255); // Green or red
		cv::rectangle(image, cv::Point(x, y), cv::Point(x + barWidth, height - 40), color, cv::FILLED);

		// Write classificationName() below
		std::string label = outputNeurons->at(i).getClassificationName();
		cv::putText(image, label, cv::Point(x, height - 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
	}

	cv::imshow("Balkendiagramm", image);
	cv::waitKey(1);
}

void GRAPHICS::drawLoss(const double _accuracy, int& x, std::vector<cv::Point>& _lossPoints, std::chrono::duration<double> _duration) {
	cv::Mat lossGraph(200, 800, CV_8UC3, cv::Scalar(0, 0, 0));
	const int graphWidth = 700;
	const int graphOffset = 50;

	// Format the estimated duration
	double estimatedDuration = _duration.count();
	std::ostringstream stream;
	if (estimatedDuration < 1) {
		estimatedDuration *= 60;
		stream << std::round(estimatedDuration) << " sec.";
	}
	else {
		stream << std::round(estimatedDuration) << " min.";
	}
	std::string estimatedDurationText = "Geschaetze Dauer: " + stream.str();

	// Draw axes
	cv::line(lossGraph, cv::Point(graphOffset, 20), cv::Point(graphOffset, 180), cv::Scalar(255, 255, 255), 2); // Y-axis
	cv::line(lossGraph, cv::Point(graphOffset, 180), cv::Point(graphOffset + graphWidth, 180), cv::Scalar(255, 255, 255), 2); // X-axis

	// Add labels
	cv::putText(lossGraph, "Genauigkeit (%)", cv::Point(10, 12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
	cv::putText(lossGraph, "Batch", cv::Point(700, 195), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
	cv::putText(lossGraph, estimatedDurationText, cv::Point(400, 12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

	// Draw Y-axis ticks
	for (int y = 0; y <= 100; y += 20) {
		int yPos = 180 - y * 1.6;
		cv::putText(lossGraph, std::to_string(y), cv::Point(10, yPos + 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
		cv::line(lossGraph, cv::Point(45, yPos), cv::Point(55, yPos), cv::Scalar(255, 255, 255), 2);
	}

	// Draw y-axis lines
	for (int y = 0; y <= 100; y += 10) {
		int yPos = 180 - y * 1.6;
		cv::line(lossGraph, cv::Point(45, yPos), cv::Point(750, yPos), cv::Scalar(100, 100, 100), 1);
	}

	// Add new point
	_lossPoints.push_back(cv::Point(graphOffset + x, 180 - static_cast<int>(_accuracy * 1.6)));

	// Scroll the graph if it reaches the end
	if (x >= graphWidth) {
		// Shift all points left
		for (auto& point : _lossPoints) {
			point.x -= 20;
		}

		// Remove points that move out of the visible area
		if (!_lossPoints.empty() && _lossPoints.front().x < graphOffset) {
			_lossPoints.erase(_lossPoints.begin());
		}
	}

	// Draw loss line
	for (size_t i = 1; i < _lossPoints.size(); ++i) {
		cv::line(lossGraph, _lossPoints[i - 1], _lossPoints[i], cv::Scalar(0, 0, 255), 1);
	}

	// Show the graph
	cv::imshow("Loss", lossGraph);
	cv::waitKey(1);

	// Increment X for the next point
	if (x < graphWidth) {
		x += 20;
	}
}


