#ifndef SHUTTER_FACTORY_H_
#define SHUTTER_FACTORY_H_

#include <ConfigReader.h>
#include <Shutter.h>
#include <GlobalStateEnums.h>
#include <vector>
class ShutterFactory
{
public:
	ShutterFactory();
	ShutterFactory(STATE mode);
	ShutterFactory(const ShutterFactory& copyFactory);
	~ShutterFactory();
	LoggingSystem messenger;
	bool hasBeenSetup;
	STATE mode;


	void populateShutterMap();
	/*! iterates over all valves in valve map and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();

	bool setup(const std::string version);



	bool open(const std::string& name);
	bool close(const std::string& name);
	bool isOpen(const std::string& name);
	bool isClosed(const std::string& name);
	STATE getState(const std::string& name)const;
	int getCMI(const std::string& name)const;
	std::map<std::string, bool> getCMIBitMap(const std::string& name)const;
	boost::python::dict getCMIBitMap_Py(const std::string& name)const;

	std::string getFullName(const std::string& name_to_check) const;


	std::vector<std::string> getAliases(const std::string& name) const;

//private:
	void updateAliasNameMap(const Shutter& shutter);
	std::map<std::string, std::string> alias_name_map;



	std::map<std::string, Shutter> shutterMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();

	void setMonitorStatus(pvStruct& pvStruct);


	// private
	ConfigReader reader;

};

#endif // SHUTTER_FACTORY_H_