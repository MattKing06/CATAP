#include "Controller.h"
#include <iostream>
Controller::Controller(std::string type, std::string name, LoggingSystem messenger)
{
    // assign constructor values to member variables of controller
    Controller::controller_type = type;
    Controller::controller_name = name;
    Controller::controller_messenger = messenger;

    // setup the messenger system to be on in Debug mode.
    if (messenger.isDebugOn())
    {
        std::cout << "DEBUGGING ON." << std::endl;
    }
    else
    {
        std::cout << "DEBUGGING OFF." << std::endl;
    }

    if (messenger.isMessagingOn())
    {
        std::cout << "MESSAGING ON" << std::endl;
    }
    else
    {
        std::cout << "MESSAGING OFF" << std::endl;
    }
}
