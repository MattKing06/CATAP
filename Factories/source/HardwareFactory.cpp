#include "HardwareFactory.h"

HardwareFactory::HardwareFactory() : HardwareFactory(false)
{
}
HardwareFactory::~HardwareFactory()
{
	printDebugMessage("HardwareFactory Destruction Called");
}
HardwareFactory::HardwareFactory(bool createVirtualHardwareFactory) {
	//messenger = LoggingSystem(false, false);
	printDebugMessage("Hardware Factory Constructed");
	isVirtual = createVirtualHardwareFactory;
	magnetFactory = MagnetFactory(isVirtual);
	bpmFactory = BPMFactory(isVirtual);
	chargeFactory = ChargeFactory(isVirtual);
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
	else if (hardwareType == "BPM")
	{
		if (!bpmFactory.hasBeenSetup)
		{
			setup = bpmFactory.setup(version);
		}
	}
	else if (hardwareType == "Charge")
	{
		if (!chargeFactory.hasBeenSetup)
		{
			setup = chargeFactory.setup(version);
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
			printMessage("Unable to setup MagnetFactory");
		}
	}
	else
	{
		return magnetFactory;
	}

}
BPMFactory& HardwareFactory::getBPMFactory()
{
	if (!bpmFactory.hasBeenSetup)
	{
		bool setup = bpmFactory.setup("nominal");
		if (setup)
		{
			return bpmFactory;
		}
		else
		{
			printMessage("Unable to setup BPMFactory");
		}
	}
	else
	{
		return bpmFactory;
	}

}
ChargeFactory& HardwareFactory::getChargeFactory()
{
	if (!chargeFactory.hasBeenSetup)
	{
		bool setup = chargeFactory.setup("nominal");
		if (setup)
		{
			return chargeFactory;
		}
		else
		{
			printMessage("Unable to setup ChargeFactory");
		}
	}
	else
	{
		return chargeFactory;
	}

}

bool HardwareFactory::operator==(const HardwareFactory& HardwareFactory) const
{
	/*return(HardwareFactory::HardwareFactory_name.compare(HardwareFactory.HardwareFactory_name)
		&& HardwareFactory::HardwareFactory_type.compare(HardwareFactory.HardwareFactory_type));*/
	return true;
}

//void HardwareFactory::debugMessagesOn()
//{
//	debugMessagesOn();
//	printDebugMessage("HARDWARE-FAC - ", "DEBUG ON");
//	magnetFactory.debugMessagesOn();
//}
//
//void HardwareFactory::debugMessagesOff()
//{
//	printDebugMessage("HARDWARE-FAC", "DEBUG OFF");
//	debugMessagesOff();
//	magnetFactory.debugMessagesOff();
//}
//
//void HardwareFactory::messagesOn()
//{
//	messagesOn();
//	printMessage("HARDWARE-FAC - MESSAGES ON");
//	magnetFactory.messagesOn();
//}
//
//void HardwareFactory::messagesOff()
//{
//	printMessage("HARDWARE-FAC - MESSAGES OFF");
//	messagesOff();
//	magnetFactory.messagesOff();
//}
//
//bool HardwareFactory::isMessagingOn()
//{
//	return isMessagingOn();
//}
//
//bool HardwareFactory::isDebugOn()
//{
//	return isDebugOn();
//}
