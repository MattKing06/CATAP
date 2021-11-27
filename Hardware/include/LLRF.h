#ifndef LLRF_H_
#define LLRF_H_
#include <Hardware.h>
#include <RunningStats.h>
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
#include <chrono> // for the high resolution clock DAQ freq estimate 

class EPICSLLRFInterface;
typedef boost::shared_ptr<EPICSLLRFInterface> EPICSLLRFInterface_sptr;


class LLRFInterlock
{
public:
	LLRFInterlock();
	LLRFInterlock(const LLRFInterlock& copy_trace_data);
	~LLRFInterlock();
	
	std::string name;

	std::pair<epicsTimeStamp, double > u_level;
	std::pair<epicsTimeStamp, double > p_level;
	std::pair<epicsTimeStamp, double > pdbm_level;
	/*! is this interlock for a phase or a power trace */
	TYPE interlock_type;

	std::pair<epicsTimeStamp, bool > status;

	std::pair<epicsTimeStamp, bool> enable;
};

class TraceData
{
	public:
		TraceData();
		TraceData(const TraceData& copy_trace_data);
		~TraceData();
		/*! name of trace, e.g. LRRG_KLYSTRON_FORWARD_POWER */
		std::string name;
		/*! trace_values latest trace values  */
		std::vector<double> trace_values;
		/*! trace_values time stamp of the most recent trace*/
		epicsTimeStamp trace_timestamp;

		/*! how mamny elements in trace_values */
		size_t trace_data_size;
		/*! circular buffer of trace_values, latest values are at the end of the buffer  */
		boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> trace_values_buffer;
		/*! number of traces to keep in trace_values_buffer */
		size_t trace_data_buffer_size;
		/* Maximum value in latest acquired trace */
		double trace_max;
		/*! The start and stop index over which to calculate the trace mean */
		std::pair<size_t, size_t> mean_start_stop;
		/*! The start and stop time over which to calculate the trace mean */
		std::pair<double, double> mean_start_stop_time;
		/*! number of steps between start and stop index, used to calculate mean value */
		size_t stop_minus_start;    
		
		/*! mean of values between  mean_start_index, mean_stop_index */
		double trace_cut_mean;
		/*   __   __                    __                ___  __        __   ___  __
		    |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
			|  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/
			
		// rolling sum is the sum of the traces to be averaged,
		// when a new trace arrives it adds it to rolling_sum and subtracts traces[sub_trace]
		// rolling_average is then calculated by dividing rolling_sum by average_size
		*/
		/*! Flag to set if rolling averages should be calcaulted for this trace */
		bool keep_rolling_average;
		/*! True if enough rolling_average_count ==  rolling_average_size and a rolling_average has been calculated */
		bool has_average;
		/*! The number of traces to use for the rolling average  */
		size_t rolling_average_size;
		/*! The number of traces acquired for the rolling average  */
		size_t rolling_average_count;
		/*! Rolling average of the last rolling_average_count trace_values */
		std::vector<double> rolling_average;
		/*! Rolling sum of the last rolling_average_count trace_values */
		std::vector<double> rolling_sum;
		/*! rolling_average_trace_buffer */
		std::vector<std::vector<double>> rolling_average_trace_buffer;
		//std::vector<double> rolling_max;
		//std::vector<double> rolling_min;
		//std::vector<double> rolling_sd;
		//std::vector<std::vector<double>> average_trace_values;
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


		bool check_mask;


		// INTERLOCK STUFF (inl;y power traces

		/*! state of the LLRF trace interlock (interlocks are only applicable to POWER traces) */
		std::pair<epicsTimeStamp, bool> interlock_state;
		/*! u_level of the LLRF trace interlock (interlocks are only applicable to POWER traces) */
		std::pair<epicsTimeStamp, double > u_level;
		/*! p_level of the LLRF trace interlock (interlocks are only applicable to POWER traces) */
		std::pair<epicsTimeStamp, double > p_level;
		/*! pdbm_level of the LLRF trace interlock (interlocks are only applicable to POWER traces) */
		std::pair<epicsTimeStamp, double > pdbm_level;

		std::pair<epicsTimeStamp, bool> enable;

		/*! is this a PHASE or a POWER trace */
		TYPE trace_type;
};




