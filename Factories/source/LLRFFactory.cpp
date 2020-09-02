#include "LLRFFactory.h"
#include "GlobalStateEnums.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"
#include "PythonTypeConversions.h"
#include <algorithm>

LLRFFactory::LLRFFactory() : 
LLRFFactory(STATE::OFFLINE)
{
}

LLRFFactory::LLRFFactory(STATE mode) :
mode(mode), 
hasBeenSetup(false),
reader(ConfigReader("LLRF", mode)),
messenger(LoggingSystem(true, true)),
machineAreas(std::vector<TYPE>{TYPE::ALL_VELA_CLARA}),
dummy_llrf(LLRF()),
dummy_trace_data(std::vector<double>(1017,GlobalConstants::double_min))
{
	messenger.printDebugMessage("LLRFFactory constructed");
}

LLRFFactory::LLRFFactory(const LLRFFactory& copyFactory) :
hasBeenSetup(copyFactory.hasBeenSetup),
mode(copyFactory.mode),
messenger(copyFactory.messenger),
reader(copyFactory.reader),
machineAreas(machineAreas)
{
}

LLRFFactory::~LLRFFactory()
{
}


bool LLRFFactory::setup()
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool LLRFFactory::setup(const std::string& version)
{
	return setup(version, machineAreas);
}
bool LLRFFactory::setup(TYPE machineArea)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool LLRFFactory::setup(const std::string& version, TYPE machineArea)
{
	return setup(GlobalConstants::nominal, std::vector<TYPE>{machineArea});
}
bool LLRFFactory::setup(const std::vector<TYPE>& machineAreas)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool LLRFFactory::setup(const boost::python::list& machineAreas)
{
	return setup(GlobalConstants::nominal, to_std_vector<TYPE>(machineAreas));
}
bool LLRFFactory::setup(const std::string& version, const boost::python::list& machineAreas)
{
	return setup(version, to_std_vector<TYPE>(machineAreas));
}
bool LLRFFactory::setup(const std::string& version, const std::vector<TYPE>& machineAreas_IN)
{
	machineAreas = machineAreas_IN;
	// we CANNOT HAVE HRRG_GUN AND LRG_GUN, default to LRRG_GUN
	if(GlobalFunctions::entryExists(machineAreas, TYPE::HRRG_GUN))
	{
		if (GlobalFunctions::entryExists(machineAreas, TYPE::LRRG_GUN))
		{
			machineAreas.erase(std::remove(machineAreas.begin(), machineAreas.end(), TYPE::HRRG_GUN), machineAreas.end());
		}
	}
	messenger.printDebugMessage("called LLRF Factory  setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup LLRF Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	// epics valve interface has been initialized in valve constructor
	// but we have a lot of PV information to retrieve from EPICS first
	// so we will cycle through the PV structs, and set up their values.
	populateLLRFMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	cutLLRFMapByMachineAreas();

	setupChannels();
	EPICSInterface::sendToEPICS();


	for (auto& llrf : LLRFMap)
	{
		// update aliases for valve in map
		updateAliasNameMap(llrf.second);
		// update deafult tarce mean indecies 
		llrf.second.setDefaultPowerTraceMeanTimes();
		
		std::map<std::string, pvStruct>& pvstruct = llrf.second.getPVStructs();
		for (auto& pv : pvstruct)
		{
			// sets the monitor state in the pvstruict to true or false
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrieveMonitorStatus(pv.second);
				llrf.second.epicsInterface->retrieveCHTYPE(pv.second);
				llrf.second.epicsInterface->retrieveCOUNT(pv.second);
				llrf.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read = ", std::to_string(ca_read_access(pv.second.CHID)),
					", write = ", std::to_string(ca_write_access(pv.second.CHID)),
					", state = ", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					llrf.second.epicsInterface->createSubscription(llrf.second, pv.second);
					EPICSInterface::sendToEPICS();
				}
			}
			else
			{
				messenger.printMessage(pv.second.fullPVName, " CANNOT CONNECT TO EPICS");
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}


void LLRFFactory::populateLLRFMap()
{
	messenger.printDebugMessage("LLRFFactory is populating the LLRF object map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive LLRF configuration parameters from ConfigReader, "
			"please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("LLRFFactory calling parseNextYamlFile");
		reader.parseNextYamlFile(LLRFMap);
	}
	messenger.printDebugMessage("LLRFFactory has finished populating "
		"the LLRF MAP, found ", LLRFMap.size(), " LLRF objects");
}


void LLRFFactory::cutLLRFMapByMachineAreas()
{
	size_t start_size = LLRFMap.size();
	// loop over each magnet
	for (auto it = LLRFMap.begin(); it != LLRFMap.end() /* not hoisted */; /* no increment */)
	{
		// flag for if we should erase this entry, default to true 
		bool should_erase = true;
		// now we loop over every area in machineAreas and checl against isInMachineArea

		messenger.printDebugMessage(it->first, " is in area = ", ENUM_TO_STRING(it->second.getMachineArea()));

		for (auto&& machineArea : machineAreas)
		{
			// if this returns true then we should keep the LLRF and can break out the for loop 
			if (GlobalFunctions::isInMachineArea(machineArea, it->second.getMachineArea()))
			{
				should_erase = false;
				break;
			}
			// if this returns true then we should keep the LLRF and can break out the for loop 
			else if(it->second.getMachineArea() == TYPE::GUN )
			{
				if (GlobalFunctions::entryExists(machineAreas, TYPE::HRRG_GUN))
				{
					it->second.setMachineArea(TYPE::HRRG_GUN);
					should_erase = false;
					break;
				}
				else if (GlobalFunctions::entryExists(machineAreas, TYPE::LRRG_GUN))
				{
					it->second.setMachineArea(TYPE::LRRG_GUN);
					should_erase = false;
					break;
				}
				else
				{
					//messenger.printDebugMessage("ERROR NEVER SHOW THIS ");
				}
			}
			else
			{ 
			
			}
			it->second.setMachineArea(it->second.getMachineArea());
		}
		// if should_erase is still true, erase object from  magnetMap
		if (should_erase)
		{
			messenger.printDebugMessage("LLRF Factory erasing " + it->second.getHardwareName());
			it = LLRFMap.erase(it); //  m.erase(it++);    
		}
		else
		{
			++it;
		}
	}
	size_t end_size = LLRFMap.size();
	messenger.printDebugMessage("cutLLRFMapByMachineAreas LLRFMap.size() went from ", start_size," to ", end_size);
}

void LLRFFactory::updateAliasNameMap(const LLRF& llrf)
{
	// first add in the magnet full name
	std::string full_name = llrf.getHardwareName();
	//messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		//messenger.printMessage("!!ERROR!! ", full_name, " magnet name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases
	std::vector<std::string> aliases = llrf.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", llrf.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}

void LLRFFactory::setupChannels()
{
	for (auto& device : LLRFMap)
	{
		messenger.printMessage(device.second.getHardwareName(), " getting pvStructs.");
		std::map<std::string, pvStruct>& pvStructs = device.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			// thsi is connecting to a CHID
			device.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
	EPICSInterface::sendToEPICS();
	size_t count = 0;
	size_t error_count = 0;
	for (auto& device : LLRFMap)
	{
		std::map<std::string, pvStruct>& pvStructs = device.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			++count;
			++error_count;
			// this is connecting to a CHID
			channel_state state = ca_state(pv.second.CHID);
			switch (state)
			{
			case cs_never_conn:
				messenger.printMessage(pv.second.fullPVName, " cs_never_conn");
				break;
			case cs_prev_conn:
				messenger.printMessage(pv.second.fullPVName, " cs_prev_conn");
				break;
			case cs_conn:
				--error_count;
				//messenger.printMessage(pv.second.fullPVName, " cs_conn");
				break;
			case cs_closed:
				messenger.printMessage(pv.second.fullPVName, " cs_closed");
				break;
			default:
				messenger.printMessage("!!! Unexpected error while searching ca_state: ");
			}
		}
	}
	messenger.printMessage("Checking CHID state for connection errors... Found ", error_count,
		" / ", count, " errors.");
}


std::vector<std::string> LLRFFactory::getLLRFNames()
{
	std::vector<std::string> r;
	for (auto&& it : LLRFMap)
	{
		r.push_back(it.first);
	}
	return r;
}

boost::python::list LLRFFactory::getLLRFNames_Py()
{
	return to_py_list<std::string>(getLLRFNames());
}


LLRF& LLRFFactory::getLLRF(const std::string& llrf_name)
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name);
	}
	return dummy_llrf;
}

