#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <string>
#include <tuple>
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
	// vector of PV structs 
	// string fullname (PV ROOT)
	// string hardware type
	// vector of strings for aliases relating to enums
	LoggingSystem logger;
	std::string machineArea;
	std::vector<pvStruct> pvStructs;
	std::vector< std::tuple<std::string, std::string> > specificHardwareParameters;
};

#endif //HARDWARE_H_