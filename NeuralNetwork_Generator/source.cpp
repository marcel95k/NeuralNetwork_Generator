#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

#include "Defines.h"
#include "ErrorHandling.h"
#include "FileHandling.h"
#include "MathFunctions.h"
#include "ExceptionHandling.h"
#include "Test.h"
#include "UI.h"
#include "Training.h"
#include "Graphics.h"
#include "Network.h"
#include "Controller.h"


int main() {

    Controller controller;
    controller.run();

    return 0;
}