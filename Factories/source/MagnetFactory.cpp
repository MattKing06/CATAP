#include "MagnetFactory.h"
#include <boost/filesystem.hpp>
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
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Magnet", mode))
	//:dummy_magnet(Magnet("DUMMY_MAGNET"))
{
	messenger = LoggingSystem(true, true);
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
	messenger.printDebugMessage("MagnetFactory Copy cOntructor");
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
		throw std::runtime_error("Did not receive cOnfiguratiOn parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{

		messenger.printDebugMessage("Magnet Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(magnetMap);
	}
	messenger.printDebugMessage("MagnetFactory has finished populating the magnet map");
}

void MagnetFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == MagnetRecords::GETSETI ||
		pvStruct.pvRecord == MagnetRecords::RPOWER ||
		pvStruct.pvRecord == MagnetRecords::READI ||
		pvStruct.pvRecord == MagnetRecords::RILK)
	{
		pvStruct.monitor = true;
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
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in Magnet constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.

	//std::cout << "populateMagnetMap()" << std::endl;
	populateMagnetMap();
	//std::cout << "populateMagnetMap() fin" << std::endl;;
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}

	for (auto& magnet : magnetMap)
	{
		//std::cout << "magnet.first = " << magnet.first << std::endl;
		std::map<std::string, pvStruct>& magPVStructs = magnet.second.getPVStructs();

		updateAliasNameMap(magnet.second);
		for (auto& pv : magPVStructs)
		{

			// sets the monitor state in the pvstruict to true or false
			retrieveMonitorStatus(pv.second);

			magnet.second.epicsInterface->retrieveCHID(pv.second);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				//std::cout << "cs_conn, getting some values " << std::endl;
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
				//// update the alias_name_map so we can use fullNames or Aliases
			}
			else
			{
				//std::cout << magnet.first << " CANNOT CONNECT TO EPICS" << std::endl;
				messenger.printMessage(magnet.first, " CANNOT CONNECT TO EPICS");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
			updateAliasNameMap(magnet.second);
		}
	}
	hasBeenSetup = true;
	//std::cout << "hasBeenSetup = true " << std::endl;
	return hasBeenSetup;
}

void MagnetFactory::updateAliasNameMap(const Magnet& magnet)
{
	// first add in the magnet full name
	std::string full_name = magnet.getHardwareName();
	messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		messenger.printMessage("!!ERROR!! ", full_name, " magnet name already exists! ");
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
	std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	std::cout << name_to_check << " NOT found " << std::endl;
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
	return to_py_dict(getAllPSUState());
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
	return to_py_dict(getAllILKState());
}


magnetStateStruct MagnetFactory::getMagnetState()const
{
	magnetStateStruct magState;
	magState.magNames = getAllMagnetNames();
	magState.numMags = magState.magNames.size();
	//std::cout << "getMagnetState getttign magnet states" << std::endl;
	for (auto&& magnet : magState.magNames)
	{
		magState.psuStates.push_back(getPSUState(magnet));
		magState.setiValues.push_back(getSETI(magnet));
		magState.readiValues.push_back(getREADI(magnet));
		//std::cout << "magState.magnet " << magnet << std::endl;
		//std::cout << "magState.psuStates " << ENUM_TO_STRING(magState.psuStates.back()) << std::endl;
		//std::cout << "magState.setiValues " << magState.setiValues.back() << std::endl;
		//std::cout << "magState.readiValues " << magState.readiValues.back() << std::endl;
	}
	magState.magNames_Py = to_py_list(magState.magNames);
	magState.psuStates_Py = to_py_list(magState.psuStates);
	magState.setiValues_Py = to_py_list(magState.setiValues);
	magState.readiValues_Py = to_py_list(magState.readiValues);
	return magState;
}







