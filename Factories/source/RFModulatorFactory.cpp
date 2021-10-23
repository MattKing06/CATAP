#include <RFModulatorFactory.h>
#include <RFModulatorPVRecords.h>
#include <GlobalFunctions.h>

RFModulatorFactory::RFModulatorFactory()
{
}

RFModulatorFactory::RFModulatorFactory(STATE mode):
RFModulatorFactory(mode, MASTER_LATTICE_FILE_LOCATION)
{
}

RFModulatorFactory::RFModulatorFactory(STATE mode, const std::string& primeLatticeLocation) :
messenger(LoggingSystem(true, true)),
mode(mode),
hasBeenSetup(false),
reader(ConfigReader("RFModulator", mode, primeLatticeLocation))
{
}


RFModulatorFactory::RFModulatorFactory(const RFModulatorFactory& copyFactory)
{
}

RFModulatorFactory::~RFModulatorFactory()
{
}



// get a LLRF object (GUN, L01, etc... ) 
RFModulator& RFModulatorFactory::getModulator(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name);
	}
	return dummy_mod;
}
std::string RFModulatorFactory::getFullName(const std::string& name_to_check) const
{
	std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	std::cout << name_to_check << " NOT found " << std::endl;
	return dummy_mod.getHardwareName();
}
std::vector<std::string> RFModulatorFactory::getAllRFModulatorNames()
{
	std::vector<std::string> rfModNames;
	for (auto& rfProt : RFModulatorMap)
	{
		rfModNames.push_back(rfProt.first);
	}
	return rfModNames;
}
boost::python::list RFModulatorFactory::getAllRFModulatorNames_Py()
{
	return to_py_list<std::string>(getAllRFModulatorNames());
}
std::vector<std::string> RFModulatorFactory::getAliases(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getAliases();
	}
	return std::vector<std::string>();
}
boost::python::list RFModulatorFactory::getAliases_Py(const std::string& name) const
{
	return to_py_list<std::string>(getAliases(name));
}
STATE RFModulatorFactory::getMainState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getMainState();
	}
	return STATE::UNKNOWN_NAME;

}
bool RFModulatorFactory::isInRFOn(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isInRFOn();
	}
	return false;
}
bool RFModulatorFactory::isInHVOn(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isInHVOn();
	}
	return false;
}
bool RFModulatorFactory::isInStandby(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isInStandby();
	}
	return false;
}
bool RFModulatorFactory::isInOff(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isInOff();
	}
	return false;
}
STATE RFModulatorFactory::getErrorState(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getErrorState();
	}
	return STATE::UNKNOWN_NAME;
}
bool RFModulatorFactory::isErrorStateGood(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isErrorStateGood();
	}
	return false;
}
STATE RFModulatorFactory::getStateSetRead(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getStateSetRead();
	}
	return STATE::UNKNOWN_NAME;
}
bool RFModulatorFactory::isWarm(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isWarm();
	}
	return false;
}
bool RFModulatorFactory::isNotWarm(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isNotWarm();
	}
	return false;
}
bool RFModulatorFactory::isInHoldRFOn(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isInHoldRFOn();
	}
	return false;
}
bool RFModulatorFactory::isInManualOperation(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isInManualOperation();
	}
	return false;
}
bool RFModulatorFactory::isInHoldRFOnCon(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).isInHoldRFOnCon();
	}
	return false;
}
STATE RFModulatorFactory::getHoldRFOnState(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getHoldRFOnState();
	}
	return STATE::UNKNOWN;
}

void RFModulatorFactory::setHoldRFOnState(const std::string& name, STATE holdRFOnState)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		RFModulatorMap.at(full_name).setHoldRFOnState(holdRFOnState);
	}
}

void RFModulatorFactory::setHoldRFOnToManualOperation(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		RFModulatorMap.at(full_name).setHoldRFOnToManualOperation();
	}
}

void RFModulatorFactory::setHoldRFOnToHoldRFOnCon(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		RFModulatorMap.at(full_name).setHoldRFOnToHoldRFOnCon();
	}
}

