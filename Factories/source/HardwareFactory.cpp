#include "HardwareFactory.h"

HardwareFactory::HardwareFactory() : HardwareFactory(STATE::OFFLINE)
{
}
HardwareFactory::~HardwareFactory()
{
	messenger.printDebugMessage("HardwareFactory DestructiOn Called");
}
HardwareFactory::HardwareFactory(STATE mode) :
	magnetFactory(MagnetFactory(mode)),
	bpmFactory(BPMFactory(mode)),
	chargeFactory(ChargeFactory(mode)),
	valveFactory(ValveFactory(mode)),
	imgFactory(IMGFactory(mode)),
	mode(mode)
{
	messenger = LoggingSystem(true, true);
	messenger.printDebugMessage("Hardware Factory constructed, mode = ", ENUM_TO_STRING(mode));
}
bool HardwareFactory::setup(const std::string& hardwareType, const std::string& VERSION)
{
	bool setup = false;
	if (hardwareType == "IMG")
	{
		if (!imgFactory.hasBeenSetup)
		{
			setup = imgFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Magnet")
	{
		if (!magnetFactory.hasBeenSetup)
		{
			setup = magnetFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "BPM")
	{
		if (!bpmFactory.hasBeenSetup)
		{
			setup = bpmFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Charge")
	{
		if (!chargeFactory.hasBeenSetup)
		{
			setup = chargeFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Valve")
	{
		if (!valveFactory.hasBeenSetup)
		{
			setup = valveFactory.setup(VERSION);
		}
	}
	return setup;
}
MagnetFactory& HardwareFactory::getMagnetFactory()
{
	std::cout << "called getMagnetFactory() " << std::endl;
	if (!magnetFactory.hasBeenSetup)
	{
		std::cout << "!magnetFactory.hasBeenSetup " << std::endl;
		bool setup = magnetFactory.setup("nominal");
		if (setup)
		{
			std::cout << "magnetFactory setup" << std::endl;
			return magnetFactory;
		}
		else
		{
			std::cout << "Unable to setup MagnetFactory" << std::endl;
			messenger.printMessage("Unable to setup MagnetFactory");
		}
	}
	else
	{
		return magnetFactory;
	}
}
ValveFactory& HardwareFactory::getValveFactory()
{
	if (!valveFactory.hasBeenSetup)
	{
		bool setup = valveFactory.setup("nominal");
		if (setup)
		{
			return valveFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup ValveFactory");
		}
	}
	else
	{
		return valveFactory;
	}
}
IMGFactory& HardwareFactory::getIMGFactory()
{
	if (!imgFactory.hasBeenSetup)
	{
		bool setup = imgFactory.setup("nominal");
		if (setup)
		{
			return imgFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup IMGFactory");
		}
	}
	else
	{
		return imgFactory;
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
			messenger.printMessage("Unable to setup BPMFactory");
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
	bpmFactory.debugMessagesOn();
	chargeFactory.debugMessagesOn();
}

void HardwareFactory::debugMessagesOff()
{
	messenger.printDebugMessage("HARDWARE-FAC - ", "DEBUG OFF");
	messenger.debugMessagesOff();
	magnetFactory.debugMessagesOff();
	bpmFactory.debugMessagesOff();
	chargeFactory.debugMessagesOff();
	valveFactory.debugMessagesOff();
}

void HardwareFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("HARDWARE-FAC - MESSAGES ON");
	magnetFactory.messagesOn();
	bpmFactory.messagesOn();
	chargeFactory.messagesOn();
	valveFactory.messagesOn();
}

void HardwareFactory::messagesOff()
{
	messenger.printMessage("HARDWARE-FAC - MESSAGES OFF");
	messenger.messagesOff();
	magnetFactory.messagesOff();
	bpmFactory.messagesOff();
	chargeFactory.messagesOff();
	valveFactory.messagesOff();
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
