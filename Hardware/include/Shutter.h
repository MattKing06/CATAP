#ifndef SHUTTER_H_
#define SHUTTER_H_

#include <Hardware.h>
#include <EPICSShutterInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSShutterInterface;

typedef boost::shared_ptr<EPICSShutterInterface> EPICSShutterInterface_sptr;

class Shutter : public Hardware
{
public:
	Shutter();
	Shutter(const std::map<std::string, std::string>& paramMap, STATE mode);
	Shutter(const Shutter& copyShutter);
	~Shutter();
	void setPVStructs();
	EPICSShutterInterface_sptr epicsInterface;
	std::map<std::string, std::string> ShutterParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //SHUTTER_H_