#include <LLRF.h>
#include "boost/algorithm/string/split.hpp"
#include "LLRFPVRecords.h"
#include "GlobalFunctions.h"
#include "PythonTypeConversions.h"
#include <numeric>




TraceData::TraceData():
trace_data_size(1017),
name("unknown"),
trace_max(GlobalConstants::double_min),
mean_start_stop(std::pair<size_t,size_t>( GlobalConstants::nine99999,GlobalConstants::nine99999)),
mean_start_stop_time(std::pair<double, double>(GlobalConstants::double_min, GlobalConstants::double_min)),
//mean_start_index(GlobalConstants::zero_sizet),
//mean_stop_index(GlobalConstants::zero_sizet),
//trace_cut_mean(GlobalConstants::double_min),
trace_data_buffer_size(GlobalConstants::two_sizet)
{

}

TraceData::TraceData(const TraceData& copy_trace_data) :
trace_data_size(copy_trace_data.trace_data_size),
name(copy_trace_data.name),
trace_max(copy_trace_data.trace_max),
mean_start_stop(copy_trace_data.mean_start_stop),
mean_start_stop_time(copy_trace_data.mean_start_stop_time),
//mean_start_index(copy_trace_data.mean_start_index),
//mean_stop_index(copy_trace_data.mean_stop_index),
trace_cut_mean(copy_trace_data.trace_cut_mean)
{

}
TraceData::~TraceData()
{

}


LLRF::LLRF() :
	trace_data_size(1017)
{
}

LLRF::LLRF(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	crest_phase(std::stof(paramMap.find("crest_phase")->second)),
	trace_data_size(1017),
	// calls copy constructor and destroys 
	epicsInterface(boost::make_shared<EPICSLLRFInterface>(EPICSLLRFInterface()))
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LLRF Constructor");
	epicsInterface->ownerName = hardwareName;
	setPVStructs();
	// TODO name_alias should be in harwdare constructor?? 
	boost::split(aliases, paramMap.find("name_alias")->second, [](char c) {return c == ','; });
	// TODOD add in the channel number to trace name map data
	//boost::split(aliases, paramMap.find("chanel_to_trace_map")->second, [](char c) {return c == ','; });
	

	addDummyTraces(paramMap);


	power_trace_names.push_back(fullLLRFTraceName(GlobalConstants::KLYSTRON_FORWARD_POWER));
	power_trace_names.push_back(fullLLRFTraceName(GlobalConstants::KLYSTRON_REVERSE_POWER));
	power_trace_names.push_back(fullLLRFTraceName(GlobalConstants::CAVITY_FORWARD_POWER));
	power_trace_names.push_back(fullLLRFTraceName(GlobalConstants::CAVITY_REVERSE_POWER));
	if (machine_area != TYPE::LRRG_GUN)
	{
		power_trace_names.push_back(fullLLRFTraceName(GlobalConstants::CAVITY_PROBE_POWER));
	}



}

LLRF::LLRF(const LLRF& copyLLRF) :
	Hardware(copyLLRF),
	//trace_data_size(copyLLRF.trace_data_size),
	epicsInterface(copyLLRF.epicsInterface)
{
}

LLRF::~LLRF()
{
}

void LLRF::setPVStructs()
{
	for (auto&& record : LLRFRecords::llrfRecordList)
	{
		messenger.printDebugMessage("LLRF PV record = " + record);
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;

		// TODO NO ERROR CHECKING! (we assum config file is good??? 
		std::string PV = specificHardwareParameters.find(record)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		//messenger.printDebugMessage("Constructing PV information for ", record);

		/*TODO
		  This should be put into some general function: generateVirtualPV(PV) or something...
		  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
		  The config reader level if that is the case.
		  DJS maybe they should, how certian can we be all virtual PVs will get a VM- prefix???
		  */
		if (mode == STATE::VIRTUAL)
		{
			pvStructs[record].fullPVName = "VM-" + PV;
		}
		else
		{
			pvStructs[record].fullPVName = PV;
		}
		messenger.printDebugMessage(record, " = ","PV  = " + pvStructs[record].fullPVName);
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}

void LLRF::setMachineArea(const TYPE area) // called from factory
{
	machine_area = area;
	machine_area_str = ENUM_TO_STRING(machine_area);
	messenger.printDebugMessage("NEW Machien Area =  " + machine_area_str);
	setTraceDataMap();
}

void LLRF::setDefaultPowerTraceMeanTimes()
// called from factory
{
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("kfpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("kfpow_mean_end_time")->second),
		GlobalConstants::KLYSTRON_FORWARD_POWER);
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("krpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("krpow_mean_end_time")->second),
		GlobalConstants::KLYSTRON_REVERSE_POWER);
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("cfpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("cfpow_mean_end_time")->second),
		GlobalConstants::CAVITY_FORWARD_POWER);
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("crpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("crpow_mean_end_time")->second),
		GlobalConstants::CAVITY_REVERSE_POWER);
	if(GlobalFunctions::entryExists(specificHardwareParameters, "cppow_mean_start_time"))
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, "cppow_mean_end_time"))
		{
			setMeanStartEndTime(
				std::stof(specificHardwareParameters.find("cppow_mean_start_time")->second),
				std::stof(specificHardwareParameters.find("cppow_mean_end_time")->second),
				GlobalConstants::CAVITY_PROBE_POWER);
		}
	}
}