class LLRF : public Hardware
{




public:
	LLRF();
	LLRF(const std::map<std::string, std::string>& paramMap, const  STATE mode);
	LLRF(const LLRF& copyLLRF);
	LLRF& operator=(const LLRF&) = default;
	~LLRF();
//-----------------------------------------------------------------------------------------------------------------------
/// 
///  __   ___ ___       __
/// /__` |__   |  |  | |__)
/// .__/ |___  |  \__/ |
/// 
//	After initial construction varius internal variables and settings are appl;ied depending on prime-lattice data 
//	and which RF-cavity this object is connected to. 
private:
	/*! Both the HRRG and LRRG use the same LLRF box, the main difference is which channels are used for traces 
		therefore this needs to be set by the LLRFFactory based on passed in user settings. 
		A similar scheme may be required if other LLRF boxes support mulitple RF structures 
	@param[in] set: TYPE, new vlaue to set (currently should ONLY be HRRG or LRRG) */
	void setGunType(const TYPE area); 
	/*! When the machine area has been correctly set we can setup the rest of this LLRF object (called from factory during setup). 
	It sets up the tracesdata and one-record paramters, then calls other setup functions  */
	void setupAfterMachineAreaSet();
	/*! Setup the PVStructs map for this hardware (called from factory during setup) */
	void setPVStructs();
	/*! Set up the trace_data_map for this cavity, called from setupAfterMachineAreaSet */
	void setTraceDataMap();
	/*! This function actually adds a new TraceData object trace_data_map, called from setTraceDataMap 
	@param[in] name: string given name of trace to add */
	void addToTraceDataMap(const std::string& name);
	/*! For each possible trace, (not just those that have TraceData objects) the SCNA and ACQM paramters are stored in
	all_trace_scan and all_trace_acqm */
	void initAllTraceSCANandACQM();
	/*! Map of all trace acqm for this LLRF, keyed by their generic name (channel and trace type). More typically useful named traces 
	also have their own version of this in their TraceData object */
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_acqm;
	/*! Map of all trace scan for this LLRF, keyed by their generic name (channel and trace type). More typically useful named traces
	also have their own version of this in their TraceData object */
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_scan;
	/*! READ-ONLY interlock data is stored in a map of LLRFInterlock objects, setup by this function.*/
	void setupInterlocks();
	/*! Map of all trace LLRFInterlock objects for this LLRF, keyed by their generic name (channel and trace type).*/
	std::map<std::string, LLRFInterlock> all_trace_interlocks;
	/* When connecting to a VIRTUAL machine dummy RF trace values are loaded from the prime-lattice config into each TraceData. 
	@param[in] paramMap: map<string, string> of prime-lattice config data */
	void addDummyTraces(const std::map<std::string, std::string>& paramMap);
	/* This function actually adds the dummy traces 
	@param[in] paramMap: map<string, string> of prime-lattice config data 
	@param[in] trace_name: string of the trace name to update with dummy data 
	@param[in] trace_to_update: vector<double> of the trace name to update with dummy data */
	void addDummyTrace(const std::map<std::string, std::string>& paramMap, const std::string& trace_name, std::vector< double>& trace_to_update);
	/* Default trace-cut-mean positions from the prime-lattice config file are set  */
	void setDefaultPowerTraceMeanTimes();
	/*! Map of TraceData objects keyed by the trace name (Klystron_Forward_Power etc,see LLRFPVRecords.h).
	Each physical llrf box has more traces avialable than we typically monitor. Detailed trace data is only 
	kept for the main power and phase traces for this particular LLRF object. TraceData objects know 
	"everything" about the trace, more can be added as needed.*/
	std::map<std::string, TraceData> trace_data_map;
	/*! smart pointer to epicsInterface  */
	EPICSLLRFInterface_sptr epicsInterface;
	/*! The parameter map passed in from the prime-lattice config file on construction*/
	std::map<std::string, std::string> LLRFParamMap;
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
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
/// ___  __        __   ___                         ___  __
///  |  |__)  /\  /  ` |__     \  /  /\  |    |  | |__  /__`
///  |  |  \ /~~\ \__, |___     \/  /~~\ |___ \__/ |___ .__/
/// 
/// Trace values (for power and phase) are typically acquired on demand only - this is to reduce network load, as continuous 
/// monitoring is not  required for most applications. Trace Monitoring can be enabled if needed. The data  is acquired 
/// from the demodulated vector internal to the LLRF box, ensuring each trace-data is synchronized.  
//-----------------------------------------------------------------------------------------------------------------------
public:
	/*! Start monitoring of all traces, sets is_trace_monitoring to true. (Please think about if you reallly need to monitor all traces, as monitoring can 
	significantly increase network traffic / computer load). Probably, only use this if you really need to acquire every 
	individual trace, otherwise  data can be acquired on-demand.
	@param[out] bool: if the command got sent to epics (not if setting that value was successfull!) */
	bool startTraceMonitoring();
	/*! stop the trace monitoring for this object, sets is_trace_monitoring to false. (can significantly reduce network traffic / computer load).
	@param[out] bool: if the monitoring was succesfully stopped */
	bool stopTraceMonitoring();
	/* Update the trace values "manaully" (i.e. without continuous monitoring).*/
	void updateTraceValues();
	/*! get a trace_timestamp for requested trace
	@param[in] name: string, trace name to get data for
	@param[out] pair<unsigned int, unsigned int>:> time stamp for the requested trace, 1st element is 'secPastEpoch' (seconds since 0000 Jan 1, 1990) 2nd element nanoseconds within second */ 
	std::pair<unsigned int, unsigned int> getTraceTimeStamp(const std::string & name)const;
	/*! get a trace_timestamp for requested trace, Python version
	@param[in] name: string, trace name to get data for
	@param[out] list: time stamp for the requested trace, 1st element is 'secPastEpoch' (seconds since 0000 Jan 1, 1990) 2nd element nanoseconds within second */
	boost::python::list getTraceTimeStamp_Py(const std::string & name)const;
	/*! get a pair of a trace_name : trace_values for 1 trace
	@param[in] name: trace name to get data for
	@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
	/*! get a trace_values values for requested trace
	@param[in] name: string, trace name to get data for
	@param[out] vector<double> values for the requested trace */
	std::vector<double> getTraceValues(const std::string& name)const;
	/*! get trace_values for requested trace, Python version
	@param[in] name: string, trace name to get data for
	@param[out] list, trace_values  */
	boost::python::list getTraceValues_Py(const std::string& name)const;
	/*! get a pair of a trace_name : trace_values for 1 trace
	@param[in] name: trace name to get data for
	@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
	std::pair<std::string, std::vector<double>> getTraceNameValuesPair(const std::string& name)const;
	/*!  get a dict of a trace_name : trace_values for 1 trace, Python version
	@param[in] name: trace name to get data for
	@param[out] dict, "trace_name" (string) and "trace_data_values" (list) */
	boost::python::dict getTraceNameValuesPair_Py(const std::string& name);
	/*! Get a TraceData object 
	@param[in] name: string name to get TraceData for
	@param[out] TraceData: reference to the trace data object*/
	TraceData& getTraceData(const std::string& trace_name);
	/*! Get the data from a TraceData object as a python dict. 
	@param[in] trace_name: string, name to get TraceData for
	@param[out] dict: data */
	boost::python::dict getTraceDataDict(const std::string& trace_name) const;
	/*! Get the data for all the TraceData objects as a python dict.
	@param[out] dict: data */
	boost::python::dict getAllTraceDataDict() const;
	/*! Get the latest cavity reverse power trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getCavRevPwr()const;
	/*! Get the latest cavity forward power trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getCavFwdPwr()const;
	/*! Get the latest klystron reverse power trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getKlyRevPwr()const;
	/*! Get the latest klystron forward power trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getKlyFwdPwr()const;
	/*! Get the latest cavity reverse phase trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getCavRevPha()const;
	/*! Get the latest cavity forward phase trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getCavFwdPha()const;
	/*! Get the latest klystron reverse phase trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getKlyRevPha()const;
	/*! Get the latest klystron forward phase trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getKlyFwdPha()const;
	/*! Get the latest probe power trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getProbePwr()const;
	/*! Get the latest probe phase trace_values
	@param[out] vector<double>: values  */
	std::vector<double> getProbePha()const;
	/*! Get the latest cavity reverse power trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getCavRevPwr_Py()const;
	/*! Get the latest cavity forward power trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getCavFwdPwr_Py()const;
	/*! Get the latest klystron reverse power trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getKlyRevPwr_Py()const;
	/*! Get the latest klystron forward power trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getKlyFwdPwr_Py()const;
	/*! Get the latest cavity reverse phase trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getCavRevPha_Py()const;
	/*! Get the latest cavity forward phase trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getCavFwdPha_Py()const;
	/*! Get the latest klystron reverse phase trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getKlyRevPha_Py()const;
	/*! Get the latest klystron forward phase trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getKlyFwdPha_Py()const;
	/*! Get the latest probe power trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getProbePwr_Py()const;
	/*! Get the latest probe phase trace_values, Python version 
	@param[out] vector<double>: values  */
	boost::python::list getProbePha_Py()const;


