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

	bool enable;
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
		/*! how mamny elements in trace_values */
		size_t trace_data_size;
		/*! circular buffer of trace_values, latest values are at the end of the buffer  */
		boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> trace_values_buffer;
		/*! number of traces to keep in trace_values_buffer */
		size_t  trace_data_buffer_size;
		/* Maximum value in latest acquired trace */
		double trace_max;
		/*! The start and stop index over which to calculate the trace mean */
		std::pair<size_t, size_t> mean_start_stop;
		/*! The start and stop time over which to calculate the trace mean */
		std::pair<double, double> mean_start_stop_time;
		/*! number of steps between start and stop index, used to calculate mean value */
		size_t  stop_minus_start;    
		
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


	/*! Set the size of each TraceData circular buffer 'trace_values_buffer' (default is 5)
	@param[in] size_t, new buffer size */
	bool setAllTraceDataBufferSize(const size_t value);
	/*! Set the size of a single TraceData circular buffer 'trace_values_buffer' (default is 5)
	@param[in] size_t, name TraceData object to set the biuffer size 
	@param[in] size_t, new buffer size 
	@param[out] bool, is bugffere size equal requested size */
	bool setTraceDataBufferSize(const std::string& name, const size_t new_size);

		
	/*! get all the trace data for this LLRF structure (calls a ca_array_get once, to reduce network traffic does not start monitoring of all traces)
	@param[out] map<string, vector<double>>, */
	void updateTraceValues();
	
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


	std::map<std::string, STATE> getAllTraceSCAN()const;
	boost::python::dict getAllTraceSCAN_Py()const;

	std::map<std::string, STATE> getAllTraceACQM()const;
	boost::python::dict getAllTraceACQM_Py()const;



	// GET ACTUAL TRACE VALUES 

	/*! get a trace_values values for requested trace
	@param[in] name, trace name to get data for
	@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getTraceValues(const std::string& name)const;
	/*! get trace_values for requested trace, Python version 
		@param[in] name, trace name to get data for
		@param[out] list, trace_values  */
	boost::python::list getTraceValues_Py(const std::string& name)const;
	/*! get a pair of a trace_name : trace_values for 1 trace
	@param[in] name, trace name to get data for
	@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
	std::pair<std::string, std::vector<double>> getTraceNameValuesPair(const std::string& name)const;
	/*!  get a dict of a trace_name : trace_values for 1 trace, Python version
	@param[in] name, trace name to get data for
	@param[out] dict, "trace_name" (string) and "trace_data_values" (list) */
	boost::python::dict getTraceNameValuesPair_Py(const std::string& name);
	///*! get a map of all the trace values 
	//	@param[out] trace data, map of "trace_name" (string) and "trace_data_values" (vector of doubles) */
	//std::map<std::string, std::vector<double>> getAllTraceValues()const;
	TraceData& getTraceData(const std::string& trace_name);

	boost::python::dict getTraceDataDict(const std::string& trace_name) const;

	boost::python::dict getAllTraceDataDict() const;



	///*! get a map of all the trace data */
	//std::map<std::string, std::vector<double>> getAllTraceData()const;



	/*! get a dict of all the trace data, python version
		@param[out] trace data, dict of "trace_name" (string) and "trace_data_values" (list) */
	boost::python::dict getAllTraceData_Py();


	//---------------------------------------------------------------------------------------------------------
	public:
		bool trigOff();
		bool trigInt();
		bool trigExt();
		STATE getTrigSource()const;
	private:
		bool setTrig(const STATE new_state);
	//---------------------------------------------------------------------------------------------------------

	private:
		void updateRollingAverage(TraceData& data);
	public:
		

		
		
		/*! Rest all the rolling average data, clears containers, reset rollign_avergae_count to zero,  (does not change values such as rolling_average_size). */
		void resetAllRollingAverage();
		/*! 
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		bool resetRollingAverage(const std::string& name);
		//void setKeepRollingAverageNoReset(const bool value);
		//bool setKeepRollingAverageNoReset(const std::string& name, const bool value);
		
		/*! 
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		void setShouldKeepRollingAverage();
		/*!
				@param[in] name, trace name to get data for
				@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		bool setShouldKeepRollingAverage(const std::string& name);
		/*!
				@param[in] name, trace name to get data for
				@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		void setShouldNotKeepRollingAverage();
		/*!
				@param[in] name, trace name to get data for
				@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */

		bool setShouldNotKeepRollingAverage(const std::string& name);
		/*!
				@param[in] name, trace name to get data for
				@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		bool setKeepRollingAverage(const std::string& name, bool value);
		/*!
				@param[in] name, trace name to get data for
				@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		bool setRollingAverageSize(const std::string& name, const size_t value);
		/*!
				@param[in] name, trace name to get data for
				@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		void setAllRollingAverageSize(const size_t value);
		/*!
				@param[in] name, trace name to get data for
				@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		std::vector<double> getRollingAverage(const std::string& name)const;
		/*!
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		boost::python::list getRollingAverage_Py(const std::string& name)const;
		/*!
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		boost::python::dict getRollingAverage_Py()const;
		/*!
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		std::vector<std::vector<double>> getAllRollingAverage(const std::string& name)const;
		/*!
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		size_t getRollingAverageSize(const std::string& name)const;
		/*!
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		size_t getRollingAverageCount(const std::string& name)const;
		/*!
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		bool isKeepingRollingAverage(const std::string& name)const;
		/*!
		@param[in] name, trace name to get data for
		@param[out] pair of "trace_name" (string) and "trace_data_values" (vector of doubles) */
		bool hasRollingAverage(const std::string& name)const;
		/*! Get the individual traces used to calculate the trace rolling average 
		@param[in] name, trace name to get data for
		@param[out] vector<vector<double>> vector of trace_value vectors  */
		std::vector<std::vector<double>> getRollingAverageTraceBuffer(const std::string& name) const;
		/*! Get the individual traces used to calculate the trace rolling average, Python Version
		@param[in] name, trace name to get data for
		@param[out] list of trace_value lists */
		boost::python::list getRollingAverageTraceBuffer_Py(const std::string& name)const;
		/*! Get the individual traces used to calculate the trace rolling average for all traces
		@param[out] map<string, vector<vector<double>> >  map of RollingAverageTraceBuffer keyed by trace name */
		std::map<std::string, std::vector<std::vector<double>>> getAllRollingAverageTraceBuffer() const;
		/*! Get the individual traces used to calculate the trace rolling average for all TraceData, Python Version
		@param[out] dict of RollingAverageTraceBuffer keyed by the trace name */
		boost::python::dict getAllRollingAverageTraceBuffer_Py()const;




		/*
			Klystron Forward Power Running Stats. These are the running mean and sigma for the KlyFwdPow
			Note, they depend on the position of We keep the amp_set vs KLY_FWD_POWER running stat variables
			m_n, m_oldM, m_oldS, see running_stat.h
		*/
		bool keep_kly_fwd_pow_running_stat;
		void setKeepKlyFwdPwrRS(bool val);
		void keepKlyFwdPwrRS();
		void dontKeepKlyFwdPwrRS();
		// these are the running stats
		std::map<int, RunningStats> amp_set_kly_fwd_rs;
		// This is the state of each RunningStat, so we can save and re-apply settings
		std::map<int, std::tuple<size_t, double, double>> amp_set_kly_fwd_rs_state;
		std::tuple<size_t, double, double> dummy_amp_set_kly_fwd_rs_state;
		void update_amp_set_kly_fwd_rs();
		std::tuple<size_t, double, double> getKlyFwdPwrRSState(int ampSP_setting);
		void setKlyFwdPwrRSState(int amp_sp, size_t n, double old_mean, double old_variance);


	/*! get a cavity reverse power trace_values 
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavRevPwr()const;
	/*! get a cavity forward power trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavFwdPwr()const;
	/*! get a klystron reverse power trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyRevPwr()const;
	/*! get a klystron forward power trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyFwdPwr()const;
	/*! get a cavity reverse phase trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavRevPha()const;
	/*! get a cavity forward phase trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getCavFwdPha()const;
	/*! get a klystron reverse phase trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyRevPha()const;
	/*! get a klystron forward phase trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getKlyFwdPha()const;
	/*! get a cavity probe power trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getProbePwr()const;
	/*! get a cavity probe phase trace_values
		@param[out] "trace_data_values" (vector of doubles) */
	std::vector<double> getProbePha()const;


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

	std::map<std::string, double> getAllCutMean()const;
	boost::python::dict getAllCutMean_Py()const;

	// DAQ freq getters / settings 
	void setNumTracesToEstimateRepRate(size_t value);
	size_t getNumTracesToEstimateRepRate() const;
	double getTraceRepRate() const;


	// don;t use anymore ... 
	double getMeanStartTime(const std::string& name)const;
	double getMeanStopTime(const std::string& name)const;

	std::map<std::string, std::string> getLLRFStateMap()const;

	/*! retun active_pulse_kly_power_limit (default 500 W)
	@param[out] size_t, */
	size_t getActivePulsePowerLimit() const;
	/*! Set active_pulse_kly_power_limit (default 500 W)
	@param[in] double, */
	void setActivePulsePowerLimit(const size_t value);
	
	/*! retun active_pulse_count
	@param[out] size_t, */
	size_t getActivePulseCount()const;;
	/*! Set active_pulse_count to value
	@param[in] size_t, */
	void setActivePulseCount(const size_t value);
	/*! increment active_pulse_count by value
	@param[in] size_t, */
	void addActivePulseCountOffset(const size_t value);

	/*! retun inactive_pulse_count
	@param[out] size_t, */
	size_t getInactivePulseCount()const;
	/*! Set inactive_pulse_count to value
	@param[in] size_t, */
	void setInactivePulseCount(const size_t value);
	/*! increment inactive_pulse_count by value
	@param[in] size_t, */
	void addInactivePulseCountOffset(const size_t value);

	/*! retun duplicate_pulse_count
	@param[out] size_t, */
	size_t getDuplicatePulseCount()const;
	/*! Set duplicate_pulse_count to value
	@param[in] size_t, */
	void setDuplicatePulseCount(const size_t value);
	/*! increment duplicate_pulse_count by value
	@param[in] size_t, */
	void addDuplicatePulseCountOffset(const size_t value);

	/*! retun duplicate_pulse_count
	@param[out] size_t, */
	size_t getTotalPulseCount()const;
	/*! Set total_pulse_count to value
	@param[in] size_t, */
	void setTotalPulseCount(const size_t value);
	/*! increment total_pulse_count by value
	@param[in] size_t, */
	void addTotalPulseCountOffset(const size_t value);



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

	/*! maximum kylstron forward power from latest trace*/
	std::pair<epicsTimeStamp, double > kly_fwd_power_max;

	/*! source of LLRF trigger one of: OFF, INTERNAL, EXTERNAL */
	std::pair<epicsTimeStamp, STATE > trig_source;
		
	/*! pulse duration from the LLRF, !!warning!! BUT it is not accurate if the pulse shape applied is not a square wave */
	std::pair<epicsTimeStamp, double > llrf_pulse_duration;

	/*! pulse offset */
	std::pair<epicsTimeStamp, double > pulse_offset;

	/*! RF PSS Heartbeat, when in RF mode this signal must keep changing to keep the RF system alive */
	std::pair<epicsTimeStamp, double > heartbeat;


	/*! Size of circular buffer used to store trace data */
	size_t trace_data_buffer_size;
	
	
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

	bool getTraceFromChannelData(const std::string& channel, std::string& string_to_put_trace_name_in);

