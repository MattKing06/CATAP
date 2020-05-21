#ifndef CAMERA_FACTORY_H_
#define CAMERA_FACTORY_H_

#include <ConfigReader.h>
#include <Camera.h>
#include <GlobalStateEnums.h>
#include <vector>
class CameraFactory
{
public:
	CameraFactory();
	CameraFactory(STATE mode);
	CameraFactory(const CameraFactory& copyFactory);
	~CameraFactory();
	LoggingSystem messenger;
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, Camera> CameraMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // CAMERA_FACTORY_H_