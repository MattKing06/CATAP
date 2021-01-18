#ifndef LLRF_H_
#define LLRF_H_
#include <Hardware.h>
#include <EPICSLLRFInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include "PythonTypeConversions.h"
#include <boost/make_shared.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/circular_buffer.hpp>
#include <utility> 
#include <boost/shared_ptr.hpp>

class EPICSLLRFInterface;
typedef boost::shared_ptr<EPICSLLRFInterface> EPICSLLRFInterface_sptr;


class LLRFInterlock
{
public:
	LLRFInterlock();
	LLRFInterlock(const LLRFInterlock& copy_trace_data);
	~LLRFInterlock();
	
	std::pair<epicsTimeStamp, double > u_level;
	std::pair<epicsTimeStamp, double > p_level;
	std::pair<epicsTimeStamp, double > pdbm_level;

	std::pair<epicsTimeStamp, bool > status;
	std::pair<epicsTimeStamp, bool > enable;
};

class TraceData
{
	public:
		TraceData();
		TraceData(const TraceData& copy_trace_data);
		~TraceData();
		// name of trace, e.g. LRRG_KLYSTRON_FORWARD_POWER
		std::string name;
		/*   __   __                    __                ___  __        __   ___  __
		    |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
			|  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/
			
		// rolling sum is the sum of the traces to be averaged,
		// when a new trace arrives it adds it to rolling_sum and subtracts traces[sub_trace]
		// rolling_average is then calculated by dividing rolling_sum by average_size
		*/
		//bool keep_rolling_average, has_average;
		//size_t rolling_average_size, rolling_average_count;
		//std::vector<double> rolling_average, rolling_sum, rolling_max, rolling_min, rolling_sd;
		//std::vector<std::vector<double>> average_trace_values;
		/*
			all the trace data goes here (online data)
		*/
		boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> trace_data_buffer;
		size_t trace_data_buffer_size;
		std::vector<double> trace_data;
		// max value in trace
		double trace_max;
		// traces indices over which to calculate trace_cut_mean
		//size_t mean_start_index, mean_stop_index;
		
		std::pair<size_t, size_t> mean_start_stop;
		std::pair<double, double> mean_start_stop_time;
		
		size_t  stop_minus_start;
		
		// mean of values between  mean_start_index, mean_stop_index
		double trace_cut_mean;

		// TODO if we make this a "const size_t  trace_data_size;" it breaks the copy constructor 
		size_t  trace_data_size;
		// the state of the scan and acqm 
		std::pair<epicsTimeStamp, STATE> scan;
		std::pair<epicsTimeStamp, STATE> acqm;
		
		bool interlock_state;
};



class LLRF : public Hardware
{
public:
	LLRF();
	LLRF(const std::map<std::string, std::string>& paramMap, const  STATE mode);
	LLRF(const LLRF& copyLLRF);
	~LLRF();
	
	/*! Both the HRRG and LRRG use the same LLRF box, the main difference is which channels are used for traces 
		therefore this needs to be set by the LLRFFactory  based on passed in user settings. 
		A similar scheme may be required if other LLRF boxes support mulitple RF structures 
	@param[in] TYPE, new vlaue to set (currenlty should ONLY be HRRG or LRRG) 
	*/
	void setGunType(const TYPE area); // called from factory
	/*! When the machein area has been correctly set we cna set up the trace data correctly. 
	Diferent channels numbers refer to different traces. This cannot be done easily   */
	void setupTraceChannels(const std::map<std::string, std::string>& paramMap);
	void setPVStructs();
	/*! For debugging to see if maps and trace names get set correctly */
	void printSetupData()const;


	// TODO private ?? 
	EPICSLLRFInterface_sptr epicsInterface;
	std::map<std::string, std::string> LLRFParamMap;

/*! set the LLRF amplitude in LLRF units 		
	@param[in] value, new value to be set 
	@param[out] bool, if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setAmp(double value);
/*! set the LLRF amplitude in MW units
	@param[in] value, new value to be set
	@param[out] bool, if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setAmpMW(double value);
	/*! get the LLRF amplitude in LLRF units
		@param[out] value, current value	*/
	double getAmp()const;
	/*! get the LLRF amplitude in MW units
		@param[out] value, current value	*/
	double getAmpMW()const;

