#include "HardwareFactory.h"

HardwareFactory::HardwareFactory() : HardwareFactory(false)
{
}
HardwareFactory::~HardwareFactory()
{
	messenger.printDebugMessage("HardwareFactory Destruction Called");
}
HardwareFactory::HardwareFactory(bool createVirtualHardwareFactory) {
	messenger.printDebugMessage("Hardware Factory Constructed");
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
			messenger.printMessage("Unable to setup MagnetFactory");
		}
	}
	return magnetFactory;

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
			messenger.printMessage("Unable to setup BPMFactory");
		}
	}
	return bpmFactory;

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
			messenger.printMessage("Unable to setup ChargeFactory");
		}
	}
	return chargeFactory;

}

void HardwareFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("HARDWARE-FAC - ", "DEBUG ON");
	magnetFactory.debugMessagesOn();
}

void HardwareFactory::debugMessagesOff()
{
	messenger.printDebugMessage("HARDWARE-FAC", "DEBUG OFF");
	messenger.debugMessagesOff();
	magnetFactory.debugMessagesOff();
}

void HardwareFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("HARDWARE-FAC - MESSAGES ON");
	magnetFactory.messagesOn();
}

void HardwareFactory::messagesOff()
{
	messenger.printMessage("HARDWARE-FAC - MESSAGES OFF");
	messenger.messagesOff();
	magnetFactory.messagesOff();
}

bool HardwareFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}

bool HardwareFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool HardwareFactory::operator==(const HardwareFactory& HardwareFactory) const
{
	/*return(HardwareFactory::HardwareFactory_name.compare(HardwareFactory.HardwareFactory_name)
		&& HardwareFactory::HardwareFactory_type.compare(HardwareFactory.HardwareFactory_type));*/
	return true;
}
