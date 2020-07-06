#ifndef HARDWARE_STATE_H
#define HARDWARE_STATE_H

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include <map>
#include <string>

class HardwareState
{
public:
	HardwareState();
	HardwareState(const HardwareState& copyHardwareState);
	~HardwareState();
	// Probably need to find the most appropriate map structure for nested states.
	std::map<std::string, std::string> readStateFromYaml();
	void writeStateFromMap(std::map<std::string, std::string> mapToWrite);
};




#endif