#undef main
#include <iostream>
#include "Controller.h"

int main()
{
    LoggingSystem logger(true, true);
    std::string type = "Magnet";
    std::string name = "MAG-DIP-01";

    Controller new_controller(type, name, logger);
	std::cout << new_controller.controller_messenger.isDebugOn() << std::endl;
	std::string message_to_print = "Hello World";
	new_controller.controller_messenger.printDebugMessage(message_to_print);
	return 0;
}
