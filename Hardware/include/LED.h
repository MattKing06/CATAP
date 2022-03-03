#ifndef LED_H_
#define LED_H_

#include <Hardware.h>
#include <EPICSLEDInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSLEDInterface;

typedef boost::shared_ptr<EPICSLEDInterface> EPICSLEDInterface_sptr;

class LED : public Hardware
{
public:
	LED();
	LED(const std::map<std::string, std::string>& paramMap, STATE mode);
	LED(const LED& copyLED);
	~LED();
	void setPVStructs();
	EPICSLEDInterface_sptr epicsInterface;
	/*! Attach to the initial CA Context for use in Multi-threaded applications*/
	void attachToInitialContext();
	/*! Detach from initial CA Context for use in Multi-threaded applications*/
	void detachFromInitialContext();
	std::map<std::string, std::string> LEDParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //LED_H_