#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <map>
#include <iostream>
#include <LoggingSystem.h>
#ifndef PV_H_
#include <PV.h>
#endif
#include <GlobalStateEnums.h>
#include "GlobalConstants.h"

class Hardware
{
public:
	// vector of PV structs 
	// string hardware type
	// vector of strings for aliases relating to enums
	Hardware();

	// for creating "proper" objects that are defeined in a yaml file 
	Hardware(const std::map<std::string, std::string>& paramMap, STATE mode);
	// copy-contructor (forced by c++ standard due to passing in "const Hardware&" )
	Hardware(const Hardware& copyHardware);

	
	std::string getMachineArea() const;
	std::string getHardwareType() const;
	std::string getHardwareName() const;
	STATE getMode() const;

	std::map<std::string, pvStruct>& getPVStructs();
	std::map<std::string, std::string> getSpecificHardwareParameters() const;
	bool operator==(Hardware rhs);
	virtual void debugMessagesOn();
	virtual void debugMessagesOff();
	virtual void messagesOn();
	virtual void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();

// need to sort out private/protected access for these variables
	std::string machineArea;
	std::string hardwareType;
	std::string hardwareName;
	
	

	std::map<std::string, pvStruct> pvStructs;
	std::map<std::string, std::string> specificHardwareParameters;
	LoggingSystem messenger;

	STATE mode; // PHYSICAL VIRTUAL OFFLINE

};

#endif //HARDWARE_H_