void RFModulatorFactory::setHoldRFOnToHoldRFOn(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		RFModulatorMap.at(full_name).setHoldRFOnToHoldRFOn();
	}
}

std::map<std::string, double> RFModulatorFactory::getLowLevelNumericalData(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getLowLevelNumericalData();
	}
	return std::map<std::string, double>();
}
boost::python::dict RFModulatorFactory::getLowLevelNumericalData_Py(const std::string& name)const
{
	return to_py_dict< std::string, double >(getLowLevelNumericalData(name));
}
std::map<std::string, std::string> RFModulatorFactory::getLowLevelStringData(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getLowLevelStringData();
	}
	std::map<std::string, std::string> r;
	return r;
}
boost::python::dict RFModulatorFactory::getLowLevelStringData_Py(const std::string& name)const
{
	return to_py_dict< std::string, std::string >(getLowLevelStringData(name));
}
boost::python::dict RFModulatorFactory::getLowLevelData(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFModulatorMap, full_name))
	{
		return RFModulatorMap.at(full_name).getLowLevelData();
	}
	return boost::python::dict();
}



void RFModulatorFactory::populateRFModulatorMap()
{
	messenger.printDebugMessage("RFModulatorFactory is populating the RFModulator map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("RFModulatorFactory calling parseNextYamlFile");
		reader.parseNextYamlFile<RFModulator>(RFModulatorMap);
	}
	size_t start_size = RFModulatorMap.size();
}

void RFModulatorFactory::setMachineAreaAndCutRFModulatorMap(const std::vector<TYPE>& machine_areas_in)
{
	for (auto&& item : machine_areas_in)
	{
		messenger.printMessage("setMachineAreaAndCutRFModulatorMap passed ", ENUM_TO_STRING(item));
		machine_areas = machine_areas_in;
		// hardcode logical possiblities, in te efuture when we have many more modualtors this will need a refactor
		if (GlobalFunctions::entryExists(machine_areas, TYPE::ALL_VELA_CLARA))
		{
			return;
		}
		bool should_use_gun = false;
		bool should_use_l01 = false;
		if (GlobalFunctions::entryExists(machine_areas, TYPE::GUN))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::VELA_GUN))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::CLARA_GUN))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::HRRG))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::LRRG))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::CLARA_HRRG))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::CLARA_LRRG))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::VELA_HRRG))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::VELA_LRRG))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::HRRG_GUN))
		{
			should_use_gun = true;
		}
		if (GlobalFunctions::entryExists(machine_areas, TYPE::LRRG_GUN))
		{
			should_use_gun = true;
		}
		//linac
		if (GlobalFunctions::entryExists(machine_areas, TYPE::L01))
		{
			should_use_l01 = true;
		}
		if (should_use_l01 && should_use_gun)
		{
			return;
		}
		if (should_use_l01)
		{
			messenger.printMessage("eraseFromRFModulatorMap passed GUN");
			eraseFromRFModulatorMap(std::vector<TYPE>{TYPE::GUN});
			return;
		}
		if (should_use_gun)
		{
			messenger.printMessage("eraseFromRFModulatorMap passed L01");
			eraseFromRFModulatorMap(std::vector<TYPE>{TYPE::L01});
			return;
		}
		messenger.printMessage("!!ERROR!! never show this from the RF Modulator Factory setup");
	}
}