bool LLRF::setAmpMW(double value)
{
	// atm we are going to fake power traces
	amp_MW.second = value;
	//scaleAllDummyTraces();
	return true;
}
bool LLRF::setAmp(double value)
{
	amp_sp.second = value;
	return true;
}
double LLRF::getAmp()const
{
	return amp_sp.second;
}
double LLRF::getAmpMW()const
{
	return amp_MW.second;
}

bool LLRF::setPhi(double value)
{
	phi_sp.second = value;
	return true;
}
bool LLRF::setPhiDEG(double value)
{
	phi_degrees.second = value;
	operating_phase = phi_degrees.second - crest_phase;
	return true;
}
bool LLRF::setCrestPhase(double value)
{
	crest_phase = value;
	return true;
}
bool LLRF::setOperatingPhase(double value)
{
	operating_phase = value;
	phi_sp.second = crest_phase + operating_phase;
	phi_sp.first = epicsTimeStamp();
	return true;
}
double LLRF::getPhi()const
{
	return phi_sp.second;
}
double LLRF::getCrestPhase()const
{
	return crest_phase;
}
double LLRF::getOperatingPhase()const
{
	return operating_phase;
}
double LLRF::getPhiDEG()const
{
	return phi_degrees.second;
}


std::map<std::string, std::vector<double>> LLRF::getAllTraceData()const
{
	std::map<std::string, std::vector<double>> r;
	for (auto&& data : trace_data_map)
	{
		r[data.first] = getTraceValues(data.first);
	}
	return r;
}

std::pair<std::string, std::vector<double>> LLRF::getTraceData(const std::string& name)const
{
	std::pair<std::string, std::vector<double>> r;
	r.first  = fullLLRFTraceName(name);
	r.second = getTraceValues(r.first);
	return r;
}

