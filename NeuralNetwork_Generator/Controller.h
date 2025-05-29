#pragma once
#include <string>

#include "Network.h"
#include "UI.h"

class Controller {

private:
    Network network; 
    std::string message;

public:
    void run();
    Network& getNetwork();
    std::string& getMessage();
};

