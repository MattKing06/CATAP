#ifndef LASER_MIRROR_FACTORY_H_
#define LASER_MIRROR_FACTORY_H_

#include <ConfigReader.h>
#include <LaserMirror.h>
#include <GlobalStateEnums.h>
#include <GlobalFunctions.h>
#include <vector>
class LaserMirrorFactory
{
public:
	LaserMirrorFactory();
	LaserMirrorFactory(STATE mode);
	LaserMirrorFactory(const LaserMirrorFactory& copyFactory);
	~LaserMirrorFactory();
	
	bool setup(const std::string& version);
	void populateLaserMirrorMap();
	void retrieveMonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	LaserMirror& getLaserMirror(const std::string& name);
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
	LoggingSystem messenger;
	ConfigReader reader;
	bool hasBeenSetup;
	STATE mode;
	std::map<std::string, LaserMirror> laserMirrorMap;
};

#endif // LASER_MIRROR_FACTORY_H_