void MagnetFactory::SETI(const std::string& name, const double& value)
{
	// TODO: check name exits in map
	magnetMap.at(name).SETI(value);
}
void MagnetFactory::SETI(const std::map<std::string, double>& namesAndCurrentsMap)
{
	for (const auto& entry : namesAndCurrentsMap)
	{
		SETI(entry.first, entry.second);
	}
	//return true;
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

STATE MagnetFactory::switchOFF(const std::string& name)
{
	if (GlobalFunctions::entryExists(magnetMap, name))
	{
		magnetMap.at(name).switchOFF();
		return STATE::SUCCESS;
	}
	return STATE::UNKNOWN_NAME;
}
//bool MagnetFactory::turnOff(const std::string& name)
//{
//	messenger.printDebugMessage("TURNING ", name, " OFF");
//	return magnetMap.at(name).setEPICSPSUState(STATE::OFF);
//}
std::map<std::string, STATE> MagnetFactory::switchOFF(const std::vector<std::string>& names)
{
	std::map<std::string, STATE> return_map;
	for (auto& name : names)
	{
		return_map[name] = switchOFF(name);
	}
	return return_map;
}
boost::python::dict MagnetFactory::switchOFF_Py(const boost::python::list names)
{
	return to_py_dict<std::string,STATE>(switchOFF(to_std_vector<std::string>(names) ));
}






//









bool MagnetFactory::writeDBURT(const std::string& filePath, const std::string& fileName)
{
	boost::filesystem::path directory(filePath);
	boost::filesystem::path file(fileName);
	boost::filesystem::path full_path = directory / file;

	magnetStateStruct ms = getMagnetState();

	std::map<std::string, std::string> magnet_data;
	std::map< std::string, std::map<std::string, std::string> > magnet_data_to_write;

	messenger.printDebugMessage("writeDBURT is generating DBURT data ");


	for(auto i = 0 ; i < ms.magNames.size(); ++i)
	{
		magnet_data.clear();
		magnet_data[MagnetRecords::SETI] = std::to_string(ms.setiValues[i]);
		magnet_data[MagnetRecords::READI] = std::to_string(ms.readiValues[i]);
		magnet_data[MagnetRecords::RPOWER] = ENUM_TO_STRING(ms.psuStates[i]);
		magnet_data_to_write[ms.magNames[i]] = magnet_data;

		messenger.printDebugMessage(ms.magNames[i] + " SETI / READI / RPOWER = " + std::to_string(ms.setiValues[i]) + " / "  + std::to_string(ms.readiValues[i]) + " / " + ENUM_TO_STRING(ms.psuStates[i]) );
	}

	YAML::Node node_to_write(magnet_data_to_write);
	std::ofstream fout(full_path.c_str());

	// write a header;
	fout << "#YAML Magnet Setting Save File VERSION 1" << std::endl;
	fout << node_to_write;

	return true;
}
//--------------------------------------------------------------------------------------------------
magnetStateStruct MagnetFactory::readDBURT(const std::string& filePath, const std::string& fileName)
{
	std::cout << "readDBURT()" << std::endl;

	magnetStateStruct magState;
	boost::filesystem::path directory(filePath);
	boost::filesystem::path file(fileName);
	boost::filesystem::path full_path = directory / file;

	std::ifstream fileInput;
	fileInput = std::ifstream(full_path.string());
	YAML::Parser parser(fileInput);
	YAML::Node magnet_setting = YAML::LoadFile(full_path.string());

	int i = 0;
	for (auto&& it : magnet_setting)
	{
		std::cout << i << std::endl;
		std::cout << it.first << std::endl;
		magState.magNames.push_back(it.first.as<std::string>());


		for(auto&& it2 : it.second)
		{

			std::string record = it2.first.as<std::string>();
			std::cout << record << " = " << it2.second << std::endl;
			if (MagnetRecords::SETI == record)
			{
				std::cout << record << " is " << MagnetRecords::SETI << std::endl;
				magState.setiValues.push_back( it2.second.as<double>() );
			}
			else if (MagnetRecords::READI == record)
			{
				std::cout << record << " is " << MagnetRecords::READI << std::endl;
				magState.readiValues.push_back(it2.second.as<double>());
			}
			else if (MagnetRecords::RPOWER == record)
			{
				std::cout << record << " is " << MagnetRecords::RPOWER << std::endl;
				magState.readiValues.push_back(static_cast<STATE>(it2.second.as<int>()));
			}
		}
		i+=1;
	}



	//std::string controlRecords = controlsInformation["records"].as<std::string>();
	//boost::trim_left(controlRecords);
	//std::pair<std::string, std::string> pvAndRecordPair;

	//// mode tells us if we are physical, virtual or offline
	//// which tells us what the
	//if (mode == STATE::VIRTUAL)
	//{
	//	pvAndRecordPair = std::make_pair(configInformationNode["properties"]["virtual_name"].as<std::string>(), controlRecords);
	//	std::cout << pvAndRecordPair.first << " , " << pvAndRecordPair.second << std::endl;
	//}



	//std::cout << "readDBURT FULL PATH = " << full_path << std::endl;

	//magnetStateStruct magState;

	//YAML::Node config;
	//YAML::Node configTemplate;
	//std::map<std::string, std::string> parameters;
	//try
	//{
	//	std::cout << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << std::endl;
	//	std::cout << "YAML::Parser parser(fileInput); " << std::endl;
	//	std::cout << "got config " << std::endl;
	//	if (config.size() > 0)
	//	{
	//		std::cout << "config.size() > 0" << std::endl;
	//		std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + SEPARATOR + config["properties"]




	//messenger.printMessage(("\n", "**** Attempting to Read ", full_path, " ****");

	//std::string line, trimmedLine;

	//std::ifstream inputFile;

	//configVersion = -1;

	//inputFile.open(full_path, std::ios::in);
	//if (inputFile)
	//{
	//	message("File Opened from ", pathandfile);
	//	while (std::getline(inputFile, line)) /// Go through, reading file line by line
	//	{
	//		trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));

	//		message(line);

	//		if (stringIsSubString(line, UTL::VELA_MAGNET_SAVE_FILE_v1))
	//		{
	//			configVersion = 1;
	//			break;
	//		}
	//		else if (stringIsSubString(line, UTL::DBURT_HEADER_V3))
	//		{
	//			configVersion = 3;// version 2 got lost around October 2015
	//			break;
	//		}
	//		else if (stringIsSubString(line, UTL::DBURT_HEADER_V4))
	//		{
	//			configVersion = 4;// version 2 got lost around October 2015
	//			break;
	//		}
	//		else if (stringIsSubString(line, UTL::VERSION))
	//		{
	//			if (stringIsSubString(trimmedLine, UTL::VERSION))
	//				getVersion(trimmedLine);
	//		}
	//		else if (stringIsSubString(line, UTL::VELA_CLARA_DBURT_ALIAS_V1))
	//		{
	//			message("stringIsSubString(line, UTL::VELA_CLARA_DBURT_ALIAS_V1)");
	//			std::getline(inputFile, line);
	//			trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
	//			std::vector<std::string> keyvalue = getKeyVal(trimmedLine, UTL::EQUALS_SIGN_C);

	//			message(keyvalue[0]);
	//			message(keyvalue[1]);
	//			pathandfile = getFilePathFromINputs(trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[0]),
	//				trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[1]));
	//			message(pathandfile);
	//			configVersion = 4;
	//		}
	//	}
	//}
	//debugMessage("Finished preprocessing file");
	//inputFile.close();
	//magnetStructs::magnetStateStruct magState;
	//switch (configVersion)
	//{
	//case -1:
	//	debugMessage("NO DBURT VERSION FOUND EXIT");
	//	break;

	//case 1:
	//	debugMessage("VERSION 1 DBURT FOUND");
	//	//magState = readDBURTv1(filePath);
	//	break;
	//case 2:
	//	//debugMessage("VERSION 2 DBURT FOUND");
	//	break;
	//case 3:
	//	debugMessage("VERSION 3 DBURT FOUND");
	//	//magState = readDBURTv3(filePath);
	//	break;
	//case 4:
	//	debugMessage("VERSION 4 DBURT FOUND");
	//	magState = readDBURTv4(pathandfile);
	//	break;
	//default:
	//	debugMessage("UNEXPECTED DBURT VERSION, ", configVersion, ", FOUND");

	//}
	return magState;
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



//double MagnetFactory::getRICurrent(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
//	}
//	else
//	{
//		return magnetMap.find(name)->second.getREADI();
//	}
//	return std::numeric_limits<double>::min();
//}

//std::map<std::string, double> MagnetFactory::getRICurrents(const std::vector<std::string>& names)
//{
//	std::map<std::string, double> readis;
//	for (auto& name : names)
//	{
//		double current = magnetMap.find(name)->second.getREADI();
//		readis[name] = current;
//	}
//	return readis;
//}


// This is being moved so that
//boost::python::dict MagnetFactory::getCurrents_Py(boost::python::list magNames)
//{
//	std::map<std::string, double> currents;
//	std::vector<std::string> magNamesVector = to_std_vector<std::string>(magNames);
//	currents = getCurrents(magNamesVector);
//	boost::python::dict newPyDict = to_py_dict(currents);
//	return newPyDict;
//}
//boost::python::dict MagnetFactory::getRICurrents_Py(boost::python::list names)
//{
//	std::map<std::string, double> RICurrents;
//	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
//	RICurrents = getRICurrents(namesVector);
//	boost::python::dict newPyDict = to_py_dict(RICurrents);
//	return newPyDict;
//}
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

//bool MagnetFactory::setCurrents_Py(boost::python::dict magnetNamesAndCurrents)
//{
//	std::map<std::string, double> magnetNamesAndCurrentsToSet;
//	std::vector<std::string> magnetNames = to_std_vector<std::string>(magnetNamesAndCurrents.keys());
//	std::vector<double> currentsToSet = to_std_vector<double>(magnetNamesAndCurrents.values());
//	auto magnetNamesIterator = magnetNames.begin();
//	auto currentsToSetIterator = currentsToSet.begin();
//	while (magnetNamesIterator != magnetNames.end() && currentsToSetIterator != currentsToSet.end())
//	{
//		std::pair<std::string, double> entryToInsert = std::pair<std::string, double>(magnetNamesIterator->data(), *currentsToSetIterator);
//		magnetNamesAndCurrentsToSet.emplace(entryToInsert);
//		magnetNamesIterator++;
//		currentsToSetIterator++;
//	}
//	SETI(magnetNamesAndCurrentsToSet);
//	return true;
//}