void RFModulatorFactory::eraseFromRFModulatorMap(const std::vector<TYPE>& to_erase)
{
	size_t start_size = RFModulatorMap.size();
	// loop over every mod object 
	for (auto it = RFModulatorMap.begin(); it != RFModulatorMap.end() /* not hoisted */; /* no increment */)
	{
		// flag for if we should erase this entry, default to false
		bool should_erase = false;
		// now we loop over every area in to_erase and check against isInMachineArea, if in to_erase set flag to true 
		messenger.printDebugMessage(it->first, " is in area = ", ENUM_TO_STRING(it->second.getMachineArea()));
		for (auto&& item : to_erase)
		{
			if (GlobalFunctions::entryExists(to_erase, it->second.getMachineArea()))
			{
				should_erase = true;
				break;
			}
		}
		// do, or do not, the erasing 
		if (should_erase)
		{
			messenger.printDebugMessage("RFModulator Factory erasing " + it->second.getHardwareName());
			it = RFModulatorMap.erase(it); //  m.erase(it++);    
		}
		else
		{
			++it;
		}
	}
	size_t end_size = RFModulatorMap.size();
	messenger.printDebugMessage("eraseFromRFModulatorMap RFModulatorMap.size() went from ", start_size, " to ", end_size);
}
bool RFModulatorFactory::setup(const std::string& version)
{
	return setup(version, std::vector<TYPE>(TYPE::ALL_VELA_CLARA));
}
bool RFModulatorFactory::setup(const std::string& version, const std::vector<TYPE>& machine_areas_in)
{
	if (hasBeenSetup)
	{
		messenger.printMessage("RF Modulator Factory has already been setup");
		return true;
	}
	messenger.printMessage("setup populateRFModulatorMap");
	populateRFModulatorMap();
	messenger.printMessage("setup setMachineAreaAndCutRFModulatorMap");
	setMachineAreaAndCutRFModulatorMap(machine_areas_in);
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		// Could not find any YAML files for configuration..
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	messenger.printMessage("setup setupChannels");
	setupChannels();
	EPICSInterface::sendToEPICS();
	messenger.printMessage("setup connections");
	for (auto& prot : RFModulatorMap)
	{
		updateAliasNameMap(prot.second);
		std::map<std::string, pvStruct>& pvStructs = prot.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printDebugMessage("Setup ", pv.first);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				setMonitorStatus(pv.second);
				prot.second.epicsInterface->retrieveCOUNT(pv.second);
				prot.second.epicsInterface->retrieveCHTYPE(pv.second);
				prot.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.first, " ", pv.second.pvRecord, " r, w, s = ", std::to_string(ca_read_access(pv.second.CHID)),
					std::to_string(ca_write_access(pv.second.CHID)), std::to_string(ca_state(pv.second.CHID)));

				if (pv.second.monitor)
				{
					prot.second.epicsInterface->createSubscription(prot.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(pv.second.fullPVName, " COULD NOT CONNECT TO EPICS.");
			}
		}
		EPICSInterface::sendToEPICS();
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}
void RFModulatorFactory::updateAliasNameMap(const RFModulator& mod)
{
	// first add in the full name
	std::string full_name = mod.getHardwareName();
	//messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		//messenger.printMessage("!!ERROR!! ", full_name, " name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases
	std::vector<std::string> aliases = mod.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", mod.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}
void RFModulatorFactory::setupChannels()
{
	for (auto& device : RFModulatorMap)
	{
		std::map<std::string, pvStruct>& pvStructs = device.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printMessage(device.first, ", retrieveCHID ", pv.first);
			device.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}
void RFModulatorFactory::setMonitorStatus(pvStruct& pvStruct)
{
	messenger.printMessage("setMonitorStatus checking ", pvStruct.pvRecord);
	if (GlobalFunctions::entryExists(RFModulatorRecords::rf_mod_monitor_records_list, pvStruct.pvRecord))
	{
		pvStruct.monitor = true;
		messenger.printMessage("setMonitorStatus ", pvStruct.pvRecord, ", status = true");
	}
	else
	{
		pvStruct.monitor = false;
		messenger.printMessage("setMonitorStatus ", pvStruct.pvRecord, ", status = false ");
	}
}


void RFModulatorFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("RF Modulator Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.debugMessagesOn();
	}
}
void RFModulatorFactory::debugMessagesOff()
{
	messenger.printDebugMessage("RF Modulator Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.debugMessagesOff();
	}
}
void RFModulatorFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("RF Modulator Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.messagesOn();
	}
}
void RFModulatorFactory::messagesOff()
{
	messenger.printMessage("RF Modulator Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.messagesOff();
	}
}
bool RFModulatorFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool RFModulatorFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