std::string LLRFFactory::getFullName(const std::string& name_to_check) const
{
	std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	std::cout << name_to_check << " NOT found " << std::endl;
	return dummy_llrf.getHardwareName();
}


bool LLRFFactory::setPhi(const std::string& llrf_name, double value)
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).setPhi(value);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return false;
}
bool LLRFFactory::setAmp(const std::string& llrf_name, double value)
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).setAmp(value);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return false;
}
bool LLRFFactory::setAmpMW(const std::string& llrf_name, double value)
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).setAmpMW(value);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return false;
}
bool LLRFFactory::setPhiDEG(const std::string& llrf_name, double value)
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).setPhiDEG(value);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return false;
}
double LLRFFactory::getPhi(const std::string& llrf_name)const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).getPhi();
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return GlobalConstants::double_min;
}
double LLRFFactory::getAmp(const std::string& llrf_name)const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).getAmp();
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return GlobalConstants::double_min;
}
double LLRFFactory::getAmpMW(const std::string& llrf_name)const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).getAmpMW();
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return GlobalConstants::double_min;
}
double LLRFFactory::getPhiDEG(const std::string& llrf_name)const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(LLRFMap, llrf_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return LLRFMap.at(llrf_name).getPhiDEG();
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, std::vector<double>> LLRFFactory::getAllTraceData(const std::string& llrf_name)const
{	
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getAllTraceData();
	}
	return 	std::map<std::string, std::vector<double>>{ { llrf_name, dummy_trace_data}};
}

