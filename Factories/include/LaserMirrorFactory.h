#ifndef LASER_MIRROR_FACTORY_H_
#define LASER_MIRROR_FACTORY_H_

#include <ConfigReader.h>
#include <LaserMirror.h>
#include <GlobalStateEnums.h>
#include <vector>
class LaserMirrorFactory
{
public:
	LaserMirrorFactory();
	LaserMirrorFactory(STATE mode);
	LaserMirrorFactory(STATE mode, const std::string& primeLatticeLocation);
	LaserMirrorFactory(const LaserMirrorFactory& copyFactory);
	~LaserMirrorFactory();
	LoggingSystem messenger;
	ConfigReader reader;
	STATE mode;
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, LaserMirror> LaserMirrorMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // LASER_MIRROR_FACTORY_H_