std::vector<double> LLRF::getTraceValues(const std::string& name)const
{
	const std::string n = fullLLRFTraceName(name);
	//messenger.printDebugMessage("getTraceValues trace " + n);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).trace_data_buffer.back().second;
	}
	messenger.printDebugMessage("LLRF::getTraceValues ERROR, trace ", n, " does not exist");
	std::vector<double> r(trace_data_size, GlobalConstants::double_min);//MAGIC_NUMBER
	return r;
}
std::vector<double> LLRF::getCavRevPwr()const
{
	return getTraceValues(GlobalConstants::CAVITY_REVERSE_POWER);
}
std::vector<double> LLRF::getCavFwdPwr()const
{
	return getTraceValues(GlobalConstants::CAVITY_FORWARD_POWER);
}
std::vector<double> LLRF::getKlyRevPwr()const
{
	return getTraceValues(GlobalConstants::KLYSTRON_REVERSE_POWER);
}
std::vector<double> LLRF::getKlyFwdPwr()const
{
	//for (auto&& tb : trace_data_map.at(GlobalConstants::KLYSTRON_FORWARD_POWER).trace_data_buffer)
	//{
	//	messenger.printDebugMessage("getKlyFwdPwr NEW TB");
	//	int jj = 0;
	//	for (auto&& it : tb.second)
	//	{
	//		messenger.printDebugMessage(it);
	//		jj += 1;
	//		if (jj == 10)
	//		{
	//			break;
	//		}
	//	}
	//}
	return getTraceValues(GlobalConstants::KLYSTRON_FORWARD_POWER);
}
std::vector<double> LLRF::getCavRevPha()const
{
	return getTraceValues(GlobalConstants::CAVITY_REVERSE_PHASE);
}
std::vector<double> LLRF::getCavFwdPha()const
{
	return getTraceValues(GlobalConstants::CAVITY_FORWARD_PHASE);
}
std::vector<double> LLRF::getKlyRevPha()const
{
	return getTraceValues(GlobalConstants::KLYSTRON_REVERSE_PHASE);
}
std::vector<double> LLRF::getKlyFwdPha()const
{
	return getTraceValues(GlobalConstants::KLYSTRON_FORWARD_PHASE);
}
std::vector<double> LLRF::getProbePwr()const
{
	return getTraceValues(GlobalConstants::CAVITY_PROBE_POWER);
}
std::vector<double> LLRF::getProbePha()const
{
	return getTraceValues(GlobalConstants::CAVITY_PROBE_PHASE);
}
//-------------------------------------------------------------------------------
boost::python::dict LLRF::getAllTraceData_Py()
{
	return to_py_dict<std::string, std::vector<double>>(getAllTraceData());
}
boost::python::dict LLRF::getTraceData_Py(const std::string& name)
{
	std::pair<std::string, std::vector<double>> r = getTraceData(name);
	std::map<std::string, std::vector<double>> r2{ {r.first, r.second} };
	return to_py_dict<std::string, std::vector<double>>(r2);
}
boost::python::list LLRF::getTraceValues_Py(const std::string& name)const
{
	return to_py_list(getTraceValues(name));
}
boost::python::list LLRF::getCavRevPwr_Py()const
{
	return to_py_list(getCavRevPwr());
}
boost::python::list LLRF::getCavFwdPwr_Py()const
{
	return to_py_list(getCavFwdPwr());
}
boost::python::list LLRF::getKlyRevPwr_Py()const
{
	return to_py_list(getKlyRevPwr());
}
boost::python::list LLRF::getKlyFwdPwr_Py()const
{
	return to_py_list(getKlyFwdPwr());
}
boost::python::list LLRF::getCavRevPha_Py()const
{
	return to_py_list(getCavRevPha());
}
boost::python::list LLRF::getCavFwdPha_Py()const
{
	return to_py_list(getCavFwdPha());
}
boost::python::list LLRF::getKlyRevPha_Py()const
{
	return to_py_list(getKlyRevPha());
}
boost::python::list LLRF::getKlyFwdPha_Py()const
{
	return to_py_list(getKlyFwdPha());
}
boost::python::list LLRF::getProbePha_Py()const
{
	return to_py_list(getProbePha());
}
boost::python::list LLRF::getProbePwr_Py()const
{
	return to_py_list(getProbePwr());
}


std::vector<std::string> LLRF::getAliases() const
{
	return aliases;
}
boost::python::list LLRF::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
}



