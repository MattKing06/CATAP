#include "MagnetFactory.h"
#include "SnapshotFileManager.h"
#include <map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
//#include <filesystem> ?? not using  c++ 17 ???
#include <cadef.h>

#include <PythonTypeConversions.h>
#include "yaml-cpp/emitter.h"


MagnetFactory::MagnetFactory() :
	MagnetFactory(STATE::OFFLINE)
{
	// std::cout << "MagnetFactory DEFAULT CONSTRUCTOR called " << std::endl;
}
MagnetFactory::MagnetFactory(STATE mode) :
MagnetFactory(mode, MASTER_LATTICE_FILE_LOCATION)
{}

MagnetFactory::MagnetFactory(STATE mode, const std::string& primeLatticeLocation) :
messenger(LoggingSystem(true, true)),
mode(mode),
hasBeenSetup(false),
reader(ConfigReader("Magnet", mode, primeLatticeLocation)),
machineAreas(std::vector<TYPE>{TYPE::ALL_VELA_CLARA})
{
	// messenger.printDebugMessage("Magnet Factory constructed");
}


//MagnetFactory::MagnetFactory(STATE mode, TYPE machineArea) :
//MagnetFactory(mode, std::vector<TYPE>{ machineArea })
//{
//	// messenger.printDebugMessage("Magnet Factory constructed");
//}
//MagnetFactory::MagnetFactory(STATE mode, const boost::python::list& machineAreas):
//MagnetFactory(mode, to_std_vector<TYPE>(machineAreas))
//{}
//MagnetFactory::MagnetFactory(STATE mode, const std::vector<TYPE>& machineAreas) :
//messenger(LoggingSystem(true, true)),
//mode(mode),
//hasBeenSetup(false),
//reader(ConfigReader("Magnet", mode)),
//machineAreas(machineAreas)
//{}

//Copy Constructor
MagnetFactory::MagnetFactory(const MagnetFactory& copyMagnetFactory) :
	hasBeenSetup(copyMagnetFactory.hasBeenSetup),
	mode(copyMagnetFactory.mode),
	machineAreas(machineAreas),
	messenger(copyMagnetFactory.messenger),
	reader(copyMagnetFactory.reader),
	dummy_magnet(copyMagnetFactory.dummy_magnet)
{
	messenger.printDebugMessage("MagnetFactory Copy contructor");
	magnetMap.insert(copyMagnetFactory.magnetMap.begin(), copyMagnetFactory.magnetMap.end());
}

MagnetFactory::~MagnetFactory()
{
	messenger.printDebugMessage("MagnetFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& magnet : magnetMap)
		{
			auto pvStructsList = magnet.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					if (pvStruct.second.EVID)
					{
						magnet.second.epicsInterface->removeSubscription(pvStruct.second);
						ca_flush_io();
					}
				}
				if (pvStruct.second.CHID)
				{
					magnet.second.epicsInterface->removeChannel(pvStruct.second);
					ca_pend_io(CA_PEND_IO_TIMEOUT);
				}
			}
		}
	}
}

void MagnetFactory::populateMagnetMap()
{
	messenger.printDebugMessage("MagnetFactory is populating the magnet map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Magnet Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(magnetMap);
	}
	// Now we go through and select only the magnets that correspond to the correct machineArea
	cutMagnetMapByMachineAreas();

	messenger.printDebugMessage("MagnetFactory has finished populating the magnet map, found ", magnetMap.size(), " magnets objects");
}

void MagnetFactory::cutMagnetMapByMachineAreas()
{
	// loop over each magnet
	for (auto it = magnetMap.cbegin(); it != magnetMap.cend() /* not hoisted */; /* no increment */)
	{
		// flag for if we should erase this entry, default to true 
		bool should_erase = true;
		// now we loop over every area in machineAreas and checl against isInMachineArea

		messenger.printDebugMessage(it->first, " is in area = ", ENUM_TO_STRING(it->second.getMachineArea()));

		for (auto&& machineArea : machineAreas)
		{
			// if this returns true then we should keep the magnet and can break out the for loop 
			if (GlobalFunctions::isInMachineArea(machineArea, it->second.getMachineArea()))
			{
				should_erase = false;
				break;
			}
			else
			{
			}
		}
		// if should_erase is still true, erase object from  magnetMap
		if (should_erase)
		{
			messenger.printDebugMessage("Magnet Factory calling parseNextYamlFile");
			it = magnetMap.erase(it); //  m.erase(it++);    
		}
		else
		{
			++it;
		}
	}
}


// TODO move to  MagnetRecords 
void MagnetFactory::setMonitorStatus(pvStruct& pvStruct)
{
	if( GlobalFunctions::entryExists(MagnetRecords::magnetMonitorRecordList,pvStruct.pvRecord) )
	{
		pvStruct.monitor = true;
		messenger.printDebugMessage("monitor = true");
	}
	else
	{
		pvStruct.monitor = false;
		messenger.printDebugMessage("monitor = false");
	}
}



