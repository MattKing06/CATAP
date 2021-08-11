#include <Lighting.h>

Lighting::Lighting()
{
}

Lighting::Lighting(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	vela_led_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	clara_led_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	injector_hall_lighting_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN))
{
}

Lighting::Lighting(const Lighting& copyLighting)
{
}

Lighting::~Lighting()
{
}

void Lighting::setPVStructs()
{
}


std::map<std::string, STATE> Lighting::getLEDState()const
{
	std::map < std::string, STATE> r;
	return r;
}
std::map<std::string, STATE> Lighting::getLightingState()const
{
	std::map < std::string, STATE> r;
	return r;

}
bool Lighting::allLEDOn()
{
	return false;
}
bool Lighting::allLEDOff()
{
	return false;
}


STATE Lighting::getClaraLEDState()const
{
	return clara_led_state.second;
}
bool Lighting::setClaraLEDOn()
{
	return false;
}
bool Lighting::setClaraLEDOff()
{
	return false;
}
bool Lighting::isClaraLEDOn()const
{
	return getClaraLEDState() == STATE::ON;
}
bool Lighting::isClaraLEDOff()const
{
	return getClaraLEDState() == STATE::OFF;
}
bool Lighting::toggleClaraLED()
{
	if (isClaraLEDOn())
		return setClaraLEDOff();
	if (isClaraLEDOff())
		return setClaraLEDOn();
	return false;
}





STATE Lighting::getVelaLEDState()const
{
	return vela_led_state.second;
}
bool Lighting::setVelaLEDOn()
{
	return false;
}
bool Lighting::setVelaLEDOff()
{
	return false;
}
bool Lighting::isVelaLEDOn()const
{
	return getVelaLEDState() == STATE::ON;
}
bool Lighting::isVelaLEDOff()const
{
	return getVelaLEDState() == STATE::OFF;
}
bool Lighting::toggleVelaLED()
{
	if (isVelaLEDOn())
		return setVelaLEDOff();
	if (isVelaLEDOff())
		return setVelaLEDOn();
	return false;
}




void Lighting::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Lighting::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Lighting::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Lighting::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}