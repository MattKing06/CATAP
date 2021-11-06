#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <map>
#include <iostream>
#include <boost/variant.hpp>
#include <LoggingSystem.h>
#ifndef PV_H_
#include <PV.h>
#endif
#include <GlobalStateEnums.h>
#include <GlobalTypeEnums.h>
#include <GlobalFunctions.h>
#include "GlobalConstants.h"
#include <boost/python/dict.hpp>
#include <HardwareSnapshot.h>

/** @defgroup hardware Hardware
 *  @brief A collection of classes that represent hardware components of VELA/CLARA with parameters defined by configuration files.
 *	@{
 */

class Hardware
{
public:
	// vector of PV structs 
	// string hardware type
	// vector of strings for aliases relating to enums
	Hardware();

	// for creating "proper" objects that are defeined in a yaml file 
	Hardware(const std::map<std::string, std::string>& specificValueMap, STATE mode);
	
	// for creating hardware objects that do not have yaml files (used specifically for the EPICSTools construction)
	Hardware(STATE mode);

	// copy-cOntructor (forced by c++ standard due to passing in "const Hardware&" )
	Hardware(const Hardware& copyHardware);

	
	std::string getMachineAreaStr() const;
	TYPE getMachineArea() const;
	std::string getHardwareTypeStr() const;
	TYPE getHardwareType() const;

	std::string getHardwareName() const;


	STATE getMode() const;
	bool isVirtual()const;
	bool isPhysical()const;
	bool isOffline()const;

	bool isNotPhysical()const;



	std::map<std::string, std::string> getSpecificHardwareParameters() const;
	std::map<std::string, std::string> getOfflineProperties()const;
	boost::python::dict getOfflineProperties_Py();
	void setOfflineProperties(const std::map<std::string, std::string>& properties);
	std::map<std::string, std::string> getOnlineProperties()const;
	boost::python::dict getOnlineProperties_Py();
	void setOnlineProperties(const std::map<std::string, std::string>& properties);

	virtual HardwareSnapshot getSnapshot();
	virtual boost::python::dict getSnapshot_Py();
	std::map<std::string, std::string> offlineProperties;
	std::map<std::string, std::string> onlineProperties;
	// TODO: do we need this? can't an child of these class just access pvStructs,
	// and no other class should be able ot get this map??? 
	std::map<std::string, pvStruct>& getPVStructs();
	// TODO  for some reaons i think these should be keyed by an enum giving their type 
	//std::map<int, pvStruct>& getPVStructs2();

	// should be updated when you getSnapoht in the specific hardware class
	HardwareSnapshot currentSnapshot;

	HardwareSnapshot getHardwareSnapshot();
	boost::python::dict getHardwareSnapshot_Py();



	/*PLAN TO MAKE THIS A VIRTUAL FUNCTION TO BE OVERRIDEN BY SPECIFIC HARDWARE CLASSES*/
	//virtual void setPVStructs(std::map<std::string, std::string> recordAndPVStructs);


	bool operator==(Hardware rhs);
	virtual void debugMessagesOn();
	virtual void debugMessagesOff();
	virtual void messagesOn();
	virtual void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();

	std::string hardwareName;
	
	// need to sort out private/protected access for these variables
	// TODOD THESE SHOULD NOT BE STRINGS, they are TYPE enums
	std::string machine_area_str;
	std::string hardware_type_str;
	TYPE machine_area;
	TYPE hardware_type;

	std::map<std::string, pvStruct> pvStructs;
	
	// this will be an enum in the dervied class ...
	//std::map<int, pvStruct> pvStructs2;

	std::map<std::string, std::string> specificHardwareParameters;
	LoggingSystem messenger;

	STATE mode; // PHYSICAL, VIRTUAL, OFFLINE

};
/** @}*/
#endif //HARDWARE_H_