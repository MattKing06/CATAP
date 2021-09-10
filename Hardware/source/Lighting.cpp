#include <Lighting.h>
#include <LightingPVRecords.h>

Lighting::Lighting()
{
}

Lighting::Lighting(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	vela_led_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	clara_led_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	ba1_lighting_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	accelerator_hall_lighting_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN))
{
	setPVStructs();
}

Lighting::Lighting(const Lighting& copyLighting)
{
}

Lighting::~Lighting()
{
}

void Lighting::setPVStructs()
{
	for (auto&& record : LightingRecords::lightRecordList)
	{

		if (GlobalFunctions::entryExists(specificHardwareParameters, record))
		{
			pvStructs[record] = pvStruct();
			pvStructs[record].pvRecord = record;
			// TODO NO ERROR CHECKING! (we assum config file is good??? 
			std::string PV = specificHardwareParameters.find(record)->second.data();
			// iterate through the list of matches and set up a pvStruct to add to pvStructs.
			//messenger.printDebugMessage("Constructing PV information for ", record);
			/*TODO
			  This should be put into some general function: generateVirtualPV(PV) or something...
			  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
			  The config reader level if that is the case.
			  DJS maybe they should, how certian can we be all virtual PVs will get a VM- prefix???
			  */
			if (mode == STATE::VIRTUAL)
			{
				pvStructs[record].fullPVName = "VM-" + PV;
				std::cout << "Virtual Lighting PV " + pvStructs[record].fullPVName << std::endl;
			}
			else
			{
				pvStructs[record].fullPVName = PV;
				std::cout << "Physical Lighting PV " + pvStructs[record].fullPVName << std::endl;
			}
		}
		else
		{
			std::cout << "Can't find record = " << record << std::endl;
		}
	}
}


std::map<std::string, STATE> Lighting::getLEDState()const
{
	std::map < std::string, STATE> r;
	return r;
}
boost::python::dict Lighting::getLEDState_Py()const
{
	return to_py_dict<std::string, STATE>(getLEDState());
}
std::map<std::string, STATE> Lighting::getLightingState()const
{
	std::map < std::string, STATE> r;
	r["vela_led"] = getClaraLEDState();
	r["clara_led"] = getClaraLEDState();
	r["accelerator_hall"] = getAcceleratorHallLightState();
	r["ba1_hall"] = getBA1LightState();
	return r;
}
boost::python::dict Lighting::getLightingState_Py()const
{
	return to_py_dict<std::string, STATE>(getLightingState());
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
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::CLARA_LED_On), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::CLARA_LED_On), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
	return false;
}
bool Lighting::setClaraLEDOff()
{
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::CLARA_LED_Off), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::CLARA_LED_Off), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
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
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::VELA_LED_On), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::VELA_LED_On), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
	return false;
}
bool Lighting::setVelaLEDOff()
{
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::VELA_LED_Off), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::VELA_LED_Off), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
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




STATE Lighting::getBA1LightState()const
{
	return ba1_lighting_state.second;
}
bool Lighting::setBA1LightOn()
{
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::BA1_LIGHT_On), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::BA1_LIGHT_On), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
	return false;
}
bool Lighting::setBA1LightOff()
{
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::BA1_LIGHT_Off), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::BA1_LIGHT_Off), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
}
bool Lighting::isBA1LightOn()const
{
	return getBA1LightState() == STATE::ON;
}
bool Lighting::isBA1LightOff()const
{
	return getBA1LightState() == STATE::OFF;
}
bool Lighting::toggleBA1Light()
{
	if (isBA1LightOn())
		return setBA1LightOff();
	if (isBA1LightOff())
		return setBA1LightOn();
	return false;
}

STATE Lighting::getAcceleratorHallLightState()const
{
	return accelerator_hall_lighting_state.second;
}
bool Lighting::setAcceleratorHallLightOn()
{
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::ACCELERATOR_HALL_LIGHT_On), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::ACCELERATOR_HALL_LIGHT_On), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
	return false;
}
bool Lighting::setAcceleratorHallLightOff()
{
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::ACCELERATOR_HALL_LIGHT_Off), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(LightingRecords::ACCELERATOR_HALL_LIGHT_Off), (epicsUInt8)GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
	return false;
} 
bool Lighting::isAcceleratorHallLightOn()const
{
	return getAcceleratorHallLightState() == STATE::ON;
}
bool Lighting::isAcceleratorHallLightOff()const
{
	return getAcceleratorHallLightState() == STATE::OFF;
}
bool Lighting::toggleAcceleratorHallLight()
{
	if (isAcceleratorHallLightOn())
		return setAcceleratorHallLightOn();
	if (isAcceleratorHallLightOff())
		return setAcceleratorHallLightOff();
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