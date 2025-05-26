#include "Controller.h"

void Controller::run() {

    MenuState current = MenuState::MAIN;

    while (current != MenuState::EXIT) {
        switch (current) {
        case MenuState::MAIN:
            current = UI::MENU::mainMenu(network);
            break;
        case MenuState::NEWNET:
            current = UI::MENU::newNetMenu(network);
            break;
        case MenuState::CREATE_TRAININGDATA:
            current = UI::MENU::createTrainingdataMenu(network);
            break;
        case MenuState::SAVE:
            current = UI::MENU::saveMenu(network);
            break;
        case MenuState::LOAD:
            current = UI::MENU::loadMenu(network);
            break;
        case MenuState::TRAINING:
            current = UI::MENU::trainingMenu(network);
            break;
        case MenuState::TEST:
            current = UI::MENU::testMenu(network);
            break;
        case MenuState::VALIDATE:
            current = UI::MENU::validationCheckMenu(network);
            break;
        case MenuState::INITIALIZE:
            current = UI::MENU::initializeMenu(network);
            break;
        case MenuState::DELETE:
            current = UI::MENU::deleteMenu(network);
            break;

        case MenuState::SUB_CREATE_TRAININGDATA:
            current = UI::MENU::CREATETRAININGDATA::SUBcreateTrainingdataMenu(network);
            break;
        case MenuState::SUB_CREATE_VALIDATIONDATA:
            current = UI::MENU::CREATETRAININGDATA::SUBcreateValidationdataMenu(network);
            break;
        case MenuState::SUB_SAVE:
            current = UI::MENU::SAVE::SUBSaveNetwork(network);
            break;
        case MenuState::SUB_SAVE_AS:
            current = UI::MENU::SAVE::SUBSaveNetworkAs(network);
            break;
        case MenuState::SUB_LOAD:
            current = UI::MENU::LOAD::SUBLoadNetwork(network);
            break;

        default:
            current = MenuState::EXIT;
        }
    }
}

Network& Controller::getNetwork() { return network; }
