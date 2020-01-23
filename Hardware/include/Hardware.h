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
	// copy-cOntructor (forced by c++ standard due to passing in "const Hardware&" )
	Hardware(const Hardware& copyHardware);

	
	std::string getMachineArea() const;
	std::string getHardwareType() const;
	std::string getHardwareName() const;
	STATE getMode() const;

	// TODO: do we need this? can't an child of these class just access pvStructs,
	// and no other class should be able ot get this map??? 
	std::map<std::string, pvStruct>& getPVStructs();
	// TODO  for some reaons i think these should be keyed by an enum giving their type 
	//std::map<int, pvStruct>& getPVStructs2();

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
	
	// this will be an enum in the dervied class ...
	std::map<int, pvStruct> pvStructs2;

	std::map<std::string, std::string> specificHardwareParameters;
	LoggingSystem messenger;

	STATE mode; // PHYSICAL VIRTUAL OFFLINE

};

#endif //HARDWARE_H_