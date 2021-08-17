#include "LaserEnergyMeterFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <utility>
#include "GlobalFunctions.h"
#include "GlobalConstants.h"
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif
#include "yaml-cpp/emitter.h"

LaserEnergyMeterFactory::LaserEnergyMeterFactory() : LaserEnergyMeterFactory(STATE::OFFLINE)
{
	std::cout << "LaserFactory DEFAULT constRUCTOR called " << std::endl;
}
LaserEnergyMeterFactory::LaserEnergyMeterFactory(STATE mode) :
LaserEnergyMeterFactory(mode, MASTER_LATTICE_FILE_LOCATION)
{

}
LaserEnergyMeterFactory::LaserEnergyMeterFactory(STATE mode, const std::string& primeLatticeLocation) :
mode(mode),
hasBeenSetup(false),
reader(ConfigReader("LaserEnergyMeter", mode, primeLatticeLocation))
{
	messenger = LoggingSystem(true, true);
	//hasBeenSetup = false;
//	messenger = LoggingSystem(false, false);
	messenger.printDebugMessage("Laser Factory constructed");
}
LaserEnergyMeterFactory::LaserEnergyMeterFactory(const LaserEnergyMeterFactory& copyLaserEnergyMeterFactory)
	: hasBeenSetup(copyLaserEnergyMeterFactory.hasBeenSetup),
	mode(copyLaserEnergyMeterFactory.mode),
	messenger(copyLaserEnergyMeterFactory.messenger),
	reader(copyLaserEnergyMeterFactory.reader)
{
	messenger.printDebugMessage("LaserFactory Copy cOntructor");
	laserEnergyMeterMap.insert(copyLaserEnergyMeterFactory.laserEnergyMeterMap.begin(), copyLaserEnergyMeterFactory.laserEnergyMeterMap.end());
}

LaserEnergyMeterFactory::~LaserEnergyMeterFactory()
{
	messenger.printDebugMessage("LaserEnergyMeterFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& laser : laserEnergyMeterMap)
		{
			auto pvStructsList = laser.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					laser.second.epicsInterface->removeSubscription(pvStruct.second);
					ca_flush_io();
				}
				laser.second.epicsInterface->removeChannel(pvStruct.second);
				ca_pend_io(CA_PEND_IO_TIMEOUT);
			}
		}
	}
}

void LaserEnergyMeterFactory::populateLaserEnergyMeterMap()
{
	messenger.printDebugMessage("LaserEnergyMeterFactory is populating the laser map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive cOnfiguratiOn parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Laser Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(laserEnergyMeterMap);
	}
	messenger.printDebugMessage("LaserEnergyMeterFactory has finished populating the laser map");
}

void LaserEnergyMeterFactory::retrievemonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == LaserEnergyMeterRecords::OVERRANGE ||
		pvStruct.pvRecord == LaserEnergyMeterRecords::RUNSP ||
		pvStruct.pvRecord == LaserEnergyMeterRecords::RANGESP ||
		pvStruct.pvRecord == LaserEnergyMeterRecords::ENERGYREADBACK
		)
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
}

void LaserEnergyMeterFactory::setupChannels()
{
	for (auto& laser : laserEnergyMeterMap)
	{
		std::map<std::string, pvStruct>& pvStructs = laser.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			laser.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

bool LaserEnergyMeterFactory::setup(const std::string& VERSION)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in laser constructor
	//// but we have a lot of PV informatiOn to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateLaserEnergyMeterMap();

	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto& laser : laserEnergyMeterMap)
	{
		std::map<std::string, pvStruct>& laserPVStructs = laser.second.getPVStructs();
		for (auto& pv : laserPVStructs)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrievemonitorStatus(pv.second);
				laser.second.epicsInterface->retrieveCHTYPE(pv.second);
				laser.second.epicsInterface->retrieveCOUNT(pv.second);
				laser.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					laser.second.epicsInterface->createSubscription(laser.second, pv.second);
				}
				EPICSInterface::sendToEPICS();
			}
			else
			{
				messenger.printMessage(laser.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
	std::cout << "end" << std::endl;
}

std::map<std::string, LaserEnergyMeter> LaserEnergyMeterFactory::getLaserEnergyMeters(std::vector<std::string> laserNames)
{
	std::map<std::string, LaserEnergyMeter> selectedLasers;
	for (auto& laserName : laserNames)
	{
		selectedLasers[laserName] = laserEnergyMeterMap.find(laserName)->second;
	}
	return selectedLasers;
}

LaserEnergyMeter& LaserEnergyMeterFactory::getLaserEnergyMeter(const std::string& fullLaserName)
{
	return laserEnergyMeterMap.find(fullLaserName)->second;
}

std::map<std::string, LaserEnergyMeter> LaserEnergyMeterFactory::getAllLaserEnergyMeters()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("LASER EnergyMeters HAVE ALREADY BEEN constRUCTED.");
	}
	return laserEnergyMeterMap;
}