std::pair<std::string, std::vector<double>> LLRFFactory::getTraceData(const std::string& llrf_name, const std::string& trace_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getTraceData(trace_name);
	}
	return 	std::pair<std::string, std::vector<double>>(llrf_name, dummy_trace_data);
}
boost::python::dict LLRFFactory::getTraceData_Py(const std::string& llrf_name, const std::string& trace_name)
{
	std::pair<std::string, std::vector<double>> r = getTraceData(llrf_name, trace_name);
	std::map<std::string, std::vector<double>> r2 { { r.first , r.second }};
	return to_py_dict<std::string, std::vector<double>>(r2);
}


std::vector<double> LLRFFactory::getTraceValues(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getTraceValues(full_name);
	}
	return dummy_trace_data;
}
std::vector<double> LLRFFactory::getCavRevPwr(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getCavRevPwr();
	}
	return dummy_trace_data;
}
std::vector<double> LLRFFactory::getCavFwdPwr(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getCavFwdPwr();
	}
	return dummy_trace_data;
}
std::vector<double> LLRFFactory::getKlyRevPwr(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getKlyRevPwr();
	}
	return dummy_trace_data;
}
std::vector<double> LLRFFactory::getKlyFwdPwr(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getKlyFwdPwr();
	}
	return dummy_trace_data;
}
std::vector<double> LLRFFactory::getCavRevPha(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getCavRevPha();
	}
	return dummy_trace_data;
}
std::vector<double> LLRFFactory::getCavFwdPha(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getCavFwdPha();
	}
	return dummy_trace_data;
}

std::vector<double> LLRFFactory::getKlyRevPha(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getKlyRevPha();
	}
	return dummy_trace_data;
}

std::vector<double> LLRFFactory::getKlyFwdPha(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getKlyFwdPha();
	}
	return dummy_trace_data;
}

std::vector<double> LLRFFactory::getProbePwr(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getCavFwdPwr();
	}
	return dummy_trace_data;
}

std::vector<double> LLRFFactory::getProbePha(const std::string& llrf_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getCavFwdPwr();
	}
	return dummy_trace_data;
}

boost::python::dict LLRFFactory::getAllTraceData_Py(const std::string& name)
{
	return to_py_dict<std::string, std::vector<double>>(getAllTraceData(name));
}

