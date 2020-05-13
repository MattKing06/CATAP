#ifndef LED_FACTORY_H_
#define LED_FACTORY_H_

#include <ConfigReader.h>
#include <LED.h>
#include <GlobalStateEnums.h>
#include <vector>
class LEDFactory
{
public:
	LEDFactory();
	LEDFactory(STATE mode);
	LEDFactory(const LEDFactory& copyFactory);
	~LEDFactory();
	LoggingSystem messenger;
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, LED> LEDMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // LED_FACTORY_H_