/*
  _____                   ___     _     __  __
 |_   _| _ __ _ __ ___   / __|  _| |_  |  \/  |___ __ _ _ _  ___
   | || '_/ _` / _/ -_) | (_| || |  _| | |\/| / -_) _` | ' \(_-<
   |_||_| \__,_\__\___|  \___\_,_|\__| |_|  |_\___\__,_|_||_/__/
*/
//-------------------------------------------------------------------------------------------------------------------
bool LLRF::setMeanStartEndTime(const double start, const double end, const std::string& name)
{
	bool a = setMeanStartIndex(name, getIndex(start));
	if (a)
	{
		a = setMeanStopIndex(name, getIndex(end));
	}
	return a;
}
//-------------------------------------------------------------------------------------------------------------------
bool LLRF::setMeanStartEndIndex(const size_t start, const size_t end, const std::string& name)
{
	bool a = setMeanStartIndex(name, start);
	if (a)
	{
		a = setMeanStopIndex(name, end);
	}
	return a;
}
std::pair<size_t, size_t> LLRF::getMeanStartEndIndex(const std::string& name) const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).mean_start_stop;
	}
	return std::pair<size_t, size_t>(GlobalConstants::nine99999, GlobalConstants::nine99999);
}
std::pair<double, double> LLRF::getMeanStartEndTime(const std::string& name) const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).mean_start_stop_time;
	}
	return std::pair<double, double>(GlobalConstants::double_min, GlobalConstants::double_min);
}
boost::python::list LLRF::getMeanStartEndIndex_Py(const std::string& name) const
{
	return to_py_list<size_t, size_t>(getMeanStartEndIndex(name));
}
boost::python::list  LLRF::getMeanStartEndTime_Py(const std::string& name) const
{
	return to_py_list<double, double>(getMeanStartEndTime(name));
}
std::map<std::string, std::pair<size_t, size_t>> LLRF::getTraceMeanIndices()const
{
	std::map<std::string, std::pair<size_t, size_t>> r;
	for (auto&& trace : trace_data_map)
	{
		r[trace.first] = trace.second.mean_start_stop;
	}
	return r;
}
std::map<std::string, std::pair<double, double>> LLRF::getTraceMeanTimes()const
{
	std::map<std::string, std::pair<double, double>> r;
	for (auto&& trace : trace_data_map)
	{
		r[trace.first] = trace.second.mean_start_stop_time;
	}
	return r;
}
boost::python::dict LLRF::getTraceMeanIndices_Py()const
{
	return to_py_dict<std::string, size_t, size_t>(getTraceMeanIndices());
}
boost::python::dict LLRF::getTraceMeanTimes_Py()const
{
	return to_py_dict<std::string, double, double>(getTraceMeanTimes());
}
void LLRF::setTraceMeanIndices(const std::map<std::string, std::pair<size_t, size_t>>& settings)
{
	for (auto&& it : settings)
	{
		setMeanStartEndIndex(it.second.first, it.second.second, it.first);
	}
}
void LLRF::setTraceMeanTimes(const std::map<std::string, std::pair<double, double>>& settings)
{
	for (auto&& it : settings)
	{
		setMeanStartEndTime(it.second.first, it.second.second, it.first);
	}
}
void LLRF::setTraceMeanIndices_Py(const boost::python::dict& settings)
{
	setTraceMeanIndices(to_std_map_pair<std::string,size_t>(settings));
}
void LLRF::setTraceMeanTimes_Py(const boost::python::dict& settings)
{
	setTraceMeanTimes(to_std_map_pair<std::string, double>(settings));
}
//-------------------------------------------------------------------------------------------------------------------
bool LLRF::setMeanStartIndex(const std::string& name, size_t  value)
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		if (trace_data_map.at(n).trace_data_size - 1 >= value)
		{
			//trace_data_map.at(n).mean_start_index = value;
			trace_data_map.at(n).mean_start_stop.first = value;

			if (trace_data_map.at(n).mean_start_stop.first <= trace_data_map.at(n).mean_start_stop.second)
			{
				trace_data_map.at(n).stop_minus_start = trace_data_map.at(n).mean_start_stop.second - trace_data_map.at(n).mean_start_stop.first;
			}
			trace_data_map.at(n).mean_start_stop_time.first = getTime(trace_data_map.at(n).mean_start_stop.first);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------
size_t LLRF::getMeanStopIndex(const std::string& name)const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).mean_start_stop.second;
		//return trace_data_map.at(n).mean_stop_index;
	}
	return GlobalConstants::double_min;
}
//-------------------------------------------------------------------------------------------------------------------
size_t LLRF::getMeanStartIndex(const std::string& name)const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).mean_start_stop.first;
		//return trace_data_map.at(n).mean_start_index;
	}
	return GlobalConstants::double_min;
}
//-------------------------------------------------------------------------------------------------------------------
bool LLRF::setMeanStopIndex(const std::string& name, size_t  value)
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		if (trace_data_map.at(n).trace_data_size - 1 >= value)
		{
			//trace_data_map.at(n).mean_stop_index = value;
			trace_data_map.at(n).mean_start_stop.second = value;
			if (trace_data_map.at(n).mean_start_stop.first <= trace_data_map.at(n).mean_start_stop.second)
			{
				trace_data_map.at(n).stop_minus_start = trace_data_map.at(n).mean_start_stop.second - trace_data_map.at(n).mean_start_stop.first;
			}
			trace_data_map.at(n).mean_start_stop_time.second = getTime(trace_data_map.at(n).mean_start_stop.second);
			return true;
		}
	}
	//message(name," NOT FOUND!");
	return false;
}
//-------------------------------------------------------------------------------------------------------------------
double LLRF::getMeanStartTime(const std::string& name)const
{
	return getTime(getMeanStartIndex(name));
}
//-------------------------------------------------------------------------------------------------------------------
double LLRF::getMeanStopTime(const std::string& name)const
{
	return getTime(getMeanStopIndex(name));
}
//-------------------------------------------------------------------------------------------------------------------
double LLRF::getCutMean(const std::string& name)const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).trace_cut_mean;
	}
	return GlobalConstants::double_min;
}

