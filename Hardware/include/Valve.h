#ifndef VALVE_H_
#define VALVE_H_
#include <Hardware.h>
#include <EPICSValveInterface.h>
#include <boost/shared_ptr.hpp>
#include <GlobalConstants.h>
#include <ValvePVRecords.h>
#include <vector>



class EPICSValveInterface;
typedef boost::shared_ptr<EPICSValveInterface> EPICSValveInterface_sptr;

/** @addtogroup hardware
 *@{*/
/*! A class to store information and communicate via EPICS to control Vacuum Valves on VELA/CLARA.
	A collection of these classes will be stored in map in the ValveFactory class.
*/
class Valve : public Hardware
{
public:
	/*! Default constructor call for Valve object*/
	Valve();
	/*! Custom constructor for Valve object
		@param[in] valveParameterMap strings defining parameters extracted from YAML config files
		@param[in] mode Defines the STATE of Valve we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)
	*/
	Valve(const std::map<std::string, std::string>& valveParameterMap, STATE mode);
	/*! EPICSValveInterface for valve-specifc calls to EPICS, includes setting open/closed state and monitoring current states*/
	EPICSValveInterface_sptr epicsInterface;
	/*! A map for storing the parameters extracted from YAML config files and their values */
	std::map<std::string, std::string> valveParameterAndValuesMap;
	/*! Goes through all PV records stored in ValveRecords namespace, creates pvStructs and sets up their record/fullPVName before
		inserting them into a vector of pvStructs which is later used by the ValveFactory to setup connections to EPICS */
	void setPVStructs();
	/*! A function to retrieve the aliases for a given Valve object
		@param[out] vector of aliases that the Valve is also known by*/
	std::vector<std::string> getAliases() const;
	/*! sets the valve state to open. 
	If we have a Physical valve then the request is sent to CLARA EPICS to open the valve.
	If we have a Virtual valve then the request is sent to Virtual EPICS to open the valve.
	If we have an Offline valve then the valve state is simply set to OPEN. */
	void open();
	/*! sets the valve state to closed.
	If we have a Physical valve then the request is sent to EPICSValveInterface to close the valve.
	If we have a Virtual valve then the request is sent to EPICSValveInterface to close the valve.
	If we have an Offline valve then the valve state variable is simply set to CLOSED. */
	void close();

	HardwareState getSettings() const  override;
	boost::python::dict getSettings_Py() const override;

	/*! directly sets the valveState variable.
	This is mainly called by the EPICSValveInterface when updating the valveState from EPICS callback function
	@param[in] state the state which we wish to set the valve to (OPEN,CLOSED,ERR)
	*/
	void setValveState(const STATE& state);
	/*! retrieves the valveState variable for this Valve object
	@param [out] valveState the current state of the Valve object (OPEN,CLOSED,ERR) */
	STATE getValveState() const;
	/*! checks whether the current valveState is open. 
	@param[out] bool true if valveState is open, false if valveState is CLOSED or ERR */
	bool isOpen() const;
	/*! checks whether the current valveState is closed.
	@param[out] bool true if valveState is closed, false if valveState is OPEN or ERR */
	bool isClosed() const;
	/*! sets the valveState directly without EPICS connection for use with Offline Valves
	@param[in] state the state we wish to set the Offline valve to (OPEN,CLOSED,ERR). */
	void offlineSetValveState(STATE state);
	/*! turns debug messaging on for this Valve instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this Valve instance*/
	void debugMessagesOff();
	/*! turns messaging on for this Valve instance*/
	void messagesOn();
	/*! turns messaging off for this Valve instance*/
	void messagesOff();
	friend class EPICSValveInterface;
protected:
	/*! vector containing aliases of this Valve instance*/
	std::vector<std::string> aliases;
	/*! storage variable for current state of Valve (OPEN, CLOSED, ERR) and the last timestamp retrieved from EPICS.*/
	std::pair<epicsTimeStamp, STATE> valveState;

};

/**\copydoc Hardware*/
/**@}*/


#endif //VALVE_H_