	// phases 
/*! set the LLRF phase in LLRF units
	@param[in] value, new value to be set
	@param[out] bool, if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setPhi(double value);
	/*! set the LLRF phase in Degree units
		@param[in] value, new value to be set
		@param[out] bool, if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setPhiDEG(double value);
	/*! set the crest phase for this LLRF object (does not actually set any values in EPICS)
		@param[in] value, new value to be set
		@param[out] bool, if the value got set 	*/
	bool setCrestPhase(double value);
	/*! set the operating phase FOR this LLRF object (does not actually set any values in EPICS)
		@param[in] value, new value to be set
		@param[out] bool, if the value got set */
	bool setOperatingPhase(double value);

	/*! get the LLRF phase in LLRF units
		@param[out] value*/
	double getPhi()const;
	/*! get the LLRF phase in degree units
		@param[out] value*/
	double getPhiDEG()const;
	/*! get the LLRF crest phase  (atm needs to be set by user)
		@param[out] value*/
	double getCrestPhase()const;
	/*! get the LLRF operating phase (atm needs to be set by user)
		@param[out] value*/
	double getOperatingPhase()const;

	/*! get the name alises for this LLRF
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases() const;
	/*! get the name alises for this LLRF (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py() const;


	/*! get the index closest to for the passed time on a LLRF trace 
		@param[in] time, time in ms to convert to an index 
		@param[out] value, closest index to that time */
	size_t getIndex(const double time) const;
	/*! get the time for the passed index on a LLRF trace
		@param[in] index, index to convert to a time
		@param[out] time, time (ms) for the passed index */
	double getTime(const size_t index) const;

	/*! get all the trace data for this LLRF structure (calls a ca_array_get once, to reduce network traffic does not start monitoring of all traces)
	@param[out] map<string, vector<double>>, */
	std::map<std::string, std::vector<double>> getNewTraceData();
	/*! Start trace monitoring (please think about if you reallly need to monitor all traces)
	@param[out] if the command got sent to epics  (not if setting that value was successfull!) */
	void startTraceMonitoring()
	/*! stop the trace monitoring for this object (can significantly reduce network traffic)
	@param[out] bool, if the monitoring was succesfully stopped */
	bool stopTraceMonitoring();

	/*! get a map of all the trace data 
		@param[out] trace data, map of "trace_name" (string) and "trace_data_values" (vector of doubles) */
	std::map<std::string, std::vector<double>> getAllTraceData()const;
	/*! get a pair of a trace_name : trace_data for 1 trace
		@param[in] name, trace name to get data for 
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
	std::pair<std::string, std::vector<double>> getTraceData(const std::string& name)const;
	/*! get a trace_data values for requested trace
		@param[in] name, trace name to get data for
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getTraceValues(const std::string& name)const;
	/*! get a cavity reverse power trace_data 
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavRevPwr()const;
	/*! get a cavity forward power trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavFwdPwr()const;
	/*! get a klystron reverse power trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyRevPwr()const;
	/*! get a klystron forward power trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyFwdPwr()const;
	/*! get a cavity reverse phase trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavRevPha()const;
	/*! get a cavity forward phase trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavFwdPha()const;
	/*! get a klystron reverse phase trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyRevPha()const;
	/*! get a klystron forward phase trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyFwdPha()const;
	/*! get a cavity probe power trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getProbePwr()const;
	/*! get a cavity probe phase trace_data
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getProbePha()const;

	/*! get a dict of all the trace data, python version 
		@param[out] trace data, dict of "trace_name" (string) and "trace_data_values" (list) */
	boost::python::dict getAllTraceData_Py();
	/*! get a dict of all the trace data for 1 trace, python version
		@param[in] name, trace name to get data for
		@param[out] trace data, dict of "trace_name" (string) and "trace_data_values" (list) */
	boost::python::dict getTraceData_Py(const std::string& name);
	/*! get a trace_data values for requested trace
		@param[in] name, trace name to get data for
		@param[out] "trace_data_values" (vector of doubles) */
	boost::python::list getTraceValues_Py(const std::string& name)const;
	boost::python::list getCavRevPwr_Py()const;
	boost::python::list getCavFwdPwr_Py()const;
	boost::python::list getKlyRevPwr_Py()const;
	boost::python::list getKlyFwdPwr_Py()const;
	boost::python::list getCavRevPha_Py()const;
	boost::python::list getCavFwdPha_Py()const;
	boost::python::list getKlyRevPha_Py()const;
	boost::python::list getKlyFwdPha_Py()const;
	boost::python::list getProbePha_Py()const;
	boost::python::list getProbePwr_Py()const;

//--------------------------------------------------------------------------------------------------
	/*  ___  __        __   ___           ___            __
		 |  |__)  /\  /  ` |__      |\/| |__   /\  |\ | /__`
		 |  |  \ /~~\ \__, |___     |  | |___ /~~\ | \| .__/
	*/
	bool setMeanStartEndTime(const double start, const double end, const std::string& name);
	bool setMeanStartEndIndex(const size_t start, const size_t end, const std::string& name);
	
