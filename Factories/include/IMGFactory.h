#ifndef IMG_FACTORY_H_
#define IMG_FACTORY_H_

#include <ConfigReader.h>
#include <IMG.h>
#include <GlobalStateEnums.h>
#include <vector>
class IMGFactory
{
public:
	IMGFactory();
	IMGFactory(STATE mode);
	IMGFactory(const IMGFactory& copyFactory);
	~IMGFactory();
	void setup(std::string version);
	LoggingSystem messenger;
	ConfigReader reader;
	bool hasBeenSetup;
	std::map<std::string, IMG> IMGMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // IMG_FACTORY_H_