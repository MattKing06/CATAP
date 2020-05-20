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
mean_start_index(GlobalConstants::zero_sizet),
mean_stop_index(GlobalConstants::zero_sizet),
trace_cut_mean(GlobalConstants::double_min),
trace_data_buffer_size(GlobalConstants::two_sizet)
{

}

TraceData::TraceData(const TraceData& copy_trace_data) :
trace_data_size(copy_trace_data.trace_data_size),
name(copy_trace_data.name),
trace_max(copy_trace_data.trace_max),
mean_start_index(copy_trace_data.mean_start_index),
mean_stop_index(copy_trace_data.mean_stop_index),
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
	trace_data_size(1017),
	// calls copy constructor and destroys 
	epicsInterface(boost::make_shared<EPICSLLRFInterface>(EPICSLLRFInterface()))
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LLRF Constructor");
	epicsInterface->ownerName = hardwareName;
	setPVStructs();

	boost::split(aliases, paramMap.find("name_alias")->second, [](char c) {return c == ','; });
	// TODOD add in the channel number to trace name map data
	//boost::split(aliases, paramMap.find("chanel_to_trace_map")->second, [](char c) {return c == ','; });
	

	addDummyTraces(paramMap);

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
		messenger.printDebugMessage(record = " PV  = " + pvStructs[record].fullPVName);
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}




bool LLRF::setAmp(double value)
{
	amp_sp.second = value;
	return true;
}
bool LLRF::setPhi(double value)
{
	phi_sp.second = value;
	return true;
}
bool LLRF::setAmpMW(double value)
{
	messenger.printDebugMessage("setAmpMW");
	// atm we are going to fake power traces
	amp_MW.second = value;
	scaleAllDummyTraces();
	messenger.printDebugMessage("fin");
	return true;
}
bool LLRF::setPhiDEG(double value)
{
	phi_degrees.second = value;
	return true;
}

double LLRF::getAmp()const
{
	return amp_sp.second;
}
double LLRF::getPhi()const
{
	return phi_sp.second;
}
double LLRF::getAmpMW()const
{
	return amp_MW.second;
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



void LLRF::setMachineArea(const TYPE area)
{
	machine_area = area;
	machine_area_str = ENUM_TO_STRING(machine_area);
	messenger.printDebugMessage("NEW Machien Area =  " + machine_area_str);
	setTraceDataMap();
}

void  LLRF::scaleAllDummyTraces()
{
	messenger.printDebugMessage("scaleAllDummyTraces()  ");
	using namespace GlobalConstants;
	messenger.printDebugMessage("KLYSTRON_FORWARD_POWER");
	scaleDummyTrace(trace_data_map.at(KLYSTRON_FORWARD_POWER), kfpow_dummy_trace);
	messenger.printDebugMessage("KLYSTRON_REVERSE_POWER");
	scaleDummyTrace(trace_data_map.at(KLYSTRON_REVERSE_POWER), krpow_dummy_trace);
	messenger.printDebugMessage("CAVITY_FORWARD_POWER");
	messenger.printDebugMessage(fullLLRFTraceName(CAVITY_FORWARD_POWER));
	scaleDummyTrace(trace_data_map.at(fullLLRFTraceName(CAVITY_FORWARD_POWER)), cfpow_dummy_trace);
	messenger.printDebugMessage("CAVITY_REVERSE_POWER");
	scaleDummyTrace(trace_data_map.at(fullLLRFTraceName(CAVITY_REVERSE_POWER)), crpow_dummy_trace);
	messenger.printDebugMessage("calling updateTraceCutMeans");
	updateTraceCutMeans();
}


void LLRF::scaleDummyTrace(TraceData& trace_data, const std::vector<double>& dummy_trace)
{
	std::pair<epicsTimeStamp, std::vector<double>> dummy_trace_item;
	dummy_trace_item.first = epicsTimeStamp();
	dummy_trace_item.second = dummy_trace;

	double scale_factor = amp_MW.second * 1000000.0;
	for (auto& i : dummy_trace_item.second)
	{
		i = i * scale_factor;
	}
	trace_data.trace_data_buffer.push_back(dummy_trace_item);
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
bool LLRF::setMeanStartIndex(const std::string& name, size_t  value)
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		if (trace_data_map.at(n).trace_data_size - 1 >= value)
		{
			trace_data_map.at(n).mean_start_index = value;
			return true;
		}
	}
	//    else
	//        message(name," NOT FOUND!");
	return false;
}
//-------------------------------------------------------------------------------------------------------------------
size_t LLRF::getMeanStopIndex(const std::string& name)const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).mean_stop_index;
	}
	return GlobalConstants::double_min;
}
//-------------------------------------------------------------------------------------------------------------------
size_t LLRF::getMeanStartIndex(const std::string& name)const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).mean_start_index;
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
			trace_data_map.at(n).mean_stop_index = value;
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
	if (trace.mean_stop_index == trace.mean_start_index)
	{
		trace.trace_cut_mean = trace.trace_data_buffer.back().second[trace.mean_start_index];
	}
	else if (trace.mean_stop_index > trace.mean_start_index)
	{
		trace.trace_cut_mean = 
			std::accumulate(
				trace.trace_data_buffer.back().second.cbegin() + trace.mean_start_index,
				trace.trace_data_buffer.back().second.cbegin() + trace.mean_stop_index,
				GlobalConstants::zero_double) / (trace.mean_stop_index - trace.mean_start_index);
	}
}

