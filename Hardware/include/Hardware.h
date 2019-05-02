#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <string>
#include <map>
#include <vector>
#include "LoggingSystem.h"
#include "ConfigReader.h"

//epics
#ifndef __CINT__
#include <cadef.h>
#endif

struct pvStruct
{
	chid CHID;
	std::string fullPVName;
	std::string pvRecord;
	unsigned long COUNT;
	unsigned long MASK;
	chtype CHTYPE;
};

class Hardware
{
public:
	// vector of PV structs 
	// string hardware type
	// vector of strings for aliases relating to enums
	Hardware();
	Hardware(std::multimap<std::string, std::string> paramMap);
	std::string getMachineArea();
	std::string getHardwareType();
	std::vector<pvStruct> getPVStructs();
	std::multimap<std::string, std::string> getSpecificHardwareParameters();
protected:
	LoggingSystem *logger;
	std::string machineArea;
	std::string hardwareType;
	std::vector<pvStruct> pvStructs;
	std::multimap<std::string, std::string> specificHardwareParameters;
};

#endif //HARDWARE_H_