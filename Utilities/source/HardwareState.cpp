#include <HardwareState.h>

HardwareState::HardwareState()
{
}

HardwareState::HardwareState(const HardwareState& copyHardwareState)
{
}

HardwareState::~HardwareState()
{
}

std::map<std::string, std::string> HardwareState::readStateFromYaml()
{
	return std::map<std::string, std::string>();
}

void HardwareState::writeStateFromMap(std::map<std::string, std::string> mapToWrite)
{
}