	/*! Get the latest cavity reverse power trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getCavRevPwrTS()const;
	/*! Get the latest cavity forward power trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getCavFwdPwrTS()const;
	/*! Get the latest klystron reverse power trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getKlyRevPwrTS()const;
	/*! Get the latest klystron forward power trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getKlyFwdPwrTS()const;
	/*! Get the latest cavity reverse phase trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getCavRevPhaTS()const;
	/*! Get the latest cavity forward phase trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getCavFwdPhaTS()const;
	/*! Get the latest klystron reverse phase trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getKlyRevPhaTS()const;
	/*! Get the latest klystron forward phase trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getKlyFwdPhaTS()const;
	/*! Get the latest probe power trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getProbePwrTS()const;
	/*! Get the latest probe phase trace_timestamp
	@param[out] vector<double>: values  */
	std::pair<unsigned int, unsigned int> getProbePhaTS()const;
	/*! Get the latest cavity reverse power trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getCavRevPwrTS_Py()const;
	/*! Get the latest cavity forward power trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getCavFwdPwrTS_Py()const;
	/*! Get the latest klystron reverse power trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getKlyRevPwrTS_Py()const;
	/*! Get the latest klystron forward power trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getKlyFwdPwrTS_Py()const;
	/*! Get the latest cavity reverse phase trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getCavRevPhaTS_Py()const;
	/*! Get the latest cavity forward phase trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getCavFwdPhaTS_Py()const;
	/*! Get the latest klystron reverse phase trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getKlyRevPhaTS_Py()const;
	/*! Get the latest klystron forward phase trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getKlyFwdPhaTS_Py()const;
	/*! Get the latest probe power trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getProbePwrTS_Py()const;
	/*! Get the latest probe phase trace_timestamp, Python version
	@param[out] vector<double>: values  */
	boost::python::list getProbePhaTS_Py()const;


private:
	/* Flag set for if trace monitoring is enabled.*/
	bool is_trace_monitoring;
	/*! after a new one_record_trace_value has been acquired, split it up into the correct TraceData objects
	This function is called when monitoring traces and when just getting a single trace
	@param[in] const struct dbr_time_double*, pointer to trace data */
	void splitOneTraceValues(const struct dbr_time_double* dbr);
	/*! after a new one_record_trace_value has been acquired, split it up into the correct TraceData objects
	This function is called when monitoring traces and when just getting a single trace
	@param[in] const struct dbr_time_double*, pointer to trace data */
	void scaleAllDummyTraces();
	/*! after a new one_record_trace_value has been acquired, split it up into the correct TraceData objects
	This function is called when monitoring traces and when just getting a single trace
	@param[in] const struct dbr_time_double*, pointer to trace data */
	void scaleDummyTrace(const std::string& trace_name, const std::vector<double>& dummy_trace);
	/*! Each TraceData has associated meta-data (max, mean, etc.) this function updates that when new data is acquired */
	void updateTraceMetaData();
	/*! maximum kylstron forward power from latest trace*/
	std::pair<epicsTimeStamp, double > kly_fwd_power_max;
public:
	/*! set the LLRF amplitude in LLRF units 		
	@param[in] value, new value to be set 
	@param[out] bool, if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setAmp(double value);
	/*! set the LLRF amplitude in MW units
	@param[in] value: new value to be set
	@param[out] bool: if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setAmpMW(double value);
	/*! get the LLRF amplitude in LLRF units
		@param[out] value: current value	*/
	double getAmp()const;
	/*! get the LLRF amplitude in MW units
		@param[out] value: current value	*/
	double getAmpMW()const;
	/*! set the LLRF phase in LLRF units
	@param[in] double: new value to set
	@param[out] bool: if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setPhi(double value);
	/*! set the LLRF phase in Degree units
	@param[in] double: new value to set
	@param[out] bool: if the command got sent to epics  (not if setting that value was successfull!)	*/
	bool setPhiDEG(double value);
	/*! set the crest phase for this LLRF object (does not actually set any values in EPICS)
	@param[in] double: new value to set
	@param[out] bool: if the value got set 	*/
	bool setCrestPhase(double value);
	/*! set the operating phase FOR this LLRF object (does not actually set any values in EPICS)
	@param[in] double: new value to set
	@param[out] bool: if the value got set */
	bool setOperatingPhase(double value);

	/*! get the LLRF phase in LLRF units
		@param[out] double: value*/
	double getPhi()const;
	/*! get the LLRF phase in degree units (not yet fully implemented on the physical machine) 
		@param[out] double: value*/
	double getPhiDEG()const;
	/*! get the LLRF crest phase  (atm needs to be set by user, waiting for generic control system solution in EPICS)
		@param[out] double: value*/
	double getCrestPhase()const;
	/*! get the LLRF operating phase (atm needs to be set by user)
		@param[out] double: value*/
	double getOperatingPhase()const;