	std::pair<size_t, size_t> getMeanStartEndIndex(const std::string& name) const;
	std::pair<double, double> getMeanStartEndTime(const std::string& name) const;
	boost::python::list getMeanStartEndIndex_Py(const std::string& name) const;
	boost::python::list  getMeanStartEndTime_Py(const std::string& name) const;
		
	std::map<std::string, std::pair<size_t, size_t>> getTraceMeanIndices()const;
	std::map<std::string, std::pair<double, double>> getTraceMeanTimes()const;
	boost::python::dict getTraceMeanIndices_Py()const;
	boost::python::dict getTraceMeanTimes_Py()const;
	
	void setTraceMeanIndices(const std::map<std::string, std::pair<size_t, size_t>>& settings);
	void setTraceMeanTimes(const std::map<std::string, std::pair<double, double>>& settings);
	void setTraceMeanIndices_Py(const boost::python::dict& settings);
	void setTraceMeanTimes_Py(const boost::python::dict& settings);
	
	
	bool setMeanStartIndex(const std::string& name, size_t  value);
	bool setMeanStopIndex(const std::string& name, size_t  value);



	size_t getMeanStartIndex(const std::string& name)const;
	size_t getMeanStopIndex(const std::string& name)const;
	//double getMean(const std::string& name)const;
	double getCutMean(const std::string& name)const;
	std::map<std::string, double> getPowerCutMean()const;
	boost::python::dict getPowerCutMean_Py()const;

	// don;t use anymore ... 
	double getMeanStartTime(const std::string& name)const;
	double getMeanStopTime(const std::string& name)const;

	std::map<std::string, std::string> getLLRFStateMap()const;

	bool setTraceDataBufferSize(const std::string& name, const size_t new_size);


	/*! Set the current waveform as the pulse shape to be used 
	@param[out] bool, true if command got sent to epics, not a gaurantee the setting was accepted  */
	bool applyPulseShape();
	/*! Load the passed waveform as the pulse shape to be used (does not apply it)
	@param[in] vector<double>, waveform for new pulse shape
	@param[out] bool, true if command got sent to epics, not a gaurantee the setting was accepted  */
	bool setPulseShape(std::vector<double>& values);
	/*! Load the passed waveform as the pulse shape to be used (does not apply it), Python version
	@param[in] list, waveform for new pulse shape
	@param[out] bool, true if command got sent to epics, not a gaurantee the setting was accepted  */
	bool setPulseShape_Py(boost::python::list& values);
	/*! Load the passed waveform as the pulse shape to be used and apply it 
	@param[in]  vector<double>, waveform for new pulse shape
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setAndApplyPulseShape(std::vector<double>& values);
	/*! Load the passed waveform as the pulse shape to be used and apply it, Python version 
	@param[in] list, waveform for new pulse shape
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setAndApplyPulseShape(boost::python::list& values);


//--------------------------------------------------------------------------------------------------
	/*! Get the ACQM STATE for each trace data
	@param[out] map<string, STATE>, values keyed by the trace name */
	std::map<std::string, STATE> getAllTraceACQM()const;
	/*! Get the ACQM STATE for each trace data, Python version 
	@param[out] dict, values keyed by the trace name */
	boost::python::dict getAllTraceACQM_Py()const;
	/*! Get the SCAN STATE for each trace data
	@param[out] map<string, STATE>, values keyed by the trace name */
	std::map<std::string, STATE> getAllTraceSCAN()const;
	/*! Get the SCAN STATE for each trace data, Python version
	@param[out] dict, values keyed by the trace name */
	boost::python::dict getAllTraceSCAN_Py()const;
	