boost::python::list LLRFFactory::getTraceValues_Py(const std::string& name)const
{
	return to_py_list<double>(getTraceValues(name));
}
boost::python::list LLRFFactory::getCavRevPwr_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getCavRevPwr(llrf_name));
}
boost::python::list LLRFFactory::getCavFwdPwr_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getCavFwdPwr(llrf_name));
}
boost::python::list LLRFFactory::getKlyRevPwr_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getKlyRevPwr(llrf_name));
}
boost::python::list LLRFFactory::getKlyFwdPwr_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getKlyFwdPwr(llrf_name));
}
boost::python::list LLRFFactory::getCavRevPha_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getCavRevPha(llrf_name));
}
boost::python::list LLRFFactory::getCavFwdPha_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getCavFwdPha(llrf_name));
}
boost::python::list LLRFFactory::getKlyRevPha_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getKlyRevPha(llrf_name));
}
boost::python::list LLRFFactory::getKlyFwdPha_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getKlyFwdPha(llrf_name));
}
boost::python::list LLRFFactory::getProbePha_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getProbePha(llrf_name));
}
boost::python::list LLRFFactory::getProbePwr_Py(const std::string& llrf_name)const
{
	return to_py_list<double>(getProbePwr(llrf_name));
}


size_t LLRFFactory::getIndex(const std::string& name, const double time) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getIndex(time);
	}
	return GlobalConstants::double_min;
}
double LLRFFactory::getTime(const std::string& name, const size_t index) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getTime(index);
	}
	return GlobalConstants::double_min;
}
//--------------------------------------------------------------------------------------------------
/*  ___  __        __   ___           ___            __
	 |  |__)  /\  /  ` |__      |\/| |__   /\  |\ | /__`
	 |  |  \ /~~\ \__, |___     |  | |___ /~~\ | \| .__/
*/
bool LLRFFactory::setMeanStartEndTime(const std::string& llrf_name, const std::string& trace_name, const double start, const double end)
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).setMeanStartEndTime(start, end, trace_name);
	}
	return false;
}
bool LLRFFactory::setMeanStartIndex(const std::string& llrf_name, const std::string& trace_name, size_t  value)
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).setMeanStartIndex(trace_name, value);
	}
	return false;
}
bool LLRFFactory::setMeanStopIndex(const std::string& llrf_name, const std::string& trace_name, size_t  value)
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).setMeanStopIndex(trace_name, value);
	}
	return false;
}
size_t LLRFFactory::getMeanStartIndex(const std::string& llrf_name, const std::string& trace_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getMeanStartIndex(trace_name);
	}
	return 999999999;
}
size_t LLRFFactory::getMeanStopIndex(const std::string& llrf_name, const std::string& trace_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getMeanStopIndex(trace_name);
	}
	return 999999999;
}
//double LLRFFactory::getMean(const std::string& llrf_name, const std::string& trace_name)const
//{
//	std::string full_name = getFullName(llrf_name);
//	if (GlobalFunctions::entryExists(LLRFMap, full_name))
//	{
//		return LLRFMap.at(full_name).getMean(trace_name);
//	}
//	return GlobalConstants::double_min;
//}
double LLRFFactory::getCutMean(const std::string& llrf_name, const std::string& trace_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getCutMean(trace_name);
	}
	return GlobalConstants::double_min;
}
double LLRFFactory::getMeanStartTime(const std::string& llrf_name, const std::string& trace_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getMeanStartTime(trace_name);
	}
	return GlobalConstants::double_min;
}
double LLRFFactory::getMeanStopTime(const std::string& llrf_name, const std::string& trace_name)const
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).getMeanStopTime(trace_name);
	}
	return GlobalConstants::double_min;
}


bool LLRFFactory::setTraceDataBufferSize(const std::string& llrf_name, const std::string& trace_name, const size_t new_size)
{
	std::string full_name = getFullName(llrf_name);
	if (GlobalFunctions::entryExists(LLRFMap, full_name))
	{
		return LLRFMap.at(full_name).setTraceDataBufferSize(trace_name, new_size);
	}
	return false;
}








void LLRFFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (GlobalFunctions::entryExists(pv_to_monitor, pvStruct.pvRecord))
	{
		messenger.printMessage("Monitor ", pvStruct.pvRecord);
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}

}

void LLRFFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LLRF Factory - DEBUG On");
	//reader.debugMessagesOn();
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOn();
	}
}
void LLRFFactory::debugMessagesOff()
{
	messenger.printDebugMessage("LLRF Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOn();
	}
}
void LLRFFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("LLRF Factory - MESSAGES ON");
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOn();
	}
}
void LLRFFactory::messagesOff()
{
	messenger.printMessage("LLRF Factory - MESSAGES OFF");
	messenger.messagesOff();
	//reader.messagesOff();
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOff();
	}
}
bool LLRFFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool LLRFFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
