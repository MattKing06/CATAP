#ifndef SHUTTER_FACTORY_H_
#define SHUTTER_FACTORY_H_

#include <ConfigReader.h>
#include <Shutter.h>
#include <GlobalStateEnums.h>
#include <vector>
class ShutterFactory
{
public:
	ShutterFactory();
	ShutterFactory(STATE mode);
	ShutterFactory(const ShutterFactory& copyFactory);
	~ShutterFactory();
	void setup(std::string version);
	LoggingSystem messenger;
	bool hasBeenSetup;
	std::map<std::string, Shutter> shutterMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // SHUTTER_FACTORY_H_