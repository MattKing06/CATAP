#include "MagnetFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif

#include "GlobalFunctions.h"
#include "GlobalConstants.h"


MagnetFactory::MagnetFactory() : 
	MagnetFactory(STATE::OFFLINE)
{
	std::cout << "MagnetFactory DEFAULT CONSTRUCTOR called " << std::endl;
}

MagnetFactory::MagnetFactory(STATE mode):
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Magnet", mode))
//:dummy_magnet(Magnet("DUMMY_MAGNET"))
{
	messenger = LoggingSystem(true, true);
	//hasBeenSetup = false;
	messenger.printDebugMessage("Magnet Factory Constructed");
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
					magnet.second.epicsInterface->removeSubscription(pvStruct.second);
					ca_flush_io();
				}
				magnet.second.epicsInterface->removeChannel(pvStruct.second);
				ca_pend_io(CA_PEND_IO_TIMEOUT);
			}
		}
	}
}

void MagnetFactory::populateMagnetMap()
{
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		
		messenger.printDebugMessage("Magnet Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(magnetMap);
	}
	std::cout << "populateMagnetMap() fin" << std::endl;
}

void MagnetFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == "GETSETI" ||
		pvStruct.pvRecord == "RPOWER" ||
		pvStruct.pvRecord == "READI" ||
		pvStruct.pvRecord == "RILK")
	{
		pvStruct.monitor = true;
	}
}

bool MagnetFactory::setup(const std::string &version)
{
	messenger.printDebugMessage("called Magnet Factory  setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Magnet Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in Magnet constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	
	std::cout << "populateMagnetMap()" << std::endl;
	populateMagnetMap();
	std::cout << "populateMagnetMap() fin" << std::endl;;


	for (auto &magnet : magnetMap)
	{
		std::cout << "magnet.first = " <<  magnet.first << std::endl;
		std::map<std::string, pvStruct>& magPVStructs = magnet.second.getPVStructs();
		for (auto &pv : magPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;

			std::cout << "pvAndRecordName = " << pvAndRecordName << "." << std::endl;

			// sets the monitor stat in teh pvstruict to true or false 
			retrieveMonitorStatus(pv.second);
			
			magnet.second.epicsInterface->retrieveCHID(pv.second);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				std::cout << "cs_conn, getting some values " << std::endl;
				magnet.second.epicsInterface->retrieveCHTYPE(pv.second);
				magnet.second.epicsInterface->retrieveCOUNT(pv.second);
				magnet.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if(pv.second.monitor)
				{
					magnet.second.epicsInterface->createSubscription(magnet.second, pv.second);
				}
				//// update the alias_name_map so we can use fullNames or Aliases 
				updateAliasNameMap(magnet.second);
			}
			else
			{
				std::cout << magnet.first << " CANNOT CONNECT TO EPICS" << std::endl;
				messenger.printMessage(magnet.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}

		}
	}
	hasBeenSetup = true;
	std::cout << "hasBeenSetup = true " << std::endl;
	return hasBeenSetup;
}




// GET MAGNET OBJECT  FUNCTIONS
//
Magnet& MagnetFactory::getMagnet(const std::string& fullMagnetName)
{
	std::string full_name = getFullName(fullMagnetName);
	if (GlobalFunctions::entryExists(magnetMap, full_name) )
	{
		return magnetMap.at(full_name);
	}
	return dummy_magnet;
}


std::vector<std::string> MagnetFactory::getAllMagnetNames()const
{
	std::vector<std::string> return_names;
	for(auto&& item : magnetMap) 
	{
		return_names.push_back(item.first);
	}
	return return_names;
}
boost::python::list MagnetFactory::getAllMagnetNames_Py()const
{
	return to_py_list(getAllMagnetNames());
}


double MagnetFactory::getSETI(const std::string& name) const
{
	//if(!hasBeenSetup)
	//{
	//	//messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
	//	this->setup("nominal");// nominal does not mean anything yet 
	//}
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getSETI();
	}
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
	//if(!hasBeenSetup)
	//{
	//	//messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
	//	this->setup("nominal");// nominal does not mean anything yet 
	//}
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(magnetMap, fullName))
	{
		return magnetMap.at(fullName).getSETI();
	}
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
boost::python::dict MagnetFactory::getREADIs_Py(const boost::python::list& names) const
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





double MagnetFactory::getRICurrent(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
	}
	else
	{
		return magnetMap.find(name)->second.getREADI();
	}
	return std::numeric_limits<double>::min();
}

std::map<std::string, double> MagnetFactory::getRICurrents(const std::vector<std::string>& names)
{
	std::map<std::string, double> readis;
	for (auto& name : names)
	{
		double current = magnetMap.find(name)->second.getREADI();
		readis[name] = current;
	}
	return readis;
}


// This is being moved so that 
//boost::python::dict MagnetFactory::getCurrents_Py(boost::python::list magNames)
//{
//	std::map<std::string, double> currents;
//	std::vector<std::string> magNamesVector = to_std_vector<std::string>(magNames);
//	currents = getCurrents(magNamesVector);
//	boost::python::dict newPyDict = to_py_dict(currents);
//	return newPyDict;
//}

