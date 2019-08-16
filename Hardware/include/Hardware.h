#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <map>
#include <iostream>
#include "LoggingSystem.h"
#include "ConfigReader.h"
#ifndef PV_H_
#include "PV.h"
#endif
class Hardware
{
public:
	// vector of PV structs 
	// string hardware type
	// vector of strings for aliases relating to enums
	Hardware();
	Hardware(std::map<std::string, std::string> paramMap, bool isVirtual);
	std::string getMachineArea() const;
	std::string getHardwareType() const;
	std::map<std::string, pvStruct>& getPVStructs();
	std::map<std::string, std::string> getSpecificHardwareParameters() const;
	bool operator==(Hardware rhs);
// need to sort out private/protected access for these variables
	LoggingSystem logger;
	std::string machineArea;
	std::string hardwareType;
	bool isVirtual;
	std::map<std::string, pvStruct> pvStructs;
	std::map<std::string, std::string> specificHardwareParameters;
};
#endif //HARDWARE_H_