std::map<std::string, double> LLRF::getPowerCutMean()const
{
	std::map<std::string, double> r;
	for (auto&& trace : power_trace_names)
	{
		r[trace] = getCutMean(trace);
	}
	return r;
}
boost::python::dict LLRF::getPowerCutMean_Py()const
{
	return to_py_dict<std::string, double>(getPowerCutMean());
}

void LLRF::updateTraceCutMeans()
{
	messenger.printDebugMessage("updateTraceCutMeans");
	for (auto& it : trace_data_map)
	{
		calculateTraceCutMean(it.second);
	}
}
void LLRF::calculateTraceCutMean(TraceData& trace)
{
	/*
		The trace cut mean is the mean value between two indices on a trace
	*/
	if(trace.mean_start_stop.first == trace.mean_start_stop.second)
	{
		trace.trace_cut_mean = trace.trace_data_buffer.back().second[trace.mean_start_stop.first];
	}
	else if(trace.mean_start_stop.second > trace.mean_start_stop.first)
	{
		trace.trace_cut_mean =
			std::accumulate(
				trace.trace_data_buffer.back().second.cbegin() + trace.mean_start_stop.first,
				trace.trace_data_buffer.back().second.cbegin() + trace.mean_start_stop.second,
				//GlobalConstants::zero_double) / (trace.mean_stop_index - trace.mean_start_index);
				GlobalConstants::zero_double) / trace.stop_minus_start;
	}
	messenger.printDebugMessage(trace.name + " New Cut mean = ", trace.trace_cut_mean);
}









void  LLRF::scaleAllDummyTraces()
{
	//messenger.printDebugMessage("scaleAllDummyTraces()  ");
	using namespace GlobalConstants;
	scaleDummyTrace(KLYSTRON_FORWARD_POWER, kfpow_dummy_trace);
	//messenger.printDebugMessage("KLYSTRON_FORWARD_POWER AFTER SCALING");
	//for (auto&& tb : trace_data_map.at(GlobalConstants::KLYSTRON_FORWARD_POWER).trace_data_buffer)
	//{
	//	messenger.printDebugMessage("scaleAllDummyTraces NEW TB");
	//	int jj = 0;
	//	for (auto&& it : tb.second)
	//	{
	//		messenger.printDebugMessage(it);
	//		jj += 1;
	//		if (jj == 10)
	//		{
	//			break;
	//		}
	//	}
	//}
	//messenger.printDebugMessage("KLYSTRON_REVERSE_POWER");
	scaleDummyTrace(KLYSTRON_REVERSE_POWER, krpow_dummy_trace);
	//messenger.printDebugMessage("CAVITY_FORWARD_POWER");
	//messenger.printDebugMessage(fullLLRFTraceName(CAVITY_FORWARD_POWER));
	scaleDummyTrace(fullLLRFTraceName(CAVITY_FORWARD_POWER), cfpow_dummy_trace);
	//messenger.printDebugMessage("CAVITY_REVERSE_POWER");
	scaleDummyTrace(fullLLRFTraceName(CAVITY_REVERSE_POWER), crpow_dummy_trace);
	//messenger.printDebugMessage("calling updateTraceCutMeans");
	//updateTraceCutMeans();
}

void LLRF::scaleDummyTrace(const std::string& trace_name, const std::vector<double>& dummy_trace)
{
	//messenger.printDebugMessage("Scaling " + trace_name);
	//messenger.printDebugMessage("dummy_trace.size() = ", dummy_trace.size());
	std::pair<epicsTimeStamp, std::vector<double>> dummy_trace_item;
	dummy_trace_item.first = epicsTimeStamp();
	dummy_trace_item.second = dummy_trace;
	double scale_factor = amp_MW.second * 1000000.0;
	for (auto& i : dummy_trace_item.second)
	{
		i = i * scale_factor;
	}
	// assumes trace_name exists
	trace_data_map.at(trace_name).trace_data_buffer.push_back(dummy_trace_item);
	//messenger.printDebugMessage("scaleDummyTrace AFTER SCALING");
	//for (auto&& tb : trace_data_map.at(GlobalConstants::KLYSTRON_FORWARD_POWER).trace_data_buffer)
	//{
	//	messenger.printDebugMessage("scaleDummyTrace NEW TB");
	//	int jj = 0;
	//	for (auto&& it : tb.second)
	//	{
	//		messenger.printDebugMessage(it);
	//		jj += 1;
	//		if (jj == 10)
	//		{
	//			break;
	//		}
	//	}
	//}
}

