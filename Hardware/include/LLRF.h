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

	bool status, enable;
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

		std::vector<double> trace_data;
		// max value in trace
		double trace_max;
		// traces indices over which to calculate trace_cut_mean
		//size_t mean_start_index, mean_stop_index;
		
		/*! The start and stop index over which to calculate the trace mean */
		std::pair<size_t, size_t> mean_start_stop;
		/*! The start and stop time over which to calculate the trace mean */
		std::pair<double, double> mean_start_stop_time;
	
		size_t  stop_minus_start;    
		
		// mean of values between  mean_start_index, mean_stop_index
		double trace_cut_mean;

		/*! how mamny elemnts in this trace */
		size_t  trace_data_size;
		size_t  trace_data_buffer_size;

		// 
		/*! the state of the SCAN parameter for this trace */
		std::pair<epicsTimeStamp, STATE> scan;
		/*! the state of the ACQM parameter for this trace */
		std::pair<epicsTimeStamp, STATE> acqm;

		/*! which part of the one-record trace data is this trace (defined in master lattice yaml file) */
		size_t one_record_part;
		/*! the starting index for this trace in the one-record trace data  */
		size_t one_record_start_index;
		/*! the stopping index for this trace in the one-record trace data */
		size_t one_record_stop_index;


		bool interlock_state;

};




class LLRF : public Hardware
{
public:
	LLRF();
	LLRF(const std::map<std::string, std::string>& paramMap, const  STATE mode);
	LLRF(const LLRF& copyLLRF);
	LLRF& operator=(const LLRF&) = default;
	~LLRF();
	
	/*! Both the HRRG and LRRG use the same LLRF box, the main difference is which channels are used for traces 
		therefore this needs to be set by the LLRFFactory  based on passed in user settings. 
		A similar scheme may be required if other LLRF boxes support mulitple RF structures 
	@param[in] TYPE, new vlaue to set (currenlty should ONLY be HRRG or LRRG) 
	*/
	void setGunType(const TYPE area); // called from factory
	/*! When the machine area has been correctly set we can setup the rest of this LLRF object (called from factory) */
	void setupAfterMachineAreaSet();

									  
	void setPVStructs();


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




	/*! Set the size of each TraceData circular buffer 'trace_data_buffer' (default is 5)
	@param[in] size_t, new buffer size */
	bool setAllTraceDataBufferSize(const size_t value);
	/*! Set the size of a single TraceData circular buffer 'trace_data_buffer' (default is 5)
	@param[in] size_t, name TraceData object to set the biuffer size 
	@param[in] size_t, new buffer size 
	@param[out] bool, is bugffere size equal requested size */
	bool setTraceDataBufferSize(const std::string& name, const size_t new_size);


	
	/*! get all the trace data for this LLRF structure (calls a ca_array_get once, to reduce network traffic does not start monitoring of all traces)
	@param[out] map<string, vector<double>>, */
	void updateTraceValues();
	void updateTraceValues_Py();

	void splitOneTraceValues();


	//std::map<std::string, std::vector<double>> getNewTraceData();
	/*! Start trace monitoring (please think about if you reallly need to monitor all traces)
	@param[out] if the command got sent to epics  (not if setting that value was successfull!) */
	bool startTraceMonitoring();
	/*! stop the trace monitoring for this object (can significantly reduce network traffic)
	@param[out] bool, if the monitoring was succesfully stopped */
	bool stopTraceMonitoring();
	
	/*! acquire a single shot of trace data, for all traces
	@param[out] bool, if the monitoring was succesfully stopped */
	void getTraceData();
	///*! acquire a single shot of trace data, for 1 tracetraces
	//@param[in] string, name of trace to get data for 
	//@param[out] ... TBC */
	//void getTraceData(const std::string& trace_name)
	///*! acquire a single shot of trace data, for multipel traces 
	//@param[in] vector<string>, names of traces to get data for
	//@param[out] ... TBC */
	//void getTraceData(const std::vector<std::string>& trace_names)
	///*! acquire a single shot of trace data, for all traces, Python version
	//@param[out] bool, if the monitoring was succesfully stopped */
	//void getTraceData_Py()
	///*! acquire a single shot of trace data, for 1 trace, Python version
	//@param[in] string, name of trace to get data for
	//@param[out] ... TBC */
	//void getTraceData_Py(const std::string& trace_name)
	///*! acquire a single shot of trace data, for multipel traces, Python version
	//@param[in] list, names of traces to get data for
	//@param[out] ... TBC */
	//void getTraceData_Py(const boost::python::list& trace_names)


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

