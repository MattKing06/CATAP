#include "MagnetFactory.h"
#include <map>
#include <iostream>
#include <fstream>
//#include <filesystem> ?? not using  c++ 17 ???
#ifndef __CINT__
#include <cadef.h>
#endif


#include "GlobalFunctions.h"
#include "GlobalConstants.h"
#include <PythonTypeConversions.h>


#include "yaml-cpp/emitter.h"


MagnetFactory::MagnetFactory() :
	MagnetFactory(STATE::OFFLINE)
{
	std::cout << "MagnetFactory DEFAULT constRUCTOR called " << std::endl;
}

MagnetFactory::MagnetFactory(STATE mode) :
	messenger(LoggingSystem(true, true)),
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Magnet", mode))
	//:dummy_magnet(Magnet("DUMMY_MAGNET"))
{
	//hasBeenSetup = false;
	messenger.printDebugMessage("Magnet Factory constructed");
	//mode = mode;
	//reader = ;
}
MagnetFactory::MagnetFactory(const MagnetFactory& copyMagnetFactory)
	: hasBeenSetup(copyMagnetFactory.hasBeenSetup),
	mode(copyMagnetFactory.mode),
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
	messenger.printDebugMessage("MagnetFactory has finished populating the magnet map, found ", magnetMap.size()," magnets objects" );
}

void MagnetFactory::setMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == MagnetRecords::GETSETI ||
		pvStruct.pvRecord == MagnetRecords::RPOWER ||
		pvStruct.pvRecord == MagnetRecords::READI ||
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
			magnet.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

