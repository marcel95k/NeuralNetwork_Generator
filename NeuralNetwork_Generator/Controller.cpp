    #include "Controller.h"

void Controller::run() {

    MenuState current = MenuState::MAIN;

    while (current != MenuState::EXIT) {
        switch (current) {

        case MenuState::ERROR:
             //current = UI::MENU::mainMenu(network);
            current = UI::DISPLAY::displayNotificationOpenCV(message);
            break;

        case MenuState::MAIN:
           //current = UI::MENU::mainMenu(network);
            current = UI::DISPLAY::MENU::displayMainMenuOpenCV(network);
            break;
        case MenuState::NEWNET:
          //  current = UI::DISPLAY::MENU::newNetMenu(network);
            current = UI::DISPLAY::MENU::displayNewNetMenuOpenCV(network);
            break;
        case MenuState::CREATE_TRAININGDATA:
            //current = UI::MENU::createTrainingdataMenu(network);
            current = UI::DISPLAY::MENU::displayCreateTrainingdataMenuOpenCV(network);
            break;
        case MenuState::SAVE:
            //current = UI::MENU::saveMenu(network);
           current = UI::DISPLAY::MENU::displaySaveMenuOpenCV(network);
            break;
        case MenuState::LOAD:
            current = UI::DISPLAY::MENU::loadMenu(network);
            break;
        case MenuState::TRAINING:
            current = UI::DISPLAY::MENU::trainingMenu(network);
            break;
        case MenuState::TEST:
            current = UI::DISPLAY::MENU::testMenu(network);
            break;
        case MenuState::INITIALIZE:
            current = UI::DISPLAY::MENU::displayInitializeMenuOpenCV(network);
           // current = UI::MENU::initializeMenu(network);
            break;
        case MenuState::SHOW_NETWORKINFO:
            current = UI::DISPLAY::MENU::showNetworkInfoMenu(network);
            break;
        case MenuState::DELETE:
           // current = UI::DISPLAY::MENU::deleteMenu(network);
            current = UI::DISPLAY::MENU::displayDeleteMenuOpenCV(network);
            break;

        case MenuState::SUB_NEWNET:
            current = UI::DISPLAY::SUBMENU::SUBNewNetMenu(network);
            break;
        case MenuState::SUB_CREATE_TRAININGDATA:
            current = UI::DISPLAY::SUBMENU::SUBcreateTrainingdataMenu(network);
            break;
        case MenuState::SUB_CREATE_VALIDATIONDATA:
            current = UI::DISPLAY::SUBMENU::SUBcreateValidationdataMenu(network);
            break;
        case MenuState::SUB_SAVE:
            current = UI::DISPLAY::SUBMENU::SUBSaveNetworkMenu(network);
            break;
        case MenuState::SUB_SAVE_AS:
            current = UI::DISPLAY::SUBMENU::SUBSaveNetworkAsMenu(network);
            break;
        case MenuState::SUB_LOAD:
            current = UI::DISPLAY::SUBMENU::SUBLoadNetworkMenu(network);
            break;
        case MenuState::SUB_TRAIN_DEFAULT:
            current = UI::DISPLAY::SUBMENU::SUBDefaultTrainingMenu(network);
            break;
        case MenuState::SUB_TRAIN_CUSTOM:
            current = UI::DISPLAY::SUBMENU::SUBCustomTrainingMenu(network);
            break;
        case MenuState::SUB_TEST:
            current = UI::DISPLAY::SUBMENU::SUBTestMenu(network);
            break;
        case MenuState::SUB_INITIALIZE:
            current = UI::DISPLAY::SUBMENU::SUBInitializeMenu(network);
            break;
        case MenuState::SUB_DELETE:
            current = UI::DISPLAY::SUBMENU::SUBDeleteMenu(network);
            break;
        default:
            current = MenuState::EXIT;
        }
    }
}

Network& Controller::getNetwork() { return network; }
std::string& Controller::getMessage() { return message; }
