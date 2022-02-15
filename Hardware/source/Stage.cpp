#include <Stage.h>
#include <boost/algorithm/string.hpp>
Stage::Stage()
{
}

Stage::Stage(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
currentPosition(std::pair<epicsTimeStamp, double>(epicsTimeStamp(), GlobalConstants::double_min)),
positionSetpoint(std::pair<epicsTimeStamp, double>(epicsTimeStamp(), GlobalConstants::double_min)),
aliases(std::vector<std::string>()),
moving(false),
atDevice(false)
{
	messenger = LoggingSystem(true, true);
	if (GlobalFunctions::entryExists(paramMap, "min_pos"))
	{minPosition = std::stod(paramMap.at("min_pos"));}
	if (GlobalFunctions::entryExists(paramMap, "max_pos"))
	{maxPosition = std::stod(paramMap.at("max_pos"));}
	if (GlobalFunctions::entryExists(paramMap, "stage_number"))
	{stageNumber = std::stoi(paramMap.at("stage_number"));}
	if (GlobalFunctions::entryExists(paramMap, "has_yag"))
	{std::istringstream(paramMap.at("has_yag")) >> std::boolalpha >> hasYag;}
	if (GlobalFunctions::entryExists(paramMap, "precision"))
	{precision = std::stoi(paramMap.at("precision"));}
	if (GlobalFunctions::entryExists(paramMap, "in_pos"))
	{inPosition = std::stod(paramMap.at("in_pos"));}
	if (GlobalFunctions::entryExists(paramMap, "out_pos"))
	{outPosition = std::stod(paramMap.at("out_pos"));}
	if (GlobalFunctions::entryExists(paramMap, "name_alias"))
	{updateAliases(paramMap.at("name_alias"));}
	if (GlobalFunctions::entryExists(paramMap, "devices") && GlobalFunctions::entryExists(paramMap, "positions"))
	{
		std::vector<std::string> devices;
		std::vector<std::string> positions;
		boost::split(devices, paramMap.at("devices"), boost::is_any_of(","), boost::token_compress_on);
		boost::split(positions, paramMap.at("positions"), boost::is_any_of(","), boost::token_compress_on);
		if (positions.size() == devices.size())
		{
			for (size_t i = 0; i < devices.size(); i++)
			{
				deviceAndPositionMap[devices[i]] = std::stof(positions[i]);
			}
		}
		else 
		{
			messenger.printMessage("***", hardwareName, "*** \n", 
								   "DEVICES AND POSITIONS ITEMS ARE NOT EQUAL. \n",
								   "PLEASE ADJUST CONFIG FILE BEFORE TRYING AGAIN.",
								   "\n **************** \n");
		}
	}
	setPVStructs();
	epicsInterface = boost::make_shared<EPICSStageInterface>(EPICSStageInterface());
}

Stage::Stage(const Stage& copyStage) :
	epicsInterface(copyStage.epicsInterface)
{}

Stage::~Stage()
{
}

void Stage::setPVStructs()
{
	for (auto&& record : StageRecords::stageRecordList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		std::string PV = specificHardwareParameters.at(record);
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

void Stage::updateAliases(const std::string& alias)
{
	messenger.printDebugMessage("stage: ", hardwareName, " alias: ", alias);
	aliases.push_back(alias);
}

std::vector<std::string> Stage::getAliases()
{
	return aliases;
}

void Stage::setCurrentPosition(std::pair<epicsTimeStamp, double> newPosition)
{
	currentPosition = newPosition;
}

std::pair<epicsTimeStamp, double> Stage::getCurrentPosition()
{
	return currentPosition;
}

double Stage::getCurrentPositionValue()
{
	return currentPosition.second;
}

void Stage::setPositionSetpoint(std::pair<epicsTimeStamp, double> newSetpoint)
{
	positionSetpoint = newSetpoint;
}

std::pair<epicsTimeStamp, double> Stage::getPositionSetpoint()
{
	return positionSetpoint;
}

double Stage::getPositionSetpointValue()
{
	return positionSetpoint.second;
}

bool Stage::canStageMove(double newPosition)
{
	return(minPosition <= newPosition) && (maxPosition >= newPosition);
}

void Stage::moveOverRange(double startPosition, double endPosition, int numberOfSteps)
{
	if (numberOfSteps > 0)
	{
		double newPosition = startPosition;
		std::vector<double> positions = std::vector<double>();
		double stepSize = (endPosition - startPosition) / double(numberOfSteps);

		for (double i = startPosition; i != endPosition + stepSize; i += stepSize)
		{
			positions.push_back(i);
		}
		setNewPositions(positions);
	}
	else
	{
		messenger.printMessage("Cannot set number of step less than or equal to zero.");
	}
}

void Stage::setNewPositions(std::vector<double> positions)
{
	for (double& position : positions)
	{
		setNewPosition(position);
	}
}

void Stage::setNewPosition(double newPosition)
{
	if (canStageMove(newPosition))
	{
		if (!epicsInterface->setNewPosition(pvStructs.at(StageRecords::MABSS), newPosition))
		{
			messenger.printDebugMessage("Unable to send ", newPosition, " to ", pvStructs.at(StageRecords::MABSS).fullPVName);
		}
	}
	else
	{
		messenger.printMessage("Tried to move to: ", newPosition, " because it's outside of min/max limits.");
	}
}

bool Stage::moveToDevice(const std::string& device)
{
	if (GlobalFunctions::entryExists(deviceAndPositionMap, device))
	{
		setNewPosition(deviceAndPositionMap.at(device));
		return true;
	}
	return false;
}

bool Stage::isMoving()
{
	return moving;
}

bool Stage::isAtDevice(const std::string& device)
{
	if (GlobalFunctions::entryExists(deviceAndPositionMap, device))
	{
		double devicePosition = deviceAndPositionMap.at(device);
		if (isMoving())
		{
			atDevice = false;
		}
		else if (GlobalFunctions::areSame(currentPosition.second, devicePosition, double(precision)) )
		{
			atDevice = true;
		}
		return atDevice;
	}

}

std::vector<std::string> Stage::getDevices()
{
	std::vector<std::string> deviceNames;
	for (auto&& item : deviceAndPositionMap)
	{
		deviceNames.push_back(item.first);
	}
	return deviceNames;
}
#ifdef BUILD_PYTHON
boost::python::list Stage::getDevices_Py()
{
	return to_py_list(getDevices());
}
#endif //BUILD_PYTHON


bool Stage::isReadPositionEqualToSetPosition()
{
	return abs(currentPosition.second - positionSetpoint.second) <= precision;
}

double Stage::getDevicePosition(const std::string& device)
{
	if (GlobalFunctions::entryExists(deviceAndPositionMap, device))
	{
		return deviceAndPositionMap.at(device);
	}
	else
	{
		messenger.printMessage("Device:", device, " does not exist in CATAP ConfigFiles. Please try another device.");
	}
}

bool Stage::clearForBeam()
{
	return moveToDevice("CLEAR_FOR_BEAM");
}

std::map<std::string, double> Stage::getDevicesAndPositions()
{
	return deviceAndPositionMap;
}

double Stage::getInPosition()
{
	if (GlobalFunctions::entryExists(specificHardwareParameters, "in_pos"))
	{
		return std::stod(specificHardwareParameters.at("in_pos"));
	}
	else
	{
		messenger.printMessage("IN_POS not defined for: ", hardwareName);
	}
}

double Stage::getOutPosition()
{
	if (GlobalFunctions::entryExists(specificHardwareParameters, "out_pos"))
	{
		return std::stod(specificHardwareParameters.at("out_pos"));
	}
	else
	{
		messenger.printMessage("OUT_POS not defined for: ", hardwareName);
	}
}

size_t Stage::getStageNumber()
{
	if (GlobalFunctions::entryExists(specificHardwareParameters, "stage_number"))
	{
		return std::stoi(specificHardwareParameters.at("stage_number"));
	}
	else
	{
		messenger.printMessage("stage_number not defined for: ", hardwareName);
	}
}

size_t Stage::getPrecision()
{
	if (GlobalFunctions::entryExists(specificHardwareParameters, "precision"))
	{
		return std::stoi(specificHardwareParameters.at("precision"));
	}
	else
	{
		messenger.printMessage("precision not defined for: ", hardwareName);
	}
}

double Stage::getMinPosition()
{
	return minPosition;
}

bool Stage::doesHaveYag()
{
	return hasYag;
}

double Stage::getMaxPosition()
{
	return maxPosition;
}


void Stage::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Stage::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Stage::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Stage::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}





//std::string const StageDevices::CLEAR_FOR_BEAM = "CLEAR_FOR_BEAM";
//std::string const StageDevices::YAG_UP = "YAG_UP";
//std::string const StageDevices::YAG_DOWN = "YAG_DOWN";
//std::string const StageDevices::YAG_RECTICLE = "YAG_RECTICLE";
//std::string const StageDevices::COLL_D2_5MM = "COLL_D2_5MM";
//std::string const StageDevices::COLL_D2MM = "COLL_D2MM";
//std::string const StageDevices::COLL_D1_5MM = "COLL_D1_5MM";
//std::string const StageDevices::COLL_D1MM = "COLL_D1MM";
//std::string const StageDevices::OTR = "OTR";
//std::string const StageDevices::GAS_JET = "GAS_JET";
//std::string const StageDevices::DCP = "DCP";
//std::string const StageDevices::DTH = "DTH";
//std::string const StageDevices::YAG_DCP_5x5 = "5x5_YAG_DCP";
//std::string const StageDevices::V_SLIT = "V_SLIT";
//std::string const StageDevices::H_SLIT = "H_SLIT";
//std::string const StageDevices::EO_CRYSTAL = "EO_CRYSTAL";
//std::string const StageDevices::EO_YAG = "EO_YAG";
//std::vector<std::string> StageDevices::devices = {StageDevices::CLEAR_FOR_BEAM,
//													StageDevices::YAG_UP,
//													StageDevices::YAG_DOWN,
//													StageDevices::YAG_RECTICLE,
//													StageDevices::COLL_D2_5MM,
//													StageDevices::COLL_D2MM,
//													StageDevices::COLL_D1_5MM,
//													StageDevices::COLL_D1MM,
//													StageDevices::OTR,
//													StageDevices::GAS_JET,
//													StageDevices::DCP,
//													StageDevices::DTH,
//													StageDevices::YAG_DCP_5x5,
//													StageDevices::V_SLIT,
//													StageDevices::H_SLIT,
//													StageDevices::EO_CRYSTAL,
//													StageDevices::EO_YAG };