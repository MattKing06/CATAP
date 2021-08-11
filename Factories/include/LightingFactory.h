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
	bool setup(std::string version);
	LoggingSystem messenger;
	bool hasBeenSetup;


	std::map<std::string, STATE> getLEDState(const std::string& name)const;
	std::map<std::string, STATE> getLightingState(const std::string& name)const;
	bool allLEDOn(const std::string& name);
	bool allLEDOff(const std::string& name);
	STATE getClaraLEDState(const std::string& name)const;
	bool setClaraLEDOn(const std::string& name);
	bool setClaraLEDOff(const std::string& name);
	bool isClaraLEDOn(const std::string& name)const;
	bool isClaraLEDOff(const std::string& name)const;
	bool toggleClaraLED(const std::string& name);
	STATE getVelaLEDState(const std::string& name)const;
	bool setVelaLEDOn(const std::string& name);
	bool setVelaLEDOff(const std::string& name);
	bool isVelaLEDOn(const std::string& name)const;
	bool isVelaLEDOff(const std::string& name)const;
	bool toggleVelaLED(const std::string& name);

	std::string getFullName(const std::string& name)const;

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();

private:
	
	void setMonitorStatus(pvStruct& pvStruct);
	void setupChannels();

	std::map<std::string, Lighting> lightingMap;


	// private
	ConfigReader reader;

};

#endif // LIGHTING_FACTORY_H_