void LLRF::setTraceDataMap()
{
	messenger.printDebugMessage(getHardwareName() + ", Settign the Trace Data Map");
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
		addToTraceDataMap(GlobalConstants::CAVITY_FORWARD_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::CAVITY_FORWARD_PHASE);// = TraceData();
		addToTraceDataMap(GlobalConstants::CAVITY_REVERSE_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::CAVITY_REVERSE_PHASE);// = TraceData();
		addToTraceDataMap(GlobalConstants::CAVITY_PROBE_POWER);// = TraceData();
		addToTraceDataMap(GlobalConstants::CAVITY_PROBE_PHASE);// = TraceData();
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
	std::vector<std::string> dummy_trace_string;
	if (GlobalFunctions::entryExists(paramMap, "kfpow_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("kfpow_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			kfpow_dummy_trace.push_back(std::stof(value));
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "krpow_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("krpow_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			krpow_dummy_trace.push_back(std::stof(value));
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "kfpha_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("kfpha_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			kfpha_dummy_trace.push_back(std::stof(value));
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "krpha_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("krpha_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			krpha_dummy_trace.push_back(std::stof(value));
		}
	}
	if (machine_area == TYPE::LRRG_GUN)
	{
		if (GlobalFunctions::entryExists(paramMap, "lrrg_cfpow_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("lrrg_cfpow_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				cfpow_dummy_trace.push_back(std::stof(value));
			}
		}
		if (GlobalFunctions::entryExists(paramMap, "lrrg_crpow_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("lrrg_crpow_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				crpow_dummy_trace.push_back(std::stof(value));
			}
		}
		if (GlobalFunctions::entryExists(paramMap, "lrrg_cfpha_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("lrrg_cfpha_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				cfpha_dummy_trace.push_back(std::stof(value));
			}
		}
		if (GlobalFunctions::entryExists(paramMap, "lrrg_crpha_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("lrrg_crpha_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				crpha_dummy_trace.push_back(std::stof(value));
			}
		}
	}
	else if (machine_area == TYPE::HRRG_GUN)

	{
		if (GlobalFunctions::entryExists(paramMap, "hrrg_cfpow_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("hrrg_cfpow_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				cfpow_dummy_trace.push_back(std::stof(value));
			}
		}
		if (GlobalFunctions::entryExists(paramMap, "hrrg_crpow_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("hrrg_crpow_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				crpow_dummy_trace.push_back(std::stof(value));
			}
		}
		if (GlobalFunctions::entryExists(paramMap, "hrrg_cfpha_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("hrrg_cfpha_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				cfpha_dummy_trace.push_back(std::stof(value));
			}
		}
		if (GlobalFunctions::entryExists(paramMap, "hrrg_crpha_dummy_trace"))
		{
			boost::split(dummy_trace_string, paramMap.find("hrrg_crpha_dummy_trace")->second, [](char c) {return c == ','; });
			for (auto value : dummy_trace_string)
			{
				//messenger.printDebugMessage(value);
				crpha_dummy_trace.push_back(std::stof(value));
			}
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "time_vector"))
	{
		boost::split(dummy_trace_string, paramMap.find("time_vector")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			time_vector.push_back(std::stof(value));
		}
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
