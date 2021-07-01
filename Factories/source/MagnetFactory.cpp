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
	messenger(LoggingSystem(true, true)),
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Magnet", mode)),
	machineAreas(std::vector<TYPE>{TYPE::ALL_VELA_CLARA}),
	LEGACY_DBURT_IDENT(".dburt")
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
	if (pvStruct.pvRecord == MagnetRecords::GETSETI ||
		pvStruct.pvRecord == MagnetRecords::RPOWER ||
		pvStruct.pvRecord == MagnetRecords::READI ||
		pvStruct.pvRecord == MagnetRecords::K_DIP_P ||
		pvStruct.pvRecord == MagnetRecords::INT_STR_MM ||
		pvStruct.pvRecord == MagnetRecords::INT_STR ||
		pvStruct.pvRecord == MagnetRecords::K_SET_P ||
		pvStruct.pvRecord == MagnetRecords::K_ANG ||
		pvStruct.pvRecord == MagnetRecords::K_MRAD ||
		pvStruct.pvRecord == MagnetRecords::K_VAL ||
		pvStruct.pvRecord == MagnetRecords::RILK)
	{
		pvStruct.monitor = true;
	}
}



void MagnetFactory::setupChannels()
{
	for (auto& magnet : magnetMap)
	{
		std::map<std::string, pvStruct>& pvStructs = magnet.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			// thsi is connecting to a CHID
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
		//messenger.printMessage("Set up EPICS suscriptions for " + magnet.second.getHardwareName());
		/*
			NOW CHANNELS HAVE BEEN SENT TO EPICS, SET UP EVERYTHING ELSE
		*/
		updateAliasNameMap(magnet.second);
		std::map<std::string, pvStruct>& magPVStructs = magnet.second.getPVStructs();
		for (auto& pv : magPVStructs)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				//messenger.printMessage("Connected!, getting some channel data (COUNT, CHTYPE, ... )");
				setMonitorStatus(pv.second);
				magnet.second.epicsInterface->retrieveCHTYPE(pv.second);
				magnet.second.epicsInterface->retrieveCOUNT(pv.second);
				magnet.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				//messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
				//	"write", std::to_string(ca_write_access(pv.second.CHID)),
				//	"state", std::to_string(ca_state(pv.second.CHID)));
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
		int status = EPICSInterface::caFlushIO("ca_create_subscription");
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
	std::cout << "!!ERROR!! MagnetFactory::getSETI cannot find magnet with name = " << name << std::endl;
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
		if (isADip(item.first))
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
		if (isADip(item.first))
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
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		if (magnetMap.at(name).switchOff())
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


//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------





void MagnetFactory::readVCCDBURTFile(const boost::filesystem::path& full_path) const
{

}
void MagnetFactory::readCATAPSnapshotYAML(const boost::filesystem::path& full_path) const
{
	//dburt read_dburt;
	std::ifstream fileInput;
	fileInput = std::ifstream(full_path.string());
	YAML::Parser parser(fileInput);
	YAML::Node file_data = YAML::LoadFile(full_path.string());

	////int i = 0;
	//for (auto&& it : file_data)
	//{
	//	// If the first string is "HEADER" get header info
	//	if (it.first.as<std::string>() == GlobalConstants::HEADER)
	//	{
	//		for (auto&& it2 : it.second)
	//		{
	//			std::string record = it2.first.as<std::string>();
	//			if (record == GlobalConstants::TIMESTAMP)
	//			{
	//				read_dburt.timestamp = it2.second.as<std::string>();
	//			}
	//			else if (record == GlobalConstants::COMMENT)
	//			{
	//				read_dburt.comment = it2.second.as<std::string>();
	//			}
	//		}

	//		messenger.printDebugMessage("timestamp: ", read_dburt.timestamp);
	//		messenger.printDebugMessage("comment: ", read_dburt.comment);
	//	}
	//	else // If the string **must** be a magnet name
	//	{
	//		// The First 
	//		magnetState mag_state_to_fill = magnetState();
	//		//read_dburt.magnetstates.magNames.push_back(it.first.as<std::string>());
	//		mag_state_to_fill.name = it.first.as<std::string>();
	//		for (auto&& it2 : it.second)
	//		{
	//			std::string record = it2.first.as<std::string>();
	//			//std::cout << record << " = " << it2.second << std::endl;
	//			if (MagnetRecords::SETI == record)
	//			{
	//				//std::cout << record << " is " << MagnetRecords::SETI << std::endl;
	//				mag_state_to_fill.seti = it2.second.as<double>();
	//			}
	//			else if (MagnetRecords::READI == record)
	//			{
	//				//std::cout << record << " is " << MagnetRecords::READI << std::endl;
	//				mag_state_to_fill.readi = it2.second.as<double>();
	//			}
	//			else if (MagnetRecords::RPOWER == record)
	//			{
	//				std::string val = it2.second.as<std::string>();
	//				if (val == "ON")
	//				{
	//					//std::cout << record << " is " << MagnetRecords::RPOWER << std::endl;
	//					mag_state_to_fill.psu_state = STATE::ON;
	//				}
	//				else
	//				{
	//					mag_state_to_fill.psu_state = STATE::OFF;
	//				}
	//			}
	//		}
	//		// update the magnetStatesMap with the magnetState for this magnet 
	//		read_dburt.magnetstates.magnet_states_map[mag_state_to_fill.name] = mag_state_to_fill;
	//		std::cout << "added " << mag_state_to_fill.name << " to magnetStatesMap" << std::endl;
	//		std::cout << ENUM_TO_STRING(mag_state_to_fill.psu_state) << " " << mag_state_to_fill.seti << " " << mag_state_to_fill.readi << std::endl;
	//	}
	//}
	/*read_dburt.magnetstates.numMags = read_dburt.magnetstates.magNames.size();
	read_dburt.magnetstates.magNames_Py = to_py_list<std::string>(read_dburt.magnetstates.magNames);
	read_dburt.magnetstates.psuStates_Py = to_py_list<STATE>(read_dburt.magnetstates.psuStates);
	read_dburt.magnetstates.readiValues_Py = to_py_list<double>(read_dburt.magnetstates.readiValues);
	read_dburt.magnetstates.setiValues_Py = to_py_list<double>(read_dburt.magnetstates.setiValues);*/
	//return read_dburt;
}
//std::pair<bool, std::string> MagnetFactory::isDBURTFileAlias(const std::string& full_path)const
//{
//	//messenger.printDebugMessage("isDBURTFileAlias checking file: ", full_path);
//	//std::ifstream fileInput;
//	//fileInput = std::ifstream(full_path);
//	//YAML::Parser parser(fileInput);
//	//YAML::Node file_data = YAML::LoadFile(full_path);
//
//	std::pair<bool, std::string> r(false, "");
//
//	//for (auto&& it : file_data)
//	//{
//	//	if (it.first.as<std::string>() == GlobalConstants::FILE_ALIAS)
//	//	{
//	//		r.first = true;
//	//		r.second = it.second.as<std::string>();
//	//		messenger.printDebugMessage("isDBURTFileAlias found FILE_ALIAS pointing to " + r.second);
//	//	}
//	//}
//	//messenger.printDebugMessage("isDBURTFileAlias returning ", r.first);
//	return r;
//}



std::map<std::string, HardwareSnapshot> MagnetFactory::getSnapshot()
{
	std::map<std::string, HardwareSnapshot> allStates;
	for (auto& item : magnetMap)
	{
		allStates[item.first] = item.second.getSnapshot();
	}
	return allStates;
}
boost::python::dict MagnetFactory::getSnapshot_Py()
{
	std::map<std::string, boost::python::dict> snapshot_dict;
	for (auto& item : magnetMap)
	{
		snapshot_dict[item.first] = item.second.getSnapshot_Py();
	}
	return to_py_dict<std::string, boost::python::dict>(snapshot_dict);
}
bool MagnetFactory::applySnapshot_Py(boost::python::dict& snap)
{
	return false;
}


STATE MagnetFactory::applyYAMLNode(const YAML::Node& input_node)
{
	STATE r = STATE::UNKNOWN_STATE;
	messenger.printMessage("loop over input_node ");
	for (auto& it : input_node["MAGNET"])
	{
		std::string object_name = getFullName(it.first.as<std::string>());
		std::cout << "(objectname) key = " << object_name << std::endl;
		std::map<std::string, std::string >  value = it.second.as<std::map<std::string, std::string >>();
		//return_map[object_name] = HardwareSnapshot();
		for (auto&& map_it : value)
		{
			std::string record = map_it.first;
			//std::cout << "(pv record, value) key / value = " << map_it.first << "/" << map_it.second << std::endl;
			//std::cout  << "record = " << record  << std::endl;
			if (record == MagnetRecords::K_VAL) {}
			else if (record == MagnetRecords::SPOWER) {}
			else if (record == MagnetRecords::RILK) {}
			else if (record == MagnetRecords::K_MRAD) {}
			else if (record == MagnetRecords::K_ANG) {}
			else if (record == MagnetRecords::K_SET_P) {}
			else if (record == MagnetRecords::K_DIP_P) {}
			else if (record == MagnetRecords::INT_STR) {}
			else if (record == MagnetRecords::INT_STR_MM) {}
			else if (record == MagnetRecords::READI) {}
			else if (record == MagnetRecords::RPOWER)
			{	//std::cout << "record is MagnetRecords::RPOWER" << std::endl;
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				//std::cout << "new_val is " << new_val << ", " <<  ENUM_TO_STRING(new_val) << std::endl;
				if (new_val == STATE::ON)
				{
					messenger.printMessage("loop over Magnet snapshot data");
					switchOn(object_name);
				}
				else
				{
					switchOff(object_name);
				}
			}
			if (record == MagnetRecords::SETI)
			{
				std::cout << "record is MagnetRecords::SETI" << std::endl;
				double new_val = std::stod(map_it.second);
				//std::cout << "ADDED " << record << " = " << return_map[object_name].get<double>(record) << std::endl;
				SETI(object_name, new_val);
			}
		}
	}	// loop over each node (map) in the YAML.NODE
	messenger.printMessage("applyYAMLNode COMPLETE");
	return r;
}
STATE MagnetFactory::loadSnapshot(const std::string location, const std::string& snapFile)
{
	STATE r_state = STATE::FAIL;
	/* combine the path and file name */
	const boost::filesystem::path directory(location);
	const boost::filesystem::path file(snapFile);
	const boost::filesystem::path full_path = directory / file;
	messenger.printMessage("loadSnapshot location - snapfile:", location," - ", snapFile);
	/* Next parse file data into  YAML node, For magnets (ONLY!) we provide backwards compatability with VCC */
	last_snapshot_yaml_node_read.reset();
	if (SnapshotFileManager::containsDBURTExtension(snapFile) )
	{
		messenger.printMessage("This file is a legacy DBURT file, converting to YAML::Node");
		last_snapshot_yaml_node_read = SnapshotFileManager::DBURT2YAMLNode(full_path);
	}
	else
	{
		messenger.printMessage("This file is a CATAP snapshot file");
		std::ifstream inFile(full_path.c_str());
		if (!inFile) { return r_state; }
		last_snapshot_yaml_node_read = YAML::LoadFile(full_path.string());
	}
	// APPLY TO THE MACHINE 
	if (last_snapshot_yaml_node_read[ENUM_TO_STRING(TYPE::MAGNET)]) {
		messenger.printMessage("YAML::Node has magnet Data, passing to applyYAMLNode");
		r_state = applyYAMLNode(last_snapshot_yaml_node_read);
	}
	//yamlNodeToHardwareSnapshotMap(snapshot_data["MAGNET"]); // todo MAGIC_STRING
	return r_state;
}

STATE MagnetFactory::saveSnapshot(const std::string& location, const std::string& filename)
{
	STATE r_state = STATE::FAIL;

	std::cout << "MagnetFactory::saveSnapshot" << std::endl;

	messenger.printMessage("MagnetFactory::saveSnapshot");

	const boost::filesystem::path directory(location);
	const boost::filesystem::path file(filename);
	const boost::filesystem::path full_path = directory / file;
	//const std::string fullpath = location + "/" + filename;
	std::ofstream outFile(full_path.c_str());
	messenger.printMessage("Full path = ", full_path);
	if (!outFile)
	{
		messenger.printMessage("Error creating output file stream");
		return	r_state;
	}
	YAML::Node outputNode;
	for (auto& item : magnetMap)
	{
		HardwareSnapshot currentState = item.second.getSnapshot();

		for (auto& stateItem : currentState.state)
		{
			if (stateItem.first == MagnetRecords::K_VAL)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::K_MRAD)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::K_ANG)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::K_SET_P)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::K_DIP_P)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::INT_STR)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::INT_STR_MM)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::READI)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = currentState.get<double>(stateItem.first);
			}
			else if (stateItem.first == MagnetRecords::RPOWER)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = ENUM_TO_STRING(currentState.get<STATE>(stateItem.first));
			}
			else if (stateItem.first == MagnetRecords::RILK)
			{
				outputNode[ENUM_TO_STRING(item.second.hardware_type)][item.first][stateItem.first] = ENUM_TO_STRING(currentState.get<STATE>(stateItem.first));
			}
		}
		//messenger.printMessage(item.first, " DATA ");
		//for (auto&& thing : outputNode[item.first])
		//{
		//	messenger.printMessage(thing.first, " = ", thing.second);
		//}
	}
	messenger.printMessage("Writign File");
	SnapshotFileManager::writeSnapshotToYAML(location, filename, outputNode, mode);
	return r_state;
}


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
	//std::cout << "snapshot_data from yamlNodeToHardwareSnapshotMap " << std::endl;
	//std::cout << input_node << std::endl;
	//std::cout << std::endl;
	std::map<std::string, HardwareSnapshot> return_map;
	messenger.printMessage("loop over input_node ");
	for (auto& it : input_node["MAGNET"])
	{
		std::string object_name = getFullName(it.first.as<std::string>());
		std::cout << "(objectname) key = " << object_name << std::endl;
		std::map<std::string, std::string >  value = it.second.as<std::map<std::string, std::string >>();
		//return_map[object_name] = HardwareSnapshot();
		for (auto&& map_it : value)
		{
			std::string record = map_it.first;
			//std::cout << "(pv record, value) key / value = " << map_it.first << "/" << map_it.second << std::endl;
			//std::cout  << "record = " << record  << std::endl;
			if (record == MagnetRecords::K_VAL) {}
			else if (record == MagnetRecords::SPOWER) {}
			else if (record == MagnetRecords::RILK) {}
			else if (record == MagnetRecords::K_MRAD) {}
			else if (record == MagnetRecords::K_ANG) {}
			else if (record == MagnetRecords::K_SET_P) {}
			else if (record == MagnetRecords::K_DIP_P) {}
			else if (record == MagnetRecords::INT_STR) {}
			else if (record == MagnetRecords::INT_STR_MM) {}
			else if (record == MagnetRecords::READI) {}
			else if (record == MagnetRecords::RPOWER)
			{	//std::cout << "record is MagnetRecords::RPOWER" << std::endl;
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				//std::cout << "new_val is " << new_val << ", " <<  ENUM_TO_STRING(new_val) << std::endl;
				if (new_val == STATE::ON)
				{
					return_map[object_name].update(record, new_val);
					messenger.printMessage("loop over Magnet snapshot data");
					switchOn(object_name);
				}
				else
				{
					return_map[object_name].update(record, STATE::OFF);
					switchOff(object_name);
				}
			}
			if (record == MagnetRecords::SETI)
			{
				std::cout << "record is MagnetRecords::SETI" << std::endl;
				double new_val = std::stod(map_it.second);
				return_map[object_name].update(record, new_val);
				//std::cout << "ADDED " << record << " = " << return_map[object_name].get<double>(record) << std::endl;
				SETI(object_name, new_val);
			}
			//			return_map[map_it.first].add(MagnetRecords::RPOWER, new_val);
		}
	}
	// loop over each node (map) in the YAML.NODE
	messenger.printMessage("loop over Magnet snapshot data");
	std::cout << "yamlNodeToHardwareSnapshotMap COMPLETE" << std::endl;
	return return_map;
}