	/*! get the name alises for this LLRF
	@param[out] vector<string>: vector containing  all the alias names */
	std::vector<std::string> getAliases() const;
	/*! get the name alises for this LLRF (python version)
		@param[out] list: python list containing all the alias names */
	boost::python::list getAliases_Py() const;


	std::map<std::string, std::string> channel_to_tracesource_map;
	std::string getTraceFromChannelData(const std::string& channel_data) const;

	//std::string getTraceFromTracePVKey(const std::string& trace)const;



	/*! All the "main" channel names, main refers to the power and phase traces  */
	std::vector<std::string> allMainChannelTraceNames;
	/*! All the "main" channel names, main refers to the power and phase traces  */
	std::vector<std::string> allChannelTraceNames;

	/*! get the index closest to for the passed time on a LLRF trace 
		@param[in] double: time in ms to convert to an index 
		@param[out] size_t:  closest index to that time */
	size_t getIndex(const double time) const;
	/*! get the time for the passed index on a LLRF trace
		@param[in] double: index to convert to a time
		@param[out] size_t: time (ms) for the passed index */
	double getTime(const size_t index) const;


	/*! Set the size of each TraceData circular buffer 'trace_values_buffer' (default is 5)
	@param[in] size_t, new buffer size */
	bool setAllTraceDataBufferSize(const size_t value);
	/*! Set the size of a single TraceData circular buffer 'trace_values_buffer' (default is 5)
	@param[in] size_t, name TraceData object to set the biuffer size 
	@param[in] size_t, new buffer size 
	@param[out] bool, is bugffere size equal requested size */
	bool setTraceDataBufferSize(const std::string& name, const size_t new_size);

		

	

	///*! acquire a single shot of trace data, for 1 tracetraces
	//@param[in] string, name of trace to get data for 
	//@param[out] ... TBC */
	//void getTraceData(const std::string& trace_name)+

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


	//---------------------------------------------------------------------------------------------------------
	//	TRIGGERS AND TRIGGER SOURCE
	public:
		bool isTrigOff()const;
		bool isTrigExternal()const;
		bool isTrigInternal()const;
		/* Disable the trigger 
		@param[out] success: bool, if command got sent to EPICS, (not if it was recieved or acted on!).*/
		bool trigOff();
		/* Set the trigger to Internal 
		@param[out] success: bool, if command got sent to EPICS, (not if it was recieved or acted on!).*/
		bool trigInt();
		/* Set the trigger to External
		@param[out] success: bool, if command got sent to EPICS, (not if it was recieved or acted on!).*/
		bool trigExt();
		/* Get the current trigger source (internal or external) 
		@param[out] trig source: STATE of current trigger .*/
		STATE getTrigSource()const;
	private:
		/* Set the trigger source 
		@param[in] new_state: STATE, shoud be one of STATE::INTERNAL or STATE::EXTERNAL 
		@param[out] success: STATE of current trigger .*/
		bool setTrig(const STATE new_state);
		/*! source of LLRF trigger one of: OFF, INTERNAL, EXTERNAL */
		std::pair<epicsTimeStamp, STATE > trig_source;



	//---------------------------------------------------------------------------------------------------------
	//	Interlocks
	public:
		bool setInterlockActive();
		bool setInterlockNonActive();
		STATE getInterlock()const;
		bool isInterlockActive()const;
		bool isInterlockNotActive()const;
		std::map<std::string, std::pair<std::string, std::string>> getAllInterlocks() const;
		boost::python::dict getAllInterlocks_Py() const;
	private:
		bool setInterlock(const STATE new_state);
		/*! State of the top level LLRF interlock, value and epicstimestamp */
		std::pair<epicsTimeStamp, STATE> interlock_state;

	//---------------------------------------------------------------------------------------------------------
	//	RF output
	public:
		bool isRFOutput() const;
		bool isNotRFOutput() const;
		bool RFOutput()const;
		bool enableRFOutput();
		bool disableRFOutput();
	private:
		bool setRFOutput(const bool new_state);
		/*! State of the top level LLRF RF output, value and epicstimestamp */
		std::pair<epicsTimeStamp, bool> rf_output;

	//---------------------------------------------------------------------------------------------------------
	//	amp phase locking 
	public:
		bool isAmpFFconnected();
		bool lockAmpFF();
		bool lockPhaseFF();
		bool unlockAmpFF();
		bool unlockPhaseFF();
		bool isFFLocked()const;
		bool isFFNotLocked()const;
		bool isAmpFFLocked()const;
		bool isAmpFFNotLocked()const;
		bool isPhaseFFLocked()const;
		bool isPhaseFFNotLocked()const;
	private:
		/*! Flag for state of ff_phase_lock checkbox, value and epicstimestamp */
		std::pair<epicsTimeStamp, bool> ff_phase_lock;
		/*! Flag for state of ff_amp_lock checkbox, value and epicstimestamp */
		std::pair<epicsTimeStamp, bool> ff_amp_lock;


	//---------------------------------------------------------------------------------------------------------
	//	combo RF and locking 
	public:
		bool enableRFandLock();
	private:

		/*! Size of circular buffer used to store trace data */
		size_t trace_data_buffer_size;
		
		/*! pulse duration from the LLRF, !!warning!! BUT it is not accurate if the pulse shape applied is not a square wave */
		std::pair<epicsTimeStamp, double > llrf_pulse_duration;

		/*! pulse offset */
		std::pair<epicsTimeStamp, double > pulse_offset;