size_t LLRF::getIndex(const double time) const
{
	//std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
	auto r = std::lower_bound(time_vector.begin(), time_vector.end(), time);
	return r - time_vector.begin();
}
//____________________________________________________________________________________________
double LLRF::getTime(const size_t index) const
{
	if (index > time_vector.size() - 1)
	{
		return GlobalConstants::double_min;
	}
	return time_vector[index];
}




void LLRF::setTraceDataMap()
{
	messenger.printDebugMessage(getHardwareName() + ", Setting the Trace Data Map");
	addToTraceDataMap(GlobalConstants::KLYSTRON_FORWARD_POWER);
	addToTraceDataMap(GlobalConstants::KLYSTRON_FORWARD_PHASE);
	addToTraceDataMap(GlobalConstants::KLYSTRON_REVERSE_POWER);
	addToTraceDataMap(GlobalConstants::KLYSTRON_REVERSE_PHASE);
	if (machine_area == TYPE::LRRG_GUN)
	{
		addToTraceDataMap(GlobalConstants::LRRG_CAVITY_FORWARD_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::LRRG_CAVITY_FORWARD_PHASE);// = TraceData();
		addToTraceDataMap(GlobalConstants::LRRG_CAVITY_REVERSE_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::LRRG_CAVITY_REVERSE_PHASE);// = TraceData();
	}
	else if (machine_area == TYPE::HRRG_GUN)
	{
		addToTraceDataMap(GlobalConstants::HRRG_CAVITY_FORWARD_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::HRRG_CAVITY_FORWARD_PHASE);// = TraceData();
		addToTraceDataMap(GlobalConstants::HRRG_CAVITY_REVERSE_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::HRRG_CAVITY_REVERSE_PHASE);// = TraceData();
		addToTraceDataMap(GlobalConstants::CAVITY_PROBE_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::CAVITY_PROBE_PHASE);// = TraceData();
	}
	else if(machine_area == TYPE::L01)
	{
		addToTraceDataMap(GlobalConstants::L01_CAVITY_FORWARD_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::L01_CAVITY_FORWARD_PHASE);// = TraceData();
		addToTraceDataMap(GlobalConstants::L01_CAVITY_REVERSE_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::L01_CAVITY_REVERSE_PHASE);// = TraceData();
		addToTraceDataMap(GlobalConstants::L01_CAVITY_PROBE_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::L01_CAVITY_PROBE_PHASE);// = TraceData();
	}
	std::stringstream ss;
	ss << "Added trace names: ";
	for (auto&& it : trace_data_map)
	{

		ss << it.first;
		ss << ", ";
	}
	messenger.printDebugMessage(ss.str());

}
void LLRF::addToTraceDataMap(const std::string& name)
{
	trace_data_map[name];
	auto& td = trace_data_map.at(name);
	td.name = name;
	setTraceDataBufferSize(td, td.trace_data_buffer_size);
}

void LLRF::setTraceDataBufferSize(TraceData& trace_data, const size_t new_size)
{
	//  init circular buffer and fill it with dummy data 
	std::pair<epicsTimeStamp, std::vector<double>> temp_all_trace_item;
	temp_all_trace_item.first = epicsTimeStamp();
	std::vector<double> temp2(trace_data.trace_data_size, GlobalConstants::double_min);
	temp_all_trace_item.second = temp2;
	trace_data.trace_data_buffer.assign(new_size, temp_all_trace_item);
}
bool LLRF::setTraceDataBufferSize(const std::string& name, const size_t new_size)
{
	std::string trace_name = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, trace_name))
	{
		setTraceDataBufferSize(trace_data_map.at(trace_name), new_size);
		return true;
	}
	return false;
}



