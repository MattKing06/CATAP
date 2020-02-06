#ifndef VALVE_FACTORY_H_
#define VALVE_FACTORY_H_


#include <LoggingSystem.h>
#include <ConfigReader.h>
#include <Valve.h>
#include <map>
#include <vector>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>


class ValveFactory
{
public:
	ValveFactory();
	ValveFactory(STATE mode);
	ValveFactory(const ValveFactory& copyValveFactory);
	~ValveFactory();
	STATE mode;
	bool hasBeenSetup;
	std::map<std::string, Valve> valveMap;
	ConfigReader reader;
	LoggingSystem messenger;


	bool setup(const std::string& VERSION);
	void populateValveMap();
	void setupChannels();
	void retrieveMonitorStatus(pvStruct& pvStruct) const;
	Valve& getValve(const std::string& fullValveName);
	std::string getFullName(const std::string& nameToCheck) const;
	std::vector<std::string> getAllValveNames() const;
	boost::python::list getAllValveNames_Py() const;
	STATE getValveState(const std::string& name) const;
	std::map<std::string, STATE> getValveStates(const std::vector<std::string>& names) const;
	boost::python::dict getValveStates_Py(const boost::python::list& names) const;
	std::map<std::string, STATE> getAllValveStates() const;
	boost::python::dict getAllValveStates_Py() const;
	void setValveState(const std::string& name, const STATE& state);
	void close(const std::string& name);
	void close(const std::vector<std::string>& names);
	void close_Py(const boost::python::list& names);
	void closeAllValves();
	void open(const std::string& name);
	void open(const std::vector<std::string>& names);
	void open_Py(const boost::python::list& names);
	void openAllValves();
	bool isOpen(std::string name) const;
	bool isClosed(std::string name) const;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
private:
	void updateAliasNameMap(const Valve& valve);
	std::map<std::string, std::string> aliasNameMap;
	Valve dummyValve;

};


#endif //VALVE_FACTORY_H_