		/*! RF PSS Heartbeat, when in RF mode this signal must keep changing to keep the RF system alive */
		std::pair<epicsTimeStamp, double > heartbeat;

		
		
		
		
		
		
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
/// ___  __        __   ___     __   __                    __                ___  __        __   ___  __
///  |  |__)  /\  /  ` |__     |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
///  |  |  \ /~~\ \__, |___    |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/
/// 
///	Rolling averages can be calcuated for each trace  
//-----------------------------------------------------------------------------------------------------------------------
	private:
		/* Updates the rolling average for a trace data, only called when trace monitoring is enabled 
		@param[in] TraceData: trace to update rolling average for.*/
		void updateRollingAverage(TraceData& data);
	public:
		/*! Rest all the rolling average data, clears containers, reset rollinG_avergae_count to zero,  
		(does not change values such as rolling_average_size). */
		void resetAllRollingAverage();
		/*! 
		@param[in] name: string, trace name to reset data for.
		@param[out] bool: true if rolling average data was reset. If false is returned then the passed name was not recognized. */
		bool resetRollingAverage(const std::string& name);
		/*! Sets keep_rolling_average to true for all TraceData objects */
		void setShouldKeepRollingAverage();
		/*! Sets keep_rolling_average flag to true for a particular trace 
		@param[in] name: string, trace name to set flag for.
		@param[out] bool: true if flag was set, false if the passed name was not recognized. */
		bool setShouldKeepRollingAverage(const std::string& name);
		/*! Sets keep_rolling_average to false for all TraceData objects */
		void setShouldNotKeepRollingAverage();
		/*! Sets keep_rolling_average flag to false for a particular trace
		@param[in] name: string, trace name to set flag for.
		@param[out] bool: true if flag was set, false if the passed name was not recognized. */
		bool setShouldNotKeepRollingAverage(const std::string& name);
		/*! Set keep_rolling_average flag for a named trace
		@param[in] name: string, trace name to set flag for.
		@param[in] bool: new value.
		@param[out] bool: true if flag was set, false if the passed name was not recognized. */
		bool setKeepRollingAverage(const std::string& name, bool value);
		/*! Set rolling_average_size parameter for a named trace. (How many traces to use for the rolling average).
			@param[in] name: trace name to set rolling_average_size for
			@param[in] name: string, new value.
			@param[out] bool: if value got set correctly  */
		bool setRollingAverageSize(const std::string& name, const size_t value);
		/*! Set rolling_average_size parameter for all named traces in this LLRF
			@param[in] name: string, new value.*/
		void setAllRollingAverageSize(const size_t value);
		/*! Get the rolling_average_size parameter for a named TraceData in this LLRF
			@param[in] name: string, name of trace 
			@param[out] size_t: rolling_average_size for this trace */
		size_t getRollingAverageSize(const std::string& name)const;
		/*! Get the rolling_average_count parameter for a named TraceData in this LLRF
			@param[in] name: string, name of trace
			@param[out] size_t: rolling_average_count for this trace */
		size_t getRollingAverageCount(const std::string& name)const;
		/*! Get the rolling_average for a named TraceData in this LLRF
		@param[in] name: string, name of trace
		@param[out] vector<double>: rolling_average for the requested trace. */
		std::vector<double> getRollingAverage(const std::string& name)const;
		/*! Get the rolling_average for a named TraceData in this LLRF, Python version 
		@param[in] name: string, name of trace
		@param[out] list: rolling_average for the requested trace. */
		boost::python::list getRollingAverage_Py(const std::string& name)const;
		/*! Get the rolling_average for each TraceData in this LLRF
			@param[in] dict: values of rolling_average keyed by their name*/
		boost::python::dict getRollingAverage_Py()const;
		///*! Get the rolling_average for each TraceData in this LLRF
		//@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		//std::vector<std::vector<double>> getAllRollingAverage(const std::string& name)const;
		/*! Check if a TraceData is keeping a rolling average 
		@param[in] name: string, trace name to check
		@param[out] bool: true if keep_rolling_average for the requested TraceData is true. */
		bool isKeepingRollingAverage(const std::string& name)const;
		/*! Check if value of has_average is true for a TraceData 
		@param[in] name: string, trace name to check
		@param[out] bool: true if has_average for the requested TraceData is true. */
		bool hasRollingAverage(const std::string& name)const;
		/*! Get the individual traces used to calculate the trace rolling average 
		@param[in] name: string, trace name to check
		@param[out] vector<vector<double>>, rolling_average_trace_buffer for this TraceData*/
		std::vector<std::vector<double>> getRollingAverageTraceBuffer(const std::string& name) const;
		/*! Get the individual traces used to calculate the trace rolling average, Python version 
		@param[in] name: string, trace name to check
		@param[out] list, rolling_average_trace_buffer for this TraceData*/
		boost::python::list getRollingAverageTraceBuffer_Py(const std::string& name)const;
		/*! Get the individual traces used to calculate the trace rolling average for each TraceData 
		@param[out] map<string, vector<vector<double>>>, rolling_average_trace_buffer for each TraceData, keyed by the TraceData name. */
		std::map<std::string, std::vector<std::vector<double>>> getAllRollingAverageTraceBuffer() const;
		/*! Get the individual traces used to calculate the trace rolling average for each TraceData, Python Version
		@param[out] dict, rolling_average_trace_buffer for each TraceData, keyed by the TraceData name. */
		boost::python::dict getAllRollingAverageTraceBuffer_Py()const;
		/*! Actually 'clears' and reset the values for TraceData rolling average.
		@param[in] trace: TraceData, object to reset rolling average data */
		void clearRollingAverage(TraceData& trace);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//        ___  __   __           __                         __      __  ___      ___  __
//  |__/ |__  |__) /  \ |  |    |__) |  | |\ | |\ | | |\ | / _`    /__`  |   /\   |  /__`
//  |  \ |    |    \__/ |/\|    |  \ \__/ | \| | \| | | \| \__>    .__/  |  /~~\  |  .__/
//  
//	Klystron Forward Power Running Stats. These are the running mean and variance for the KlyFwdPow
//		Note, they depend on the position of the cut_mean, (so if those indices are not set this will be potentially garbage
//		It is only updated when there is an active pulse, AND if there was no OME Detected
//  We keep the amp_set vs KLY_FWD_POWER running stat variables
//		m_n, m_oldM, m_oldS, see running_stat.h
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
	private:
	/*! Flag, when set to true the klystron forward power runnings stats are updated each RF pulse. */
		bool keep_kly_fwd_pow_running_stat;
	public:
		/*! Set the keep_kly_fwd_pow_running_stat flag. 
			@param[in] bool: value to set for keep_kly_fwd_pow_running_stat		*/
		void setKeepKlyFwdPwrRS(bool val);
		/*! Set the keep_kly_fwd_pow_running_stat flag to true. */
		void keepKlyFwdPwrRS();
		/*! Set the keep_kly_fwd_pow_running_stat flag to false. */
		void dontKeepKlyFwdPwrRS();
		/*! map of RunningStats objects fro eahc amp_set value (crocodiles) */
		std::map<int, RunningStats> amp_set_kly_fwd_rs;
		/*! Parameters defining the internal state of each amp_set_kly_fwd_rs RunningStat, required so data can be saved and re-applied. 
		they are m_n, m_oldM, m_oldS, see running_stat.h */
		std::map<int, std::tuple<size_t, double, double>> amp_set_kly_fwd_rs_state;
		/*! A dummy internal state for a RunningStat. */
		std::tuple<size_t, double, double> dummy_amp_set_kly_fwd_rs_state;
		/*! Update the amp_set_kly_fwd_rs (clled if keep_kly_fwd_pow_running_stat is true). */
		void update_amp_set_kly_fwd_rs();
		/*! Get the klystron forward power running stats state for a paticular LLRF amp_set (crocodiles). 
		* 	@param[in] ampSP_setting: int, amp_sp value to get data for 
			@param[out] tuple<size_t, double, double>, m_n, m_oldM, m_oldS, see running_stat.h   */
		std::tuple<size_t, double, double> getKlyFwdPwrRSState(int ampSP_setting);
		/*! Set the klystron forward power running stats state for a paticular LLRF amp_set (crocodiles). 
		This is used when loading in data saved to keep the histroy of the klystron_fwd_pow vs amp_sp crocodiles 
		@param[in] amp_sp: int, value to set data for
		@param[in] n: size_t, number of items the running mean and variabnce has been calculated for 
		@param[in] old_mean : double the initial value of the running mean to use
		@param[in] old_variance : double, initial value of the running variance to use */
		void setKlyFwdPwrRSState(int amp_sp, size_t n, double old_mean, double old_variance);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/// .       __  ___         ___ .     __             __   ___  __  
/// '  /\  /  `  |  | \  / |__  '    |__) |  | |    /__` |__  /__` 
///   /~~\ \__,  |  |  \/  |___      |    \__/ |___ .__/ |___ .__/ 
///                                                                
// Counts of active, inactive, duplicate and total number of pulses are kept, (defined based on the Klysytron 
// Forward Power trace data). These can also be affset by passed values to enable totals to be transferred to 
// new instances of the interface. 
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
	public:
		/*! retun active_pulse_kly_power_limit (default 500 W)
			@param[out] size_t: value */
		size_t getActivePulsePowerLimit() const;
		/*! Set active_pulse_kly_power_limit (default 500 W)
		@param[in] value: size_t */
		void setActivePulsePowerLimit(const size_t value);
		/*! retun active_pulse_count
		@param[out] value: size_t */
		size_t getActivePulseCount()const;
		/*! Set active_pulse_count to value
		@param[in] value: size_t */
		void setActivePulseCount(const size_t value);
		/*! increment active_pulse_count by value
		@param[in] value: size_t */
		void addActivePulseCountOffset(const size_t value);
		/*! retun inactive_pulse_count
		@param[out] size_t, */
		size_t getInactivePulseCount()const;
		/*! Set inactive_pulse_count to value
		@param[in] value: size_t */
		void setInactivePulseCount(const size_t value);
		/*! increment inactive_pulse_count by value
		@param[in] value: size_t */
		void addInactivePulseCountOffset(const size_t value);
		/*! retun duplicate_pulse_count
		@param[out] size_t, */
		size_t getDuplicatePulseCount()const;
		/*! Set duplicate_pulse_count to value
		@param[in] value: size_t */
		void setDuplicatePulseCount(const size_t value);
		/*! increment duplicate_pulse_count by value
		@param[in] value: size_t */
		void addDuplicatePulseCountOffset(const size_t value);
		/*! retun duplicate_pulse_count
		@param[out] size_t */
		size_t getTotalPulseCount()const;
		/*! Set total_pulse_count to value
		@param[in] value: size_t */
		void setTotalPulseCount(const size_t value);
		/*! increment total_pulse_count by value
		@param[in] value: size_t */
		void addTotalPulseCountOffset(const size_t value);
	private:
		/*! If kly_fwd_power_max is above this value can_increase_active_pulses is set to true  */
		double active_pulse_kly_power_limit;
		/*! Flag to determine if the pulse count can be increased (true if kly_fwd_power_max > active_pulse_kly_power_limit)  */
		bool can_increase_active_pulses;
		/*! kly_fwd_power_max for previous trace, this is important becasue the LLRF box can be in a state where it sends out a copy of the previous trace data,
		so this value is used from comparison. If the last vlaue is the same as the current value duplicate_pulse_count is incremented*/
		double last_kly_fwd_power_max;
		/*! Pulse count where where kly_fwd_power_max > active_pulse_kly_power_limit */
		size_t active_pulse_count;
		/*! Pulse count where  where kly_fwd_power_max < active_pulse_kly_power_limit */
		size_t inactive_pulse_count;
		/*! Pulse count where kly_fwd_power_max == last_kly_fwd_power_max */
		size_t duplicate_pulse_count;
		/*! Pulse count of any type */
		size_t total_pulse_count;
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
///	/*  ___  __        __   ___           ___            __
///		 |  |__)  /\  /  ` |__      |\/| |__   /\  |\ | /__`
///		 |  |  \ /~~\ \__, |___     |  | |___ /~~\ | \| .__/
///	*/
// Trace means between user-defined times/indexes are can be calculated for each TraceData object 
public:
	/*! Set the start/end cursor position for the trace mean calculation, in micro-seconds from the start of the pulse  
	@param[in] start: double, start time in usec 
	@param[in] end: double, end time in usec
	@param[in] name: string trace name 
	@param[out] success: bool indicating if settings got applied */
	bool setMeanStartEndTime(const double start, const double end, const std::string& name);
	/*! Set the start/end cursor position for the trace mean calculation, in from the start of the pulse
	@param[in] start: double, start index
	@param[in] end: double, end index
	@param[in] name: string trace name
	@param[out] success: bool indicating if settings got applied */
	bool setMeanStartEndIndex(const size_t start, const size_t end, const std::string& name);
	/*! Get the start/end cursor position for the trace mean calculation
	@param[in] name: string trace name 
	@param[out] values: pair<size_t, size_t, > start and end indices*/
	std::pair<size_t, size_t> getMeanStartEndIndex(const std::string& name) const;
	/*! Set the start/end cursor position for the trace mean calculation
	@param[in] name: string trace name 
	@param[out] values: pair<double, double> start and end times */
	std::pair<double, double> getMeanStartEndTime(const std::string& name) const;
	/*! Get the start/end cursor position indices for the trace mean calculation, Python Version
	@param[in] name: string trace name 
	@param[out] values: list start and end indices */
	boost::python::list getMeanStartEndIndex_Py(const std::string& name) const;
	/*! Get the start/end cursor position times for the trace mean calculation, Python Version
	@param[out] values: list start and end times (micro-sec) */
	boost::python::list  getMeanStartEndTime_Py(const std::string& name) const;
	/*! Get the start/end cursor position indices for the trace mean calculation fro every trace
	@param[out] values: map<string, <pair, size_t, size_t>> start and end indices, keyed by trace name  */
	std::map<std::string, std::pair<size_t, size_t>> getTraceMeanIndices()const;
	/*! Get the start/end cursor position times for the trace mean calculation for every trace
	@param[out] values: map<string, <pair, size_t, size_t>> start and end times (micro-sec), keyed by trace name  */
	std::map<std::string, std::pair<double, double>> getTraceMeanTimes()const;
	/*! Get the start/end cursor position times for the trace mean calculation for every trace
	@param[out] values: dict start and end indices, keyed by trace name  */
	boost::python::dict getTraceMeanIndices_Py()const;
	/*! Get the start/end cursor position times for the trace mean calculation for every trace
	@param[out] values: dict start and end times (micro-sec), keyed by trace name  */
	boost::python::dict getTraceMeanTimes_Py()const;
	/*! Set the start/end cursor position indices for the trace mean calculations for multiple traces
	@param[in] settings: map<string, <pair, size_t, size_t>> start and end indices, keyed by trace name  */
	void setTraceMeanIndices(const std::map<std::string, std::pair<size_t, size_t>>& settings);
	/*! Set the start/end cursor position times for the trace mean calculations for multiple traces
	@param[in] settings: map<string, <pair, double, double>> start and end times, keyed by trace name  */
	void setTraceMeanTimes(const std::map<std::string, std::pair<double, double>>& settings);
	/*! Set the start/end cursor position indices for the trace mean calculations for multiple traces, Python version 
	@param[in] settings: dict start and end indices, keyed by trace name  */
	void setTraceMeanIndices_Py(const boost::python::dict& settings);
	/*! Set the start/end cursor position indices for the trace mean calculations for multiple traces, Python version
	@param[in] settings: dict start and end times (micro-sec), keyed by trace name  */
	void setTraceMeanTimes_Py(const boost::python::dict& settings);
	