	/*! Set the SCAN for a trace 
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setSCAN(const std::string& trace, STATE new_scan);
	/*! Set the ACQM for a trace
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setACQM(const std::string& trace, STATE new_acqm);



	/*! Set every trace SCAN to passive 
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setAllSCANPassive();
	
	/*! Set every main trace SCAN to passive (main traces are the phase and power remote 
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setMainSCANPassive();
	/*! Set every main trace SCAN to 10 Hz (main traces are the phase and power remote
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setMainSCAN10Hz();

	/*! Helper function to convert the SCAN STATE enum to the actual number used by EPICS 
	@param[in] scan, STATE vlaue to convert 
	@param[out] int, number to pass to EPCIS for this scan */
	int convert_SCAN_to_EPICS_Number(STATE scan) const;
	/*! Helper funciton to convert the ACQM STATE enum to the actual number used by EPICS
	@param[in] scan, STATE vlaue to convert
	@param[out] int, number to pass to EPCIS for this acqm */
	int convert_ACQM_to_EPICS_Number(STATE scan) const;



	       // bool setTraceSCAN(const std::string& name, const llrfStructs::LLRF_PV_TYPE pv, const llrfStructs::LLRF_SCAN value);
        //bool setAllSCANToPassive();
        //bool setPowerRemoteTraceSCAN10sec(const std::string& name);


        //bool setAllTraceSCAN(const llrfStructs::LLRF_SCAN value);
        //bool setTORSCANToIOIntr();
        //bool setTORSCANToPassive();
        //bool resetTORSCANToIOIntr();
        //int getTORSCAN()const;
        //void updateSCAN(const std::string& name, const llrfStructs::LLRF_PV_TYPE pv, const event_handler_args& args);

        //bool setTORACQMEvent();
        //int getTORACQM()const;


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();

	friend class EPICSLLRFInterface;
	friend class LLRFFactory;
protected:

	/*! latest amp_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > amp_sp;
	/*! latest amp_ff value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > amp_ff;
	/*! latest phi_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > phi_sp;
	/*! latest phi_ff value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > phi_ff;


	/*! maximum amp_sp that can be applied (set in the main control system, NOT through CATAP)*/
	std::pair<epicsTimeStamp, double > max_amp_sp;

	/*! latest phi_ff value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, std::vector< double>> pulse_shape;


	/*! pulse duration from the LLRF, !!warning!! BUT it is not accurate if the pulse shape applied is not a square wave */
	std::pair<epicsTimeStamp, double > llrf_pulse_duration;

	/*! pulse offset */
	std::pair<epicsTimeStamp, double > pulse_offset;

	/*! RF PSS Heartbeat, when in RF mode this signal must keep changing to keep the RF system alive */
	std::pair<epicsTimeStamp, double > heartbeat;


	/*! latest amp_MW value and epicstimestamp (should be Klystron Fowrad Power in MW at beam time in pulse */
	std::pair<epicsTimeStamp, double > amp_MW;
	/*! latest phi_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > phi_degrees;
	
	
	
	/*! State of the LLRF trigger, value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, STATE> trig_state;

	/*! State of the top level LLRF interlock, value and epicstimestamp */
	std::pair<epicsTimeStamp, STATE> interlock_state;

