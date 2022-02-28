#include <LinacPID.h>
#include <GlobalFunctions.h>
#include <boost/make_shared.hpp>
#include "boost/algorithm/string/split.hpp"
#include <string>

LinacPID::LinacPID() {}

LinacPID::LinacPID(const std::map<std::string, std::string>& paramMap, STATE mode)
	: Hardware(paramMap, mode),
	phase(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	crest_phase(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	operating_phase(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	forward_phase_wrapped(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	forward_phase_weight(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	probe_phase_wrapped(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	probe_phase_weight(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	oval(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	forward_channel(GlobalConstants::size_zero),
	probe_channel(GlobalConstants::size_zero),
	max_phase(GlobalConstants::double_min),
	min_phase(GlobalConstants::double_min),
	min_phase_weight(GlobalConstants::double_min),
	max_phase_weight(GlobalConstants::double_min)
{
	setPVStructs();
	epicsInterface = boost::make_shared<EPICSLinacPIDInterface>(EPICSLinacPIDInterface());
	epicsInterface->ownerName = hardwareName;
	messenger = LoggingSystem(true, true);

	messenger.printDebugMessage(hardwareName, " find enable");
	if (GlobalFunctions::entryExists(paramMap, "enable"))
	{
		ml_enable_state = GlobalFunctions::stringToSTATE(paramMap.find("enable")->second);
		ml_enable_number = GlobalFunctions::scanSTATEToNumber(ml_enable_state);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!ERROR!! could not find enable");
		// TODO THROW AN EXCEPTION HERE
	}
	messenger.printDebugMessage(hardwareName, " find disable");
	if (GlobalFunctions::entryExists(paramMap, "disable"))
	{
		ml_disable_state = GlobalFunctions::stringToSTATE(paramMap.find("disable")->second);
		ml_disable_number = GlobalFunctions::scanSTATEToNumber(ml_disable_state);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!ERROR!! could not find disable");
		// TODO THROW AN EXCEPTION HERE
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find name_alias");
	if (GlobalFunctions::entryExists(paramMap, "name_alias"))
	{
		boost::split(aliases, paramMap.find("name_alias")->second, [](char c) {return c == ','; });
		for (auto& name : aliases)
		{
			name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
			messenger.printDebugMessage(hardwareName, " added aliase " + name);
		}
	}
	messenger.printDebugMessage(hardwareName, " find forward_channel");
	if (GlobalFunctions::entryExists(paramMap, "forward_channel"))
	{
		forward_channel = (size_t)std::stoi(paramMap.find("forward_channel")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find forward_channel");
	}
	std::vector<std::string> dummy_vec_str;
	std::vector<double> dummy_vec_double;
	//convert list of phase ranges to max and minm settings 
	messenger.printDebugMessage(hardwareName, " find phase ranges ");
	boost::split(dummy_vec_str, paramMap.find("phase_range")->second, [](char c) {return c == ','; });
	for (auto value : dummy_vec_str)
	{
		dummy_vec_double.push_back(std::stod(value));
	}
	min_phase = *min_element(dummy_vec_double.begin(), dummy_vec_double.end());
	max_phase = *max_element(dummy_vec_double.begin(), dummy_vec_double.end());
	dummy_vec_str.clear();
	dummy_vec_double.clear();
	messenger.printDebugMessage(hardwareName, " find phase weight ranges ");
	boost::split(dummy_vec_str, paramMap.find("phase_weight_range")->second, [](char c) {return c == ','; });
	for (auto value : dummy_vec_str)
	{
		dummy_vec_double.push_back(std::stod(value));
	}
	min_phase_weight = *min_element(dummy_vec_double.begin(), dummy_vec_double.end());
	max_phase_weight = *max_element(dummy_vec_double.begin(), dummy_vec_double.end());
}

void LinacPID::setPVStructs()
{
	for (auto&& record : LinacPIDRecords::linacPIDRecordList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		std::string PV = specificHardwareParameters.find(record)->second.data();
		switch (mode)
		{
		case STATE::VIRTUAL:
			pvStructs[record].fullPVName = "VM-" + PV;
			break;
		default:
			pvStructs[record].fullPVName = PV;
			break;
		}
	}
}

bool LinacPID::setPhase(double value)
{
	return epicsInterface->putValue2(pvStructs.at(LinacPIDRecords::AVG_PHASE), value);
}
double LinacPID::getPhase()const
{
	return phase.second;
}
bool LinacPID::setCrestPhase(double value)
{
	return false;
}
double LinacPID::getCrestPhase()const
{
	return crest_phase.second;
}
bool LinacPID::setOperatingPhase(double value)
{
	return false;
}
double LinacPID::getOperatingPhase()const
{
	return operating_phase.second;
}
bool LinacPID::setForwardPhaseWeight(double value)
{
	return epicsInterface->putValue2(pvStructs.at(LinacPIDRecords::FORWARD_PHASE_WEIGHT), value);
}
double LinacPID::getForwardPhaseWeight()const
{
	return forward_phase_weight.second;
}
double LinacPID::getForwardPhaseWrapped()const
{
	return forward_phase_wrapped.second;
}
bool LinacPID::setProbePhaseWeight(double value)
{
	return epicsInterface->putValue2(pvStructs.at(LinacPIDRecords::PROBE_PHASE_WEIGHT), value);
}
double LinacPID::getProbePhaseWeight()const
{
	return probe_phase_weight.second;
}
double LinacPID::getProbePhaseWrapped()const
{
	return probe_phase_wrapped.second;
}
double LinacPID::getOVAL()const
{
	return GlobalConstants::double_min;
}
bool LinacPID::enable()
{
	messenger.printDebugMessage(hardwareName, " - send enable");
	return 	epicsInterface->putValue2(pvStructs.at(LinacPIDRecords::SCAN), ml_enable_number);
}
bool LinacPID::disable()
{
	messenger.printDebugMessage(hardwareName, " - send disable");
	return 	epicsInterface->putValue2(pvStructs.at(LinacPIDRecords::SCAN), ml_disable_number);
}
bool LinacPID::isEnabled()const
{
	return enable_state == STATE::ENABLED;
}
bool LinacPID::isDisabled()const
{
	return enable_state == STATE::DISABLED;
}
STATE LinacPID::getEnabledState()const
{
	return enable_state;
}
double LinacPID::getMaxPhase()const
{
	return max_phase;
}
double LinacPID::getMinPhase()const
{
	return min_phase;
}
double LinacPID::getMaxPhaseWeight()const
{
	return min_phase_weight;
}
double LinacPID::getMinPhaseWeight()const
{
	return max_phase_weight;
}



std::vector<std::string> LinacPID::getAliases() const
{
	return aliases;
}

void LinacPID::attachToInitialContext()
{
	epicsInterface->attachTo_thisCaContext();
}

void LinacPID::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void LinacPID::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LinacPID::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void LinacPID::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}