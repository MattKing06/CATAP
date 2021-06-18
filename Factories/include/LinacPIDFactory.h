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


	//bool setPhase(const std::string& name, const double& value);
	//bool setPhase(const std::map<std::string, double>& name_value_map);
	//
	//double getPhase(const std::string& name)const;
	//std::map<std::string, double> getPhase()const;
	//boost::python::dict getPhase_Py()const;

	//bool setCh3PhaseWeight(const std::string& name, const double& value);

	//double getCh3PhaseWeight(const std::string& name)const;
	//std::map<std::string, double> getCh3PhaseWeight()const;
	//boost::python::dict getCh3PhaseWeight_Py()const;

	//bool setCh6PhaseWeight(const std::string& name, const double& value);
	//double getCh6PhaseWeight(const std::string& name)const;
	//std::map<std::string, double> getCh6PhaseWeight()const;
	//boost::python::dict getCh6PhaseWeight_Py()const;

	//boost::python::dict getPhaseWeights()const;

	//bool enable(const std::string& name);
	//bool enable();
	//bool disable(const std::string& name);
	//bool disable();

	//bool isEnabled(const std::string& name)const;
	//std::map<std::string, bool> isEnabled()const;
	//boost::python::dict isEnabled_Py()const;

	//bool isDisabled(const std::string& name)const;
	//std::map<std::string, bool> isDisabled()const;
	//boost::python::dict isDisabled_Py()const;


	//bool getEnabledState(const std::string& name)const;
	//std::map<std::string, STATE> getEnabledState()const;
	//boost::python::dict getEnabledState_Py()const;

	//double getCh3PhaseWrapped(const std::string& name)const;
	//std::map<std::string, double> getCh6PhaseWeight()const;
	//boost::python::dict getCh6PhaseWeight_Py()const;

	//double getCh6PhaseWrapped(const std::string& name)const;


	/// TODO  for futre LLRF boxes the channel Numbers are likely to be different, so perhaps not hardcode the Channel NUmbers
	/// use more descriptive Names, Forward Reverse 
	




	std::map<std::string, double> getPhaseWrapped()const;
	boost::python::dict getCh6PhaseWrapped_Py()const;
		
	double getOVAL(const std::string& name)const;
	std::map<std::string, double> getOVAL()const;
	boost::python::dict getOVAL_Py()const;

		

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