void MagnetFactory::setupChannels()
{
	for (auto& magnet : magnetMap)
	{
		std::map<std::string, pvStruct>& pvStructs = magnet.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			// this is connecting to a CHID
			messenger.printMessage("Connect ", pv.first);
			magnet.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
	EPICSInterface::sendToEPICS();
	messenger.printMessage("Checking CHID state for conenction errors.");
	size_t error_count = 0;
	for (auto& magnet : magnetMap)
	{
		std::map<std::string, pvStruct>& pvStructs = magnet.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
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
	messenger.printMessage("Checking CHID state for connection errors... Found ", error_count, " errors.");
}


bool MagnetFactory::setup()
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool MagnetFactory::setup(const std::string& version)
{
	return setup(version, machineAreas);
}
bool MagnetFactory::setup(TYPE machineArea)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool MagnetFactory::setup(const std::string& version, TYPE machineArea)
{
	return setup(GlobalConstants::nominal, std::vector<TYPE>{machineArea});
}
bool MagnetFactory::setup(const std::vector<TYPE>& machineAreas)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool MagnetFactory::setup(const boost::python::list& machineAreas)
{
	return setup(GlobalConstants::nominal, to_std_vector<TYPE>(machineAreas));
}
bool MagnetFactory::setup(const std::string& version, const boost::python::list& machineAreas)
{
	return setup(version, to_std_vector<TYPE>(machineAreas));
}
bool MagnetFactory::setup(const std::string& version, const std::vector<TYPE>& machineAreas)
{
	MagnetFactory::machineAreas = machineAreas;
	messenger.printDebugMessage("Magnet Factory setup with Machine_Areas = ", GlobalFunctions::toString(MagnetFactory::machineAreas));
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Magnet Factory : it has been setup");
		return true;
	}

	//std::cout << "populateMagnetMap()" << std::endl;
	populateMagnetMap();
	//std::cout << "populateMagnetMap() fin" << std::endl;;
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	// 
	//convertConfigStringsToGlobalTypeEnums();
	setupChannels();
	EPICSInterface::sendToEPICS();

	messenger.printMessage("All MAGNET CHIDs setup, creating subscriptions");
	/*
		LOOP OVER ALL MAGNETS AGAIN TO SET MORE EPICS INFO.
	*/
	for (auto& magnet : magnetMap)
	{
		messenger.printMessage(magnet.second.getHardwareName(), " Subscriptions");
		/*
			NOW CHANNELS HAVE BEEN SENT TO EPICS, SET UP EVERYTHING ELSE
		*/
		updateAliasNameMap(magnet.second);
		std::map<std::string, pvStruct>& magPVStructs = magnet.second.getPVStructs();
		for (auto& pv : magPVStructs)
		{
			messenger.printMessage("Set up " + pv.first);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				//messenger.printMessage("Connected!, getting some channel data (COUNT, CHTYPE, ... )");
				setMonitorStatus(pv.second);
				magnet.second.epicsInterface->retrieveCHTYPE(pv.second);
				magnet.second.epicsInterface->retrieveCOUNT(pv.second);
				magnet.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": can_read = ", std::to_string(ca_read_access(pv.second.CHID)),
					", can_write = ", std::to_string(ca_write_access(pv.second.CHID)),
					", state = ", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					magnet.second.epicsInterface->createSubscription(magnet.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(magnet.first, ":", pv.second.pvRecord, " CANNOT CONNECT TO EPICS");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
		}
	}
	//int status = EPICSInterface::caFlushIO("ca_create_subscription");
	int status = EPICSInterface::sendToEPICSReturnStatus();
	switch (status)
	{
	case ECA_NORMAL:
		messenger.printMessage("success");
		break;
	case ECA_TIMEOUT:
		messenger.printMessage("timeout");
		break;
	case ECA_BADTYPE:
		messenger.printMessage("Invalid DBR_XXXX type");
		break;
	case ECA_BADCHID:
		messenger.printMessage("Corrupted CHID");
		break;
	case ECA_BADCOUNT:
		messenger.printMessage("Requested count larger than native element count");
		break;
	case ECA_GETFAIL:
		messenger.printMessage("A local database get failed");
		break;
	case ECA_NORDACCESS:
		messenger.printMessage("Read access denied");
		break;
	case ECA_DISCONN:
		messenger.printMessage("Unable to allocate memory");
		break;
	default:
		messenger.printMessage("!!! Unexpected error while searching: ", ca_message(status));
	}

	hasBeenSetup = true;
	//std::cout << "hasBeenSetup = true " << std::endl;
	return hasBeenSetup;
}

//void MagnetFactory::convertConfigStringsToGlobalTypeEnums()
//{/
//
//}
void MagnetFactory::updateAliasNameMap(const Magnet& magnet)
{
	// first add in the magnet full name
	std::string full_name = magnet.getHardwareName();
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
	std::vector<std::string> aliases = magnet.getAliases();

	std::ostringstream vts;
	if (!aliases.empty())
	{
		// Convert all but the last element to avoid a trailing "," 
		std::copy(aliases.begin(), aliases.end() - 1,
			std::ostream_iterator<std::string>(vts, ", "));

		// Now add the last element with no delimiter 
		vts << aliases.back();
	}

	messenger.printMessage("Got Aliases: ", vts.str(), " for " + full_name);


	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", magnet.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}

std::string MagnetFactory::getFullName(const std::string& name_to_check) const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return dummy_magnet.getHardwareName();
}


bool MagnetFactory::isAType(const std::string& name, const TYPE type)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMagnetType() == type;
	}
	messenger.printMessage("!!ERROR!! MagnetFactory::isAType cannot find magnet with name = ", name);
	return GlobalConstants::double_min;
}
bool MagnetFactory::isAQuad(const std::string& name)const
{
	return isAType(name, TYPE::QUADRUPOLE);
}
bool MagnetFactory::isADip(const std::string& name)const
{
	return isAType(name, TYPE::DIPOLE);
}
bool MagnetFactory::isASol(const std::string& name)const
{
	return isAType(name, TYPE::SOLENOID);
}
bool MagnetFactory::isABSol(const std::string& name)const
{
	return isAType(name, TYPE::SOLENOID);
}
bool MagnetFactory::isAVCor(const std::string& name)const
{
	return isAType(name, TYPE::VERTICAL_CORRECTOR);
}
bool MagnetFactory::isAHCor(const std::string& name)const
{
	return isAType(name, TYPE::HORIZONTAL_CORRECTOR);
}
bool MagnetFactory::isACor(const std::string& name)const
{
	return isAVCor(name) || isAHCor(name);
}