	/*! State of the top level LLRF RF output, value and epicstimestamp */
	std::pair<epicsTimeStamp, bool> rf_output;
	
	/*! Flag for state of ff_phase_lock checkbox, value and epicstimestamp */
	std::pair<epicsTimeStamp, bool> ff_phase_lock;

	/*! Flag for state of ff_amp_lock checkbox, value and epicstimestamp */
	std::pair<epicsTimeStamp, bool> ff_amp_lock;
	
	/*! Map of TraceData objects keyed by the trace name (klystron forward power etc).
	Each physical llrf box has more traces avialable than we typically monitor,
	Detailed trace data is only kept for the main power and phase traces for this particular LLRF object.
	More can be added if needed. 	*/
	std::map<std::string, TraceData> trace_data_map;

	/*! Map of the ACQM for each trace, keyed by their generic name (channel 1, channel 2 etc)
	this data is stored for all possible traces in this LLRF box */
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_acqm;

	/*! Map of the SCAN for each trace, keyed by their generic name (channel 1, channel 2 etc)
	this data is stored for all possible traces in this LLRF box */
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_scan;

	/*! Map of the interlock states for each trace, keyed by their generic name (channel 1, channel 2 etc)
	this data is stored for all possible traces in this LLRF box */
	std::map<std::string, LLRFInterlock> all_trace_interlocks;


	/*! pointer to dbr_time_stuct, used to get timestmp for trace data ... NO ?? not sure, maybe */
	std::pair<epicsTimeStamp, struct dbr_time_long*> all_trace_data;

	// special case for the HRRG_GUN and LRRG_GUN
	//void setMachineArea(const TYPE area);
	void setDefaultPowerTraceMeanTimes();

private:
	double crest_phase;
	double operating_phase;
	/*! alternative names for the magnet (usually shorter thna the full PV root), 
		defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;


	void setMasterLatticeData(const std::map<std::string, std::string>& paramMap);

	void setTraceDataMap();
	void addToTraceDataMap(const std::string& name);
	void setTraceDataBufferSize(TraceData& trace_data, const size_t new_size);

	void updateTraceCutMeans();
	void calculateTraceCutMean(TraceData& trace);

	void scaleAllDummyTraces();
	//void scaleDummyTrace(TraceData& trace_data, const std::vector<double>& dummy_trace);
	void scaleDummyTrace(const std::string& trace_name, const std::vector<double>& dummy_trace);

	std::string fullLLRFTraceName(const std::string& name_in)const;

	void addDummyTraces(const std::map<std::string, std::string>& paramMap);
	void addDummyTrace(const std::map<std::string, std::string>& paramMap, 
		const std::string& trace_name, std::vector< double>& trace_to_update);

	/*! The time for each point on a rf trace */
	std::vector<double> time_vector;
	

	/*! Dummy Klystron Forward Power trace for use in the Virtual Machine */
	std::vector<double> kfpow_dummy_trace;
	/*! Dummy Klystron Reverse Power trace for use in the Virtual Machine */
	std::vector<double> krpow_dummy_trace;
	/*! Dummy Cavity Forward Power trace for use in the Virtual Machine */
	std::vector<double> cfpow_dummy_trace;
	/*! Dummy Cavity Reverse Power trace for use in the Virtual Machine */
	std::vector<double> crpow_dummy_trace;
	/*! Dummy Klystron Forward Phase trace for use in the Virtual Machine */
	std::vector<double> kfpha_dummy_trace;
	/*! Dummy Klystron Reverse Phase trace for use in the Virtual Machine */
	std::vector<double> krpha_dummy_trace;
	/*! Dummy Cavity Forward Phase trace for use in the Virtual Machine */
	std::vector<double> cfpha_dummy_trace;
	/*! Dummy Cavity Reverse Phase trace for use in the Virtual Machine */
	std::vector<double> crpha_dummy_trace;

	/*!Full Names for the power traces for this LLRF  */ // why do i need these??? 
	std::vector<std::string> power_trace_names;


	// TODO if we make this a "const size_t  trace_data_size;" it breaks the copy constructor ??? 
	size_t trace_data_size;
	void setTraceDataBufferSize(const size_t new_size);





