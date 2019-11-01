#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <map>
#include <iostream>
#include <LoggingSystem.h>
#ifndef PV_H_
#include <PV.h>
#endif
#include <GlobalStateEnums.h>
class Hardware
{
public:
	// vector of PV structs 
	// string hardware type
	// vector of strings for aliases relating to enums
	Hardware();
	Hardware(const std::map<std::string, std::string>& paramMap, bool isVirtual);
	std::string getMachineArea() const;
	std::string getHardwareType() const;
	std::string getHardwareName() const;
	Hardware(const Hardware& copyHardware);
	std::map<std::string, pvStruct>& getPVStructs();
	std::map<std::string, std::string> getSpecificHardwareParameters() const;
	bool operator==(Hardware rhs);
// need to sort out private/protected access for these variables
	LoggingSystem messenger;
	std::string machineArea;
	std::string hardwareType;
	std::string hardwareName;
	bool isVirtual;
	std::map<std::string, pvStruct> pvStructs;
	std::map<std::string, std::string> specificHardwareParameters;
};

#endif //HARDWARE_H_