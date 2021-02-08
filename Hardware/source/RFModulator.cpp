#include <RFModulator.h>
#include <GlobalFunctions.h>
#include <RFModulatorPVRecords.h>

RFModulator::RFModulator()
{
}

RFModulator::RFModulator(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
epicsInterface(boost::make_shared<EPICSRFModulatorInterface>(EPICSRFModulatorInterface()))
{
	messenger.printMessage("setMasterLatticeData data for: ", hardwareName);
	//setMasterLatticeData();
	messenger.printMessage("constructing PV data for: ", hardwareName);
	setPVStructs();
}
RFModulator::RFModulator(const RFModulator& copyRFModulator)
{
}
RFModulator::~RFModulator(){}




void RFModulator::setPVStructs()
{
	for (auto&& record : RFModulatorRecords::rfProtectionRecordsList)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, record))
		{
			messenger.printDebugMessage(hardwareName, " found ", record);
			pvStructs[record] = pvStruct();
			pvStructs[record].pvRecord = record;
			std::string PV = specificHardwareParameters.find(record)->second;
			switch (mode)
			{
			case(STATE::VIRTUAL):
				pvStructs[record].fullPVName = "VM-" + PV;
				break;
			case(STATE::PHYSICAL):
				pvStructs[record].fullPVName = PV;
				break;
			default:
				pvStructs[record].fullPVName = PV;
				break;
			}
		}

	}
}

void RFModulator::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void RFModulator::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void RFModulator::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void RFModulator::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}