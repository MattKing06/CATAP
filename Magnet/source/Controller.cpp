#include "Controller.h"
#include <iostream>

Controller::Controller(){
	Controller::controller_type = "None";
	Controller::controller_name = "None";
	Controller::controller_messenger = LoggingSystem();
}

Controller::Controller(std::string type, std::string name)
{
	Controller::controller_type = type;
	Controller::controller_name = name;
	Controller::controller_messenger = LoggingSystem(true, true);
}

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

bool Controller::operator==(const Controller &controller) const
{
	return(Controller::controller_name.compare(controller.controller_name)
		&& Controller::controller_type.compare(controller.controller_type));
}