std::string LaserEnergyMeterFactory::getLaserEnergyMeterName(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call LaserEnergyMeterFactory.setup(VERSION)");
	}
	else
	{
		return laserEnergyMeterMap.find(name)->second.getLaserEnergyMeterName();
	}
	return "0";
}

bool LaserEnergyMeterFactory::isMonitoring(const std::string& name) const
{
	return laserEnergyMeterMap.find(name)->second.isMonitoring();
}

bool LaserEnergyMeterFactory::isEnergyBufferFull(const std::string& name) const
{
	return laserEnergyMeterMap.find(name)->second.isEnergyBufferFull();
}

bool LaserEnergyMeterFactory::isEnergyVectorFull(const std::string& name) const
{
	return laserEnergyMeterMap.find(name)->second.isEnergyVectorFull();
}

bool LaserEnergyMeterFactory::getAcquiring(const std::string& name) const
{
	return laserEnergyMeterMap.find(name)->second.getAcquiring();
}

bool LaserEnergyMeterFactory::setStart(const std::string& name)
{
	return laserEnergyMeterMap.find(name)->second.setStart();
}

bool LaserEnergyMeterFactory::setStop(const std::string& name)
{
	return laserEnergyMeterMap.find(name)->second.setStop();
}

bool LaserEnergyMeterFactory::setRange(const std::string& name, int range)
{
	return laserEnergyMeterMap.find(name)->second.setRange(range);
}

int LaserEnergyMeterFactory::getOverRange(const std::string& name) const
{
	return laserEnergyMeterMap.find(name)->second.getOverRange();
}

int LaserEnergyMeterFactory::getRange(const std::string& name) const
{
	return laserEnergyMeterMap.find(name)->second.getRange();
}

double LaserEnergyMeterFactory::getEnergy(const std::string& name) const
{
	return laserEnergyMeterMap.find(name)->second.getEnergy();
}

std::vector< double > LaserEnergyMeterFactory::getEnergyVector(const std::string& name) const
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call LaserEnergyMeterFactory.setup(VERSION)");
	}
	else
	{
		return laserEnergyMeterMap.find(name)->second.getEnergyVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< double > LaserEnergyMeterFactory::getEnergyBuffer(const std::string& name) const
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call LaserEnergyMeterFactory.setup(VERSION)");
	}
	else
	{
		return laserEnergyMeterMap.find(name)->second.getEnergyBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::python::list LaserEnergyMeterFactory::getEnergyVector_Py(const std::string& name)
{
	std::vector< double > vec;
	vec = getEnergyVector(name);
	boost::python::list newPyList = to_py_list(vec);
	return newPyList;
}

boost::python::list LaserEnergyMeterFactory::getEnergyBuffer_Py(const std::string& name)
{
	boost::circular_buffer< double > buf;
	buf = getEnergyBuffer(name);
	boost::python::list newPyList = to_py_list(buf);
	return newPyList;
}

void LaserEnergyMeterFactory::monitorForNShots(const std::string& name, const size_t& value)
{
	laserEnergyMeterMap.find(name)->second.monitorForNShots(value);
}

void LaserEnergyMeterFactory::setVectorSize(const std::string& name, const size_t& value)
{
	laserEnergyMeterMap.find(name)->second.setVectorSize(value);
}

void LaserEnergyMeterFactory::setBufferSize(const std::string& name, const size_t& value)
{
	laserEnergyMeterMap.find(name)->second.setBufferSize(value);

}

void LaserEnergyMeterFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LASER-EM-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& laser : laserEnergyMeterMap)
	{
		laser.second.debugMessagesOn();
	}
}

void LaserEnergyMeterFactory::debugMessagesOff()
{
	messenger.printDebugMessage("LASER-EM-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& laser : laserEnergyMeterMap)
	{
		laser.second.debugMessagesOff();
	}
}

void LaserEnergyMeterFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("LASER-EM-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& laser : laserEnergyMeterMap)
	{
		laser.second.messagesOn();
	}
}

void LaserEnergyMeterFactory::messagesOff()
{
	messenger.printMessage("LASER-EM-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& laser : laserEnergyMeterMap)
	{
		laser.second.messagesOff();
	}
}

bool LaserEnergyMeterFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool LaserEnergyMeterFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}