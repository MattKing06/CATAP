#include "HardwareFactory.h"

HardwareFactory::HardwareFactory() : HardwareFactory(false)
{
}
HardwareFactory::~HardwareFactory()
{
	messenger.messagesOn();
	messenger.printMessage("HardwareFactory Destruction Called");
}
HardwareFactory::HardwareFactory(bool createVirtualHardwareFactory){
	messenger = LoggingSystem(false, false);
	messenger.printDebugMessage(std::string("Hardware Factory Constructed"));
	isVirtual = createVirtualHardwareFactory;
	magnetFactory = MagnetFactory(isVirtual);
}
bool HardwareFactory::setup(const std::string& hardwareType, const std::string& version)
{
	bool setup = false;
	if (hardwareType == "Magnet")
	{
		if (!magnetFactory.hasBeenSetup)
		{
			setup = magnetFactory.setup(version);
		}
	}
	return setup;
}
MagnetFactory& HardwareFactory::getMagnetFactory()
{
	if (!magnetFactory.hasBeenSetup)
	{
		bool setup = magnetFactory.setup("nominal");
		if (setup)
		{
			return magnetFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup MagnetFactory");
		}
	}
	else
	{
		return magnetFactory;
	}

}
bool HardwareFactory::operator==(const HardwareFactory &HardwareFactory) const
{
	/*return(HardwareFactory::HardwareFactory_name.compare(HardwareFactory.HardwareFactory_name)
		&& HardwareFactory::HardwareFactory_type.compare(HardwareFactory.HardwareFactory_type));*/
	return true;
}