bool MagnetFactory::setup(const std::string& version)
{
	messenger.printDebugMessage("called Magnet Factory  setup ");
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
		updateAliasNameMap(magnet.second);
		//std::cout << "magnet.first = " << magnet.first << std::endl;
		/*
			NOW CHANNELS HAVE BEEN SENT TO EPICS, SET UP EVERYTHING ELSE
		*/
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
				if(pv.second.monitor)
				{
					magnet.second.epicsInterface->createSubscription(magnet.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(magnet.first, " CANNOT CONNECT TO EPICS");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
		}
		EPICSInterface::sendToEPICS();
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




// GET MAGNET OBJECT  FUNCTIONS
//
Magnet& MagnetFactory::getMagnet(const std::string& fullMagnetName)
{
	std::string full_name = getFullName(fullMagnetName);
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
	return to_py_list(getAllMagnetNames());
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
	if(GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getManufacturer();
	}
	return GlobalConstants::DUMMY_NAME;
}
std::map<std::string, std::string> MagnetFactory::getManufacturer(const std::vector<std::string>& names) const
{
	std::map<std::string, std::string> return_map;
	for(auto& name : names)
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

std::string MagnetFactory::getMagnetType(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getMagnetType();
	}
	return GlobalConstants::DUMMY_NAME;
}
std::map<std::string, std::string> MagnetFactory::getMagnetType(const std::vector<std::string>& names) const
{
	std::map<std::string, std::string> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getMagnetType(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::getMagnetType_Py(const boost::python::list& names) const
{
	return to_py_dict<std::string, std::string>(getMagnetType(to_std_vector<std::string>(names)));
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
boost::python::dict MagnetFactory::getDegaussValuesMulti_Py(const boost::python::list & names) const
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
	for(auto&& name : names)
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
		return magnetMap.at(fullName).getILKState();
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
		return_map[magnet.first] = magnet.second.getILKState();
	}
	return return_map;
}
boost::python::dict MagnetFactory::getAllILKState_Py() const
{
	return to_py_dict<std::string, STATE>(getAllILKState());
}


STATE MagnetFactory::SETI(const std::string& name, const double& value)
{
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		magnetMap.at(name).SETI(value);
		return STATE::SUCCESS;
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
boost::python::dict MagnetFactory::SETI_Py(boost::python::dict& namesAndCurrentsDict)
{
	return to_py_dict<std::string, STATE>(SETI(to_std_map<std::string,double>(namesAndCurrentsDict)));
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


STATE MagnetFactory::switchOn(const std::string& name)
{
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		magnetMap.at(name).switchOn();
		return STATE::SUCCESS;
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
		magnetMap.at(name).switchOff();
		return STATE::SUCCESS;
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
	return to_py_dict<std::string,STATE>(switchOff(to_std_vector<std::string>(names) ));
}


//--------------------------------------------------------------------------------------------------
bool MagnetFactory::writeDBURT(const std::string& fileName)const
{
	return writeDBURT(fileName, "NO COMMENT");
}
bool MagnetFactory::writeDBURT(const std::string& fileName, const std::string& commment)const
{
	return writeDBURT(GlobalConstants::DBURT_FILEPATH, fileName, commment);
}
bool MagnetFactory::writeDBURT(const std::string& filePath, const std::string& fileName, const std::string& commment)const
{
	dburt db;
	db.comment = commment;
	db.timestamp = GlobalFunctions::getTimeAndDateString();
	db.magnetstates = getMagnetStates();

	boost::filesystem::path directory(filePath);
	boost::filesystem::path file(fileName);
	boost::filesystem::path full_path = directory / file;
	return writeDBURTToFile(full_path, db);
}
//--------------------------------------------------------------------------------------------------
bool MagnetFactory::writeDBURTToFile(const boost::filesystem::path& full_path, const dburt& dburt_to_write) const
{
	std::ofstream fout(full_path.c_str());
	if (!fout)
	{
		messenger.printDebugMessage("writeDBURT failed to open file at ", full_path);
		return false;
	}
	// temp objects for header to write to file 
	std::map<std::string, std::string> header_data;
	std::map<std::string, std::map<std::string, std::string> > header_data_to_write;

	// WRITE HEADER FIRST
	header_data[GlobalConstants::COMMENT] = dburt_to_write.comment;
	header_data[GlobalConstants::TIMESTAMP] = dburt_to_write.timestamp;
	header_data_to_write[GlobalConstants::HEADER] = header_data;
	YAML::Node header_node(header_data_to_write);
	// write a header; TODO: maybe this version comment should be defeind in a better place
	fout << "#YAML VELA/CLARA Magnet Settings Save File VERSION 1" << std::endl;
	fout << header_node;
	fout << std::endl;

	// empty magnet data to write 
	std::map<std::string, std::string> magnet_data;
	std::map<std::string, std::map<std::string, std::string> > magnet_data_to_write;

	// dburt_to_write.magnetstates.magnetStatesMap
		
	//std::map<std::string, magnetState>::iterator magnetStatesMap_it = dburt_to_write.magnetstates.magnetStatesMap.begin();

	for(auto&& mag_state : dburt_to_write.magnetstates.magnetStatesMap)
	{
		magnet_data.clear();
		magnet_data[MagnetRecords::SETI] = std::to_string(mag_state.second.seti);
		magnet_data[MagnetRecords::READI] = std::to_string(mag_state.second.readi);
		magnet_data[MagnetRecords::RPOWER] = ENUM_TO_STRING(mag_state.second.psuState);
		magnet_data_to_write[mag_state.first] = magnet_data;
	} 
	YAML::Node data_node(magnet_data_to_write);
	fout << data_node;
	messenger.printDebugMessage("writeDBURT generated DBURT at ", full_path);
	return true;
}
//--------------------------------------------------------------------------------------------------
dburt MagnetFactory::readDBURT(const std::string& fileName)const
{
	return readDBURT(GlobalConstants::DBURT_FILEPATH, fileName);
}
dburt MagnetFactory::readDBURT(const std::string& filePath, const std::string& fileName)const
{
	boost::filesystem::path directory(filePath);
	boost::filesystem::path file(fileName);
	boost::filesystem::path full_path = directory / file;
	return readDBURTFile(full_path);
}
dburt MagnetFactory::readDBURTFile(const boost::filesystem::path& full_path) const
{
	std::pair<bool, std::string> r = isDBURTFileAlias(full_path);
	if(r.first)
	{
		messenger.printDebugMessage(full_path, " is a dburt alias");
		boost::filesystem::path alias_path(r.second);
		//TODO potential infinte loop ??? 
		return readDBURTFile(alias_path);
	}
	else
	{
		//std::cout << full_path << " is NOT dburt alias" << std::endl;
	}

	
	dburt read_dburt;
	std::ifstream fileInput;
	fileInput = std::ifstream(full_path.string());
	YAML::Parser parser(fileInput);
	YAML::Node file_data = YAML::LoadFile(full_path.string());

	//int i = 0;
	for (auto&& it : file_data)
	{
		// If the first string is "HEADER" get header info
		if (it.first.as<std::string>() == GlobalConstants::HEADER)
		{
			for (auto&& it2 : it.second)
			{
				std::string record = it2.first.as<std::string>();
				if (record == GlobalConstants::TIMESTAMP)
				{
					read_dburt.timestamp = it2.second.as<std::string>();
				}
				else if (record == GlobalConstants::COMMENT)
				{
					read_dburt.comment = it2.second.as<std::string>();
				}
			}
		}
		else // If the string **must** be a magnet name
		{
			// The First 
			magnetState mag_state_to_fill = magnetState();
			//read_dburt.magnetstates.magNames.push_back(it.first.as<std::string>());
			mag_state_to_fill.name = it.first.as<std::string>();
			for (auto&& it2 : it.second)
			{
				std::string record = it2.first.as<std::string>();
				//std::cout << record << " = " << it2.second << std::endl;
				if (MagnetRecords::SETI == record)
				{
					//std::cout << record << " is " << MagnetRecords::SETI << std::endl;
					mag_state_to_fill.seti = it2.second.as<double>();
				}
				else if (MagnetRecords::READI == record)
				{
					//std::cout << record << " is " << MagnetRecords::READI << std::endl;
					mag_state_to_fill.readi= it2.second.as<double>();
				}
				else if (MagnetRecords::RPOWER == record)
				{
					std::string val = it2.second.as<std::string>();
					if (val == "ON")
					{
						//std::cout << record << " is " << MagnetRecords::RPOWER << std::endl;
						mag_state_to_fill.psuState = STATE::ON;
					}
					else
					{
						mag_state_to_fill.psuState = STATE::OFF;
					}
				}
			}
			// update the magnetStatesMap with the magnetState for this magnet 
			read_dburt.magnetstates.magnetStatesMap[mag_state_to_fill.name] = mag_state_to_fill;
			std::cout << "added " << mag_state_to_fill.name << " to magnetStatesMap" << std::endl;
			std::cout << ENUM_TO_STRING(mag_state_to_fill.psuState) << " " << mag_state_to_fill.seti << " " << mag_state_to_fill.readi << std::endl;
		}
	}
	/*read_dburt.magnetstates.numMags = read_dburt.magnetstates.magNames.size();
	read_dburt.magnetstates.magNames_Py = to_py_list<std::string>(read_dburt.magnetstates.magNames);
	read_dburt.magnetstates.psuStates_Py = to_py_list<STATE>(read_dburt.magnetstates.psuStates);
	read_dburt.magnetstates.readiValues_Py = to_py_list<double>(read_dburt.magnetstates.readiValues);
	read_dburt.magnetstates.setiValues_Py = to_py_list<double>(read_dburt.magnetstates.setiValues);*/
	return read_dburt;
}

std::pair<bool, std::string> MagnetFactory::isDBURTFileAlias(const boost::filesystem::path& full_path)const
{
	std::ifstream fileInput;
	fileInput = std::ifstream(full_path.string());
	YAML::Parser parser(fileInput);
	YAML::Node file_data = YAML::LoadFile(full_path.string());

	std::pair<bool, std::string> r(false, "");

	for (auto&& it : file_data)
	{
		if (it.first.as<std::string>() == GlobalConstants::FILE_ALIAS)
		{
			r.first = true;
			r.second = it.second.as<std::string>();
		}
	}
	return r;
}



magnetState MagnetFactory::getMagnetState(const std::string& name)const
{
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		return magnetMap.at(name).getMagnetState();
	}
	return magnetState();
}
magnetStates MagnetFactory::getMagnetStates()const
{
	magnetStates ms;
	for (auto&& magnet : getAllMagnetNames())
	{
		ms.magnetStatesMap[magnet] = getMagnetState(magnet);
	}
	ms.numMags = ms.magnetStatesMap.size();
	return ms;
}
bool MagnetFactory::setMagnetState(const magnetState& ms)
{
	if (GlobalFunctions::entryExists(magnetMap, ms.name))
	{
		return magnetMap.at(ms.name).setMagnetState(ms);
	}
	return false;
}

std::map<std::string, bool> MagnetFactory::applyMagnetStates(const magnetStates & ms)
{
	std::cout << "applyMagnetStates " << std::endl;
	std::map<std::string, bool> r;
	for (auto&& mag_state : ms.magnetStatesMap)
	{
		std::cout << "APPLY magnet State for " << mag_state.second.name << std::endl;
		r[mag_state.first] = setMagnetState(mag_state.second);
	}
	return r;
}

bool MagnetFactory::readAndApplyDBURT(const std::string& fileName)
{
	std::cout << "readAndApplyDBURT " << std::endl;
	std::map<std::string, std::map<std::string, STATE>> r;
	auto db = readDBURT(fileName);
	auto apply_result = applyMagnetStates(db.magnetstates);
	return true;
}


bool MagnetFactory::isMagnetStateEqualDBURT(const std::string& fileName)
{
	auto db = readDBURT(fileName);
	auto ms = getMagnetStates();

	//auto& db_ms = db.magnetstates;
	//auto& ms_ms = ms;

	//std::vector<std::string> magNames;
	//std::vector<STATE> psuStates;
	//std::vector<double> setiValues, readiValues;

	//if (db_ms.numMags == ms_ms.numMags)
	//{

	//}
	//else
	//{ 
	//	
	//}

	//for(auto i = 0; i< )
	
	return false;
}





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
