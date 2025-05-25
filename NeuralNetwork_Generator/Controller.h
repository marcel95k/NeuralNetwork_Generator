#pragma once
#include "Network.h"
#include "UI.h"

class Controller {

private:
    Network network; 

public:
    void run();
    Network& getNetwork();
};

