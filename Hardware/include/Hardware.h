#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <map>
#include <iostream>
#include <LoggingSystem.h>
#ifndef PV_H_
#include <PV.h>
#endif
#include <GlobalStateEnums.h>
#include <GlobalTypeEnums.h>
#include "GlobalConstants.h"
#include "HardwareState.h"

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


	// TODO: do we need this? can't an child of these class just access pvStructs,
	// and no other class should be able ot get this map??? 
	std::map<std::string, pvStruct>& getPVStructs();
	// TODO  for some reaons i think these should be keyed by an enum giving their type 
	//std::map<int, pvStruct>& getPVStructs2();


	/*PLAN TO MAKE THIS A VIRTUAL FUNCTION TO BE OVERRIDEN BY SPECIFIC HARDWARE CLASSES*/
	//virtual void setPVStructs(std::map<std::string, std::string> recordAndPVStructs);

	std::map<std::string, std::string> getSpecificHardwareParameters() const;
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

	STATE mode; // PHYSICAL VIRTUAL OFFLINE


	HardwareState state_IO;

// <<<<<<< HEAD

// =======
	// static std::map<std::string, TYPE> create_map()
	// {
		// std::map<std::string, TYPE> m;
		// m["Magnet"] = TYPE::MAGNET;
		// return m;
	// }
	// static const std::map<std::string, TYPE> string_to_hardware_type_map;
// >>>>>>> 57_magnet_llrf_cam_bug_fix_fro_sim-frame_integration
};
/** @}*/
#endif //HARDWARE_H_