	/*! Set the start cursor position index for the trace mean calculation 
	@param[in] name: string trace name to set values for  
	@param[in] value: size_t start index 
	@param[out] success: bool if requested trace's value was set */
	bool setMeanStartIndex(const std::string& name, size_t  value);
	/*! Set the stop cursor position index for the trace mean calculation
	@param[in] name: string trace name to set values for
	@param[in] value: size_t start index
	@param[out] success: bool if requested trace's value was set */
	bool setMeanStopIndex(const std::string& name, size_t  value);
	/*! Get the start cursor position index for the trace mean calculation
	@param[in] name: string trace name to set values for
	@param[out] value: size_t start index */
	size_t getMeanStartIndex(const std::string& name)const;
	/*! Get the stop cursor position index for the trace mean calculation
	@param[in] name: string trace name to get values for
	@param[out] value: size_t start index */
	size_t getMeanStopIndex(const std::string& name)const;
	/*! Get the start time for the trace mean calculation
	@param[in] name: string trace name to get values for
	@param[out] value: double, start time (micro-sec) */
	double getMeanStartTime(const std::string& name)const;
	/*! Get the stop time for the trace mean calculation
	@param[in] name: string trace name to get values for
	@param[out] value: double, stop time (micro-sec) */
	double getMeanStopTime(const std::string& name)const;
	/*! Get the mean value of the a trace between the start and stop positions 
	@param[in] name: string trace name to get values for
	@param[out] value: size_t start index */
	double getCutMean(const std::string& name)const;
	/*! Get the mean value of the all power traces between the start and stop positions
	@param[out] values: mpa<string, double> values keyed by the trace name */
	std::map<std::string, double> getPowerCutMean()const;
	/*! Get the mean value of the all power traces between the start and stop positions, Python version 
	@param[out] values: dict values keyed by the trace name */
	boost::python::dict getPowerCutMean_Py()const;
	/*! Get the mean value of the all traces between the start and stop positions
	@param[out] values: map<string, double> values keyed by the trace name */
	std::map<std::string, double> getAllCutMean()const;
	/*! Get the mean value of the all traces between the start and stop positions
	@param[out] values: dict values keyed by the trace name */
	boost::python::dict getAllCutMean_Py()const;






