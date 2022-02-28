#include <LinacPIDFactory.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>

LinacPIDFactory::LinacPIDFactory() :
	LinacPIDFactory(STATE::OFFLINE)
{
}

LinacPIDFactory::LinacPIDFactory(STATE mode) :
	mode(mode), hasBeenSetup(false),
	reader(ConfigReader("LinacPID", mode)),
	messenger(LoggingSystem(true, true))
{

}

LinacPIDFactory::LinacPIDFactory(const LinacPIDFactory& copyLinacPIDFactory) :
	hasBeenSetup(copyLinacPIDFactory.hasBeenSetup),
	mode(copyLinacPIDFactory.mode),
	messenger(copyLinacPIDFactory.messenger),
	reader(copyLinacPIDFactory.reader)
{
	messenger.printDebugMessage("LinacPIDFactory copy constructor called");
	linacPIDMap.insert(copyLinacPIDFactory.linacPIDMap.begin(), copyLinacPIDFactory.linacPIDMap.end());
}

LinacPIDFactory::~LinacPIDFactory()
{
	messenger.printDebugMessage("LinacPIDFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& linacPID : linacPIDMap)
		{
			auto pvStructsList = linacPID.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					if (pvStruct.second.EVID)
					{
						linacPID.second.epicsInterface->removeSubscription(pvStruct.second);
						ca_flush_io();
					}
				}
				if (pvStruct.second.CHID)
				{
					linacPID.second.epicsInterface->removeChannel(pvStruct.second);
					ca_pend_io(CA_PEND_IO_TIMEOUT);
				}
			}
		}
	}
}

void LinacPIDFactory::attachContext(const std::string& LinacPIDName)
{
	if (GlobalFunctions::entryExists(linacPIDMap, LinacPIDName))
	{
		linacPIDMap.at(LinacPIDName).attachToInitialContext();
	}
	else
	{
		messenger.printMessage("Could not find ", LinacPIDName, " in hardware map.");
	}
}

void LinacPIDFactory::attachContext(std::vector<std::string>& LinacPIDNames)
{
	for (auto&& name : LinacPIDNames)
	{
		attachContext(name);
	}
}

void LinacPIDFactory::attachContext_Py(boost::python::list LinacPIDNames)
{
	std::vector<std::string> names = to_std_vector<std::string>(LinacPIDNames);
	attachContext(names);
}

void LinacPIDFactory::attachContext()
{
	for (auto&& LinacPID : linacPIDMap)
	{
		LinacPID.second.attachToInitialContext();
	}
}