double MagnetFactory::getMinI(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMinI();
	}
	std::cout << "!!ERROR!! MagnetFactory::getMinI cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getMinIs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getMinI(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getMinIs_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getMinIs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllMinI() const
{
	return getMinIs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllMinI_Py() const
{
	return to_py_dict<std::string, double>(getAllMinI());
}
// get max I 
double MagnetFactory::getMaxI(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMaxI();
	}
	std::cout << "!!ERROR!! MagnetFactory::getMinI cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getMaxIs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getMaxI(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getMaxIs_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getMaxIs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllMaxI() const
{
	return getMaxIs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllMaxI_Py() const
{
	return to_py_dict<std::string, double>(getAllMaxI());
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

double MagnetFactory::getKDipP(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getKDipP();
	}
	std::cout << "!!ERROR!! MagnetFactory::getKDipP cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getKDipPs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getKDipP(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getKDipPs_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getKDipPs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllKDipP() const
{
	return getKDipPs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllKDipP_Py() const
{
	return to_py_dict<std::string, double>(getAllKDipP());
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
double MagnetFactory::getIntStr_mm(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getIntStr_mm();
	}
	std::cout << "!!ERROR!! MagnetFactory::getKDipP cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getIntStrs_mm(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getIntStr_mm(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getIntStrs_mm_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getIntStrs_mm(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllIntStr_mm() const
{
	return getIntStrs_mm(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllIntStr_mm_Py() const
{
	return to_py_dict<std::string, double>(getAllIntStr_mm());
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
double MagnetFactory::getIntStr(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getIntStr();
	}
	std::cout << "!!ERROR!! MagnetFactory::getKDipP cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getIntStrs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getIntStr(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getIntStrs_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getIntStrs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllIntStr() const
{
	return getIntStrs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllIntStr_Py() const
{
	return to_py_dict<std::string, double>(getAllIntStr());
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

double MagnetFactory::getKSetP(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getKSetP();
	}
	std::cout << "!!ERROR!! MagnetFactory::getKDipP cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getKSetPs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getKSetP(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getKSetPs_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getKSetPs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllKSetP() const
{
	return getKSetPs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllKSetP_Py() const
{
	return to_py_dict<std::string, double>(getAllKSetP());
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

double MagnetFactory::getKAng(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getKAng();
	}
	std::cout << "!!ERROR!! MagnetFactory::getKDipP cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getKAngs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getKAng(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getKAngs_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getKAngs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllKAng() const
{
	return getKAngs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllKAng_Py() const
{
	return to_py_dict<std::string, double>(getAllKAng());
}


std::map<std::string, double> MagnetFactory::getAllDipoleKAng() const
{
	return getKAngs(getAllDipoleNames());
}
boost::python::dict MagnetFactory::getAllDipoleKAng_Py() const
{
	return to_py_dict<std::string, double>(getAllDipoleKAng());
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

double MagnetFactory::getKmrad(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getKmrad();
	}
	std::cout << "!!ERROR!! MagnetFactory::getKDipP cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getKmrads(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getKmrad(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getKmrad_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getKmrads(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllKmrad() const
{
	return getKmrads(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllKmrad_Py() const
{
	return to_py_dict<std::string, double>(getAllKmrad());
}

std::map<std::string, double> MagnetFactory::getAllCorrectorKmrad() const
{
	return getKmrads(getAllCorrectorNames());
}
boost::python::dict MagnetFactory::getAllCorrectorKmrad_Py() const
{
	return to_py_dict<std::string, double>(getAllCorrectorKmrad());
}
std::map<std::string, double> MagnetFactory::getAllHCorrKmrad() const
{
	return getKmrads(getAllHCorrectorNames());
}
boost::python::dict MagnetFactory::getAllHCorrKmrad_Py() const
{
	return to_py_dict<std::string, double>(getAllHCorrKmrad());
}
std::map<std::string, double> MagnetFactory::getAllVCorrKmrad() const
{
	return getKmrads(getAllVCorrectorNames());
}
boost::python::dict MagnetFactory::getAllVCorrKmrad_Py() const
{
	return to_py_dict<std::string, double>(getAllVCorrKmrad());
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


double MagnetFactory::getKVal(const std::string& name)const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getKVal();
	}
	std::cout << "!!ERROR!! MagnetFactory::getKDipP cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getKVals(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getKVal(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getKVals_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getKVals(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllKVal() const
{
	return getKVals(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllKVal_Py() const
{
	return to_py_dict<std::string, double>(getAllKVal());
}


std::map<std::string, double> MagnetFactory::getAllQuadKVals() const
{
	return getKVals(getAllQuadNames());
}
boost::python::dict MagnetFactory::getAllQuadKVals_Py() const
{
	return to_py_dict<std::string, double>(getAllQuadKVals());
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// GET MAGNET OBJECT  FUNCTIONS
//
Magnet& MagnetFactory::getMagnet(const std::string& magnetName)
{
	std::string full_name = getFullName(magnetName);
	if (GlobalFunctions::entryExists(magnetMap, full_name))
	{
		return magnetMap.at(full_name);
	}
	return dummy_magnet;
}


std::vector<std::string> MagnetFactory::getAllMagnetNames()const
{
	std::vector<std::string> return_names;
	for (auto&& item : magnetMap)
	{
		return_names.push_back(item.first);
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllMagnetNames_Py()const
{
	return to_py_list<std::string>(getAllMagnetNames());
}


std::vector<std::string> MagnetFactory::getAllDipoleNames()const
{
	std::vector<std::string> return_names;
	for (auto&& item : magnetMap)
	{
		if(isADip(item.first))
		{
			return_names.push_back(item.first);
		}
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllDipoleNames_Py()const
{
	return to_py_list<std::string>(getAllDipoleNames());
}

std::vector<std::string> MagnetFactory::getAllQuadNames()const
{
	std::vector<std::string> return_names;
	for (auto&& item : magnetMap)
	{
		if (isAQuad(item.first))
		{
		return_names.push_back(item.first);
		}
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllQuadNames_Py()const
{
	return to_py_list<std::string>(getAllQuadNames());
}
std::vector<std::string> MagnetFactory::getAllSolNames()const
{
	std::vector<std::string> return_names;
	for (auto&& item : magnetMap)
	{
		if (isASol(item.first))
		{
			return_names.push_back(item.first);
		}
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllSolNames_Py()const
{
	return to_py_list<std::string>(getAllSolNames());
}
std::vector<std::string> MagnetFactory::getAllCorrectorNames()const
{
	std::vector<std::string> return_names;
	for (auto&& item : magnetMap)
	{
		if (isACor(item.first))
		{
			return_names.push_back(item.first);
		}
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllCorrectorNames_Py()const
{
	return to_py_list<std::string>(getAllCorrectorNames());
}
std::vector<std::string> MagnetFactory::getAllHCorrectorNames()const
{
	std::vector<std::string> return_names;
	for (auto&& item : magnetMap)
	{
		if (isAHCor(item.first))
		{
			return_names.push_back(item.first);
		}
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllHCorrectorNames_Py()const
{
	return to_py_list<std::string>(getAllHCorrectorNames());
}
std::vector<std::string> MagnetFactory::getAllVCorrectorNames()const
{
	std::vector<std::string> return_names;
	for (auto&& item : magnetMap)
	{
		if (isAVCor(item.first))
		{
			return_names.push_back(item.first);
		}
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllVCorrectorNames_Py()const
{
	return to_py_list<std::string>(getAllVCorrectorNames());
}

double MagnetFactory::getSETI(const std::string& name)  const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getSETI();
	}
	std::cout << "!!ERROR!! MagnetFactory::getSETI cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getSETIs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getSETI(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getSETIs_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getSETIs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllSETI() const
{
	return getSETIs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllSETI_Py() const
{
	return to_py_dict<std::string, double>(getAllSETI());
}



double MagnetFactory::getREADI(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getREADI();
	}
	std::cout << "!!ERROR!! MagnetFactory::getREADI cannot find magnet with name = " << name << std::endl;
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getREADIs(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getREADI(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getREADI_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getREADIs(to_std_vector<std::string>(names)));
}
std::map<std::string, double> MagnetFactory::getAllREADI() const
{
	return getREADIs(getAllMagnetNames());
}
boost::python::dict MagnetFactory::getAllREADI_Py() const
{
	return to_py_dict<std::string, double>(getAllREADI());
}



STATE MagnetFactory::getPSUState(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getPSUState();
	}
	std::cout << "!!ERROR!! MagnetFactory::getPSUState cannot find magnet with name = " << name << std::endl;
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::getPSUStates(const std::vector<std::string>& names) const
{
	std::map<std::string, STATE> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getPSUState(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getPSUState_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, STATE>(getPSUStates(to_std_vector<std::string>(names)));
}
std::map<std::string, STATE> MagnetFactory::getAllPSUState() const
{
	std::map<std::string, STATE> return_map;
	for (auto&& magnet : magnetMap)
	{
		return_map[magnet.first] = magnet.second.getPSUState();
	}
	return return_map;
}
boost::python::dict MagnetFactory::getAllPSUState_Py() const
{
	return to_py_dict<std::string, STATE>(getAllPSUState());
}

std::map<std::string, STATE> MagnetFactory::switchOffAll()
{
	return switchOff(getAllMagnetNames());
}

boost::python::dict MagnetFactory::switchOffAll_Py()
{
	return to_py_dict<std::string, STATE>(switchOffAll());
}
std::map<std::string, STATE> MagnetFactory::switchOnAll()
{
	return switchOn(getAllMagnetNames());
}
boost::python::dict MagnetFactory::switchOnAll_Py()
{
	return to_py_dict<std::string, STATE>(switchOnAll());
}


bool MagnetFactory::degauss(const std::string& name, const bool reset_to_zero)
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).degauss(reset_to_zero);
	}
	return false;
}
bool MagnetFactory::degauss(const std::string& name, const double set_value_after_degauss)
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).degauss(set_value_after_degauss);
	}
	return false;
}


std::map<std::string, bool> MagnetFactory::degauss(const std::vector<std::string>& names, const bool reset_to_zero)
{
	std::map<std::string, bool> return_map;
	for (auto&& name : names)
	{
		return_map[name] = degauss(name, reset_to_zero);
	}
	return return_map;
}
boost::python::dict MagnetFactory::degauss_Py(const boost::python::list& names, const bool reset_to_zero)
{
	return to_py_dict<std::string, bool>(degauss(to_std_vector<std::string>(names), reset_to_zero));
}
std::map<std::string, bool> MagnetFactory::degaussALL(const bool reset_to_zero)
{
	return degauss(getAllMagnetNames(), reset_to_zero);
}
boost::python::dict MagnetFactory::degaussAll_Py(const bool reset_to_zero)
{
	return degauss_Py(to_py_list<std::string>(getAllMagnetNames()), reset_to_zero);
}


std::vector<std::string> MagnetFactory::getAliases(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getAliases();
	}
	std::vector<std::string> dummy;
	return dummy;
}
boost::python::list MagnetFactory::getAliases_Py1(const std::string& name) const
{
	return to_py_list(getAliases(name));
}
std::map<std::string, std::vector<std::string>> MagnetFactory::getAliases(const std::vector<std::string>& names) const
{
	std::map<std::string, std::vector<std::string>> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getAliases(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getAliases_Py2(const boost::python::list& names) const
{
	return to_py_dict<std::string, std::vector<std::string>>(getAliases(to_std_vector<std::string>(names)));
}



std::string MagnetFactory::getManufacturer(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getManufacturer();
	}
	return GlobalConstants::DUMMY_NAME;
}
std::map<std::string, std::string> MagnetFactory::getManufacturer(const std::vector<std::string>& names) const
{
	std::map<std::string, std::string> return_map;
	for (auto& name : names)
	{
		return_map[name] = getManufacturer(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getManufacturer_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, std::string>(getManufacturer(to_std_vector<std::string>(names)));
}










//// TODO Should be string
//int MagnetFactory::getSerialNumber(const std::string& name) const;
//std::map<std::string, int> MagnetFactory::getSerialNumber(const std::vector<std::string>& name) const;
//boost::python::dict MagnetFactory::getManufacturerPy(const boost::python::list& name) const;
//

TYPE MagnetFactory::getMagnetType(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMagnetType();
	}
	return TYPE::UNKNOWN_TYPE;
}
std::map<std::string, TYPE> MagnetFactory::getMagnetType(const std::vector<std::string>& names) const
{
	std::map<std::string, TYPE> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getMagnetType(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getMagnetType_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, TYPE>(getMagnetType(to_std_vector<std::string>(names)));
}


std::string MagnetFactory::getMagnetRevType(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMagnetRevType();
	}
	return GlobalConstants::DUMMY_NAME;
}
std::map<std::string, std::string> MagnetFactory::getMagnetRevType(const std::vector<std::string>& names) const
{
	std::map<std::string, std::string> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getMagnetRevType(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getMagnetRevType_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, std::string>(getMagnetRevType(to_std_vector<std::string>(names)));
}


std::string MagnetFactory::getFullPSUName(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getFullPSUName();
	}
	return GlobalConstants::DUMMY_NAME;
}
std::map<std::string, std::string> MagnetFactory::getFullPSUName(const std::vector<std::string>& names) const
{
	std::map<std::string, std::string> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getFullPSUName(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getFullPSUName_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, std::string>(getFullPSUName(to_std_vector<std::string>(names)));
}

std::string MagnetFactory::getMeasurementDataLocation(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMeasurementDataLocation();
	}
	return GlobalConstants::DUMMY_NAME;
}
std::map<std::string, std::string> MagnetFactory::getMeasurementDataLocation(const std::vector<std::string>& names) const
{
	std::map<std::string, std::string> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getMeasurementDataLocation(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getMeasurementDataLocation_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, std::string>(getMeasurementDataLocation(to_std_vector<std::string>(names)));
}

size_t MagnetFactory::getNumberOfDegaussSteps(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getNumberOfDegaussSteps();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, size_t> MagnetFactory::getNumberOfDegaussSteps(const std::vector<std::string>& names) const
{
	std::map<std::string, size_t> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getNumberOfDegaussSteps(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getNumberOfDegaussSteps_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, size_t>(getNumberOfDegaussSteps(to_std_vector<std::string>(names)));
}



std::vector<double> MagnetFactory::getDegaussValues(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getDegaussValues();
	}
	std::vector<double> dummy;
	return dummy;
}
boost::python::list MagnetFactory::getDegaussValuesSingle_Py(const std::string& name) const
{
	return to_py_list<double>(getDegaussValues(name));
}
std::map<std::string, std::vector<double>> MagnetFactory::getDegaussValues(const std::vector<std::string>& names) const
{
	std::map<std::string, std::vector<double>> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getDegaussValues(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getDegaussValuesMulti_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, std::vector<double>>(getDegaussValues(to_std_vector<std::string>(names)));
}


double MagnetFactory::getDegaussTolerance(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getDegaussTolerance();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getDegaussTolerance(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getDegaussTolerance(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getDegaussTolerance_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getDegaussTolerance(to_std_vector<std::string>(names)));
}

std::vector<double> MagnetFactory::setDegaussValues(const std::string& name, const std::vector<double>& values)
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).setDegaussValues(values);
	}
	std::vector<double> dummy;
	return dummy;
}
boost::python::list MagnetFactory::setDegaussValuesSingle_Py(const std::string& name, const boost::python::list& values)
{
	return to_py_list(setDegaussValues(name, to_std_vector<double>(values)));
}
std::map<std::string, std::vector<double>> MagnetFactory::setDegaussValues(const std::vector < std::string>& names, const std::vector<double>& values)
{
	std::map<std::string, std::vector<double>> r;
	for (auto&& name : names)
	{
		r[name] = setDegaussValues(name, values);
	}
	return r;
}
boost::python::dict MagnetFactory::setDegaussValuesMulti_Py(const boost::python::list& names, const boost::python::list& values)
{
	return to_py_dict <std::string, std::vector<double>>(setDegaussValues(to_std_vector<std::string>(names), to_std_vector<double>(values)));
}


double MagnetFactory::setDegaussTolerance(const std::string& name, const double value)
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).setDegaussTolerance(value);
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::setDegaussTolerance(const std::vector < std::string>& names, const double value)
{
	std::map<std::string, double> r;
	for (auto&& name : names)
	{
		r[name] = setDegaussTolerance(name, value);
	}
	return r;
}
boost::python::dict MagnetFactory::setDegaussTolerance_Py(const boost::python::list& names, const double value)
{
	return to_py_dict <std::string, double>(setDegaussTolerance(to_std_vector<std::string>(names), value));
}


double MagnetFactory::setREADITolerance(const std::string& name, const double value)
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).setREADITolerance(value);
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::setREADITolerance(const std::vector < std::string>& names, const double value)
{
	std::map<std::string, double> r;
	for (auto&& name : names)
	{
		r[name] = setREADITolerance(name, value);
	}
	return r;
}
boost::python::dict MagnetFactory::setREADITolerance_Py(const boost::python::list& names, const double value)
{
	return to_py_dict <std::string, double>(setREADITolerance(to_std_vector<std::string>(names), value));
}

bool MagnetFactory::isREADIequalValue(const std::string& name, const double value) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).isREADIequalValue(value);
	}
	return false;
}
bool MagnetFactory::isREADIequalValue(const std::string& name, const double value, const double tolerance) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).isREADIequalValue(value, tolerance);
	}
	return false;
}


double MagnetFactory::getMagneticLength(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMagneticLength();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getMagneticLength(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getMagneticLength(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getMagneticLength_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getMagneticLength(to_std_vector<std::string>(names)));
}

double MagnetFactory::getREADITolerance(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getREADITolerance();
	}
	return GlobalConstants::double_min;
}
std::map<std::string, double> MagnetFactory::getREADITolerance(const std::vector < std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getREADITolerance(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getREADITolerance_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, double>(getMagneticLength(to_std_vector<std::string>(names)));

}
STATE MagnetFactory::getILKState(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getIlkState();
	}
	std::cout << "!!ERROR!! MagnetFactory::getILKState cannot find magnet with name = " << name << std::endl;
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::getILKStates(const std::vector<std::string>& names) const
{
	std::map<std::string, STATE> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getILKState(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getILKState_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, STATE>(getILKStates(to_std_vector<std::string>(names)));
}
std::map<std::string, STATE>  MagnetFactory::getAllILKState() const
{
	std::map<std::string, STATE> return_map;
	for (auto&& magnet : magnetMap)
	{
		return_map[magnet.first] = magnet.second.getIlkState();
	}
	return return_map;
}
boost::python::dict MagnetFactory::getAllILKState_Py() const
{
	return to_py_dict<std::string, STATE>(getAllILKState());
}
STATE MagnetFactory::SETI(const std::string& name, const double& value)
{
	std::string  full_name = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, full_name))
	{
		if (magnetMap.at(full_name).SETI(value))
		{
			return STATE::SUCCESS;
		}
		return STATE::FAIL;
	}
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::SETI(const std::map<std::string, double>& namesAndCurrentsMap)
{
	std::map<std::string, STATE> r;
	for (const auto& entry : namesAndCurrentsMap)
	{
		r[entry.first] = SETI(entry.first, entry.second);
	}
	return r;
}
boost::python::dict MagnetFactory::SETI_Py(const boost::python::dict& namesAndCurrentsDict)
{
	return to_py_dict<std::string, STATE>(SETI(to_std_map<std::string, double>(namesAndCurrentsDict)));
}
std::map<std::string, STATE> MagnetFactory::SETIAllZero()
{
	std::map<std::string, double> s;
	for (auto&& name : getAllMagnetNames())
	{
		s[name] = GlobalConstants::zero_double;
	}
	return SETI(s);
}
boost::python::dict MagnetFactory::SETIAllZero_Py()
{
	return to_py_dict<std::string, STATE>(SETIAllZero());
}
STATE MagnetFactory::setKSetP(const std::string& name, const double value)
{
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		if (magnetMap.at(name).setKSetP(value))
		{
			return STATE::SUCCESS;
		}
		return STATE::FAIL;
	}
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::setKSetP(const std::vector<std::string>& names, const double value)
{
	std::map<std::string, STATE> r;
	for (const auto& entry : names)
	{
		r[entry] = setKSetP(entry, value);
	}
	return r;
}
boost::python::dict  MagnetFactory::setKSetP_Py(const boost::python::list& names, const double value)
{
	return to_py_dict<std::string, STATE>(setKSetP(to_std_vector<std::string>(names), value));
}
std::map<std::string, STATE> MagnetFactory::setKSetP(const TYPE area, const double value)
{
	return setKSetP(getNamesInArea(area), value);
}
boost::python::dict MagnetFactory::setKSetP_area_Py(TYPE area, const double value)
{
	return to_py_dict<std::string, STATE>(setKSetP(area, value));
}
std::map<std::string, STATE> MagnetFactory::setKSetP(const std::vector<TYPE>& areas, const double value)
{
	return setKSetP(getNamesInAreas(areas), value);
}
boost::python::dict  MagnetFactory::setKSetP_Areas_py(const boost::python::list& areas, const double value)
{
	return to_py_dict<std::string, STATE>(setKSetP(to_std_vector<std::string>(areas), value));
}
STATE MagnetFactory::switchOn(const std::string& name)
{
	std::string  full_name = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, full_name))
	{
		if (magnetMap.at(full_name).switchOn())
		{
			return STATE::SUCCESS;
		}
		return STATE::FAIL;
	}
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::switchOn(const std::vector<std::string>& names)
{
	std::map<std::string, STATE> return_map;
	for (auto& name : names)
	{
		return_map[name] = switchOn(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::switchOn_Py(const boost::python::list names)
{
	//messenger.printDebugMessage("TURNING ", name, " On");
	return to_py_dict<std::string, STATE>(switchOn(to_std_vector<std::string>(names)));
}
STATE MagnetFactory::switchOff(const std::string& name)
{
	std::string  full_name = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, full_name))
	{
		if (magnetMap.at(full_name).switchOff())
		{
			return STATE::SUCCESS;
		}
		return STATE::FAIL;
	}
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::switchOff(const std::vector<std::string>& names)
{
	std::map<std::string, STATE> return_map;
	for (auto& name : names)
	{
		return_map[name] = switchOff(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::switchOff_Py(const boost::python::list& names)
{
	return to_py_dict<std::string, STATE>(switchOff(to_std_vector<std::string>(names)));
}



std::vector<std::string> MagnetFactory::getNamesInArea(TYPE area) const
{
	std::vector<std::string> r;
	for (auto&& mag : magnetMap)
	{
		if (mag.second.getMachineArea() == area)
		{
			r.push_back(mag.first);
		}
	}
	return r;
}
boost::python::list MagnetFactory::getNamesInArea_Py(TYPE area) const
{
	return to_py_list<std::string>(getNamesInArea(area));
}
std::vector<std::string> MagnetFactory::getNamesInAreas(const std::vector<TYPE>& areas)const 
{
	std::vector<std::string> r;
	for (auto&& area : areas)
	{
		auto b = getNamesInArea(area);
		r.insert(r.end(), b.begin(), b.end());
		
	}
	return r;
}
boost::python::list MagnetFactory::getNamesInAreas_Py(const boost::python::list& areas) const
{
	return to_py_list<std::string>(getNamesInAreas(to_std_vector<TYPE>(areas)));
}




STATE MagnetFactory::resetILK(const std::string& name) const
{
	std::string  full_name = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, full_name))
	{
		if (magnetMap.at(full_name).resetILK())
		{
			return STATE::SUCCESS;
		}
		return STATE::FAIL;
	}
	return STATE::UNKNOWN_NAME;
}

STATE MagnetFactory::resetAllILK() const
{
	messenger.printDebugMessage("resetAllILK  ");
	STATE r = STATE::UNKNOWN_STATE;
	for (auto& mag : magnetMap)
	{
		messenger.printDebugMessage("Reset  ", mag.first);
		if (mag.second.resetILK()) 
		{
		}
		else
		{ 
			r = STATE::FAIL; 
		}
		messenger.printDebugMessage("Move to Next Magnet ");

	}
	if (r != STATE::FAIL) 
	{ 
		return STATE::SUCCESS; 
	}
	return r;
}

//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------





STATE MagnetFactory::saveSnapshot(const std::string& comments)
{
	//magnet_snapshot_default_path
	return saveSnapshot(SnapshotFileManager::magnet_snapshot_default_path, GlobalFunctions::getTimeAndDateString(), comments);
}
STATE MagnetFactory::saveSnapshot(const std::string& filepath, const std::string& filename, const std::string& comments)
{
	hardwareSnapshotMap = getSnapshot();
	bool written = SnapshotFileManager::writeSnapshotToYAML(filepath, filename, hardwareSnapshotMapToYAMLNode(hardwareSnapshotMap), mode,  comments);
	if (written)
	{
		return STATE::SUCCESS;
	}
	return STATE::FAIL;
}
STATE MagnetFactory::saveSnapshot_Pydict(const boost::python::dict& snapshot_dict, const std::string& comments)
{
	messenger.printDebugMessage("saveSnapshot_Pydict");
	return saveSnapshot_Pyfile(SnapshotFileManager::magnet_snapshot_default_path, GlobalFunctions::getTimeAndDateString(), snapshot_dict, comments);
}
STATE MagnetFactory::saveSnapshot_Pyfile(const std::string& filepath, const std::string& filename, const boost::python::dict& snapshot_dict, const std::string& comments)
{
	messenger.printDebugMessage("saveSnapshot_Pyfile, convert dict to hssm ");
	auto hssm = pyDictToHardwareSnapshotMap(snapshot_dict);
	messenger.printDebugMessage("hssm size = ", hssm.size());
	auto yn = hardwareSnapshotMapToYAMLNode(hssm);
	messenger.printDebugMessage("yn size = ", yn.size());
	bool written = SnapshotFileManager::writeSnapshotToYAML(filepath, filename, yn, mode, comments);
	if (written)
	{
		return STATE::SUCCESS;
	}
	return STATE::FAIL;
}



STATE MagnetFactory::loadSnapshot(const std::string filepath, const std::string& filename) // read into hardwareSnapshotMap
{
	messenger.printDebugMessage("loadSnapshot");
	YAML::Node file_node = SnapshotFileManager::readSnapshotFile(filepath, filename);
	messenger.printDebugMessage("loadSnapshot get a file_node with size() ", file_node.size());
	hardwareSnapshotMap = yamlNodeToHardwareSnapshotMap(file_node);
	return STATE::SUCCESS; // TODO do we need some error checking 
}
STATE MagnetFactory::loadSnapshot_Py(const boost::python::dict& snapshot_dict) // put d into hardwareSnapshotMap
{
	hardwareSnapshotMap = pyDictToHardwareSnapshotMap(snapshot_dict);
	return STATE::SUCCESS;
}
std::map<std::string, HardwareSnapshot> MagnetFactory::getSnapshot() // c++ version 
{
	std::map<std::string, HardwareSnapshot> snapshot_map;
	for (auto& item : magnetMap)
	{
		messenger.printDebugMessage("getSnapshot, found ", item.first);
		snapshot_map[item.first] = item.second.getSnapshot();
	}
	messenger.printDebugMessage("returning snapshot_map with size =  ", snapshot_map.size());
	return snapshot_map;
}
boost::python::dict MagnetFactory::getSnapshot_Py() // return current state as py dict 
{
	messenger.printDebugMessage("getSnapshot_Py, calling getSnapshot ");
	// FIRST GET A snaphot that is current (i.e. live!)! 
	std::map<std::string, HardwareSnapshot> current_snapshot =  getSnapshot();
	boost::python::dict r; // TODO MAGIC STRING
	messenger.printDebugMessage("loop over current_snapshot (.size() =", current_snapshot.size());
	for (auto&& item : current_snapshot)
	{
		// THIS IS NOT UPDATEING ANY DATA IN THE HARDWARE SNAPSHOT _ ITS CONVERTING WHAT ALREADY EXIST "
		messenger.printDebugMessage(item.first, " get snap data");
		r[item.first] = item.second.getSnapshot_Py();
	}
	boost::python::dict r2;
	r2[ ENUM_TO_STRING(TYPE::MAGNET) ] = r;
	return r2;
}
boost::python::dict MagnetFactory::getSnapshotFromFile_Py(const std::string& location, const std::string& filename) // return file contents as py dict 
{
	messenger.printDebugMessage("getSnapshotFromFile_Py loadSnapshot");
	loadSnapshot(location, filename);
	boost::python::dict r;
	messenger.printDebugMessage("loop over hardwareSnapshotMap, with size = ", hardwareSnapshotMap.size());
	for (auto&& item : hardwareSnapshotMap)
	{
		messenger.printDebugMessage(item.first, " get snap data");
		// THIS IS NOT UPDTAEING ANY DATA IN TH EHARDWRAE SNAPSHOT _ ITS CONVERTING WAHT ALREADY EXIST "
		r[item.first] = item.second.getSnapshot_Py();
	}
	boost::python::dict r2;
	r2[ENUM_TO_STRING(TYPE::MAGNET)] = r;
	return r2;
}


STATE MagnetFactory::applySnaphot(const boost::python::dict& snapshot_dict, TYPE magnet_type )
{
	hardwareSnapshotMap = pyDictToHardwareSnapshotMap(snapshot_dict);
	return applyhardwareSnapshotMap(hardwareSnapshotMap, magnet_type);
}
STATE MagnetFactory::applySnaphot(const std::string& filepath, const std::string& filename, TYPE magnet_type )
{
	loadSnapshot(filepath, filename); // this sets the hardwareSnapshotMap member variables 
	return applyhardwareSnapshotMap(hardwareSnapshotMap, magnet_type);
}
STATE MagnetFactory::applyLoadedSnaphost(TYPE magnet_type )
{
	return applyhardwareSnapshotMap(hardwareSnapshotMap, magnet_type);
}
STATE MagnetFactory::applyhardwareSnapshotMap(const std::map<std::string, HardwareSnapshot>& hardwaresnapshot_map, TYPE magnet_type )
{
	for (auto&& item : hardwaresnapshot_map)
	{
		std::string fullName = getFullName(item.first);
		if (GlobalFunctions::entryExists(magnetMap, fullName))
		{
			// only apply to magnets that match magnet_type
			bool correct_magnet_type = false;
			switch (magnet_type)
			{
			case TYPE::CORRECTOR: 
				correct_magnet_type = isACor(fullName); 
				break;
			case TYPE::VERTICAL_CORRECTOR: 
				correct_magnet_type = isAVCor(fullName); 
				break;
			case TYPE::HORIZONTAL_CORRECTOR: correct_magnet_type = 
				isAHCor(fullName); 
				break;
			case TYPE::QUADRUPOLE: 
				correct_magnet_type = isAQuad(fullName); 
				break;
			case TYPE::MAGNET:
				correct_magnet_type = true;
				break;
			default:
				correct_magnet_type = false;
			}
			
			if (correct_magnet_type)
			{
				for (auto&& state_items : item.second.state)
				{
					if (state_items.first == "GETSETI")
					{
						magnetMap.at(fullName).SETI(item.second.get<double>(state_items.first));
						messenger.printDebugMessage(fullName, " SETI ", item.second.get<double>(state_items.first));
					}
					else if (state_items.first == "RPOWER")
					{
						STATE saved_psu_state = item.second.get<STATE>(state_items.first);
						if (saved_psu_state == STATE::ON)
						{
							magnetMap.at(fullName).switchOn();
						}
						else if (saved_psu_state == STATE::OFF)
						{
							magnetMap.at(fullName).switchOff();
						}
					}
				}
			}
		}
	}
	return STATE::SUCCESS;
}

// CONVERTERS between boost:python::dict yaml::node and hssm (other conversion happen else where, e.g. hssm to pydict / yaml::node HardwareSnapshot.h
std::map<std::string, HardwareSnapshot> MagnetFactory::yamlNodeToHardwareSnapshotMap(const YAML::Node& input_node)
{
	// This function returns a map of <OBJECT NAME: snap parameters> 
	// We fill it from the yaml node data 
	// map of < hardwarename, HardwareSnapshot > 
	// each HardwareSnapshot will be passed to its correpsonding hardware object in another function  
	//
	// Without reading more of the manual, it seem that you can access "nodes within the main node" 
	//  so auto& lower_level_node = snapshot_data[ENUM_TO_STRING(TYPE::MAGNET)] APPEARS NOT TO WORK!!! 
	// (BUT IT MIGHT IF YOU DO IT CORRECTLY) 
	//
	std::map<std::string, HardwareSnapshot> return_map;
	messenger.printMessage("yamlNodeToHardwareSnapshotMap");
	messenger.printMessage("loop over input_node ");
	for (auto& it : input_node["MAGNET"])
	{
		std::string object_name = getFullName(it.first.as<std::string>());
		messenger.printDebugMessage("(objectname) key = ", object_name);
		std::map<std::string, std::string >  value = it.second.as<std::map<std::string, std::string >>();
		//return_map[object_name] = HardwareSnapshot();
		for (auto&& map_it : value)
		{
			std::string record = map_it.first;
			//std::cout << "(pv record, value) key / value = " << map_it.first << "/" << map_it.second << std::endl;
			//std::cout  << "record = " << record  << std::endl;
			if (record == MagnetRecords::K_VAL) {}
			else if (record == MagnetRecords::SPOWER)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == MagnetRecords::RILK)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == MagnetRecords::K_MRAD)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == MagnetRecords::K_ANG)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == MagnetRecords::K_SET_P)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == MagnetRecords::K_DIP_P)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == MagnetRecords::INT_STR)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == MagnetRecords::INT_STR_MM)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == MagnetRecords::READI)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == MagnetRecords::RPOWER)
			{	//std::cout << "record is MagnetRecords::RPOWER" << std::endl;
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			if (record == MagnetRecords::SETI)
			{
				messenger.printDebugMessage("record is MagnetRecords::SETI");
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			if (record == MagnetRecords::GETSETI)
			{
				//std::cout << "record is MagnetRecords::GETSETI" << std::endl;
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
		}
	}
	// loop over each node (map) in the YAML.NODE
	messenger.printMessage("loop over Magnet snapshot data");
	messenger.printMessage("yamlNodeToHardwareSnapshotMap COMPLETE");
	return return_map;
}
std::map<std::string, HardwareSnapshot> MagnetFactory::pyDictToHardwareSnapshotMap(const boost::python::dict& input_dict)
{
	// This function returns a map of <OBJECT NAME: HardwareSnapshot > 
	// it is created from a python dictionary that ius assuemd to contain a 3 deep nested dict stcuture
	// IT COULD LOOK Something like :
//{'MAGNET': 
//{'CLA-C2V-MAG-DIP-01': 
//	{'INT_STR': 0.0, 'INT_STR_MM': 0.0, 'K_ANG': 0.0, 'K_DIP_P': 0.0, 'K_MRAD': 0.0, 'K_SET_P': 0.0, 'READI': 5.67, 
//		'RILK': _HardwareFactory.STATE.OK, 'RPOWER': _HardwareFactory.STATE.ON}, 
//	'CLA-C2V-MAG-DIP-02': 
//	{'INT_STR': 0.0, 'INT_STR_MM': 0.0, 'K_ANG': 0.0, 'K_DIP_P': 0.0, 'K_MRAD': 0.0, 'K_SET_P': 0.0, 'READI': 2.09, 
//		'RILK': _HardwareFactory.STATE.OK, 'RPOWER': _HardwareFactory.STATE.ON},
// 'CLA-C2V-MAG-HCOR-01': 
//	{...
//	}
//	...
	std::map<std::string, HardwareSnapshot> return_map; 
	messenger.printMessage("pyDictToHardwareSnapshotMap");
	messenger.printMessage("loop over input_node ");

	// TO SOLVE THIS PROPBLEM WE HAVE HAD TO FIURE OUT HOW TO ITERATE OVER NESTED boost::python::dict, 
	// HOW TO ITERATE OVER A BOOST::PYTHON::DICT ???? (after 7 year sof trying ... :( )
	// https://stackoverflow.com/questions/58096040/how-iterate-key-value-boostpythondict
	// 
	auto input_dict_items = input_dict.items();
	for (ssize_t i = 0; i < len(input_dict_items); ++i)
	{
		//messenger.printMessage("loop over input_node ");
		boost::python::object key = input_dict_items[i][0];  // SHOULD BE ''MAGNET'
		boost::python::dict magnet_objects = boost::python::extract<boost::python::dict>(input_dict_items[i][1]); // SHOULD BE dict of {magnames: dict of mag snapshot data }
		std::string key_str = boost::python::extract<std::string>(key);
		//messenger.printMessage(key_str, " == MAGNET   ... ??? ");
		auto magnet_objects_list = magnet_objects.items();
		for (ssize_t i = 0; i < len(magnet_objects_list); ++i)
		{
			boost::python::object magnet_name = magnet_objects_list[i][0];  // SHOULD BE ''MAGNET'
			boost::python::dict magnet_snapshotdata = boost::python::extract<boost::python::dict>(magnet_objects_list[i][1]); // SHOULD BE dict of {magnames: dict of mag snapshot data }
			auto magnet_snapshotdata_items_list= magnet_snapshotdata.items();

			std::string object_name = boost::python::extract<std::string>(magnet_name);
			//messenger.printMessage(object_name, " == object_name   ... ??? ");

			for (ssize_t i = 0; i < len(magnet_snapshotdata_items_list); ++i)
			{
				std::string record = boost::python::extract<std::string>(magnet_snapshotdata_items_list[i][0]);   // SHOULD BE ''MAGNET'
				boost::python::object record_value = magnet_snapshotdata_items_list[i][1]; // SHOULD BE dict of {magnames: dict of mag snapshot data }
				//messenger.printMessage("Next record is ", record);
				if (record == MagnetRecords::SPOWER)
				{
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == MagnetRecords::RILK)
				{
					//messenger.printMessage("Next record is ", record);
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == MagnetRecords::K_MRAD)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == MagnetRecords::K_ANG)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == MagnetRecords::K_SET_P)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == MagnetRecords::K_DIP_P)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == MagnetRecords::INT_STR)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == MagnetRecords::INT_STR_MM)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == MagnetRecords::READI)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == MagnetRecords::RPOWER)
				{	//std::cout << "record is MagnetRecords::RPOWER" << std::endl;
					STATE new_val = boost::python::extract<STATE>(record_value);
					//STATE new_val = GlobalFunctions::stringToState(value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				if (record == MagnetRecords::SETI)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				if (record == MagnetRecords::GETSETI)
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
			}

		}

	}

	//// loop over each node (map) in the YAML.NODE
	//messenger.printMessage("loop over Magnet snapshot data");
	//std::cout << "yamlNodeToHardwareSnapshotMap COMPLETE" << std::endl;
	return return_map;
}
YAML::Node MagnetFactory::hardwareSnapshotMapToYAMLNode(const std::map<std::string, HardwareSnapshot>& hardwaresnapshot_map)
{
	YAML::Node return_node;
	for (auto& item : hardwaresnapshot_map)
	{
		return_node[ ENUM_TO_STRING(TYPE::MAGNET) ][item.first] =  item.second.getYAMLNode();
	}
	return return_node;
}


STATE MagnetFactory::checkLastAppliedSnapshot( TYPE mag_type_to_check)
{
	STATE return_state = STATE::SUCCESS;
	std::map<std::string, HardwareSnapshot> current_snapshot = getSnapshot();

	std::vector<std::string> wrong_seti;
	std::vector<std::string> wrong_psu;
	std::vector<std::string> wrong_name;

	set_wrong_seti.clear();
	set_wrong_psu.clear();
	set_wrong_name.clear();

	for (auto&& item : hardwareSnapshotMap)
	{
		std::string fullName = getFullName(item.first);
		
		// only apply to magnets that match magnet_type
		bool correct_magnet_type = false;
		switch (mag_type_to_check)
		{
		case TYPE::CORRECTOR:
			correct_magnet_type = isACor(fullName);
			break;
		case TYPE::VERTICAL_CORRECTOR:
			correct_magnet_type = isAVCor(fullName);
			break;
		case TYPE::HORIZONTAL_CORRECTOR: correct_magnet_type =
			isAHCor(fullName);
			break;
		case TYPE::QUADRUPOLE:
			correct_magnet_type = isAQuad(fullName);
			break;
		case TYPE::MAGNET:
			correct_magnet_type = true;
			break;
		default:
			correct_magnet_type = false;
		}
		
		if (correct_magnet_type)
		{

			if (GlobalFunctions::entryExists(current_snapshot, fullName))
			{
				auto& hss_ref = current_snapshot.at(fullName);

				for (auto&& state_items : item.second.state)
				{
					if (state_items.first == "GETSETI")
					{
						if (GlobalFunctions::entryExists(hss_ref.state, state_items.first))
						{
							double val_now = hss_ref.get<double>("GETSETI");
							double val_ref = item.second.get<double>("GETSETI");
							double tol = magnetMap.at(fullName).READI_tolerance;
							bool compare = GlobalFunctions::areSame(val_now, val_ref, tol);
							if (!compare)
							{
								return_state = STATE::FAIL;
								std::pair<double, double> new_seti_pair;
								new_seti_pair.first = val_ref;
								new_seti_pair.second = val_now;
								set_wrong_seti[std::string(fullName) ] = new_seti_pair;
								messenger.printDebugMessage(fullName, " has wrong GETSETI, (now, ref) ", val_now, " !+ ", val_ref, "  tol set_wrong_seti.size() = ", set_wrong_psu.size());
							}
						}
					}
					else if (state_items.first == "RPOWER")
					{
						if (GlobalFunctions::entryExists(hss_ref.state, state_items.first))
						{
							bool comp = hss_ref.get<STATE>("RPOWER") == item.second.get<STATE>("RPOWER");
							if (!comp)
							{
								return_state = STATE::FAIL;
								std::pair<STATE, STATE> new_psu_pair;
								new_psu_pair.first = hss_ref.get<STATE>("RPOWER");
								new_psu_pair.second = item.second.get<STATE>("RPOWER");
								set_wrong_psu[ std::string(fullName) ] = new_psu_pair;
								messenger.printDebugMessage(fullName, " has wrong RPOWER, set_wrong_psu.size() = ", set_wrong_psu.size());
							}
						}
					}
				}
			}
			else
			{
				return_state = STATE::FAIL;
				set_wrong_name.push_back(item.first);
				messenger.printDebugMessage(item.first, " has wrong NAME ");
			}
		}
	}
	return return_state;
}

std::map<std::string, std::pair<double, double>> MagnetFactory::getSetWrongSETI()const
{
	return set_wrong_seti;
}
boost::python::dict  MagnetFactory::getSetWrongSETI_Py()const
{
	return to_py_dict_pair<std::string, double>(set_wrong_seti);
}
std::map<std::string, std::pair<STATE, STATE>> MagnetFactory::getSetWrongPSU()
{
	return set_wrong_psu;
}
boost::python::dict  MagnetFactory::getSetWrongPSU_Py()const
{
	return to_py_dict_pair<std::string, STATE>(set_wrong_psu);
}
std::vector<std::string> MagnetFactory::getWrongName()const
{
	return set_wrong_name;
}
boost::python::list  MagnetFactory::getWrongName_Py()const
{
	return to_py_list<std::string>(set_wrong_name);
}

boost::python::dict MagnetFactory::checkLastAppliedSnapshotReturnResults_Py( TYPE mag_type_to_check )
{			
	messenger.printDebugMessage("checkLastAppliedSnapshotReturnResults_Py adding ");
	boost::python::dict r;
	if (checkLastAppliedSnapshot(mag_type_to_check) != STATE::SUCCESS)
	{
		boost::python::dict seti_err = getSetWrongSETI_Py();
		if (len(seti_err) > 0)
		{
			messenger.printDebugMessage("checkLastAppliedSnapshotReturnResults_Py adding ", len(seti_err)," seti errors ");
			r[ std::string("SETI_ERROR") ] = seti_err;
		}
		else
		{
			messenger.printDebugMessage("checkLastAppliedSnapshotReturnResults_Py no seti errors ");
		}
		boost::python::dict psu_err = getSetWrongPSU_Py();
		if (len(psu_err) > 0)
		{
			messenger.printDebugMessage("checkLastAppliedSnapshotReturnResults_Py adding ", len(psu_err), " psu errors ");
			r[std::string("PSU_ERROR") ] = psu_err;
		}
		else
		{
			messenger.printDebugMessage("checkLastAppliedSnapshotReturnResults_Py no psu_err errors ");
		}
		boost::python::list name_err = getWrongName_Py();
		if (len(name_err) > 0)
		{
			messenger.printDebugMessage("checkLastAppliedSnapshotReturnResults_Py adding ", len(name_err), " name errors ");
			r[std::string("NAME_ERROR")  ] = psu_err;
		}
		else
		{
			messenger.printDebugMessage("checkLastAppliedSnapshotReturnResults_Py no name_err errors ");
		}

	}
	return r;
}
std::string MagnetFactory::getDefaultMagnetSnapshotPath()const
{
	return SnapshotFileManager::defaultMachineSnapshotLocation;
}
std::string MagnetFactory::getDefaultMachineSnapshotPath()const
{
	return SnapshotFileManager::magnet_snapshot_default_path;
}


//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------


void MagnetFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("MAG-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& magnet : magnetMap)
	{
		magnet.second.debugMessagesOn();
	}
}
void MagnetFactory::debugMessagesOff()
{
	messenger.printDebugMessage("MAG-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& magnet : magnetMap)
	{
		magnet.second.debugMessagesOff();
	}
}
void MagnetFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("MAG-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& magnet : magnetMap)
	{
		magnet.second.messagesOn();
	}
}
void MagnetFactory::messagesOff()
{
	messenger.printMessage("MAG-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& magnet : magnetMap)
	{
		magnet.second.messagesOff();
	}
}
bool MagnetFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool MagnetFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
