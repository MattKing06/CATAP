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


	// TODO move to private write setter/getter ??? 
	bool hasBeenSetup;

	bool setup(const std::string& version);
	void populateLinacPIDMap();
	void setupChannels();
	void setMonitorStatus(pvStruct& pvStruct) const;
	LinacPID& getLinacPID(const std::string& fullLinacPIDName);
	std::string getFullName(const std::string& nameToCheck) const;
	std::vector<std::string> getAllLinacPIDNames() const;
	boost::python::list getAllLinacPIDNames_Py() const;





	/*! Set the Cavity phase
	*	@param[in] string, name of cavity to set
	*	@param[in] double, value to set
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool setPhase(const std::string& name, const double value);
	/*! Set multiple Cavity phases
	*	@param[in] map<string, double>, map of names of cavities to set and the values to apply
	*	@param[out] bool, true if all values got sent to epics (not if it was received) */
	bool setPhase(const std::map<std::string, double>& name_value_map);
	/*! Set multiple Cavity phases, Python version 
	*	@param[in] dict, dictionary of names of cavities to set and the values to apply
	*	@param[out] bool, true if all values got sent to epics (not if it was received) */
	bool setPhase_Py(const boost::python::dict& name_value_map);
	/*! Get the Cavity phase 
	*	@param[in] string, name of cavity to get 
	*	@param[out] double, value */
	double getPhase(const std::string& name)const;
	/*! Get the phase for each cavity in teh factory
	*	@param[out] map<string, double>, map of phase values, keyed by the cavity names */
	std::map<std::string, double> getPhase()const;
	/*! Get the phase for each cavity in teh factory, Python version 
	*	@param[out] dict, dictionary of phase values, keyed by the cavity names */
	boost::python::dict getPhase_Py()const;


	/*! Set the Cavity Forward phase wrapped
	*	@param[in] string, name of cavity to set
	*	@param[out] double, value */
	bool setForwardPhaseWeight(const std::string& name, double value);

	/*! Get the Cavity Forward phase weight
	* 	@param[in] string, name of cavity to get
	* @param[out] double, value */
	double getForwardPhaseWeight(const std::string& name)const;
	/*! Get the Forward phase weights for each cavity
	* @param[out] map, map of forward phase weights, keyed by the cavity name */
	std::map<std::string, double> getForwardPhaseWeight()const;
	/*! Get the Forward phase weights for each cavity, Python version 
	* @param[out] dict, dictionary of forward phase weights, keyed by the cavity name */
	boost::python::dict getForwardPhaseWeight_Py()const;


	/*! Get the Cavity Forward Phase Wrapped
	*	@param[in] string, name of cavity to get
	*	@param[out] double, value */
	double getForwardPhaseWrapped(const std::string& name)const;
	/*! Get the Forward phase wrapped values for each cavity
	* @param[out] map, map of forward phase weights, keyed by the cavity name */
	std::map<std::string, double> getForwardPhaseWrapped()const;
	/*! Get the Forward phase wrapped values for each cavity, Python version 
	* @param[out] dict, dictionary of forward phase weights, keyed by the cavity name */
	boost::python::dict getForwardPhaseWrapped_Py()const;
	
	
	
	/*! Set the Cavity Probe phase weight
	*	@param[in] string, name of cavity to set 
	*	@param[in] double, value to be set
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool setProbePhaseWeight(const std::string& name, double value);
	/*! Get the Cavity Probe phase weight
	*	@param[in] string, name of cavity to get value for
	* @param[out] double, value */
	double getProbePhaseWeight(const std::string& name)const;
	/*! Get the Probe phase weight values for each cavity
	* @param[out] map, map of forward phase weights, keyed by the cavity name */
	std::map<std::string, double> getProbePhaseWeight()const;
	/*! Get the Probe phase weight values for each cavity, Python version
	* @param[out] dict, dictionary of forward phase weights, keyed by the cavity name */
	boost::python::dict  getProbePhaseWeight_Py(const std::string& name)const;
	
	/*! Set the Cavity Probe phase wrapped value 
	* 	@param[in] string, name of cavity to get value for
	* @param[out] double, value */
	double getProbePhaseWrapped(const std::string& name)const;
	/*! Get the Cavity Probe phase wrapped values for each cavity
	* @param[out] map, map of forward phase weights, keyed by the cavity name */
	std::map<std::string, double> getProbePhaseWrapped()const;
	/*! Get the Cavity Probe phase wrapped values for each cavity, Python version
	* @param[out] dict, dictionary of forward phase weights, keyed by the cavity name */
	boost::python::dict  getProbePhaseWrapped_Py()const;

	/*! Get the current OVAL
	* 	@param[in] string, name of cavity to get value for
	* @param[out] double, value */
	double getOVAL(const std::string& name)const;
	/*! Get the current OVAL for all cavities
	* @param[out] double, value */
	std::map<std::string, double> getOVAL()const;
	/*! Get the current OVAL for all cavities, Python version 
	* @param[out] double, value */
	boost::python::dict  getOVAL_Py()const;

	/*! Enable the phase control feedback
	* 	@param[in] string, name of cavity to enable
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool enable(const std::string& name);
	/*! Enable all the phase control feedback
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool enableAll();


	/*! Disable the phase control feedback
	* 	@param[in] string, name of cavity to get value for
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool disable(const std::string& name);
	/*! Disable all the phase control feedback
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool disableAll();



	/*! Is the phase control feedback enabled?
	* 	@param[in] string, name of cavity to get value for
	*	@param[out] bool, true if enabled */
	bool isEnabled(const std::string& name)const;
	/*! Is the phase control feedback disabled?
	* 	@param[in] string, name of cavity to get value for
	*	@param[out] bool, true if disabled */
	bool isDisabled(const std::string& name)const;
	/*! Get the current state of the Phase control feedback
	* 	@param[in] string, name of cavity to get value for
	*	@param[out] STATE, ENABLED, DISABLED, etc. */
	STATE getEnabledState(const std::string& name)const;
	/*! Get the current state of the Phase control feedback for each cavity
	* @param[out] map, map of forward phase weights, keyed by the cavity name */
	std::map<std::string, STATE> getEnabledState()const;
	/*! Get the current state of the Phase control feedback for each cavity, Python version
	* @param[out] dict, dictionary of forward phase weights, keyed by the cavity name */
	boost::python::dict  getEnabledState_Py()const;


	/*! Get the maximum phase that can be set (defined in the master lattice).
 	* 	@param[in] string, name of cavity to get value for
	*	@param[out] double, value */
	double getMaxPhase(const std::string& name)const;
	/*! Get the minimum phase that can be set (defined in the master lattice).
	* 	@param[in] string, name of cavity to get value for
	*	@param[out] double, value */
	double getMinPhase(const std::string& name)const;
	/*! Get the maximum phase weight that can be set (defined in the master lattice).
	* 	@param[in] string, name of cavity to get value for
	*	@param[out] double, value */
	double getMaxPhaseWeight(const std::string& name)const;
	/*! Get the minimum phase weight that can be set (defined in the master lattice).
	* 	@param[in] string, name of cavity to get value for
	*	@param[out] double, value */
	double getMinPhaseWeight(const std::string& name)const;
		

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


	LoggingSystem messenger;

private:
	/*! function to update map of aliases to full-names of valves
	* @param[in] valve : The Valve object we want to update the alias map with.*/
	void updateAliasNameMap(const LinacPID& linacPID);
	/*! a map that provides the link between full names and associated aliases.*/
	std::map<std::string, std::string> aliasNameMap;


	STATE mode; // TODO NOT IN HARDWARE BASE CLASS???

	std::map<std::string, LinacPID> linacPIDMap;
	ConfigReader reader;



};

#endif // !LINAC_PID_FACTORY_H_