boost::python::dict MagnetFactory::getRICurrents_Py(boost::python::list names)
{
	std::map<std::string, double> RICurrents;
	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
	RICurrents = getRICurrents(namesVector);
	boost::python::dict newPyDict = to_py_dict(RICurrents);
	return newPyDict;
}

//boost::python::dict MagnetFactory::getAllMagnetCurrents_Py()
//{
//	std::map<std::string, double> allCurrents = getAllMagnetCurrents();
//	boost::python::dict allCurrentsPyDict = to_py_dict(allCurrents);
//	return allCurrentsPyDict;
//}
//std::map<std::string, double> MagnetFactory::getAllMagnetCurrents()
//{
//	std::map<std::string, double> magnetsAndCurrentsMap;
//	for (const auto &mag : magnetMap)
//	{
//		double current = mag.second.getCurrent();
//		magnetsAndCurrentsMap[mag.first] = current;
//	}
//	return magnetsAndCurrentsMap;
//}

void MagnetFactory::SETI(const std::string& name, const double& value)
{
	// TODO: check name exits in map
	magnetMap.at(name).SETI(value);
}
void MagnetFactory::SETI(const std::map<std::string, double> &namesAndCurrentsMap)
{
	for (const auto& entry : namesAndCurrentsMap)
	{
		SETI(entry.first, entry.second);
	}
	//return true;
}

STATE MagnetFactory::turnOn(const std::string& name)
{
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		magnetMap.at(name).setEPICSPSUState(STATE::ON);
		return STATE::SUCCESS;
	}
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::turnOn(const std::vector<std::string>& names)
{
	std::map<std::string, STATE> return_map;
	for (auto& name : names)
	{
		return_map[name] = turnOn(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::turnOn_Py(const boost::python::list names)
{
	return to_py_dict<std::string, STATE>(turnOn(to_std_vector<std::string>(names)));
}

STATE MagnetFactory::turnOff(const std::string& name)
{
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		magnetMap.at(name).setEPICSPSUState(STATE::OFF);
		return STATE::SUCCESS;
	}
	return STATE::UNKNOWN_NAME;
}
std::map<std::string, STATE> MagnetFactory::turnOff(const std::vector<std::string>& names)
{
	std::map<std::string, STATE> return_map;
	for (auto& name : names)
	{
		return_map[name] = turnOff(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::turnOff_Py(const boost::python::list names)
{
	return to_py_dict<std::string,STATE>( turnOff(to_std_vector<std::string>(names) ));
}


STATE MagnetFactory::getPSUState(const std::string& name) const
{
	return magnetMap.find(name)->second.getPSUState();
}

std::map<std::string, STATE> MagnetFactory::getAllPSUState() const
{
	std::map<std::string, STATE> return_map;
	for (auto&& magnet : magnetMap)
	{
		return_map[magnet.second.getHardwareName()] = magnet.second.getPSUState();
	}
	return return_map;
}

boost::python::dict MagnetFactory::getAllPSUState_Py() const
{
	return to_py_dict(getAllPSUState());
}

int MagnetFactory::getILKState(const std::string& name) const
{
	return magnetMap.find(name)->second.getILKState();
}
//bool MagnetFactory::turnOn_Py(boost::python::list names)
//{
//	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
//	return turnOn(namesVector);
//}
//bool MagnetFactory::turnOff_Py(boost::python::list names)
//{
//	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
//	return turnOff(namesVector);
//
//}

bool MagnetFactory::setCurrents_Py(boost::python::dict magnetNamesAndCurrents)
{
	std::map<std::string, double> magnetNamesAndCurrentsToSet;
	std::vector<std::string> magnetNames = to_std_vector<std::string>(magnetNamesAndCurrents.keys());
	std::vector<double> currentsToSet = to_std_vector<double>(magnetNamesAndCurrents.values());
	auto magnetNamesIterator = magnetNames.begin();
	auto currentsToSetIterator = currentsToSet.begin();
	while (magnetNamesIterator != magnetNames.end() && currentsToSetIterator != currentsToSet.end())
	{
		std::pair<std::string, double> entryToInsert = std::pair<std::string, double>(magnetNamesIterator->data(), *currentsToSetIterator);
		magnetNamesAndCurrentsToSet.emplace(entryToInsert);
		magnetNamesIterator++;
		currentsToSetIterator++;
	}
	SETI(magnetNamesAndCurrentsToSet);
	return true;
}



void MagnetFactory::updateAliasNameMap(const Magnet& magnet)
{
	// first add in the magnet full name
	std::string full_name = magnet.getHardwareName();
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		messenger.printMessage("!!ERROR!! ", full_name, " magnet name already exists! ");
	}
	else
	{
		alias_name_map[full_name] = full_name;
	}
	
	// now add in the aliases
	std::vector<std::string> aliases = magnet.getAliases();
	for(auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ",magnet.getHardwareName(), " alias = ",next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
		}
	}
}

std::string MagnetFactory::getFullName(const std::string& name_to_check) const
{
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		return alias_name_map.at(name_to_check);
	}
	return dummy_magnet.getHardwareName();
}