void LLRF::addDummyTraces(const std::map<std::string, std::string>& paramMap)
{
	// for each cavity there are dummy traces,
	// they have been given specific names in the config for each cavity 
	
	if (machine_area == TYPE::LRRG_GUN)
	{
		messenger.printDebugMessage("Adding dummy traces for LRRG");
		addDummyTrace(paramMap, "kfpow_dummy_trace", kfpow_dummy_trace);
		addDummyTrace(paramMap, "krpow_dummy_trace", krpow_dummy_trace);
		addDummyTrace(paramMap, "kfpha_dummy_trace", kfpha_dummy_trace);
		addDummyTrace(paramMap, "krpha_dummy_trace", krpha_dummy_trace);
		addDummyTrace(paramMap, "lrrg_cfpow_dummy_trace", cfpow_dummy_trace);
		addDummyTrace(paramMap, "lrrg_crpow_dummy_trace", crpow_dummy_trace);
		addDummyTrace(paramMap, "lrrg_cfpha_dummy_trace", cfpha_dummy_trace);
		addDummyTrace(paramMap, "lrrg_crpha_dummy_trace", crpha_dummy_trace);
		messenger.printDebugMessage("Added Dummy traces ");
		messenger.printDebugMessage("kfpow_dummy_trace", kfpow_dummy_trace.size());
		messenger.printDebugMessage("krpow_dummy_trace", krpow_dummy_trace.size());
		messenger.printDebugMessage("kfpha_dummy_trace", kfpha_dummy_trace.size());
		messenger.printDebugMessage("krpha_dummy_trace", krpha_dummy_trace.size());
		messenger.printDebugMessage("lrrg_cfpow_dummy_trace", cfpow_dummy_trace.size());
		messenger.printDebugMessage("lrrg_crpow_dummy_trace", crpow_dummy_trace.size());
		messenger.printDebugMessage("lrrg_cfpha_dummy_trace", cfpha_dummy_trace.size());
		messenger.printDebugMessage("lrrg_crpha_dummy_trace", crpha_dummy_trace.size());
	}
	if (machine_area == TYPE::HRRG_GUN)
	{
		messenger.printDebugMessage("Adding dummy traces for HRRG");
		addDummyTrace(paramMap, "kfpow_dummy_trace", kfpow_dummy_trace);
		addDummyTrace(paramMap, "krpow_dummy_trace", krpow_dummy_trace);
		addDummyTrace(paramMap, "kfpha_dummy_trace", kfpha_dummy_trace);
		addDummyTrace(paramMap, "krpha_dummy_trace", krpha_dummy_trace);
		addDummyTrace(paramMap, "hrrg_cfpow_dummy_trace", cfpow_dummy_trace);
		addDummyTrace(paramMap, "hrrg_crpow_dummy_trace", crpow_dummy_trace);
		addDummyTrace(paramMap, "hrrg_cfpha_dummy_trace", cfpha_dummy_trace);
		addDummyTrace(paramMap, "hrrg_crpha_dummy_trace", crpha_dummy_trace);
		messenger.printDebugMessage("Added Dummy traces ");
		messenger.printDebugMessage("kfpow_dummy_trace",kfpow_dummy_trace.size());
		messenger.printDebugMessage("krpow_dummy_trace", krpow_dummy_trace.size());
		messenger.printDebugMessage("kfpha_dummy_trace", kfpha_dummy_trace.size());
		messenger.printDebugMessage("krpha_dummy_trace", krpha_dummy_trace.size());
		messenger.printDebugMessage("hrrg_cfpow_dummy_trace", cfpow_dummy_trace.size());
		messenger.printDebugMessage("hrrg_crpow_dummy_trace", crpow_dummy_trace.size());
		messenger.printDebugMessage("hrrg_cfpha_dummy_trace", cfpha_dummy_trace.size());
		messenger.printDebugMessage("hrrg_crpha_dummy_trace", crpha_dummy_trace.size());
	}
	if (machine_area == TYPE::L01)
	{
		messenger.printDebugMessage("Adding dummy traces for L01");
		addDummyTrace(paramMap, "l01_kfpow_dummy_trace", kfpow_dummy_trace);
		addDummyTrace(paramMap, "l01_krpow_dummy_trace", krpow_dummy_trace);
		addDummyTrace(paramMap, "l01_kfpha_dummy_trace", kfpha_dummy_trace);
		addDummyTrace(paramMap, "l01_krpha_dummy_trace", krpha_dummy_trace);
		addDummyTrace(paramMap, "l01_cfpow_dummy_trace", cfpow_dummy_trace);
		addDummyTrace(paramMap, "l01_crpow_dummy_trace", crpow_dummy_trace);
		addDummyTrace(paramMap, "l01_cfpha_dummy_trace", cfpha_dummy_trace);
		addDummyTrace(paramMap, "l01_crpha_dummy_trace", crpha_dummy_trace);

		messenger.printDebugMessage("Added Dummy traces ");
		messenger.printDebugMessage("l01_kfpow_dummy_trace", kfpow_dummy_trace.size());
		messenger.printDebugMessage("l01_krpow_dummy_trace", krpow_dummy_trace.size());
		messenger.printDebugMessage("l01_kfpha_dummy_trace", kfpha_dummy_trace.size());
		messenger.printDebugMessage("l01_krpha_dummy_trace", krpha_dummy_trace.size());
		messenger.printDebugMessage("l01_cfpow_dummy_trace", cfpow_dummy_trace.size());
		messenger.printDebugMessage("l01_crpow_dummy_trace", crpow_dummy_trace.size());
		messenger.printDebugMessage("l01_cfpha_dummy_trace", cfpha_dummy_trace.size());
		messenger.printDebugMessage("l01_crpha_dummy_trace", crpha_dummy_trace.size());
	}
	addDummyTrace(paramMap, "time_vector", time_vector);




}


