#include <RFHeartbeat.h>
#include <GlobalFunctions.h>

RFHeartbeat::RFHeartbeat()
{
}

RFHeartbeat::RFHeartbeat(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode)
{
	setPVStructs();
}

RFHeartbeat::RFHeartbeat(const RFHeartbeat& copyRFHeartbeat):
	Hardware(copyRFHeartbeat)
{
}

RFHeartbeat::~RFHeartbeat()
{
}

void RFHeartbeat::setPVStructs()
{
	messenger.printDebugMessage("setPVStructs");
	
	// TODO can we move into Hardware base class and make geenric ??? 
	for (auto&& record : RFHeartbeatRecords::rfHeartbeat_records_list)
	{
		messenger.printDebugMessage("RFHeartbeat PV record = " + record);
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		if (GlobalFunctions::entryExists(specificHardwareParameters, record))
		{
			std::string PV = specificHardwareParameters.find(record)->second.data();
			/*TODO
			  This should be put into some general function: generateVirtualPV(PV) or something...
			  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
			  The config reader level if that is the case.
			  DJS maybe they should, how certian can we be all virtual PVs will get a VM- prefix???
			  */
			if (mode == STATE::VIRTUAL)
			{
				pvStructs[record].fullPVName = "VM-" + PV;
			}
			else
			{
				pvStructs[record].fullPVName = PV;
			}
			messenger.printDebugMessage(record, ", PV  = " + pvStructs[record].fullPVName);
		}
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		//messenger.printDebugMessage("Constructing PV information for ", record);
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}


double RFHeartbeat::getValue() const
{
	return hb_value.second;
}
void RFHeartbeat::setValue(double v)
{
	epicsInterface->putValue<double>( pvStructs.at(RFHeartbeatRecords::KEEP_ALIVE), v);
}
void RFHeartbeat::pulse()
{
	if (hb_value.second == GlobalConstants::zero_double)
	{
		return setValue(GlobalConstants::one_double);
	}
	return setValue(GlobalConstants::zero_double);
}


std::vector < std::string > RFHeartbeat::getAliases()const
{
	return aliases;
}

void RFHeartbeat::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void RFHeartbeat::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void RFHeartbeat::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void RFHeartbeat::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

