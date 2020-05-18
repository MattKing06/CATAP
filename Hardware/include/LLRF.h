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
		//boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> trace_data_buffer;
		std::vector<double> trace_data;
		// max value in trace
		double trace_max;
		// traces indices over which to calculate trace_cut_mean
		size_t mean_start_index, mean_stop_index;
		// mean of values between  mean_start_index, mean_stop_index
		double trace_cut_mean;
};


class LLRF : public Hardware
{
public:
	LLRF();
	LLRF(const std::map<std::string, std::string>& paramMap, STATE mode);
	LLRF(const LLRF& copyLLRF);
	~LLRF();
	void setPVStructs();
	EPICSLLRFInterface_sptr epicsInterface;
	std::map<std::string, std::string> LLRFParamMap;



	bool setPhi(double value);
	bool setAmp(double value);
	bool setAmpMVM(double value);
	bool setPhiDEG(double value);

	double getPhi()const;
	double getAmp()const;
	double getAmpMVM()const;
	double getPhiDEG()const;

	/*! get the name alises for this LLRF
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases() const;
	/*! get the name alises for this LLRF (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py() const;

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
	std::pair<epicsTimeStamp, double > amp_MVM;
	/*! latest phi_sp value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, double > phi_degrees;


	// special aase for the HRRG_GUN and LRRG_GUN
	void setMachineArea(const TYPE area);


private:

	/*! alternative names for the magnet (usually shorter thna the full PV root), 
		defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;


	std::map<std::string, TraceData> trace_data_map;

	void setTraceDataMap();
	void scaleDummyTraces();

	std::string fullLLRFTraceName(const std::string& name_in)const;

	void addDummyTraces(const std::map<std::string, std::string>& paramMap);

	std::vector<double> kfpow_dummy_trace;
	std::vector<double> krpow_dummy_trace;
	std::vector<double> cfpow_dummy_trace;
	std::vector<double> crpow_dummy_trace;
	std::vector<double> kfpha_dummy_trace;
	std::vector<double> krpha_dummy_trace;
	std::vector<double> cfpha_dummy_trace;
	std::vector<double> crpha_dummy_trace;

};


#endif //LLRF_H_