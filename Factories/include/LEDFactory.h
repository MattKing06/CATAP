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
	/*! Attach to the initial CA context for multi-threaded applications for a given LED
	@param[in] LEDName: Name of the LED to attach to CA Context*/
	void attachContext(const std::string& LEDName);
	/*! Attach to the initial CA context for multi-threaded applications for given LEDs
	@param[in] LEDNames: Names of the LEDs to attach to CA Context*/
	void attachContext(std::vector<std::string>& LEDNames);
	/*! Attach to the initial CA context for multi-threaded applications for given LEDs
	@param[in] LEDNames: List of names of the LEDs to attach to CA Context*/
	void attachContext_Py(boost::python::list LEDNames);
	/*! Attach to the initial CA context for multi-threaded applications for all LEDs*/
	void attachContext();

	/*! detach to the initial CA context for multi-threaded applications for a given LED
	@param[in] LEDName: Name of the LED to detach to CA Context*/
	void detachContext(const std::string& LEDName);
	/*! detach to the initial CA context for multi-threaded applications for given LEDs
	@param[in] LEDNames: Names of the LEDs to detach to CA Context*/
	void detachContext(std::vector<std::string>& LEDNames);
	/*! detach to the initial CA context for multi-threaded applications for given LEDs
	@param[in] LEDNames: List of names of the LEDs to detach to CA Context*/
	void detachContext_Py(boost::python::list LEDNames);
	/*! detach to the initial CA context for multi-threaded applications for all LEDs*/
	void detachContext();
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