void LinacPIDFactory::setupChannels()
{
	for (auto& linacPID : linacPIDMap)
	{
		std::map<std::string, pvStruct>& pvStructs = linacPID.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			linacPID.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

void LinacPIDFactory::populateLinacPIDMap()
{
	messenger.printDebugMessage("LinacPIDFactory is populating LinacPID Map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support.");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("LinacPID Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(linacPIDMap);
	}
	messenger.printDebugMessage("LinacPID Factory has finished populating LinacPID Map");
}

bool LinacPIDFactory::setup(const std::string& VERSION)
{
	messenger.printDebugMessage("called LinacPID Factory  setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup LinacPID Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics linacPID interface has been initialized in linacPID constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateLinacPIDMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}


	setupChannels();
	EPICSInterface::sendToEPICS();


	for (auto& linacPID : linacPIDMap)
	{
		// update aliases for linacPID in map
		updateAliasNameMap(linacPID.second);
		std::map<std::string, pvStruct>& linacPIDPVStructs = linacPID.second.getPVStructs();
		for (auto& pv : linacPIDPVStructs)
		{
			// sets the monitor state in the pvstruict to true or false
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				setMonitorStatus(pv.second);
				linacPID.second.epicsInterface->retrieveCHTYPE(pv.second);
				linacPID.second.epicsInterface->retrieveCOUNT(pv.second);
				messenger.printMessage("retrieveUpdateFunctionForRecord");
				linacPID.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read = ", std::to_string(ca_read_access(pv.second.CHID)),
					"write = ", std::to_string(ca_write_access(pv.second.CHID)),
					"state = ", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					messenger.printMessage(linacPID.first, " createSubscription to ", pv.second.pvRecord);
					linacPID.second.epicsInterface->createSubscription(linacPID.second, pv.second);
					EPICSInterface::sendToEPICS();
				}
				else
				{
					messenger.printMessage(linacPID.first, " IS NOT creating a Subscription to ", pv.second.pvRecord);
				}
			}
			else
			{
				messenger.printMessage(linacPID.first, " CANNOT CONNECT TO EPICS");
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

void LinacPIDFactory::setMonitorStatus(pvStruct& pvStruct) const
{
	messenger.printMessage("setMonitorStatus checking ", pvStruct.pvRecord);
	if (GlobalFunctions::entryExists<std::string>(LinacPIDRecords::linacMonitorRecordsList, pvStruct.pvRecord))
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

LinacPID& LinacPIDFactory::getLinacPID(const std::string& fullLinacPIDName)
{
	std::string fullName = getFullName(fullLinacPIDName);
	if (GlobalFunctions::entryExists(linacPIDMap, fullName))
	{
		return linacPIDMap.at(fullName);
	}
}

std::string LinacPIDFactory::getFullName(const std::string& nameToCheck) const
{
	if (GlobalFunctions::entryExists(aliasNameMap, nameToCheck))
	{
		return aliasNameMap.at(nameToCheck);
	}
	else
	{
		return ENUM_TO_STRING(STATE::UNKNOWN);
	}
}

std::vector<std::string> LinacPIDFactory::getAllLinacPIDNames() const
{
	std::vector<std::string> returnNames;
	for (auto&& item : linacPIDMap)
	{
		returnNames.push_back(item.first);
	}
	return returnNames;
}

boost::python::list LinacPIDFactory::getAllLinacPIDNames_Py() const
{
	std::vector<std::string> returnNames = getAllLinacPIDNames();
	return to_py_list(returnNames);
}

void LinacPIDFactory::updateAliasNameMap(const LinacPID& linacPID)
{
	// first add in the linacPID full name
	std::string fullName = linacPID.getHardwareName();
	messenger.printMessage("updateAliasNameMap ", fullName);
	if (GlobalFunctions::entryExists(aliasNameMap, fullName))
	{
		messenger.printMessage("!!ERROR!! ", fullName, " linacPID name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", fullName, " added to alias_name_map");
		aliasNameMap[fullName] = fullName;
	}

	// now add in the aliases
	std::vector<std::string> aliases = linacPID.getAliases();
	for (auto&& nextAlias : aliases)
	{
		if (GlobalFunctions::entryExists(aliasNameMap, nextAlias))
		{
			messenger.printMessage("!!ERROR!! ", linacPID.getHardwareName(), " alias = ", nextAlias, " already exists");
		}
		else
		{
			aliasNameMap[nextAlias] = fullName;
			messenger.printMessage("Added alias " + nextAlias + " for " + fullName);
		}
	}
}





bool LinacPIDFactory::setPhase(const std::string& name, const double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).setPhase(value);
	}
	return false;
}
bool LinacPIDFactory::setPhase(const std::map<std::string, double>& name_value_map)
{
	bool r = true;
	for (auto&& item : name_value_map)
	{
		bool temp_r = setPhase(item.first, item.second);
		if (temp_r == false)
		{
			r = false;
		}
	}
	return r;
}
bool LinacPIDFactory::setPhase_Py(const boost::python::dict& name_value_map)
{
	// meh, its doing stuff like this, or use the map_indexing_suite, or re-design 
	std::map<std::string, double> r;
	boost::python::list keys = boost::python::list(name_value_map.keys());
	boost::python::list values = boost::python::list(name_value_map.values());
	for (int i = 0; i < len(keys); ++i)
	{
		boost::python::extract<std::string> k_extractor(keys[i]);
		boost::python::extract<double> v_extractor(values[i]);
		std::string k = k_extractor();
		double v = v_extractor();
		r[k] = v;
	}
	return setPhase(r);
}
//
double LinacPIDFactory::getPhase(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getPhase();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> LinacPIDFactory::getPhase()const
{
	std::map<std::string, double>  r;
	for (auto&& item : linacPIDMap)
	{
		r[item.first] = item.second.getPhase();
	}
	return r;
}
boost::python::dict LinacPIDFactory::getPhase_Py()const
{
	return to_py_dict<std::string, double>(getPhase());
}
bool LinacPIDFactory::setForwardPhaseWeight(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).setForwardPhaseWeight(value);
	}
	return false;
}
double LinacPIDFactory::getForwardPhaseWeight(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getForwardPhaseWeight();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> LinacPIDFactory::getForwardPhaseWeight()const
{
	std::map<std::string, double> r;
	for (auto&& item : linacPIDMap)
	{
		r[item.first] = item.second.getForwardPhaseWeight();
	}
	return r;
}
boost::python::dict LinacPIDFactory::getForwardPhaseWeight_Py()const
{
	return to_py_dict<std::string, double>(getForwardPhaseWeight());
}
double LinacPIDFactory::getForwardPhaseWrapped(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getForwardPhaseWrapped();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> LinacPIDFactory::getForwardPhaseWrapped()const
{
	std::map<std::string, double> r;
	for (auto&& item : linacPIDMap)
	{
		r[item.first] = item.second.getForwardPhaseWrapped();
	}
	return r;
}
boost::python::dict LinacPIDFactory::getForwardPhaseWrapped_Py()const
{
	return to_py_dict<std::string, double>(getProbePhaseWeight());
}
bool LinacPIDFactory::setProbePhaseWeight(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).setProbePhaseWeight(value);
	}
	return false;
}

double LinacPIDFactory::getProbePhaseWeight(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getProbePhaseWeight();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> LinacPIDFactory::getProbePhaseWeight()const
{
	std::map<std::string, double> r;
	for (auto&& item : linacPIDMap)
	{
		r[item.first] = item.second.getProbePhaseWeight();
	}
	return r;
}
boost::python::dict  LinacPIDFactory::getProbePhaseWeight_Py(const std::string& name)const
{
	return to_py_dict<std::string, double>(getProbePhaseWeight());
}
double LinacPIDFactory::getProbePhaseWrapped(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getProbePhaseWrapped();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> LinacPIDFactory::getProbePhaseWrapped()const
{
	std::map<std::string, double> r;
	for (auto&& item : linacPIDMap)
	{
		r[item.first] = item.second.getProbePhaseWrapped();
	}
	return r;
}
boost::python::dict  LinacPIDFactory::getProbePhaseWrapped_Py()const
{
	return to_py_dict<std::string, double>(getProbePhaseWeight());
}
double LinacPIDFactory::getOVAL(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getProbePhaseWrapped();
	}
	return GlobalConstants::double_min;
}

std::map<std::string, double> LinacPIDFactory::getOVAL()const
{
	std::map<std::string, double> r;
	for (auto&& item : linacPIDMap)
	{
		r[item.first] = item.second.getProbePhaseWrapped();
	}
	return r;
}
boost::python::dict  LinacPIDFactory::getOVAL_Py()const
{
	return to_py_dict<std::string, double>(getProbePhaseWeight());
}
bool LinacPIDFactory::enable(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).enable();
	}
	return false;
}
bool LinacPIDFactory::enableAll()
{
	bool r = false;
	for (auto&& item : linacPIDMap)
	{
		bool temp = item.second.enable();
		if (temp == false)
		{
			r = false;
		}
	}
	return r;
}
bool LinacPIDFactory::disable(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).disable();
	}
	return false;
}
bool LinacPIDFactory::disableAll()
{
	bool r = false;
	for (auto&& item : linacPIDMap)
	{
		bool temp = item.second.disable();
		if (temp == false)
		{
			r = false;
		}
	}
	return r;
}
bool LinacPIDFactory::isEnabled(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).isEnabled();
	}
	return false;
}
bool LinacPIDFactory::isDisabled(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).isDisabled();
	}
	return false;
}