void LLRF::addDummyTrace(const std::map<std::string, std::string>& paramMap, const std::string& trace_name, std::vector< double>& trace_to_update)
{
	std::vector<std::string> dummy_trace_string;
	if (GlobalFunctions::entryExists(paramMap, trace_name))
	{
		boost::split(dummy_trace_string, paramMap.find(trace_name)->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{	//messenger.printDebugMessage(value);
			//if(trace_name == "l01_kfpow_dummy_trace")
			//{
			//	messenger.printDebugMessage(value);
			//}
			trace_to_update.push_back(std::stof(value));
		}
	}
	else
	{
		messenger.printDebugMessage("!!ERROR!! can't find " + trace_name);
	}
}





//____________________________________________________________________________________________
std::string LLRF::fullLLRFTraceName(const std::string& name_in)const
{
	using namespace GlobalConstants;
	std::string name = name_in;
	if (name == KRPOW)
	{
		name = KLYSTRON_REVERSE_POWER;
	}
	else if (name == KRPHA)
	{
		name = KLYSTRON_REVERSE_PHASE;
	}
	else if (name == KFPOW)
	{
		name = KLYSTRON_FORWARD_POWER;
	}
	else if (name == KFPHA)
	{
		name = KLYSTRON_FORWARD_PHASE;
	}
	else if (name == CRPOW)
	{
		name = CAVITY_REVERSE_PHASE;
	}
	else if (name == CRPHA)
	{
		name = CAVITY_REVERSE_PHASE;
	}
	else if (name == CFPHA)
	{
		name = CAVITY_FORWARD_PHASE;
	}
	else if (name == CFPOW)
	{
		name = CAVITY_FORWARD_POWER;
	}
	else if (name == CPPOW)
	{
		name = CAVITY_PROBE_POWER;
	}
	else if (name == CPPHA)
	{
		name = CAVITY_PROBE_PHASE;
	}


	if (machine_area == TYPE::HRRG_GUN )
	{
		if (name == CAVITY_REVERSE_PHASE)
			return HRRG_CAVITY_REVERSE_PHASE;

		if (name == CAVITY_FORWARD_PHASE)
			return HRRG_CAVITY_FORWARD_PHASE;

		if (name == CAVITY_REVERSE_POWER)
			return HRRG_CAVITY_REVERSE_POWER;

		if (name == CAVITY_FORWARD_POWER)
			return HRRG_CAVITY_FORWARD_POWER;

		if (name == CAVITY_PROBE_POWER)
			return HRRG_CAVITY_PROBE_POWER;

		if (name == CAVITY_PROBE_PHASE)
			return HRRG_CAVITY_PROBE_PHASE;
	}
	else if (machine_area == TYPE::LRRG_GUN)
	{
		if (name == CAVITY_REVERSE_PHASE)
			return LRRG_CAVITY_REVERSE_PHASE;

		if (name == CAVITY_FORWARD_PHASE)
			return LRRG_CAVITY_FORWARD_PHASE;

		if (name == CAVITY_REVERSE_POWER)
			return LRRG_CAVITY_REVERSE_POWER;

		if (name == CAVITY_FORWARD_POWER)
			return LRRG_CAVITY_FORWARD_POWER;
	}
	else if (machine_area == TYPE::L01)
	{
		if (name == CAVITY_REVERSE_PHASE)
			return L01_CAVITY_REVERSE_PHASE;

		if (name == CAVITY_FORWARD_PHASE)
			return L01_CAVITY_FORWARD_PHASE;

		if (name == CAVITY_REVERSE_POWER)
			return L01_CAVITY_REVERSE_POWER;

		if (name == CAVITY_FORWARD_POWER)
			return L01_CAVITY_FORWARD_POWER;

		if (name == CAVITY_PROBE_POWER)
			return L01_CAVITY_PROBE_POWER;

		if (name == CAVITY_PROBE_PHASE)
			return L01_CAVITY_PROBE_PHASE;
	}

	return name;
}

void LLRF::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LLRF::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void LLRF::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void LLRF::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}