private:
	double crest_phase;
	double operating_phase;
	/*! alternative names for the LLRF (usually shorter thna the full PV root), 
		defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;


	TraceData dummy_trace_data;


	std::vector<std::pair<size_t, std::string>> sorted_one_record_trace_start_indices;
	std::vector<std::pair<size_t, std::string>> sorted_one_record_trace_iterator_jumps;

	/*! Set up the trace_data_map for this cavity, called after the cavity TYPE has been set 
	defined in the master lattice yaml file	*/
	void setTraceDataMap();
	/*! This function actually adds a new TraceData object trace_data_map, called from setTraceDataMap */
	void addToTraceDataMap(const std::string& name);
	/*! Set the size of each TraceData circular buffer 'trace_values_buffer' (default is 5)
	This function actally does teh re-size and is private
	@param[in] TraceData, object to change the buffer size
	@param[in] size_t, new_size */
	bool setTraceDataBufferSize(TraceData& trace_values, const size_t new_size);


	/*! Actually 'clears' and reset the values for TraceDta rolling average. 
	@param[in] TraceData, object to reset rolling average data */
	void clearRollingAverage(TraceData& trace);

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



	void scaleAllDummyTraces();
	//void scaleDummyTrace(TraceData& trace_values, const std::vector<double>& dummy_trace);
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

	/*! Full Names for the power traces for this LLRF  */ // why do i need these??? 
	std::vector<std::string> power_trace_names;

	/*! Breakdown detectio Flag set if latest set of traces had an "outside mask" trace  */
	bool new_outside_mask_event;

	/*! The number of elements in a regular RF trace (klystron forward power, etc)  */
	size_t trace_data_size;
	/*! The number of elements in the concatenated RF trace data  */
	size_t one_record_trace_data_size;

	/*! If kly_fwd_power_max is above this value can_increase_active_pulses is set to true  */
	double active_pulse_kly_power_limit;
	/*! Flag to detemrien if teh pusles count can be increaws3ed (true if kly_fwd_power_max > active_pulse_kly_power_limit  */
	bool can_increase_active_pulses;
	/*! kly_fwd_power_max for previous trace, this is important becasue the LLRF box can be in a state where it sends out mutliple copies of the same trace data */
	double last_kly_fwd_power_max;

	/*! Pulse count where where kly_fwd_power_max > active_pulse_kly_power_limit */
	size_t active_pulse_count;
	/*! Pulse count where  where kly_fwd_power_max < active_pulse_kly_power_limit */
	size_t inactive_pulse_count;
	/*! Pulse count where kly_fwd_power_max == last_kly_fwd_power_max */
	size_t duplicate_pulse_count;
	/*! Pulse count */
	size_t total_pulse_count;


	bool is_trace_monitoring;



	std::pair<epicsTimeStamp, std::vector<double>> temp_TraceData_trace_data_buffer;

	// here we keep all the traces acqm and scan STATEs
	// actual usable traces also have their own version of this in their trace_values 
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_acqm;
	std::map<std::string, std::pair<epicsTimeStamp, STATE>> all_trace_scan;


	//void splitOneTraceValues();
	/*! after a new one_record_trace_values has been acquired, split it up into the correct TraceData object  
	* This function is called when monitoring traces and when just getting a single trace
		@param[in] const struct dbr_time_double*, pointer to trace data 
	*/
	void splitOneTraceValues(const struct dbr_time_double* dbr);

	/*! Each TraceDat has ossociated meta-data (max, mena, etc. that needs updating when a new trace is acquired */
	void updateTraceMetaData();

	void setupInterlocks();

	void initAllTraceSCANandACQM();

	//void setupAllTraceACQM();
	//void setupAllTraceSCAN();

	//void updateInterLockStatus(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockEnable(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockU(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockP(const std::string& ch, const struct event_handler_args& args);
	//void updateInterLockPDBM(const std::string& ch, const struct event_handler_args& args);

	//void updateSCAN(const std::string& ch, const struct event_handler_args& args);
	//void updateACQM(const std::string& ch, const struct event_handler_args& args);


	std::map<std::string, std::string> channel_to_tracesource_map;
	std::string getTraceFromChannelData(const std::string& channel_data) const;

	//std::string getTraceFromTracePVKey(const std::string& trace)const;



	/*! All the "main" channel names, main refers to the power and phase traces  */
	std::vector<std::string> allMainChannelTraceNames;
	/*! All the "main" channel names, main refers to the power and phase traces  */
	std::vector<std::string> allChannelTraceNames;




};


#endif //LLRF_H_