	std::map<std::string, std::string> getLLRFStateMap()const;



	// DAQ freq getters / settings 
	void setNumTracesToEstimateRepRate(size_t value);
	size_t getNumTracesToEstimateRepRate() const;
	double getTraceRepRate() const;


	/*! Set the current waveform as the pulse shape to be used 
	@param[out] bool, true if command got sent to epics, not a gaurantee the setting was accepted  */
	bool applyPulseShape();
	/*! Load the passed waveform as the pulse shape to be used (does not apply it)
	@param[in] vector<double>, waveform for new pulse shape
	@param[out] bool, true if command got sent to epics, not a gaurantee the setting was accepted  */
	bool setPulseShape(const std::vector<double>& values);
	/*! Load the passed waveform as the pulse shape to be used (does not apply it), Python version
	@param[in] list, waveform for new pulse shape
	@param[out] bool, true if command got sent to epics, not a gaurantee the setting was accepted  */
	bool setPulseShape_Py(const boost::python::list& values);
	/*! Load the passed waveform as the pulse shape to be used and apply it 
	@param[in]  vector<double>, waveform for new pulse shape
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setAndApplyPulseShape(const std::vector<double>& values);
	/*! Load the passed waveform as the pulse shape to be used and apply it, Python version 
	@param[in] list, waveform for new pulse shape
	@param[out] bool, true if commands got sent to epics, not a gaurantee the setting was accepted  */
	bool setAndApplyPulseShape(const boost::python::list& values);



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
	/*! Maximum amplitude that can be set by CATAP (set in prime-alltice files) */
	double catap_max_amplitude;
	/*! Maximum amplitude that can be set by control system (set in control ssyetm IOC */
	double ioc_max_amplitude;

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



	



	

	
	///*! Map of TraceData objects keyed by the trace name (klystron forward power etc).
	//Each physical llrf box has more traces avialable than we typically monitor,
	//Detailed trace data is only kept for the main power and phase traces for this particular LLRF object.
	//TraceData objects know "everything" about the trace
	//More can be added if needed. 	*/
	//std::map<std::string, TraceData> trace_data_map;
	///*! Map of the ACQM for each trace, keyed by their generic name (channel 1, channel 2 etc)
	//this data is stored for all possible traces in this LLRF box */
	//std::map<std::string, STATE> trace_ACQM_map;
	///*! Map of the SCAN for each trace, keyed by their generic name (channel 1, channel 2 etc)
	//this data is stored for all possible traces in this LLRF box */
	//std::map<std::string, STATE> trace_SCAN_map;


	
	/*! pointer to dbr_time_stuct, used to get timestamped trace data (for all traces in the LLRF */
	struct dbr_time_double* dbr_all_trace_data;


