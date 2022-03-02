#ifndef LINAC_PID_H_
#define LINAC_PID_H_
#include <Hardware.h>
#include <EPICSLinacPIDInterface.h>
#include <boost/shared_ptr.hpp>
#include <GlobalConstants.h>
#include <LinacPIDPVRecords.h>
#include <vector>

class EPICSLinacPIDInterface;
typedef boost::shared_ptr<EPICSLinacPIDInterface> EPICSLinacPIDInterface_sptr;

/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control the Linac PID loop on VELA/CLARA.
	 A collection of these classes will be stored in map in the LinacPIDFactory class.
 */

class LinacPID : public Hardware
{
public:
	LinacPID();
	LinacPID(const std::map<std::string, std::string>& linacPIDParameterMap, STATE mode);
	//LinacPID(const LinacPID& copyLinacPID);
	//~LinacPID();
	EPICSLinacPIDInterface_sptr epicsInterface;
	std::map<std::string, std::string> linacPIDParameterAndValuesMap;
	void setPVStructs();
	std::vector<std::string> getAliases() const;
	/*! Attach to the initial CA Context for use in Multi-threaded applications*/
	void attachToInitialContext();
	/*! Detach from initial CA Context for use in Multi-threaded applications*/
	void detachFromInitialContext();
	/*! Set the Cavity Phase 
	*	@param[in] double, value to be set
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool setPhase(double value);
	/*! Get the Cavity Phase
	*	@param[out] double, value currently set*/
	double getPhase()const;
	/*! Set the Cavity 'Crest' Phase (Crest is the phase of maximum momentum gain) 
	*	@param[in] double, value to be set
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool setCrestPhase(double value);
	/*! Get the Cavity Phase
	*	@param[out] double, value currently set*/
	double getCrestPhase()const;
	/*! Set the Cavity 'Operating' Phase (Operating phase is the phase relative to crest !!!A positive/negative sign convention must be consistent here!!)
	*	@param[in] double, value to be set
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool setOperatingPhase(double value);
	/*! Get the Cavity 'Operating' Phase (Operating phase is the phase relative to crest !!!A positive/negative sign convention must be consistent here!!)
	*	@param[out] double, value currently set */
	double getOperatingPhase()const;
	/*! Set the Cavity Forward phase weight 
	*	@param[in] double, value to be set  
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool setForwardPhaseWeight(double value);
	/*! Get the Cavity Forward phase weight
	* @param[out] double, value */
	double getForwardPhaseWeight()const;
	/*! Set the Cavity Forward phase wrapped 
	* @param[out] double, value */
	double getForwardPhaseWrapped()const;
	/*! Set the Cavity Probe phase weight
	*	@param[in] double, value to be set
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool setProbePhaseWeight(double value);
	/*! Get the Cavity Probe phase weight
	* @param[out] double, value */
	double getProbePhaseWeight()const;
	/*! Set the Cavity Probe phase weight
	* @param[out] double, value */
	double getProbePhaseWrapped()const;
	/*! Get the current OVAL 
	* @param[out] double, value */
	double getOVAL()const;
	/*! Enable the phase control feedback 
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool enable();
	/*! Disable the phase control feedback
	*	@param[out] bool, true if value got sent to epics (not if it was received) */
	bool disable();
	/*! Is the phase control feedback enabled? 
	*	@param[out] bool, true if enabled */
	bool isEnabled()const;
	/*! Is the phase control feedback disabled?
	*	@param[out] bool, true if disabled */
	bool isDisabled()const;
	/*! Get the current state of the Phase control feedback
	*	@param[out] STATE, ENABLED, DISABLED, etc. */
	STATE getEnabledState()const;
	/*! Get the maximum phase that can be set (defined in the master lattice).
	*	@param[out] double, value */
	double getMaxPhase()const;
	/*! Get the minimum phase that can be set (defined in the master lattice).
	*	@param[out] double, value */
	double getMinPhase()const;
	/*! Get the maximum phase weight that can be set (defined in the master lattice).
	*	@param[out] double, value */
	double getMaxPhaseWeight()const;
	/*! Get the minimum phase weight that can be set (defined in the master lattice).
	*	@param[out] double, value */
	double getMinPhaseWeight()const;
	

	// TODOD i don't  think we should have generic channel numbers in the main function names, 
	// maybe linac froward and  linac probe >??? 
	//bool setCh3PhaseWeight(double value);
	//bool getCh3PhaseWeight()const;
	//bool setCh6PhaseWeight(double value);
	//bool getCh6PhaseWeight()const;
	//double getCh3PhaseWrapped()const;
	//double getCh6PhaseWrapped()const;
	// end

	/*! turns debug messaging on for this Valve instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this Valve instance*/
	void debugMessagesOff();
	/*! turns messaging on for this Valve instance*/
	void messagesOn();
	/*! turns messaging off for this Valve instance*/
	void messagesOff();
	friend class EPICSLinacPIDInterface;
protected:	
	/*
		In general we put data that comes back from EPCIS into a timestamp, data pair.
		This is because we expect to be able to use the time stamps to synchonize different data streams
	*/
	/*! Absolute Phase. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > phase;
	/*! Crest Phase. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > crest_phase;
	/*! Operating Phase. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > operating_phase;
	/*! Forward Phase Wrapped . Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > forward_phase_wrapped;
	/*! Forward Phase Weight. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > forward_phase_weight;
	/*! Probe Phase Wrapped. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > probe_phase_wrapped;
	/*! Probe Phase Weight. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > probe_phase_weight;
	/*! OVAL. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > oval;

	/*! State of the phase controller, e.g. ENABLED, DISABLED.	*/
	std::pair<epicsTimeStamp, unsigned short > scan;
private:
	/*! Name aliased for this hardwareobject.	*/ // TODO aliases should to be in hardware base class ??!! 
	std::vector<std::string> aliases;

	/*! This map will link the LLRF channel number with the "type of trace" (similar to all LLRF interfaces). It is defined from the master lattice	*/
	std::map<std::string, size_t> usefull_name_to_channel_number; 

	/*! Which channel in the LLRF is the Cavity Forward (defined in the master lattice).*/
	size_t forward_channel;
	/*! Which channel in the LLRF is the Cavity probe (defined in the master lattice).*/
	size_t probe_channel;
	
	/*! Maximum value of the phase weight.*/ 
	double max_phase;
	/*! Minimum value of the phase weight.*/
	double min_phase;
	/*! Minimum value of the phase weight.*/
	double min_phase_weight;
	/*! Maximum value of the phase weight.*/
	double max_phase_weight;

	/*! The scan parameter as a STATE enum .*/
	STATE scan_enum;
	STATE enable_state;

	// FROM MASTER LATTICE
	STATE ml_enable_state;
	STATE ml_disable_state;
	unsigned short ml_enable_number;
	unsigned short ml_disable_number;


};

/**\copydoc Hardware*/
/**@}*/

#endif // !LINAC_PID_H_