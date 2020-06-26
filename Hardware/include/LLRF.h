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
};


class LLRF : public Hardware
{
public:
	LLRF();
	LLRF(const std::map<std::string, std::string>& paramMap, STATE mode);
	LLRF(const LLRF& copyLLRF);
	~LLRF();
	void setPVStructs();

	// TODO private ?? 
	EPICSLLRFInterface_sptr epicsInterface;
	std::map<std::string, std::string> LLRFParamMap;


	// amplitude 
	bool setAmp(double value);
	bool setAmpMW(double value);
	double getAmp()const;
	double getAmpMW()const;

	// phases 
	bool setPhi(double value);
	bool setPhiDEG(double value);
	bool setCrestPhase(double value);
	bool setOperatingPhase(double value);
	double getPhi()const;
	double getPhiDEG()const;
	double getCrestPhase()const;
	double getOperatingPhase()const;


	/*! get the name alises for this LLRF
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases() const;
	/*! get the name alises for this LLRF (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py() const;



	size_t getIndex(const double time) const;
	double getTime(const size_t index) const;

	std::map<std::string, std::vector<double>> getAllTraceData()const;
	std::pair<std::string, std::vector<double>> getTraceData(const std::string& name)const;
	std::vector<double> getTraceValues(const std::string& name)const;
	std::vector<double> getCavRevPwr()const;
	std::vector<double> getCavFwdPwr()const;
	std::vector<double> getKlyRevPwr()const;
	std::vector<double> getKlyFwdPwr()const;
	std::vector<double> getCavRevPha()const;
	std::vector<double> getCavFwdPha()const;
	std::vector<double> getKlyRevPha()const;
	std::vector<double> getKlyFwdPha()const;
	std::vector<double> getProbePwr()const;
	std::vector<double> getProbePha()const;
	boost::python::dict getAllTraceData_Py();
	boost::python::dict getTraceData_Py(const std::string& name);
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







	bool setTraceDataBufferSize(const std::string& name, const size_t new_size);



	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();

	friend class EPICSLLRFInterface;
	friend class LLRFFactory;
protected:

	/*! latest amp_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > amp_sp;
	/*! latest phi_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > phi_sp;

	/*! latest amp_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > amp_MW;
	/*! latest phi_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > phi_degrees;

	// special case for the HRRG_GUN and LRRG_GUN
	void setMachineArea(const TYPE area);
	void setDefaultPowerTraceMeanTimes();




private:
	
	double crest_phase;
	double operating_phase;

	/*! alternative names for the magnet (usually shorter thna the full PV root), 
		defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;
		


	std::map<std::string, TraceData> trace_data_map;

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

	std::vector<double> time_vector;
	
	std::vector<double> kfpow_dummy_trace;
	std::vector<double> krpow_dummy_trace;
	std::vector<double> cfpow_dummy_trace;
	std::vector<double> crpow_dummy_trace;
	std::vector<double> kfpha_dummy_trace;
	std::vector<double> krpha_dummy_trace;
	std::vector<double> cfpha_dummy_trace;
	std::vector<double> crpha_dummy_trace;

	std::vector<std::string> power_trace_names;


	// TODO if we make this a "const size_t  trace_data_size;" it breaks the copy constructor ??? 
	size_t trace_data_size;
	void setTraceDataBufferSize(const size_t new_size);





};


#endif //LLRF_H_