	void setupInterlocks();
	void setupAllTraceACQM();
	void setupAllTraceSCAN();

	void updateInterLockStatus(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockEnable(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockU(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockP(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockPDBM(const std::string& ch, const struct event_handler_args& args);

	//void updateSCAN(const std::string& ch, const struct event_handler_args& args);
	//void updateACQM(const std::string& ch, const struct event_handler_args& args);


	void buildChannelToTraceSourceMap(const std::map<std::string, std::string>& paramMap);
	std::map<std::string, std::string> channel_to_tracesource_map;
	std::string getTraceFromChannelData(const std::string& channel_data) const;

	//std::string getTraceFromTracePVKey(const std::string& trace)const;



	/*! All the "main" channel names, main refers to the power and phase traces  */
	const std::vector<std::string> allMainChannelTraceNames{
	GlobalConstants::CH1_PWR_REM,
	GlobalConstants::CH2_PWR_REM,
	GlobalConstants::CH3_PWR_REM,
	GlobalConstants::CH4_PWR_REM,
	GlobalConstants::CH5_PWR_REM,
	GlobalConstants::CH6_PWR_REM,
	GlobalConstants::CH7_PWR_REM,
	GlobalConstants::CH8_PWR_REM,
	GlobalConstants::CH1_PHASE_REM,
	GlobalConstants::CH2_PHASE_REM,
	GlobalConstants::CH3_PHASE_REM,
	GlobalConstants::CH4_PHASE_REM,
	GlobalConstants::CH5_PHASE_REM,
	GlobalConstants::CH6_PHASE_REM,
	GlobalConstants::CH7_PHASE_REM,
	GlobalConstants::CH8_PHASE_REM };
	/*! All the "main" channel names, main refers to the power and phase traces  */
	const std::vector<std::string> allChannelTraceNames{
	GlobalConstants::CH1_AMP_DER,
	GlobalConstants::CH2_AMP_DER,
	GlobalConstants::CH3_AMP_DER,
	GlobalConstants::CH4_AMP_DER,
	GlobalConstants::CH5_AMP_DER,
	GlobalConstants::CH6_AMP_DER,
	GlobalConstants::CH7_AMP_DER,
	GlobalConstants::CH8_AMP_DER,
	GlobalConstants::CH1_PHASE_DER,
	GlobalConstants::CH2_PHASE_DER,
	GlobalConstants::CH3_PHASE_DER,
	GlobalConstants::CH4_PHASE_DER,
	GlobalConstants::CH5_PHASE_DER,
	GlobalConstants::CH6_PHASE_DER,
	GlobalConstants::CH7_PHASE_DER,
	GlobalConstants::CH8_PHASE_DER,
	GlobalConstants::CH1_PHASE_REM,
	GlobalConstants::CH2_PHASE_REM,
	GlobalConstants::CH3_PHASE_REM,
	GlobalConstants::CH4_PHASE_REM,
	GlobalConstants::CH5_PHASE_REM,
	GlobalConstants::CH6_PHASE_REM,
	GlobalConstants::CH7_PHASE_REM,
	GlobalConstants::CH8_PHASE_REM,
	GlobalConstants::CH1_PWR_LOC,
	GlobalConstants::CH2_PWR_LOC,
	GlobalConstants::CH3_PWR_LOC,
	GlobalConstants::CH4_PWR_LOC,
	GlobalConstants::CH5_PWR_LOC,
	GlobalConstants::CH6_PWR_LOC,
	GlobalConstants::CH7_PWR_LOC,
	GlobalConstants::CH8_PWR_LOC,
	GlobalConstants::CH1_PWR_REM,
	GlobalConstants::CH2_PWR_REM,
	GlobalConstants::CH3_PWR_REM,
	GlobalConstants::CH4_PWR_REM,
	GlobalConstants::CH5_PWR_REM,
	GlobalConstants::CH6_PWR_REM,
	GlobalConstants::CH7_PWR_REM,
	GlobalConstants::CH8_PWR_REM };



};


#endif //LLRF_H_