STATE LinacPIDFactory::getEnabledState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getEnabledState();
	}
	return STATE::UNKNOWN_NAME;
}

std::map<std::string, STATE> LinacPIDFactory::getEnabledState()const
{
	std::map<std::string, STATE> r;
	for (auto&& item : linacPIDMap)
	{
		r[item.first] = item.second.getEnabledState();
	}
	return r;
}
boost::python::dict  LinacPIDFactory::getEnabledState_Py()const
{
	return to_py_dict<std::string, STATE>(getEnabledState());
}
double LinacPIDFactory::getMaxPhase(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getMaxPhase();
	}
	return GlobalConstants::double_min;
}
double LinacPIDFactory::getMinPhase(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getMinPhase();
	}
	return GlobalConstants::double_min;
}
double LinacPIDFactory::getMaxPhaseWeight(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getMaxPhaseWeight();
	}
	return GlobalConstants::double_min;
}
double LinacPIDFactory::getMinPhaseWeight(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(linacPIDMap, full_name))
	{
		return linacPIDMap.at(full_name).getMinPhaseWeight();
	}
	return GlobalConstants::double_min;
}












void LinacPIDFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LinacPID-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.debugMessagesOn();
	}
}
void LinacPIDFactory::debugMessagesOff()
{
	messenger.printDebugMessage("LinacPID-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.debugMessagesOff();
	}
}
void LinacPIDFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("LinacPID-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.messagesOn();
	}
}
void LinacPIDFactory::messagesOff()
{
	messenger.printMessage("LinacPID-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.messagesOff();
	}
}
bool LinacPIDFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool LinacPIDFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}


