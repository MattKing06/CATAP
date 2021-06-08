#ifndef LINAC_PID_FACTORY_H_
#define LINAC_PID_FACTORY_H_

#include <LoggingSystem.h>
#include <ConfigReader.h>
#include <LinacPID.h>
#include <map>
#include <vector>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

/** @addtogroup factories
 @{*/
 /*! A class to store, setup, and perform operations on multiple Linac PID objects*/

class LinacPIDFactory
{
public:
	LinacPIDFactory();
	LinacPIDFactory(STATE mode);
	LinacPIDFactory(const LinacPIDFactory& copyLinacPIDFactory);
	~LinacPIDFactory();
	STATE mode;
	bool hasBeenSetup;
	std::map<std::string, LinacPID> linacPIDMap;
	ConfigReader reader;
	LoggingSystem messenger;
	bool setup(const std::string& version);
	void populateLinacPIDMap();
	void setupChannels();
	void retrieveMonitorStatus(pvStruct& pvStruct) const;
	LinacPID& getLinacPID(const std::string& fullLinacPIDName);
	std::string getFullName(const std::string& nameToCheck) const;
	std::vector<std::string> getAllLinacPIDNames() const;
	boost::python::list getAllLinacPIDNames_Py() const;
	/*! turns debug messages on for valveFactory and calls same function in all valves and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for valveFactory and calls same function in all valves and configReader*/
	void debugMessagesOff();
	/*! turns messages on for valveFactory and calls same function in all valves and configReader*/
	void messagesOn();
	/*! turns messages off for valveFactory and calls same function in all valves and configReader*/
	void messagesOff();
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	/*! returns true if messenger messages flag is true, false otherwise*/
	bool isMessagingOn();
private:
	/*! function to update map of aliases to full-names of valves
	* @param[in] valve : The Valve object we want to update the alias map with.*/
	void updateAliasNameMap(const LinacPID& linacPID);
	/*! a map that provides the link between full names and associated aliases.*/
	std::map<std::string, std::string> aliasNameMap;
};

#endif // !LINAC_PID_FACTORY_H_