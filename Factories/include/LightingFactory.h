#ifndef LIGHTING_FACTORY_H_
#define LIGHTING_FACTORY_H_

#include <ConfigReader.h>
#include <Lighting.h>
#include <GlobalStateEnums.h>
#include <vector>
class LightingFactory
{
public:
	LightingFactory();
	LightingFactory(STATE mode);
	LightingFactory(const LightingFactory& copyFactory);
	~LightingFactory();
	void setup(std::string version);
	LoggingSystem messenger;
	bool hasBeenSetup;
	std::map<std::string, Lighting> lightingMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // LIGHTING_FACTORY_H_