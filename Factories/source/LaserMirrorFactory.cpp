#include <LaserMirrorFactory.h>

LaserMirrorFactory::LaserMirrorFactory()
{
}

LaserMirrorFactory::LaserMirrorFactory(STATE mode) :
	LaserMirrorFactory(mode, MASTER_LATTICE_FILE_LOCATION)
{
}

LaserMirrorFactory::LaserMirrorFactory(STATE mode, const std::string& primeLatticeLocation)
	:
	mode(mode),
	reader(ConfigReader("LaserMirror", mode, primeLatticeLocation))
{
}

LaserMirrorFactory::LaserMirrorFactory(const LaserMirrorFactory& copyFactory)
{
}

LaserMirrorFactory::~LaserMirrorFactory()
{
}

void LaserMirrorFactory::setup(std::string version)
{
}

void LaserMirrorFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Laser Mirror FACTORY - DEBUG ON");
	// reader.debugMessagesOn();
	for (auto& laserMirror : LaserMirrorMap)
	{
		laserMirror.second.debugMessagesOn();
	}
}

void LaserMirrorFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Laser Mirror FACTORY - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& laserMirror : LaserMirrorMap)
	{
		laserMirror.second.debugMessagesOff();
	}
}

void LaserMirrorFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Laser Mirror FACTORY - MESSAGES ON");
	// reader.messagesOn();
	for (auto& laserMirror : LaserMirrorMap)
	{
		laserMirror.second.messagesOn();
	}
}

void LaserMirrorFactory::messagesOff()
{
	messenger.printMessage("Laser Mirror FACTORY - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& laserMirror : LaserMirrorMap)
	{
		laserMirror.second.messagesOff();
	}
}

bool LaserMirrorFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool LaserMirrorFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}

