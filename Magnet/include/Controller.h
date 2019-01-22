#ifndef CONTROLLER_H_
#define CONTROLER_H_

#include "LoggingSystem.h"
class Controller
{
    //what does a controller need to be a controller:
    /*
    *   - A Hardware Controller Type (ENUM/DEFINES?)
    *   - A Hardware Controller Name (string/ENUM?)
    *   - Needs to invoke LoggingSystem object without having it as an inherited member.
    *
    */
public:
    std::string controller_type;
    std::string controller_name;
    LoggingSystem controller_messenger;
	Controller();
    Controller(std::string controller_type, std::string controller_name, LoggingSystem controller_messenger);
	Controller(std::string controller_type, std::string controller_name);
	bool operator ==(const Controller &controller) const;
};


#endif // CONTROLLER_H_