	/*! latest amp_MW value and epicstimestamp (should be Klystron Fowrad Power in MW at beam time in pulse */
	std::pair<epicsTimeStamp, double > amp_MW;
	/*! latest phi_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > phi_degrees;
	
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


	/*! Size of circular buffer used to store trace data */
	size_t trace_data_buffer_size;
	
	
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
	std::map<std::string, STATE> trace_ACQM_map;
	/*! Map of the SCAN for each trace, keyed by their generic name (channel 1, channel 2 etc)
	this data is stored for all possible traces in this LLRF box */
	std::map<std::string, STATE> trace_SCAN_map;
	/*! Map of the interlock states for each trace, keyed by their generic name (channel 1, channel 2 etc)
	this data is stored for all possible traces in this LLRF box */
	std::map<std::string, LLRFInterlock> all_trace_interlocks;

	
	/*! pointer to dbr_time_stuct, used to get timestamped trace data (for all traces in the LLRF */
	struct dbr_time_double* dbr_all_trace_data;


	// special case for the HRRG_GUN and LRRG_GUN
	//void setMachineArea(const TYPE area);
	void setDefaultPowerTraceMeanTimes();

private:
	double crest_phase;
	double operating_phase;
	/*! alternative names for the LLRF (usually shorter thna the full PV root), 
		defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;


	std::vector<std::pair<size_t, std::string>> sorted_one_record_trace_start_indices;
	std::vector<std::pair<size_t, std::string>> sorted_one_record_trace_iterator_jumps;

	/*! Set up the trace_data_map for this cavity, called after the cavity TYPE has been set 
	defined in the master lattice yaml file	*/
	void setTraceDataMap();
	/*! This function actually adds a new TraceData object trace_data_map, called from setTraceDataMap */
	void addToTraceDataMap(const std::string& name);
	/*! Set the size of each TraceData circular buffer 'trace_data_buffer' (default is 5)
	This function actally does teh re-size and is private
	@param[in] TraceData, object to change the buffer size
	@param[in] size_t, new_size */
	bool setTraceDataBufferSize(TraceData& trace_data, const size_t new_size);


	void setMasterLatticeData(const std::map<std::string, std::string>& paramMap);


	bool getNewTraceValuesFromEPICS(struct dbr_time_double* dbr_struct, const pvStruct& pvs);

	
	void updateTraceCutMeans();
	void calculateTraceCutMean(TraceData& trace);

	void scaleAllDummyTraces();
	//void scaleDummyTrace(TraceData& trace_data, const std::vector<double>& dummy_trace);
	void scaleDummyTrace(const std::string& trace_name, const std::vector<double>& dummy_trace);

	std::string fullLLRFTraceName(const std::string& name_in)const;

	void addDummyTraces(const std::map<std::string, std::string>& paramMap);
	void addDummyTrace(const std::map<std::string, std::string>& paramMap, 
		const std::string& trace_name, std::vector< double>& trace_to_update);

	/*! flag for if dbr_all_trace_data poitner has been allocated enough memory */
	bool dbr_all_trace_data_not_malloced;

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

	// here we keep all the traces acqm and scan STATEs
	// actual usable traces also have their own version of this in their trace_data 
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_acqm;
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_scan;



	void setupInterlocks();

	void initAllTraceSCANandACQM();

	//void setupAllTraceACQM();
	//void setupAllTraceSCAN();

	void updateInterLockStatus(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockEnable(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockU(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockP(const std::string& ch, const struct event_handler_args& args);
	void updateInterLockPDBM(const std::string& ch, const struct event_handler_args& args);

	void updateSCAN(const std::string& ch, const struct event_handler_args& args);
	void updateACQM(const std::string& ch, const struct event_handler_args& args);


	std::map<std::string, std::string> channel_to_tracesource_map;
	std::string getTraceFromChannelData(const std::string& channel_data) const;

	//std::string getTraceFromTracePVKey(const std::string& trace)const;



	/*! All the "main" channel names, main refers to the power and phase traces  */
	std::vector<std::string> allMainChannelTraceNames;
	/*! All the "main" channel names, main refers to the power and phase traces  */
	std::vector<std::string> allChannelTraceNames;




};


#endif //LLRF_H_