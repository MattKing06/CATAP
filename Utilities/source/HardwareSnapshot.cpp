#include <HardwareSnapshot.h>

HardwareSnapshot::HardwareSnapshot()
{
}

HardwareSnapshot::HardwareSnapshot(const HardwareSnapshot& copyHardwareSnapshot)
	: state(copyHardwareSnapshot.state)
{
}

HardwareSnapshot::~HardwareSnapshot()
{
}






std::map<std::string, std::string> HardwareSnapshot::readStateFromYaml()
{
	return std::map<std::string, std::string>();
}

void HardwareSnapshot::writeStateFromMap(std::map<std::string, std::string> mapToWrite)
{
}