//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------//--------------------------------------------------------------------------------------------------
// TODO ALL THE BELOW IS DEPRACTED AND COULD BE REMOVED... or some backwards compatabiliity????? 
//magnetState MagnetFactory::getMagnetState(const std::string& name)const
//{
//	if (GlobalFunctions::entryExists(magnetMap, name))
//	{
//		return magnetMap.at(name).getMagnetState();
//	}
//	return magnetState();
//}
//magnetStates MagnetFactory::getMagnetStates()const
//{
//	magnetStates ms;
//	for (auto&& magnet : getAllMagnetNames())
//	{
//		ms.magnet_states_map[magnet] = getMagnetState(magnet);
//	}
//	ms.magnet_count = ms.magnet_states_map.size();
//	return ms;
//}
//boost::python::dict MagnetFactory::getMagnetState_Py(const std::string& name)const
//{
//	magnetState r = getMagnetState(name);
//	return r.state_dict;
//}
///*! Return magnetState for all magnets in the factory,
//@param[out] python dictionary version of a magnetStates */
//boost::python::dict MagnetFactory::getAllMagnetState_Py() const
//{
//	magnetStates s = getMagnetStates();
//	boost::python::dict r;
//	// do we need this - i don't think so  ... 
//	//r["magnet_count"] = s.magnet_count;
//	for (auto&& item : s.magnet_states_map)
//	{
//		r[item.first] = item.second.state_dict;
//	}
//	return r;
//}
//bool MagnetFactory::setMagnetState(const magnetState& ms)
//{
//	if (GlobalFunctions::entryExists(magnetMap, ms.name))
//	{
//		return magnetMap.at(ms.name).setMagnetState(ms);
//	}
//	return false;
//}
//std::map<std::string, bool> MagnetFactory::applyMagnetStates(const magnetStates& ms, const std::vector<TYPE>& types)
//{
//	std::cout << "applyMagnetStates " << std::endl;
//	std::map<std::string, bool> r;
//	for (auto&& mag_state : ms.magnet_states_map)
//	{
//		if (GlobalFunctions::entryExists(types, getMagnetType(mag_state.second.name)))
//		{
//			std::cout << "APPLY magnet State for " << mag_state.second.name << std::endl;
//			r[mag_state.first] = setMagnetState(mag_state.second);
//		}
//	}
//	return r;
//}
//bool MagnetFactory::applyDBURT(const std::string& fileName)
//{
//	std::vector<TYPE> types {TYPE::QUADRUPOLE, TYPE::DIPOLE, TYPE::VERTICAL_CORRECTOR, TYPE::HORIZONTAL_CORRECTOR,
//		TYPE::SOLENOID, TYPE::BUCKING_SOLENOID};
//	std::cout << "applyDBURT " << std::endl;
//	std::map<std::string, std::map<std::string, STATE>> r;
//	auto db = readDBURT(fileName);
//	auto apply_result = applyMagnetStates(db.magnetstates, types);
//	return true;
//}
//bool MagnetFactory::applyDBURTQuadOnly(const std::string& fileName)
//{
//	std::vector<TYPE> types{ TYPE::QUADRUPOLE};
//	std::cout << "applyDBURTQuadOnly " << std::endl;
//	std::map<std::string, std::map<std::string, STATE>> r;
//	auto db = readDBURT(fileName);
//	auto apply_result = applyMagnetStates(db.magnetstates, types);
//	return true;
//}
//bool MagnetFactory::applyDBURTCorOnly(const std::string& fileName)
//{
//	std::vector<TYPE> types{ TYPE::VERTICAL_CORRECTOR, TYPE::HORIZONTAL_CORRECTOR};
//	std::cout << "applyDBURTCorOnly " << std::endl;
//	std::map<std::string, std::map<std::string, STATE>> r;
//	auto db = readDBURT(fileName);
//	auto apply_result = applyMagnetStates(db.magnetstates, types);
//	return true;
//}
//bool MagnetFactory::applyDBURT(const std::string& filePath, const std::string& fileName)
//{
//	std::vector<TYPE> types{ TYPE::QUADRUPOLE, TYPE::DIPOLE, TYPE::VERTICAL_CORRECTOR, TYPE::HORIZONTAL_CORRECTOR,
//		TYPE::SOLENOID, TYPE::BUCKING_SOLENOID };
//	std::cout << "applyDBURT fp fn" << std::endl;
//	std::map<std::string, std::map<std::string, STATE>> r;
//	auto db = readDBURT(filePath, fileName);
//	auto apply_result = applyMagnetStates(db.magnetstates, types);
//	return true;
//}
//bool MagnetFactory::applyDBURTQuadOnly(const std::string& filePath, const std::string& fileName)
//{
//	std::vector<TYPE> types{ TYPE::QUADRUPOLE };
//	std::cout << "applyDBURTQuadOnly " << std::endl;
//	std::map<std::string, std::map<std::string, STATE>> r;
//	auto db = readDBURT(filePath,fileName);
//	auto apply_result = applyMagnetStates(db.magnetstates, types);
//	return true;
//}
//bool MagnetFactory::applyDBURTCorOnly(const std::string& filePath, const std::string& fileName)
//{
//	std::vector<TYPE> types{ TYPE::VERTICAL_CORRECTOR, TYPE::HORIZONTAL_CORRECTOR };
//	std::cout << "applyDBURTCorOnly " << std::endl;
//	std::map<std::string, std::map<std::string, STATE>> r;
//	auto db = readDBURT(filePath, fileName);
//	auto apply_result = applyMagnetStates(db.magnetstates, types);
//	return true;
//}
//bool MagnetFactory::isMagnetStateEqualDBURT(const std::string& fileName)
//{
//	auto db = readDBURT(fileName);
//	auto ms = getMagnetStates();
//
//	//auto& db_ms = db.magnetstates;
//	//auto& ms_ms = ms;
//
//	//std::vector<std::string> magNames;
//	//std::vector<STATE> psuStates;
//	//std::vector<double> setiValues, readiValues;
//
//	//if (db_ms.numMags == ms_ms.numMags)
//	//{
//
//	//}
//	//else
//	//{ 
//	//	
//	//}
//
//	//for(auto i = 0; i< )
//
//	return false;
//}
//bool MagnetFactory::writeDBURT(const std::string& fileName)const
//{
//	return writeDBURT(fileName, "NO COMMENT");
//}
//bool MagnetFactory::writeDBURT(const std::string& fileName, const std::string& commment)const
//{
//	return writeDBURT(GlobalConstants::DBURT_FILEPATH, fileName, commment);
//}
//bool MagnetFactory::writeDBURT(const std::string& filePath, const std::string& fileName, const std::string& commment)const
//{
//	dburt db;
//	db.comment = commment;
//	db.timestamp = GlobalFunctions::getTimeAndDateString();
//	db.magnetstates = getMagnetStates();
//
//	boost::filesystem::path directory(filePath);
//	boost::filesystem::path file(fileName);
//	boost::filesystem::path full_path = directory / file;
//	return writeDBURTToFile(full_path, db);
//}
//bool MagnetFactory::writeDBURTToFile(const boost::filesystem::path& full_path, const dburt& dburt_to_write) const
//{
//	std::ofstream fout(full_path.c_str());
//	if (!fout)
//	{
//		messenger.printDebugMessage("writeDBURT failed to open file at ", full_path);
//		return false;
//	}
//	// temp objects for header to write to file 
//	std::map<std::string, std::string> header_data;
//	std::map<std::string, std::map<std::string, std::string> > header_data_to_write;
//
//	// WRITE HEADER FIRST
//	header_data[GlobalConstants::COMMENT] = dburt_to_write.comment;
//	header_data[GlobalConstants::TIMESTAMP] = dburt_to_write.timestamp;
//	header_data_to_write[GlobalConstants::HEADER] = header_data;
//	YAML::Node header_node(header_data_to_write);
//	// write a header; TODO: maybe this version comment should be defeind in a better place
//	fout << "#YAML VELA/CLARA Magnet Settings Save File VERSION 1" << std::endl;
//	fout << header_node;
//	fout << std::endl;
//
//	// empty magnet data to write 
//	std::map<std::string, std::string> magnet_data;
//	std::map<std::string, std::map<std::string, std::string> > magnet_data_to_write;
//
//	// dburt_to_write.magnetstates.magnetStatesMap
//
//	//std::map<std::string, magnetState>::iterator magnetStatesMap_it = dburt_to_write.magnetstates.magnetStatesMap.begin();
//
//	for (auto&& mag_state : dburt_to_write.magnetstates.magnet_states_map)
//	{
//		magnet_data.clear();
//
//		// FIX THE NUMEBR OF DECIMAL PLACES IN SETI TO 3 
//		std::stringstream stream;
//		stream << std::fixed << std::setprecision(3) << mag_state.second.seti;
//		std::string s = stream.str();
//
//		magnet_data[MagnetRecords::SETI] = stream.str();;
//		magnet_data[MagnetRecords::READI] = std::to_string(mag_state.second.readi);
//		magnet_data[MagnetRecords::RPOWER] = ENUM_TO_STRING(mag_state.second.psu_state);
//		magnet_data_to_write[mag_state.first] = magnet_data;
//	}
//	YAML::Node data_node(magnet_data_to_write);
//	fout << data_node;
//	messenger.printDebugMessage("writeDBURT generated DBURT at ", full_path);
//	return true;
//}
//dburt MagnetFactory::readDBURT(const std::string& fileName)const
//{
//	return readDBURT(GlobalConstants::DBURT_FILEPATH, fileName);
//}
//dburt MagnetFactory::readDBURT(const std::string& filePath, const std::string& fileName)const
//{
//	boost::filesystem::path directory(filePath);
//	boost::filesystem::path file(fileName);
//	boost::filesystem::path full_path = directory / file;
//	return readDBURTFile(full_path);
//}
//
//
//
//
//dburt MagnetFactory::readDBURTFile(const boost::filesystem::path& full_path) const
//{
//	messenger.printDebugMessage("readDBURTFile passed " + full_path.string());
//	// Check if BURT file  magnet data or alias
//	std::pair<bool, std::string> r = isDBURTFileAlias(full_path.string());
//	if (r.first)
//	{
//
//		messenger.printDebugMessage(full_path.string(), " is a dburt alias");
//		boost::filesystem::path alias_path(r.second);
//		//TODO potential infinte loop ??? 
//		return readDBURTFile(alias_path);
//	}
//	else
//	{
//		messenger.printDebugMessage(full_path.string(), " is a dburt data file");
//	}
//
//
//	// NOW WE CHeck if its a new file or a legacy 
//
//	std::ifstream fileInput;
//	fileInput = std::ifstream(full_path.string());
//
//	if (fileInput.is_open())
//	{
//		/// as a quick check to provide some backwards compatbility we will read the first line 		
//		std::string first_line;
//		getline(fileInput, first_line);
//		if (GlobalFunctions::stringIsSubString(LEGACY_DBURT_IDENT, first_line) )
//		{
//			bool is_legacy_file = true;
//			return readVCCDBURTFile(full_path);
//
//		}
//		else
//		{
//			bool is_legacy_file = false;
//			return readCATAPSnapshotYAML(full_path);
//
//		}
//		//std::cout << fileInput.rdbuf() << std::endl;
//
//	}
//	else
//	{
//		std::cout << "ERROR READING FILE  readDBURTFile" << std::endl;
//	}
//
//	return readVCCDBURTFile(full_path);
//
//}


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