	bool getTraceFromChannelData(const std::string& channel, std::string& string_to_put_trace_name_in);


public:
	/*! Return the "full name" of a LLRF trace known by this hardware. 
	@param[ins] string, name to check 
	@param[out] string, name */
	std::string fullLLRFTraceName(const std::string& name_in)const;
	/*! Return the full names of the LLRF traces known by this hardware,
	@param[out] vector<string>, names */
	std::vector<std::string> getAllLLRFTraceNames()const;
	/*! Return the full names of the LLRF traces known by this hardware, Python version 
	@param[out] list, names */
	boost::python::list getAllLLRFTraceNames_Py()const;


	bool setAllSCANTo(STATE new_state);
	bool setAllSCANToPassive();
	bool setAllSCANToIoIntr();
	//bool setPowerRemoteScanto10secs();



	/*! TOR is trace-One_record !!!  */
	//bool resetTORSCANToIOIntr();
	//bool setTORSCANToPassive();
	//bool setTORSCANToIOIntr();
	bool setTORACQMEvent();
	STATE getTORACQM()const;
	STATE getTORSCAN()const;

	std::map<std::string, STATE> getAllTraceSCAN()const;
	boost::python::dict getAllTraceSCAN_Py()const;

	std::map<std::string, STATE> getAllTraceACQM()const;
	boost::python::dict getAllTraceACQM_Py()const;

private:
	double crest_phase;
	double operating_phase;
	/*! alternative names for the LLRF (usually shorter thna the full PV root), 
		defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;


	TraceData dummy_trace_data;

	/*! Contains datagiving the start index in the one-record trace for each trace name */
	std::vector<std::pair<size_t, std::string>> sorted_one_record_trace_start_indices;
	/*! Contains data giving the jump to the next starting position for for each trace name, here the order is important!!!  */
	std::vector<std::pair<size_t, std::string>> sorted_one_record_trace_iterator_jumps;

	/*! Set the size of each TraceData circular buffer 'trace_values_buffer' (default is 5)
	This function actally does teh re-size and is private
	@param[in] TraceData, object to change the buffer size
	@param[in] size_t, new_size */
	bool setTraceDataBufferSize(TraceData& trace_values, const size_t new_size);



	void setMasterLatticeData(const std::map<std::string, std::string>& paramMap);


	bool getNewTraceValuesFromEPICS( const pvStruct& pvs);


	void updateKFPowMaxAndPulseCounts();
	void checkCollectingFutureTraces();
	void checkForOutsideMaskTraceAndUpdateRollingAverages();
	void updateTraceCutMeans();
	void calculateTraceCutMean(TraceData& trace);
	//----------------------------------------------------------------------------
	// Checking masks stuff TODO needs to be converted to CATAP and checked 
	void checkForOutsideMaskTrace();
	int updateIsTraceInMask(TraceData& trace);
	void handleTraceInMaskResult(TraceData& trace, int result);
	void handleFailedMask(TraceData& trace);
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	void updateDAQFreqEstimate();
	size_t num_traces_to_estimate_rep_rate;
	double trace_rep_rate;
	std::vector<std::chrono::high_resolution_clock::time_point> trace_times_for_rep_rate;
	//----------------------------------------------------------------------------




	/*! flag for if dbr_all_trace_data poitner has been allocated enough memory */
	bool dbr_all_trace_data_not_malloced;

	/*! The time for each point on a rf trace */
	std::vector<double> time_vector;
	




	/*! Full Names for the power traces for this LLRF  */ // why do i need these??? 
	std::vector<std::string> power_trace_names;

	/*! Breakdown detectio Flag set if latest set of traces had an "outside mask" trace  */
	bool new_outside_mask_event;

	/*! The number of elements in a regular RF trace (klystron forward power, etc)  */
	size_t trace_data_size;
	/*! The number of elements in the concatenated RF trace data between seperate RF trace types */
	size_t one_trace_data_chunk_size;
	/*! The number of elements in the concatenated RF trace data  */
	size_t one_record_trace_data_size;
	/*! The number of elemnts = zero at the start of each one_record chunk (before the actual trace data) */
	size_t trace_num_of_start_zeros;









	std::pair<epicsTimeStamp, std::vector<double>> temp_TraceData_trace_data_buffer;





	//void setupAllTraceACQM();
	//void setupAllTraceSCAN();

	//void updateInterLockStatus(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockEnable(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockU(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockP(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockPDBM(const std::string& ch, const struct event_handler_args& args);

	//void updateSCAN(const std::string& ch, const struct event_handler_args& args);
	//void updateACQM(const std::string& ch, const struct event_handler_args& args);







};


#endif //LLRF_H_