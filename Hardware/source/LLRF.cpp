#include <LLRF.h>
#include "boost/algorithm/string/split.hpp"
#include "LLRFPVRecords.h"
#include "GlobalFunctions.h"
#include <numeric>
#include <mutex>


std::mutex llrf_mtx;    // mutex for critical section



TraceData::TraceData():
trace_data_size(1017),
one_record_start_index(GlobalConstants::size_zero),
one_record_stop_index(GlobalConstants::size_zero),
one_record_part(GlobalConstants::size_zero),
name("unknown"),
trace_max(GlobalConstants::double_min),
mean_start_stop(std::pair<size_t,size_t>( GlobalConstants::size_zero,GlobalConstants::size_zero)), // init to zero
mean_start_stop_time(std::pair<double, double>(GlobalConstants::zero_double, GlobalConstants::zero_double)), // init to zero
//mean_start_index(GlobalConstants::zero_sizet),
//mean_stop_index(GlobalConstants::zero_sizet),
//trace_cut_mean(GlobalConstants::double_min),
trace_type(TYPE::UNKNOWN_TYPE),
interlock_state(std::make_pair(epicsTimeStamp(), false)),
trace_data_buffer_size(GlobalConstants::five_sizet),
scan(std::make_pair(epicsTimeStamp(),STATE::UNKNOWN)), 
acqm(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
keep_rolling_average(false),
has_average(false),
check_mask(false),
rolling_average_size(GlobalConstants::one_sizet),
rolling_average_count(GlobalConstants::one_sizet)
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
trace_cut_mean(copy_trace_data.trace_cut_mean),
scan(copy_trace_data.scan),
acqm(copy_trace_data.acqm)
{}
TraceData::~TraceData(){}


LLRFInterlock::LLRFInterlock() :
	name("UNKNOWN_NAME"),
	u_level(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	p_level(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	pdbm_level(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	status(std::make_pair(epicsTimeStamp(), false)),
	interlock_type(TYPE::UNKNOWN_TYPE),
	enable(false)
{}


LLRFInterlock::LLRFInterlock(const LLRFInterlock& copy_obj):
	name(copy_obj.name),
u_level(copy_obj.u_level),
p_level(copy_obj.p_level), 
pdbm_level(copy_obj.pdbm_level),
status(copy_obj.status),
enable(copy_obj.enable)
{}

LLRFInterlock::~LLRFInterlock(){}

LLRF::LLRF()
{
}

LLRF::LLRF(const std::map<std::string, std::string>& paramMap,const STATE mode) :
	Hardware(paramMap, mode),
	dbr_all_trace_data_not_malloced(true),
	allMainChannelTraceNames{ LLRFRecords::CH1_PWR_REM,LLRFRecords::CH2_PWR_REM,
	LLRFRecords::CH3_PWR_REM,LLRFRecords::CH4_PWR_REM,
	LLRFRecords::CH5_PWR_REM,LLRFRecords::CH6_PWR_REM,
	LLRFRecords::CH7_PWR_REM,LLRFRecords::CH8_PWR_REM,
	LLRFRecords::CH1_PHASE_REM,LLRFRecords::CH2_PHASE_REM,
	LLRFRecords::CH3_PHASE_REM,LLRFRecords::CH4_PHASE_REM,
	LLRFRecords::CH5_PHASE_REM,LLRFRecords::CH6_PHASE_REM,
	LLRFRecords::CH7_PHASE_REM,LLRFRecords::CH8_PHASE_REM },
	allChannelTraceNames{ LLRFRecords::CH1_AMP_DER,LLRFRecords::CH2_AMP_DER,
	LLRFRecords::CH3_AMP_DER,LLRFRecords::CH4_AMP_DER,
	LLRFRecords::CH5_AMP_DER,LLRFRecords::CH6_AMP_DER,
	LLRFRecords::CH7_AMP_DER,LLRFRecords::CH8_AMP_DER,
	LLRFRecords::CH1_PHASE_DER,LLRFRecords::CH2_PHASE_DER,
	LLRFRecords::CH3_PHASE_DER,LLRFRecords::CH4_PHASE_DER,
	LLRFRecords::CH5_PHASE_DER,LLRFRecords::CH6_PHASE_DER,
	LLRFRecords::CH7_PHASE_DER,LLRFRecords::CH8_PHASE_DER,
	LLRFRecords::CH1_PHASE_REM,LLRFRecords::CH2_PHASE_REM,
	LLRFRecords::CH3_PHASE_REM,LLRFRecords::CH4_PHASE_REM,
	LLRFRecords::CH5_PHASE_REM,LLRFRecords::CH6_PHASE_REM,
	LLRFRecords::CH7_PHASE_REM,LLRFRecords::CH8_PHASE_REM,
	LLRFRecords::CH1_PWR_LOC,LLRFRecords::CH2_PWR_LOC,
	LLRFRecords::CH3_PWR_LOC,LLRFRecords::CH4_PWR_LOC,
	LLRFRecords::CH5_PWR_LOC,LLRFRecords::CH6_PWR_LOC,
	LLRFRecords::CH7_PWR_LOC,LLRFRecords::CH8_PWR_LOC,
	LLRFRecords::CH1_PWR_REM,LLRFRecords::CH2_PWR_REM,
	LLRFRecords::CH3_PWR_REM,LLRFRecords::CH4_PWR_REM,
	LLRFRecords::CH5_PWR_REM,LLRFRecords::CH6_PWR_REM,
	LLRFRecords::CH7_PWR_REM,LLRFRecords::CH8_PWR_REM },
	dbr_all_trace_data(nullptr),
	dummy_trace_data(TraceData()),
	amp_sp(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	amp_ff(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	phi_sp(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	phi_ff(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	amp_MW(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	phi_degrees(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	max_amp_sp(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	llrf_pulse_duration(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	trig_source(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	interlock_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	rf_output(std::make_pair(epicsTimeStamp(), false)),
	ff_phase_lock(std::make_pair(epicsTimeStamp(), false)),
	ff_amp_lock(std::make_pair(epicsTimeStamp(), false)),
	heartbeat(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	new_outside_mask_event(false),
	is_trace_monitoring(false),
	trace_rep_rate(GlobalConstants::double_min),
	//mean_freq(GlobalConstants::double_min),
	//shot_to_shot_freq(GlobalConstants::double_min),
	num_traces_to_estimate_rep_rate(GlobalConstants::one_hundred_sizet),
	duplicate_pulse_count(GlobalConstants::zero_sizet),
	active_pulse_count(GlobalConstants::zero_sizet),
	inactive_pulse_count(GlobalConstants::zero_sizet),
	epicsInterface(boost::make_shared<EPICSLLRFInterface>(EPICSLLRFInterface()))// calls copy constructor and destroys 
{
	messenger.printDebugMessage("LLRF Constructor");
	messenger.printDebugMessage("epicsInterface set ownerName ");
	epicsInterface->ownerName = hardwareName;
	
	// TODOD add to initilization list 
	std::get<GlobalConstants::size_zero>(dummy_amp_set_kly_fwd_rs_state) = GlobalConstants::size_zero;
	std::get<GlobalConstants::size_zero>(dummy_amp_set_kly_fwd_rs_state) = GlobalConstants::size_zero;
	std::get<GlobalConstants::size_zero>(dummy_amp_set_kly_fwd_rs_state) = GlobalConstants::size_zero;


	setPVStructs();
	
	setMasterLatticeData(paramMap);

	//buildChannelToTraceSourceMap(paramMap);
	//boost::split(aliases, paramMap.find("chanel_to_trace_map")->second, [](char c) {return c == ','; });
	//does not work???? 
	//setupTraceChannels(paramMap);
}

LLRF::LLRF(const LLRF& copyLLRF) :
	Hardware(copyLLRF),
	trace_data_size(copyLLRF.trace_data_size),
	amp_sp(copyLLRF.amp_sp),
	amp_ff(copyLLRF.amp_ff),
	phi_sp(copyLLRF.phi_sp),
	phi_ff(copyLLRF.phi_ff),
	amp_MW(copyLLRF.amp_MW),
	phi_degrees(copyLLRF.phi_degrees),
	max_amp_sp(copyLLRF.max_amp_sp),
	pulse_shape(copyLLRF.pulse_shape),
	kly_fwd_power_max(copyLLRF.kly_fwd_power_max),
	trig_source(copyLLRF.trig_source),
	llrf_pulse_duration(copyLLRF.llrf_pulse_duration),
	pulse_offset(copyLLRF.pulse_offset),
	heartbeat(copyLLRF.heartbeat),
	trace_data_buffer_size(copyLLRF.trace_data_buffer_size),
	interlock_state(copyLLRF.interlock_state),
	rf_output(copyLLRF.rf_output),
	ff_phase_lock(copyLLRF.ff_phase_lock),
	ff_amp_lock(copyLLRF.ff_amp_lock),
	trace_data_map(copyLLRF.trace_data_map),
	trace_ACQM_map(copyLLRF.trace_ACQM_map),
	trace_SCAN_map(copyLLRF.trace_SCAN_map),
	all_trace_interlocks(copyLLRF.all_trace_interlocks),
	dbr_all_trace_data(copyLLRF.dbr_all_trace_data),
	epicsInterface(copyLLRF.epicsInterface)
{}

LLRF::~LLRF(){}


// Functions for settigng up this LLRF (probably overly complex, good luck with the refactoring :^) 
void LLRF::setPVStructs()
{
	for (auto&& record : LLRFRecords::llrfRecordList)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, record))
		{
			messenger.printDebugMessage("LLRF PV record = " + record);
			pvStructs[record] = pvStruct();
			pvStructs[record].pvRecord = record;
			std::string PV = specificHardwareParameters.find(record)->second.data();
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
			messenger.printDebugMessage(record, " = ", "PV  = " + pvStructs[record].fullPVName);
		}
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		//messenger.printDebugMessage("Constructing PV information for ", record);
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}
void LLRF::setMasterLatticeData(const std::map<std::string, std::string>& paramMap)
{
	// "master_hardware" file should check for these too
	messenger.printDebugMessage(hardwareName, " find crest_phase");
	if (GlobalFunctions::entryExists(paramMap, "crest_phase"))
	{
		crest_phase = std::stof(paramMap.find("crest_phase")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find crest_phase");
	}

	messenger.printDebugMessage(hardwareName, " find trace_data_size");
	if (GlobalFunctions::entryExists(paramMap, "trace_data_size"))
	{
		messenger.printDebugMessage("p1");
		trace_data_size = std::stoul(paramMap.find("trace_data_size")->second);
		
		// setup vectors that are trace_data_size
		time_vector.resize(trace_data_size);

		messenger.printDebugMessage("p2");

		temp_TraceData_trace_data_buffer = std::make_pair(epicsTimeStamp(), std::vector<double>(trace_data_size, GlobalConstants::double_min));
		messenger.printDebugMessage("p3");

	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find trace_data_size");
	}

	messenger.printDebugMessage(hardwareName, " find crest_phase");
	if (GlobalFunctions::entryExists(paramMap, "crest_phase"))
	{
		crest_phase = std::stoul(paramMap.find("crest_phase")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find crest_phase");
	}
	// TODO name_alias should be in harwdare constructor?? 
	messenger.printDebugMessage(hardwareName, " find name_alias");
	if (GlobalFunctions::entryExists(paramMap, "name_alias"))
	{
		// TODOD white space trimiming probably required here for robustness 
		boost::split(aliases, paramMap.find("name_alias")->second, [](char c) {return c == ','; });


		for (auto&& next_alias : aliases)
		{
			messenger.printMessage("Found name alias = \"", next_alias, "\"");
		}

	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find name_alias");
	}
	if (GlobalFunctions::entryExists(paramMap, "one_trace_data_count"))
	{
		one_record_trace_data_size = std::stoul(paramMap.find("one_trace_data_count")->second);
		// TODOD white space trimiming probably required here for robustness 
		unsigned nBytes = dbr_size_n(DBR_TIME_DOUBLE, one_record_trace_data_size);
		dbr_all_trace_data = (struct dbr_time_double*)malloc(nBytes);
		messenger.printDebugMessage(hardwareName, " dbr_all_trace_data pointer allocated ", nBytes, " BYTES (", one_record_trace_data_size, " elements)");
		dbr_all_trace_data_not_malloced = false;
		for (auto&& next_alias : aliases)
		{
			messenger.printMessage("Found name alias = \"", next_alias, "\"");
		}
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!ERROR!! could not find one_trace_data_count");
	}
}
void LLRF::setGunType(const TYPE area) // called from factory
{
	machine_area = area;
	machine_area_str = ENUM_TO_STRING(machine_area);
	messenger.printDebugMessage("NEW Machine Area =  " + machine_area_str);
}
void LLRF::setupAfterMachineAreaSet()
{
	//messenger.printDebugMessage("PRINT specificHardwareParameters");
	//for (auto&& it : specificHardwareParameters)
	//{
	//	messenger.printDebugMessage(it.first, " = ", it.second);
	//}
	// 'manually' set-up the trace data map for each cavity 
	setTraceDataMap();
	// Set up the trace data indexes in the "one-record" concatenated trace-data
	// the below is nto pretty, i'm going to try adnget it to "work" and then worry about makign it easier to follow / read. 
	// if me or anyone else comes back to look at this in the far-flung-future its probably going to look crazy af 
	// part of teh reason fro thsi is taht the gun LLRF actually is for two seperate cavities thata re mutlaly expclusive, (but conatined one file!)
	// this was probably a bad design chocie ... 
	// we loop over all possible ONE_TRACE settings and see if they are in the master-lattice file AND in all_trace_data 
	// 
	std::vector<std::string>all_one_record_trace_names = {
	LLRFRecords::ONE_RECORD_KLYSTRON_FORWARD_POWER, LLRFRecords::ONE_RECORD_KLYSTRON_FORWARD_PHASE,
		LLRFRecords::ONE_RECORD_KLYSTRON_REVERSE_POWER,	  LLRFRecords::ONE_RECORD_KLYSTRON_REVERSE_PHASE,
		LLRFRecords::ONE_RECORD_LRRG_CAVITY_FORWARD_POWER,LLRFRecords::ONE_RECORD_LRRG_CAVITY_FORWARD_PHASE,
		LLRFRecords::ONE_RECORD_LRRG_CAVITY_REVERSE_POWER,LLRFRecords::ONE_RECORD_LRRG_CAVITY_REVERSE_PHASE,
		LLRFRecords::ONE_RECORD_HRRG_CAVITY_FORWARD_POWER,LLRFRecords::ONE_RECORD_HRRG_CAVITY_FORWARD_PHASE,
		LLRFRecords::ONE_RECORD_HRRG_CAVITY_REVERSE_POWER,LLRFRecords::ONE_RECORD_HRRG_CAVITY_REVERSE_PHASE,
		LLRFRecords::ONE_RECORD_HRRG_CAVITY_PROBE_POWER,  LLRFRecords::ONE_RECORD_HRRG_CAVITY_PROBE_PHASE,
		LLRFRecords::ONE_RECORD_L01_CAVITY_FORWARD_POWER, LLRFRecords::ONE_RECORD_L01_CAVITY_FORWARD_PHASE,
		LLRFRecords::ONE_RECORD_L01_CAVITY_REVERSE_POWER, LLRFRecords::ONE_RECORD_L01_CAVITY_REVERSE_PHASE,
		LLRFRecords::ONE_RECORD_L01_CAVITY_PROBE_POWER,	  LLRFRecords::ONE_RECORD_L01_CAVITY_PROBE_PHASE,
		LLRFRecords::ONE_RECORD_CALIBRATION_POWER,		  LLRFRecords::ONE_RECORD_CALIBRATION_PHASE };
	std::string one_record_prefix = "ONE_RECORD_";
	auto one_record_prefix_len = one_record_prefix.length();
	for (auto&& or_tn : all_one_record_trace_names)
	{
		std::string or_tn_copy = or_tn;
		if (GlobalFunctions::entryExists(specificHardwareParameters, or_tn))
		{
			messenger.printDebugMessage(or_tn, "  exists in master lattice yaml file ");

			size_t pos = or_tn.find(one_record_prefix);
			if (pos != std::string::npos)
			{
				// If found then erase it from string
				or_tn_copy.erase(pos, one_record_prefix_len);
			}
			messenger.printDebugMessage("Looking in all_trace_data for ", or_tn_copy);
			if (GlobalFunctions::entryExists(trace_data_map, or_tn_copy))
			{
				trace_data_map.at(or_tn_copy).one_record_part = std::stoul(specificHardwareParameters.find(or_tn)->second);
				trace_data_map.at(or_tn_copy).one_record_start_index = (trace_data_map.at(or_tn_copy).one_record_part - GlobalConstants::one_sizet) * 1024;
				trace_data_map.at(or_tn_copy).one_record_stop_index = trace_data_map.at(or_tn_copy).one_record_part * 1024 - GlobalConstants::one_sizet - 
					(1024 - 1017);
				messenger.printDebugMessage(or_tn_copy, "  exists in trace_data_map, adding paramters, ", trace_data_map.at(or_tn_copy).one_record_part, " ",
					trace_data_map.at(or_tn_copy).one_record_start_index, " ", trace_data_map.at(or_tn_copy).one_record_stop_index);
			}
			else
			{
				messenger.printDebugMessage(or_tn_copy, " does not exists in trace_data_map");
			}
		}
	}
	// once we have the one_record_start_index and one_record_stop_index we will create a list so that we can iterate once over the 
	// one_record data copyign values into teh relevant object in trace_data_map 

	for (auto& it : trace_data_map)
	{
		sorted_one_record_trace_start_indices.push_back(std::make_pair(it.second.one_record_start_index, it.first));
	}
	std::sort(sorted_one_record_trace_start_indices.begin(), sorted_one_record_trace_start_indices.end());
	size_t current_index = 0;
	size_t last_index = 0;
	for (auto& it : sorted_one_record_trace_start_indices)
	{
		messenger.printDebugMessage("ITERATOR ", it.first, "  ", it.second);
		if (current_index > 0)
		{
			size_t next_jump = it.first - (sorted_one_record_trace_start_indices[current_index - 1].first + 1017);
			auto next_pair = std::make_pair(next_jump, it.second);
			sorted_one_record_trace_iterator_jumps.push_back(next_pair);
		}
		else
		{
			sorted_one_record_trace_iterator_jumps.push_back(std::make_pair(0, it.second));
		}
		current_index += 1;
		messenger.printDebugMessage("next sorted_one_record_trace_iterator_jumps = ", sorted_one_record_trace_iterator_jumps.back().first );
	}

	// now we need the number of elemnts to advance between each
	//size_t current_it_pos = 0;
	//std::vector<std::pair<std::string, size_t>> one_record_iterator_advance_steps;
	//for (auto&& it : sorted_one_record_trace_start_indices)
	//{
	//	current_it_pos
	//	sorted_one_record_trace_start_indices.push_back(std::make_pair(it.second.one_record_start_index, it.first));
	//}

	messenger.printDebugMessage(hardwareName + " is building the channel_to_tracesource_map");
	const std::vector<std::string> channels{ "CH1","CH2","CH3","CH4","CH5","CH6","CH7","CH8" };
	for (auto channel : channels)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, channel))
		{
			channel_to_tracesource_map[channel] = specificHardwareParameters.find(channel)->second;
			messenger.printDebugMessage("Found " + channel + ", connected to " + channel_to_tracesource_map[channel]);
		}
		else
		{
			messenger.printDebugMessage(channel + " is NOT in the config");
			channel_to_tracesource_map[channel] = "NONE";
		}
	}
	// if this is a virtual LLRF add some dummy traces  
	if (isVirtual())
	{
		addDummyTraces(specificHardwareParameters);
	}
	//boost::split(aliases, paramMap.find("chanel_to_trace_map")->second, [](char c) {return c == ','; });
	setupInterlocks();
	initAllTraceSCANandACQM();
	setDefaultPowerTraceMeanTimes();







}
void LLRF::setTraceDataMap()
{
	messenger.printDebugMessage(getHardwareName() + ", Setting the Trace Data Map");
	addToTraceDataMap(LLRFRecords::KLYSTRON_FORWARD_POWER);
	addToTraceDataMap(LLRFRecords::KLYSTRON_FORWARD_PHASE);
	addToTraceDataMap(LLRFRecords::KLYSTRON_REVERSE_POWER);
	addToTraceDataMap(LLRFRecords::KLYSTRON_REVERSE_PHASE);
	trace_data_map.at(LLRFRecords::KLYSTRON_FORWARD_POWER).trace_type = TYPE::POWER;
	trace_data_map.at(LLRFRecords::KLYSTRON_FORWARD_PHASE).trace_type = TYPE::PHASE;
	trace_data_map.at(LLRFRecords::KLYSTRON_REVERSE_POWER).trace_type = TYPE::POWER;
	trace_data_map.at(LLRFRecords::KLYSTRON_REVERSE_PHASE).trace_type = TYPE::PHASE;
	if (machine_area == TYPE::LRRG)
	{
		addToTraceDataMap(LLRFRecords::LRRG_CAVITY_FORWARD_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::LRRG_CAVITY_FORWARD_PHASE);// = TraceData();
		addToTraceDataMap(LLRFRecords::LRRG_CAVITY_REVERSE_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::LRRG_CAVITY_REVERSE_PHASE);// = TraceData();
		trace_data_map.at(LLRFRecords::LRRG_CAVITY_FORWARD_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::LRRG_CAVITY_FORWARD_PHASE).trace_type = TYPE::PHASE;
		trace_data_map.at(LLRFRecords::LRRG_CAVITY_REVERSE_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::LRRG_CAVITY_REVERSE_PHASE).trace_type = TYPE::PHASE;


	}
	else if (machine_area == TYPE::HRRG)
	{
		addToTraceDataMap(LLRFRecords::HRRG_CAVITY_FORWARD_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::HRRG_CAVITY_FORWARD_PHASE);// = TraceData();
		addToTraceDataMap(LLRFRecords::HRRG_CAVITY_REVERSE_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::HRRG_CAVITY_REVERSE_PHASE);// = TraceData();
		addToTraceDataMap(LLRFRecords::CAVITY_PROBE_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::CAVITY_PROBE_PHASE);// = TraceData();
		trace_data_map.at(LLRFRecords::HRRG_CAVITY_FORWARD_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::HRRG_CAVITY_FORWARD_PHASE).trace_type = TYPE::PHASE;
		trace_data_map.at(LLRFRecords::HRRG_CAVITY_REVERSE_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::HRRG_CAVITY_REVERSE_PHASE).trace_type = TYPE::PHASE;
		trace_data_map.at(LLRFRecords::CAVITY_PROBE_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::CAVITY_PROBE_PHASE).trace_type = TYPE::PHASE;
	}
	else if (machine_area == TYPE::L01)
	{
		addToTraceDataMap(LLRFRecords::L01_CAVITY_FORWARD_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::L01_CAVITY_FORWARD_PHASE);// = TraceData();
		addToTraceDataMap(LLRFRecords::L01_CAVITY_REVERSE_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::L01_CAVITY_REVERSE_PHASE);// = TraceData();
		addToTraceDataMap(LLRFRecords::L01_CAVITY_PROBE_POWER);// = TraceData();
		addToTraceDataMap(LLRFRecords::L01_CAVITY_PROBE_PHASE);// = TraceData();
						  
		trace_data_map.at(LLRFRecords::L01_CAVITY_FORWARD_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::L01_CAVITY_FORWARD_PHASE).trace_type = TYPE::PHASE;
		trace_data_map.at(LLRFRecords::L01_CAVITY_REVERSE_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::L01_CAVITY_REVERSE_PHASE).trace_type = TYPE::PHASE;
		trace_data_map.at(LLRFRecords::L01_CAVITY_PROBE_POWER).trace_type = TYPE::POWER;
		trace_data_map.at(LLRFRecords::L01_CAVITY_PROBE_PHASE).trace_type = TYPE::PHASE;
	}
	std::stringstream ss;
	ss << "Added trace names: ";
	for (auto&& it : trace_data_map)
	{
		ss << it.first;
		ss << ", ";
	}
	messenger.printDebugMessage(ss.str());
	power_trace_names.push_back(fullLLRFTraceName(LLRFRecords::KLYSTRON_FORWARD_POWER));
	power_trace_names.push_back(fullLLRFTraceName(LLRFRecords::KLYSTRON_REVERSE_POWER));
	power_trace_names.push_back(fullLLRFTraceName(LLRFRecords::CAVITY_FORWARD_POWER));
	power_trace_names.push_back(fullLLRFTraceName(LLRFRecords::CAVITY_REVERSE_POWER));
	if (machine_area != TYPE::LRRG)
	{
		power_trace_names.push_back(fullLLRFTraceName(LLRFRecords::CAVITY_PROBE_POWER));
	}
}
void LLRF::addToTraceDataMap(const std::string& name)
{
	trace_data_map[name];
	auto& td = trace_data_map.at(name);
	td.name = name;
	setTraceDataBufferSize(td, td.trace_data_buffer_size);
}
bool LLRF::setAllTraceDataBufferSize(const size_t new_size)
{
	bool all_success = true;
	for(auto& it : trace_data_map)
	{
		if( setTraceDataBufferSize(it.second, new_size))
		{ 
		}
		else
		{
			all_success = false;
		}
	}
	return all_success;
}
bool LLRF::setTraceDataBufferSize(TraceData& trace_values, const size_t new_size)
{
	//  init circular buffer and fill it with dummy data 
	std::pair<epicsTimeStamp, std::vector<double>> temp_all_trace_item;
	temp_all_trace_item.first = epicsTimeStamp();
	std::vector<double> temp2(trace_values.trace_data_size, GlobalConstants::double_min);
	temp_all_trace_item.second = temp2;
	trace_values.trace_values_buffer.assign(new_size, temp_all_trace_item);
	return trace_values.trace_values_buffer.size() == new_size;
}
bool LLRF::setTraceDataBufferSize(const std::string& name, const size_t new_size)
{
	std::string trace_name = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, trace_name))
	{
		return setTraceDataBufferSize(trace_data_map.at(trace_name), new_size);
	}
	return false;
}
void LLRF::setDefaultPowerTraceMeanTimes()
// called from factory as traces need to be set up first ! 
{
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("kfpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("kfpow_mean_end_time")->second),
		LLRFRecords::KLYSTRON_FORWARD_POWER);
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("krpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("krpow_mean_end_time")->second),
		LLRFRecords::KLYSTRON_REVERSE_POWER);
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("cfpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("cfpow_mean_end_time")->second),
		LLRFRecords::CAVITY_FORWARD_POWER);
	setMeanStartEndTime(
		std::stof(specificHardwareParameters.find("crpow_mean_start_time")->second),
		std::stof(specificHardwareParameters.find("crpow_mean_end_time")->second),
		LLRFRecords::CAVITY_REVERSE_POWER);
	if (GlobalFunctions::entryExists(specificHardwareParameters, "cppow_mean_start_time"))
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, "cppow_mean_end_time"))
		{
			setMeanStartEndTime(
				std::stof(specificHardwareParameters.find("cppow_mean_start_time")->second),
				std::stof(specificHardwareParameters.find("cppow_mean_end_time")->second),
				LLRFRecords::CAVITY_PROBE_POWER);
		}
	}
}
void LLRF::addDummyTraces(const std::map<std::string, std::string>& paramMap)
{
	// for each cavity there are dummy traces,
	// they have been given specific names in the config for each cavity 

	if (machine_area == TYPE::LRRG)
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
	if (machine_area == TYPE::HRRG)
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
		messenger.printDebugMessage("kfpow_dummy_trace", kfpow_dummy_trace.size());
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
		for (auto&& value : dummy_trace_string)
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

// functions to actually get adn set paramters in teh LLRF 
bool LLRF::setAmpMW(double value)
{
	// atm we are going to fake power traces
	amp_MW.second = value;
	//scaleAllDummyTraces();
	return true;
}
bool LLRF::setAmp(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return epicsInterface->putValue2<double>(pvStructs.at(LLRFRecords::AMP_SP), value);
	}
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
	if (mode == STATE::PHYSICAL)
	{
		return epicsInterface->putValue2<double>(pvStructs.at(LLRFRecords::PHI_SP), value);
	}
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
/*
	ok, htf is this going to work ??

	we want to be able to get a single shot of trace values - to reduce network lag

	but, always update all trace vlaues + meta- values (mean, max power etc) 

	on every update update all TraceData obejcts 

	so, functions that retunr just teh valeus, and functions that return the tracedata objects

	TraceData will include meta data, TraceValues will just be lists of numbers (keyed by trace name) 

	the have bespoke funcitons that return what the user wants. 
*/
bool LLRF::getNewTraceValuesFromEPICS(const pvStruct& pvs)
{
	messenger.printDebugMessage("getNewTraceValuesFromEPICS");
	if (epicsInterface->check_ca_state(pvs))
	{
		int status = ca_array_get(DBR_TIME_DOUBLE, one_record_trace_data_size, pvs.CHID, dbr_all_trace_data);
		EPICSInterface::sendToEPICS();
		MY_SEVCHK(status);
		if (status != ECA_NORMAL)
		{
			messenger.printDebugMessage("!!ERROR!! getNewTraceValuesFromEPICS did not return ECA_NORMAL, return false");
			return false;
		}
		return true;
	}
	messenger.printDebugMessage("getNewTraceValuesFromEPICS return false");
	return false;
}
void LLRF::updateTraceValues()
{
	auto pvs = pvStructs.at(LLRFRecords::LLRF_TRACES);
	bool carry_on = getNewTraceValuesFromEPICS( pvStructs.at(LLRFRecords::LLRF_TRACES));
	if (carry_on)
	{
		messenger.printDebugMessage("updateTraceValues carry_on = true");
		splitOneTraceValues(dbr_all_trace_data);
		updateTraceMetaData();
	}
	messenger.printDebugMessage("updateTraceValues carry_on = false");
}

//
//void LLRF::splitOneTraceValues()
//{
//	messenger.printDebugMessage("splitOneTraceValues");
//	/*
//		dbr_all_trace_data is a struct, it's 'value' member is a dbr_double_t*
//	*/
//	const dbr_double_t* pValue;
//	pValue = &dbr_all_trace_data->value;
//	auto end = &pValue[one_record_trace_data_size];
//
//	// This loop iterates over the one_record_data, 
//	size_t chunk = GlobalConstants::zero_sizet;
//	std::pair<size_t, std::string>& start_index_data = sorted_one_record_trace_start_indices.at(chunk);
//
//	size_t counter = GlobalConstants::zero_sizet;
//	for (auto it = pValue; it != end; ++it)
//	{
//		if (counter == start_index_data.first)
//		{
//			messenger.printDebugMessage("counter = ", counter);
//			messenger.printDebugMessage("chunk = ", chunk);
//			messenger.printDebugMessage("adding at it value = ", *it);
//
//			// (for ease of reading)create a ref to the TraceData we are going to update 
//			TraceData& td = trace_data_map.at(start_index_data.second);
//			// add new elment to end of cricualr buffer 
//			td.trace_values_buffer.push_back(temp_TraceData_trace_data_buffer);
//			// copy raw data into new buffer vector element 
//			std::copy(it, it + trace_data_size, td.trace_values_buffer.back().second.begin());
//
//			// next we have to convert the units into watts and offset the phase by 180, so it goes 0 to 360, not -180 to 180
//
//			if (td.trace_type == TYPE::POWER)
//			{
//				messenger.printDebugMessage(start_index_data.second," is a power trace ");
//				// reset  trace max
//				td.trace_max = GlobalConstants::double_min;
//				for (auto& v : td.trace_values_buffer.back().second)
//				{
//					/* this is the mthod to convert to watts */
//					v *= v;
//					v /= GlobalConstants::one_hundred_double;
//					/* update the trace max */
//					if (v > td.trace_max)
//					{
//						td.trace_max = v;
//					}
//				}
//			}
//			else if (td.trace_type == TYPE::PHASE)
//			{
//				messenger.printDebugMessage(start_index_data.second, " is a phase trace ");
//
//				size_t temp_counter = 0;
//				for (auto& v : td.trace_values_buffer.back().second)
//				{
//					v += GlobalConstants::one_hundred_and_eighty_double;
//
//					//std::cout << v << " ";
//					temp_counter += 1;
//				}
//				std::cout << temp_counter << "\n";
//				// in gneral unwrapPhaseTrace is not trivial, as we have no idea how many factors of 2*pi to add / subtract ... 
//				//unwrapPhaseTrace(llrf.trace_values.at(it.first));
//			}
//			//increment chunk
//			chunk += GlobalConstants::one_sizet;
//			// update the start index we are now looking for, if we have run out oftrace, thenbreak 
//			if (chunk == sorted_one_record_trace_start_indices.size())
//			{
//				messenger.printDebugMessage("Chunk incremented, but no more traces to update");
//				break;
//			}
//			else 
//			{
//				start_index_data = sorted_one_record_trace_start_indices.at(chunk);
//			}
//			messenger.printDebugMessage("Chunk incremented, Next trace pValue index = ", start_index_data.first);
//		}
//		counter += GlobalConstants::one_sizet;
//	}
//	messenger.printDebugMessage("Print some numbers ");
//	for (auto&& it : trace_data_map)
//	{
//		size_t c = GlobalConstants::zero_sizet;
//		messenger.printDebugMessage(it.first);
//		for (auto&& it2 : it.second.trace_values_buffer.back().second)
//		{
//			std::cout << it2 << " ";
//			c += GlobalConstants::one_sizet;
//			if (c == 10)
//			{
//				break;
//			}
//		}
//		std::cout << std::endl;
//	}
//}

void LLRF::updateTraceMetaData()
{
	messenger.printDebugMessage("updateTraceMetaData ");
	// First we do the klystron forward power max  (trace_max is set for each trace in splitOneTraceValues
	updateKFPowMaxAndPulseCounts();

	// If we are 'collecting Future traces' (which happens when collecting breakdown events)  
	checkCollectingFutureTraces();
	// Now check for a breakdown event, 
	checkForOutsideMaskTraceAndUpdateRollingAverages();
	/*	Update the mean values for a trace */
	updateTraceCutMeans();
	/*	estimate trace DAQ frequency */
	updateDAQFreqEstimate();
	/* by the time we get to here this MUST be false! */
	new_outside_mask_event = false;
}






void LLRF::updateKFPowMaxAndPulseCounts()
{
	messenger.printDebugMessage("updateTraceMetaData ");
	total_pulse_count +=1;
	kly_fwd_power_max.second = trace_data_map.at(LLRFRecords::KLYSTRON_FORWARD_POWER).trace_max;
	messenger.printDebugMessage("kly_fwd_power_max =  ", kly_fwd_power_max.second);
	// decide if this pule of trace_values is a duplicate, active or inactive, increment counters  
	if (last_kly_fwd_power_max == kly_fwd_power_max.second)
	{
		duplicate_pulse_count += GlobalConstants::one_sizet;
		can_increase_active_pulses = false;
	}
	else if (kly_fwd_power_max.second > active_pulse_kly_power_limit)
	{
		can_increase_active_pulses = true;
		active_pulse_count += GlobalConstants::one_sizet;
	}
	else
	{
		can_increase_active_pulses = false;
		inactive_pulse_count += GlobalConstants::one_sizet;
	}
	// update last_kly_fwd_power_max
	last_kly_fwd_power_max = kly_fwd_power_max.second;
	messenger.printDebugMessage("active, inactive, duplicate, puse,counts = ", active_pulse_count, ", ",
		inactive_pulse_count, ", ", duplicate_pulse_count);
}
void LLRF::checkCollectingFutureTraces()
{
	messenger.printDebugMessage("checkCollectingFutureTraces ");
	// copy pasta from VC controller 
	//if (llrf.omed.is_collecting)
	//{
	//	llrf.omed.num_still_to_collect -= UTL::ONE_SIZET;
	//	llrf.omed.num_collected += UTL::ONE_SIZET;

	//	message("checkCollectingFutureTraces(): Num collected = ", llrf.omed.num_collected, "/",
	//		llrf.omed.extra_traces_on_outside_mask_event + UTL::THREE_SIZET,
	//		" (", llrf.omed.num_still_to_collect, ")");

	//	// we now check if there is a breakdown in the future trace
	//	// this is basically copy-pasta from the main breakdown checking routines,
	//	// so could be better integrated with those funcitons

	//	if (llrf.can_increase_active_pulses)
	//	{
	//		message("checkCollectingFutureTraces(): checking current trace for OMED");
	//		for (auto& it : llrf.trace_values) // loop over each trace
	//		{
	//			// only check the masks we should be checking
	//			//if(it.second.check_mask && it.second.hi_mask_set && it.second.lo_mask_set)
	//			if (it.second.hi_mask_set && it.second.lo_mask_set)
	//			{
	//				message("Checking future traces for ", it.first);
	//				int result = updateIsTraceInMask(it.second);
	//				if (result == UTL::ZERO_INT)
	//				{
	//					message("checkCollectingFutureTraces(): found another breakdown in ", it.second.name);
	//					llrf.omed.num_events += UTL::ONE_SIZET;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		message("checkCollectingFutureTraces(): can't check for OMED as llrf.can_increase_active_pulses = FALSE");
	//	}
	//	//
	//	// sanity check
	//	if (llrf.omed.num_still_to_collect == UTL::ZERO_SIZET)
	//	{
	//		if (llrf.omed.num_collected == llrf.omed.extra_traces_on_outside_mask_event + UTL::THREE_SIZET)
	//		{
	//			copyTraceDataToOMED();
	//			llrf.omed.is_collecting = false;
	//			llrf.omed.can_get_data = true;
	//			message("FINSHED COLLECTING EXTRA TRACES, can_get_data = True,");
	//		}
	//		else
	//		{
	//			message("ERROR NEVER SHOW THIS MESSAGE");
	//		}
	//	}
	//	else
	//	{
	//		//message("Collecting future traces = ", llrf.omed.num_still_to_collect);
	//	}
	//}
}

void LLRF::checkForOutsideMaskTraceAndUpdateRollingAverages()
{
	messenger.printDebugMessage("checkForOutsideMaskTraceAndUpdateRollingAverages ");
	///* if we have an active pulse .. */
	if (can_increase_active_pulses)
	{
		/* ... checkForOutsideMaskTrace */
		checkForOutsideMaskTrace();
		/* update masks UNLESS there is a new_outside_mask_event */
		if (new_outside_mask_event)
		{
			//
		}
		else
		{
			/*  updates the rolling average for each trace,
				and then sets maks based on the curren trolling average
			*/
			// mutex from updateLLRFVasetMaskStartIndexlue()
			for (auto&& it : trace_data_map)
			{
				updateRollingAverage(it.second);
				/*
					we might want to check that the global check mask is true here,
					we DONT want to update masks when the RF has been switched off.
				*/
				if (it.second.check_mask && it.second.has_average)
				{
					//setNewMask(it.second);
				}
			}
			/*
				if we have can_increase_active_pulses and there was no
				event, update the amp_set kly_fwd_pwr running stats
			*/
			//update_amp_set_kly_fwd_rs();
		}
	}
	else
	{
		//message("can_increase_active_pulses = false");
	}
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
	if (trace.mean_start_stop.first == trace.mean_start_stop.second)
	{
		trace.trace_cut_mean = trace.trace_values_buffer.back().second[trace.mean_start_stop.first];
	}
	else if (trace.mean_start_stop.second > trace.mean_start_stop.first)
	{
		trace.trace_cut_mean =
			std::accumulate(
				trace.trace_values_buffer.back().second.cbegin() + trace.mean_start_stop.first,
				trace.trace_values_buffer.back().second.cbegin() + trace.mean_start_stop.second,
				GlobalConstants::zero_double) / trace.stop_minus_start;
	}
	messenger.printDebugMessage(trace.name, " New Cut mean = ", trace.trace_cut_mean);
}
void LLRF::updateDAQFreqEstimate()
{
	// mutex from updateLLRFValue()
	/* add current time to vetcor of times */
	trace_times_for_rep_rate.push_back(std::chrono::high_resolution_clock::now());
	/* if vector has desired number of entries calc rep -rate */
	if (trace_times_for_rep_rate.size() > num_traces_to_estimate_rep_rate)
	{
		/* pop first entry */
		trace_times_for_rep_rate.erase(trace_times_for_rep_rate.begin());
		/* time diff (seconds) between first and last entry */
		std::chrono::duration<double> diff = trace_times_for_rep_rate.back() - trace_times_for_rep_rate.front();
		/* divide the number of entries by diff to give rep-rate in hertz */
		trace_rep_rate = (double)trace_times_for_rep_rate.size() / diff.count();
		//messenger.printDebugMessage(getHardwareName(), "trace_rep_rate = ", trace_rep_rate);
	}
}

//--------------------------------------------------------------------------------------------------
// KFP running stats 
void LLRF::setKeepKlyFwdPwrRS(bool val)
{
	keep_kly_fwd_pow_running_stat = val;
}
void LLRF::keepKlyFwdPwrRS()
{
	setKeepKlyFwdPwrRS(true);
}
void LLRF::dontKeepKlyFwdPwrRS()
{
	setKeepKlyFwdPwrRS(false);
}
void LLRF::update_amp_set_kly_fwd_rs()
{	/*
		The amp_set_kly_fwd_rs is a running stat object of Klystron forward power for different
		amp sets. It uses the cut_mean, (so if those indices are not set this will potentially garbage
		It is only updated when there is an active pulse, AND if there was no OME Detected
	*/	// mutex from updateLLRFValue()
	if(keep_kly_fwd_pow_running_stat)
	{
		RunningStats& rs_ref = amp_set_kly_fwd_rs[(int)amp_sp.second];
		std::tuple<size_t, double, double>& rs_state_ref = amp_set_kly_fwd_rs_state[(int)amp_sp.second];

		rs_ref.Push(trace_data_map.at(LLRFRecords::KLYSTRON_FORWARD_POWER).trace_cut_mean);

		rs_ref.getCurrentState(std::get<GlobalConstants::size_zero>(rs_state_ref),
			std::get<GlobalConstants::one_sizet>(rs_state_ref),
			std::get<GlobalConstants::one_sizet>(rs_state_ref));
		//
		//        message("new val  = ", llrf.trace_data.at(UTL::KLYSTRON_FORWARD_POWER).mean,
		//                ", count    = ", std::get<UTL::ZERO_SIZET>(rs_state_ref),
		//                ", old_mean = ", std::get<UTL::ONE_SIZET>(rs_state_ref),
		//                ", old_var  = ", std::get<UTL::TWO_SIZET>(rs_state_ref) );
	}
}
//-------------------------------------------------------------------------------------------------------------------
std::tuple<size_t, double, double> LLRF::getKlyFwdPwrRSState(int ampSP_setting)
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex mtx.lock();
	if (GlobalFunctions::entryExists(amp_set_kly_fwd_rs_state, ampSP_setting))
	{
		return amp_set_kly_fwd_rs_state.at(ampSP_setting);
	}
	return dummy_amp_set_kly_fwd_rs_state;
}
//-------------------------------------------------------------------------------------------------------------------
void LLRF::setKlyFwdPwrRSState(int amp_sp, size_t n, double old_mean, double old_variance)
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex mtx.lock();
	/* Get ref to running stat object, key is amp_sp*/
	RunningStats& rs_ref = amp_set_kly_fwd_rs[amp_sp];
	/* apply passed values to running stat object*/
	rs_ref.setInitialValues(n, old_mean, old_variance);
}

void LLRF::setNumTracesToEstimateRepRate(size_t value)
{
	num_traces_to_estimate_rep_rate = value;
}
size_t LLRF::getNumTracesToEstimateRepRate() const
{
	return num_traces_to_estimate_rep_rate;
}
double LLRF::getTraceRepRate() const
{
	return trace_rep_rate;
}
//--------------------------------------------------------------------------------------------------
/*       __   __                    __                ___  __        __   ___  __
		|__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
		|  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/

// Set the parameters for the rolling avergaes
*/

void LLRF::updateRollingAverage(TraceData& data)
{
	////std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	if (data.keep_rolling_average)
	{
		if (data.rolling_average_size == GlobalConstants::size_zero)
		{	// rolling_average_size of zero means no rolling averages 
			//messenger.printDebugMessage("rolling_average_size = 0");
		}
		else if (data.rolling_average_size == GlobalConstants::one_sizet)
		{	//messenger.printDebugMessage("rolling_average_size = 1");
			data.rolling_average = data.trace_values_buffer.back().second;
			data.rolling_average_count = data.rolling_average_size;
			data.has_average = true;
		}
		else
		{	// append new data to average_trace_values
			data.rolling_average_trace_buffer.push_back(data.trace_values_buffer.back().second);
			// Then update the rolling sum vector with the new data by iterating over each container 
			// handy refs to the vectors we are going to iterate over
			std::vector<double>& add = data.rolling_average_trace_buffer.back();
			std::vector<double>& sum = data.rolling_sum;
			for (auto&& s_it = sum.begin(), a_it = add.begin(); s_it < sum.end() && a_it < add.end(); ++a_it, ++s_it)
			{
				*s_it += *a_it;
			}
			// Check if we have acquired enough traces for the rolling average 
			if (data.rolling_average_trace_buffer.size() < data.rolling_average_size)
			{
				data.has_average = false;
				//message("data.average_trace_values.size() < data.rolling_average_size, data.has_average = false");
			}
			else
			{
				data.has_average = true;
				//message("data.average_trace_values.size() !< data.rolling_average_size, data.has_average = true");
			}
			// If we have one trace more than required for the average, if so subtract the first trace in average_trace_values from sum
			if (data.rolling_average_trace_buffer.size() > data.rolling_average_size)
			{
				std::vector<double>& sub = data.rolling_average_trace_buffer.front();
				for (auto i1 = sub.begin(), i2 = sum.begin(); i1 < sub.end() && i2 < sum.end(); ++i1, ++i2)
				{
					*i2 -= *i1;
				}
				// erase the trace we just subtraced
				data.rolling_average_trace_buffer.erase(data.rolling_average_trace_buffer.begin());
			}
			// calculate the average if enough data has been collected 
			if (data.has_average)
			{
				std::vector<double>& av = data.rolling_average;
				double s = (double)data.rolling_average_size;
				for (auto i1 = sum.begin(), i2 = av.begin(); i1 < sum.end() && i2 < av.end(); ++i1, ++i2)
				{
					*i2 = *i1 / s;
				}
			}
			// update the rolling_average_count 
			data.rolling_average_count = data.rolling_average_trace_buffer.size();
			//message("data.average_trace_values.size() < data.rolling_average_size, data.has_average = False");
		}
	}
}
void LLRF::resetAllRollingAverage()
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	for (auto& it : trace_data_map)
	{
		clearRollingAverage(it.second);
	}
}
bool LLRF::resetRollingAverage(const std::string& name)
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		//std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
		clearRollingAverage(trace_data_map.at(n));
		return true;
	}
	//message("liberallrfInterface ERROR, trace ", n, " does not exist");
	return false;
}
void LLRF::clearRollingAverage(TraceData& trace)
{	// any function that calls this should have got the lock !!
	trace.has_average = false;
	trace.rolling_average_count = GlobalConstants::zero_sizet;
	trace.rolling_average.clear();
	trace.rolling_average.resize(trace_data_size);
	trace.rolling_sum.clear();
	trace.rolling_sum.resize(trace_data_size);
	trace.rolling_average_trace_buffer.clear();
	//trace.average_trace_values = {};
}
void LLRF::setShouldKeepRollingAverage()
{
	for (auto&& it : trace_data_map)
	{
		setKeepRollingAverage(it.first, true);
	}
}
bool LLRF::setShouldKeepRollingAverage(const std::string& name)
{
	return setKeepRollingAverage(name, true);
}
void LLRF::setShouldNotKeepRollingAverage()
{
	for (auto&& it : trace_data_map)
	{
		setKeepRollingAverage(it.first, false);
	}
}
bool LLRF::setShouldNotKeepRollingAverage(const std::string& name)
{
	return setKeepRollingAverage(name, false);
}
bool LLRF::setKeepRollingAverage(const std::string& name, bool value)
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		//std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
		clearRollingAverage(trace_data_map.at(n));
		trace_data_map.at(n).keep_rolling_average = value;
		return true;
	}
	return false;
}
bool LLRF::setRollingAverageSize(const std::string& name, const size_t value)
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		trace_data_map.at(n).rolling_average_count = value;
		clearRollingAverage(trace_data_map.at(n));
		return true;
	}
	return false;
}
void LLRF::setAllRollingAverageSize(const size_t value)
{
	for (auto&& it : trace_data_map)
	{
		it.second.rolling_average_size = value;
		clearRollingAverage(it.second);
	}
}
std::vector<double> LLRF::getRollingAverage(const std::string& name)const
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).rolling_average;
	}
	std::vector<double> d(trace_data_size, GlobalConstants::double_min);
	return d;
}
#ifdef BUILD_PYTHON
boost::python::list LLRF::getRollingAverage_Py(const std::string& name)const
{
	return to_py_list<double>(getRollingAverage(name));
}
boost::python::dict LLRF::getRollingAverage_Py()const
{
	boost::python::dict r;
	for (auto&& it : trace_data_map)
	{
		r[it.first] = to_py_list<double>(it.second.rolling_average);
	}
	return r;
}
#endif //BUILD_PYTHON
size_t LLRF::getRollingAverageSize(const std::string& name)const
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).rolling_average_size;
	}
	return GlobalConstants::one_sizet;
}

size_t LLRF::getRollingAverageCount(const std::string& name)const
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).rolling_average_count;
	}
	return GlobalConstants::one_sizet;
}
bool LLRF::isKeepingRollingAverage(const std::string& name)const
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).keep_rolling_average;
	}
	return false;
}
bool LLRF::hasRollingAverage(const std::string& name)const
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).has_average;
	}
	return false;
}
std::vector<std::vector<double>> LLRF::getRollingAverageTraceBuffer(const std::string& name)const
{
	std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).rolling_average_trace_buffer;
	}
	std::vector<std::vector<double>> r{};
	return r;
}
#ifdef BUILD_PYTHON
boost::python::list LLRF::getRollingAverageTraceBuffer_Py(const std::string& name)const
{
	return to_py_list_vector_of_vectors<double>(getRollingAverageTraceBuffer(name));
}
boost::python::dict LLRF::getAllRollingAverageTraceBuffer_Py()const
{
	auto r_cpp = getAllRollingAverageTraceBuffer();
	boost::python::dict r;
	for (auto&& it : r_cpp)
	{
		r[it.first] = to_py_list_vector_of_vectors<double>(it.second);
	}
	return r;
}
#endif //BUILD_PYTHON
std::map<std::string, std::vector<std::vector<double>>> LLRF::getAllRollingAverageTraceBuffer()const
{
	std::map<std::string, std::vector<std::vector<double>>> r;
	for (auto&& it : trace_data_map)
	{	
		r[it.first] = it.second.rolling_average_trace_buffer;
	}
	return r;
}




//--------------------------------------------------------------------------------------------------
void LLRF::checkForOutsideMaskTrace()
{
	//std::chrono::high_resolution_clock::time_point start2 = std::chrono::high_resolution_clock::now();
	//for (auto& it : llrf.trace_values) // loop over each trace
	//{
	//	if (llrf.check_mask && it.second.check_mask && it.second.hi_mask_set && it.second.lo_mask_set) // if everything is ok, check mask
	//	{
	//		int result = updateIsTraceInMask(it.second);
	//		handleTraceInMaskResult(it.second, result);
	//		if (result == UTL::ZERO_INT)
	//		{
	//			message("Found an OME, not checking any more traces this pulse!");
	//			break;
	//		}
	//	}
	//}
	//    if(llrf.check_mask)
	//    {
	//        std::chrono::high_resolution_clock::time_point end1 = std::chrono::high_resolution_clock::now();
	//        message("Timing ",std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start2).count() );
	//    }
}

int LLRF::updateIsTraceInMask(TraceData& trace)
{
	//outside_mask_trace_message.str("");
	///* to fail, you must get consecutive points outside the mask */
	//size_t hi_breakdown_count = UTL::ZERO_INT;
	//size_t lo_breakdown_count = UTL::ZERO_INT;
	///* ref for ease of reading */
	//auto& to_check = trace.data_buffer.back().second;
	//auto& hi = trace.hi_mask;
	//auto& lo = trace.lo_mask;
	///* main loop iterate over the trace values  */
	//for (auto i = 0; i < to_check.size(); ++i)
	//{
	//	/* if we are above the mask floor */
	//	if (to_check[i] > trace.mask_floor)
	//	{
	//		/* if we are above the mask increase hi_breakdown_count */
	//		if (to_check[i] > hi[i])
	//		{
	//			hi_breakdown_count += UTL::ONE_SIZET;
	//			/* if we have too many consecutive hi_breakdown_count trace fails */
	//			if (hi_breakdown_count == trace.num_continuous_outside_mask_count)
	//			{

	//				double t;
	//				std::string str;
	//				updateTime(trace.data_buffer.back().first, t, str);

	//				/* write a message*/
	//				outside_mask_trace_message << trace.name << " HI MASK FAIL: current average = "
	//					<< trace.rolling_average[i] << ", " << to_check[i] << " > "
	//					<< hi[i] << " at i = " << i << " us = " << llrf.time_vector[i] << ". Trace time-stamp = " << str;
	//				trace.outside_mask_index = i;
	//				trace.outside_mask_trace_message = outside_mask_trace_message.str();
	//				message("LLRF CONTROLLER MESSAGE: ", trace.outside_mask_trace_message);
	//				/* return code 0 = FAIL */
	//				return UTL::ZERO_INT;
	//			}
	//		}
	//		else
	//		{
	//			/* if the value is good reset hi_breakdown_count to zero */
	//			hi_breakdown_count = UTL::ZERO_INT;
	//		}
	//		/* if we are belwo the lo mask, increase lo_breakdown_count */
	//		if (to_check[i] < lo[i])
	//		{
	//			lo_breakdown_count += UTL::ONE_INT;
	//			/* if we have too many consecutive lo_breakdown_count trace fails */
	//			if (lo_breakdown_count == trace.num_continuous_outside_mask_count)
	//			{
	//				double t;
	//				std::string str;
	//				updateTime(trace.data_buffer.back().first, t, str);
	//				/* write a message*/
	//				outside_mask_trace_message << trace.name << " LO MASK FAIL: current average = "
	//					<< trace.rolling_average[i] << ", " << to_check[i] << " < "
	//					<< lo[i] << " at i = " << i << " us = " << llrf.time_vector[i] << ". Trace time-stamp = " << str;
	//				trace.outside_mask_index = i;
	//				trace.outside_mask_trace_message = outside_mask_trace_message.str();
	//				message("LLRF CONTROLLER MESSAGE: ", trace.outside_mask_trace_message);
	//				/* return code 0 = FAIL */
	//				return UTL::ZERO_INT;
	//			}
	//		}
	//		else
	//		{
	//			/* if the value is good reset lo_breakdown_count to zero */
	//			lo_breakdown_count = UTL::ZERO_INT;
	//		}

	//	}//if(to_check[i] > trace.mask_floor)

	//}//for(auto i = 0; i < to_check.size(); ++i)
	///* return code 1 = PASS */
	return GlobalConstants::one_int;
}

void LLRF::handleTraceInMaskResult(TraceData& trace, int result)
{
	////message(trace.name, "TraceInMaskResult = ", result );
	//switch (result)
	//{
	//case UTL::ONE_INT: /* passed mask */
	//{
	//	handlePassedMask(trace);
	//	//                if(counter % 1000 == 2)
	//	//                {
	//	//                    message(trace.name, " Passed masks ");
	//	//                    for(auto i = 60; i < 200; ++i)
	//	//                    {
	//	//                        std::cout << trace.hi_mask[i] << " ";
	//	//                    }
	//	//                    std::cout <<std::endl;
	//	//                    for(auto i = 60; i < 200; ++i)
	//	//                    {
	//	//                        std::cout << trace.lo_mask[i] << " ";
	//	//                    }
	//	//                    std::cout <<std::endl;
	//	//
	//	//                }
	//	//                ++counter;
	//}
	//break;
	//case UTL::ZERO_INT: /* failed mask */
	//{
	//	//            std::chrono::high_resolution_clock::time_point end2 = std::chrono::high_resolution_clock::now();
	//	//            message("Timing ",std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - foundbreakdowntime).count() );
	//	handleFailedMask(trace);
	//	break;
	//}
	//case UTL::MINUS_ONE_INT:
	//	/* not checking masks or no active pulses */
	//	break;
	//}
}
void LLRF::handleFailedMask(TraceData& trace)
{
}
//--------------------------------------------------------------------------------------------------


// PULSE COUNTING
size_t LLRF::getActivePulsePowerLimit() const 
{
	return active_pulse_kly_power_limit;
}
void LLRF::setActivePulsePowerLimit(const size_t value)
{
	active_pulse_kly_power_limit = value;
}
size_t LLRF::getActivePulseCount()const
{
	return active_pulse_count;
}
void LLRF::setActivePulseCount(const size_t value)
{
	active_pulse_count = value;
}
void LLRF::addActivePulseCountOffset(const size_t value)
{
	active_pulse_count += value;
}
size_t LLRF::getInactivePulseCount()const
{
	return inactive_pulse_count;
}
void LLRF::setInactivePulseCount(const size_t value)
{
	inactive_pulse_count = value;
}
void LLRF::addInactivePulseCountOffset(const size_t value)
{
	inactive_pulse_count += value;
}
size_t LLRF::getDuplicatePulseCount()const
{
	return duplicate_pulse_count;
}
void LLRF::setDuplicatePulseCount(const size_t value)
{
	duplicate_pulse_count = value;
}
void LLRF::addDuplicatePulseCountOffset(const size_t value)
{
	duplicate_pulse_count += value;
}
size_t LLRF::getTotalPulseCount()const
{
	return total_pulse_count;
}
void LLRF::setTotalPulseCount(const size_t value)
{
	total_pulse_count = value;
}
void LLRF::addTotalPulseCountOffset(const size_t value)
{
	total_pulse_count += value;
}



bool LLRF::startTraceMonitoring()
{
	if (is_trace_monitoring)
	{
		return true;
	}
	else
	{
		if (GlobalFunctions::entryExists(pvStructs, LLRFRecords::LLRF_TRACES))
		{
			pvStruct& pv = pvStructs.at(LLRFRecords::LLRF_TRACES);
			if (ca_state(pv.CHID) == cs_conn)
			{
				epicsInterface->retrieveCHTYPE(pv);
				epicsInterface->retrieveCOUNT(pv);
				pv.updateFunction = epicsInterface->update_LLRF_TRACES;
				//// not sure how to set the mask from EPICS yet.
				pv.MASK = DBE_VALUE;

				//dbr_all_trace_data

				int status = ca_create_subscription(DBR_TIME_DOUBLE, pv.COUNT, pv.CHID, pv.MASK,
					pv.updateFunction,(void*)this, &pv.EVID);
				MY_SEVCHK(status);
				EPICSInterface::sendToEPICS();
				if (status == ECA_NORMAL)
				{
					is_trace_monitoring = true;
					return true;
				}
			}
			else
			{
				messenger.printMessage(pv.fullPVName, " CANNOT CONNECT TO EPICS");
			}
		}
		else
		{
			messenger.printMessage("!!ERROR!! ", hardwareName , " can't find pvstruct for ", LLRFRecords::LLRF_TRACES);
		}
	}
	return false;
}
bool LLRF::stopTraceMonitoring()
{
	if (is_trace_monitoring)
	{
		int status = ca_clear_subscription(pvStructs.at(LLRFRecords::LLRF_TRACES).EVID);
		if (status == ECA_NORMAL)
		{
			return true;
		}

	}
	return false;
}



void LLRF::splitOneTraceValues(const struct dbr_time_double *dbr)
{
	messenger.printDebugMessage("splitOneTraceValues", &(*dbr));
	/*
		dbr_all_trace_data is a struct, it's 'value' member is a dbr_double_t*
	*/
	const dbr_time_double* p_data = (const struct dbr_time_double*)dbr;
	const dbr_double_t* pValue;
	pValue = &p_data->value;
	auto end = &pValue[one_record_trace_data_size];

	// This loop iterates over the one_record_data, 
	size_t chunk = GlobalConstants::zero_sizet;
	std::pair<size_t, std::string>& start_index_data = sorted_one_record_trace_start_indices.at(chunk);

	size_t counter = GlobalConstants::zero_sizet;
	for (auto it = pValue; it != end; ++it)
	{
	
		if (counter == start_index_data.first)
		{
			messenger.printDebugMessage("counter = ", counter);
			messenger.printDebugMessage("chunk = ", chunk);
			messenger.printDebugMessage("adding at it value = ", *it);

			// (for ease of reading)create a ref to the TraceData we are going to update 
			TraceData& td = trace_data_map.at(start_index_data.second);
			// add new elment to end of cricualr buffer 
			td.trace_values_buffer.push_back(temp_TraceData_trace_data_buffer);
			// copy raw data into new buffer vector element 
			std::copy(it, it + trace_data_size, td.trace_values_buffer.back().second.begin());
						

			// next we have to convert the units into watts and offset the phase by 180, so it goes 0 to 360, not -180 to 180

			if (td.trace_type == TYPE::POWER)
			{
				messenger.printDebugMessage(start_index_data.second, " is a power trace ");
				// reset  trace max
				td.trace_max = GlobalConstants::double_min;
				for (auto& v : td.trace_values_buffer.back().second)
				{
					/* this is the mthod to convert to watts */
					v *= v;
					v /= GlobalConstants::one_hundred_double;
					/* update the trace max */
					if (v > td.trace_max)
					{
						td.trace_max = v;
					}
				}
			}
			else if (td.trace_type == TYPE::PHASE)
			{
				messenger.printDebugMessage(start_index_data.second, " is a phase trace ");

				size_t temp_counter = 0;
				for (auto& v : td.trace_values_buffer.back().second)
				{
					v += GlobalConstants::one_hundred_and_eighty_double;

					//std::cout << v << " ";
					temp_counter += 1;
				}
				std::cout << temp_counter << "\n";
				// in gneral unwrapPhaseTrace is not trivial, as we have no idea how many factors of 2*pi to add / subtract ... 
				//unwrapPhaseTrace(llrf.trace_values.at(it.first));
			}
			// TraceData also has a single copy of the trace_values 
			td.trace_values = td.trace_values_buffer.back().second;
			//increment chunk
			chunk += GlobalConstants::one_sizet;
			// update the start index we are now looking for, if we have run out oftrace, thenbreak 
			if (chunk == sorted_one_record_trace_start_indices.size())
			{
				messenger.printDebugMessage("Chunk incremented, but no more traces to update");
				break;
			}
			else
			{
				start_index_data = sorted_one_record_trace_start_indices.at(chunk);
			}
			messenger.printDebugMessage("Chunk incremented, Next trace pValue index = ", start_index_data.first);
		}
		counter += GlobalConstants::one_sizet;
	}
	messenger.printDebugMessage("Print some numbers ");
	for (auto&& it : trace_data_map)
	{
		size_t c = GlobalConstants::zero_sizet;
		messenger.printDebugMessage(it.first);
		for (auto&& it2 : it.second.trace_values_buffer.back().second)
		{
			std::cout << it2 << " ";
			c += GlobalConstants::one_sizet;
			if (c == 10)
			{
				break;
			}
		}
		std::cout << std::endl;
	}
}

std::vector<double> LLRF::getTraceValues(const std::string& name)const
{
	const std::string n = fullLLRFTraceName(name);
	if (GlobalFunctions::entryExists(trace_data_map, n))
	{
		return trace_data_map.at(n).trace_values;
	}
	messenger.printDebugMessage("LLRF::getTraceValues ERROR, trace ", n, " does not exist");
	std::vector<double> r(trace_data_size, GlobalConstants::double_min);//MAGIC_NUMBER
	return r;
}
#ifdef BUILD_PYTHON
boost::python::list LLRF::getTraceValues_Py(const std::string& name)const
{
	return to_py_list<double>(getTraceValues(name));
}
boost::python::dict LLRF::getTraceNameValuesPair_Py(const std::string& name)
{
	std::pair<std::string, std::vector<double>> r = getTraceNameValuesPair(name);
	std::map<std::string, std::vector<double>> r2{ {r.first, r.second} };
	return to_py_dict<std::string, std::vector<double>>(r2);
}
#endif //BUILD_PYTHON

std::pair<std::string, std::vector<double>> LLRF::getTraceNameValuesPair(const std::string& name)const
{
	std::pair<std::string, std::vector<double>> r;
	r.first = fullLLRFTraceName(name);
	r.second = getTraceValues(r.first);
	return r;
}



TraceData& LLRF::getTraceData(const std::string & trace_name ) 
{
	if (GlobalFunctions::entryExists(trace_data_map, trace_name))
	{
		return trace_data_map.at(trace_name);
	}
	return dummy_trace_data;
}

#ifdef BUILD_PYTHON
boost::python::dict LLRF::getTraceDataDict(const std::string& trace_name) const
{
	boost::python::dict r;
	const TraceData& td = dummy_trace_data;
	if (GlobalFunctions::entryExists(trace_data_map, trace_name))
	{
		const TraceData td = trace_data_map.at(trace_name);
	}
	r["name"] = td.name;
	r["trace_values"] = to_py_list<double>(td.trace_values);
	r["trace_data_size"] = td.trace_data_size;
	r["trace_data_buffer_size"] = td.trace_data_buffer_size;
	r["trace_max"] = td.trace_max;
	r["mean_start"] = td.mean_start_stop.first;
	r["mean_stop"] = td.mean_start_stop.second;
	r["mean_stop_minus_start"] = td.stop_minus_start;
	r["trace_cut_mean"] = td.trace_cut_mean;
	r["keep_rolling_average"] = td.keep_rolling_average;
	r["keep_rolling_average"] = td.keep_rolling_average;
	r["scan"] = td.scan.second;
	r["acqm"] = td.acqm.second;
	r["u_level"] = td.u_level.second;
	r["p_level"] = td.p_level.second;
	r["pdbm_level"] = td.pdbm_level.second;
	r["trace_type"] = td.trace_type;
	r["one_record_part"] = td.one_record_part;
	r["one_record_start_index"] = td.one_record_start_index;
	r["one_record_stop_index"] = td.one_record_stop_index;
	r["trace_data_buffer_size"] = td.trace_data_buffer_size;

	// TODO add buffer, rollign mean etc 
	//boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> trace_values_buffer;
	return r;
}

boost::python::dict LLRF::getAllTraceDataDict() const
{
	boost::python::dict r;
	for (auto&& it : trace_data_map)
	{
		r[it.first] =  getTraceDataDict(it.first);
	}
	return r;
}

#endif //BUILD_PYTHON


std::vector<double> LLRF::getCavRevPwr()const
{
	return getTraceValues(LLRFRecords::CAVITY_REVERSE_POWER);
}
std::vector<double> LLRF::getCavFwdPwr()const
{
	return getTraceValues(LLRFRecords::CAVITY_FORWARD_POWER);
}
std::vector<double> LLRF::getKlyRevPwr()const
{
	return getTraceValues(LLRFRecords::KLYSTRON_REVERSE_POWER);
}
std::vector<double> LLRF::getKlyFwdPwr()const
{
	//for (auto&& tb : trace_data_map.at(GlobalConstants::KLYSTRON_FORWARD_POWER).trace_values_buffer)
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
	return getTraceValues(LLRFRecords::KLYSTRON_FORWARD_POWER);
}
std::vector<double> LLRF::getCavRevPha()const
{
	return getTraceValues(LLRFRecords::CAVITY_REVERSE_PHASE);
}
std::vector<double> LLRF::getCavFwdPha()const
{
	return getTraceValues(LLRFRecords::CAVITY_FORWARD_PHASE);
}
std::vector<double> LLRF::getKlyRevPha()const
{
	return getTraceValues(LLRFRecords::KLYSTRON_REVERSE_PHASE);
}
std::vector<double> LLRF::getKlyFwdPha()const
{
	return getTraceValues(LLRFRecords::KLYSTRON_FORWARD_PHASE);
}
std::vector<double> LLRF::getProbePwr()const
{
	return getTraceValues(LLRFRecords::CAVITY_PROBE_POWER);
}
std::vector<double> LLRF::getProbePha()const
{
	return getTraceValues(LLRFRecords::CAVITY_PROBE_PHASE);
}
#ifdef BUILD_PYTHON
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



boost::python::list LLRF::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
}
#endif //BUILD_PYTHON

// Name alies 
std::vector<std::string> LLRF::getAliases() const
{
	return aliases;
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
#ifdef BUILD_PYTHON
boost::python::list LLRF::getMeanStartEndIndex_Py(const std::string& name) const
{
	return to_py_list<size_t, size_t>(getMeanStartEndIndex(name));
}
boost::python::list  LLRF::getMeanStartEndTime_Py(const std::string& name) const
{
	return to_py_list<double, double>(getMeanStartEndTime(name));
}
#endif //BUILD_PYTHON
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
#ifdef BUILD_PYTHON
boost::python::dict LLRF::getTraceMeanIndices_Py()const
{
	return to_py_dict<std::string, size_t, size_t>(getTraceMeanIndices());
}
boost::python::dict LLRF::getTraceMeanTimes_Py()const
{
	return to_py_dict<std::string, double, double>(getTraceMeanTimes());
}
#endif //BUILD_PYTHON
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
#ifdef BUILD_PYTHON
void LLRF::setTraceMeanIndices_Py(const boost::python::dict& settings)
{
	setTraceMeanIndices(to_std_map_pair<std::string,size_t>(settings));
}
void LLRF::setTraceMeanTimes_Py(const boost::python::dict& settings)
{
	setTraceMeanTimes(to_std_map_pair<std::string, double>(settings));
}
#endif //BUILD_PYTHON
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
std::map<std::string, double> LLRF::getAllCutMean()const
{
	std::map<std::string, double> r;
	for(auto&& trace : trace_data_map)
	{
		r[trace.first] = trace.second.trace_cut_mean;
	}
	return r;
}
#ifdef BUILD_PYTHON
boost::python::dict LLRF::getAllCutMean_Py()const
{
	return to_py_dict<std::string, double>(getAllCutMean());
}


boost::python::dict LLRF::getPowerCutMean_Py()const
{
	return to_py_dict<std::string, double>(getPowerCutMean());
}
#endif //BUILD_PYTHON
std::map<std::string, double> LLRF::getPowerCutMean()const
{
	std::map<std::string, double> r;
	for (auto&& trace : power_trace_names)
	{
		r[trace] = getCutMean(trace);
	}
	return r;
}





void  LLRF::scaleAllDummyTraces()
{
	//messenger.printDebugMessage("scaleAllDummyTraces()  ");
	using namespace LLRFRecords;
	scaleDummyTrace(KLYSTRON_FORWARD_POWER, kfpow_dummy_trace);
	//messenger.printDebugMessage("KLYSTRON_FORWARD_POWER AFTER SCALING");
	//for (auto&& tb : trace_data_map.at(GlobalConstants::KLYSTRON_FORWARD_POWER).trace_values_buffer)
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
	trace_data_map.at(trace_name).trace_values_buffer.push_back(dummy_trace_item);
	//messenger.printDebugMessage("scaleDummyTrace AFTER SCALING");
	//for (auto&& tb : trace_data_map.at(GlobalConstants::KLYSTRON_FORWARD_POWER).trace_values_buffer)
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
	//std::lock_guard<std::mutex> lg(llrf_mtx);  // This now locked your mutex llrf_mtx.lock();
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



// TODO create a generic init function for the below maps 
void LLRF::setupInterlocks()
{
	messenger.printDebugMessage("setupInterlocks");
	using namespace LLRFRecords;
	all_trace_interlocks[LLRF_CH1_INTERLOCK] = LLRFInterlock();
	all_trace_interlocks[LLRF_CH2_INTERLOCK] = LLRFInterlock();
	all_trace_interlocks[LLRF_CH3_INTERLOCK] = LLRFInterlock();
	all_trace_interlocks[LLRF_CH4_INTERLOCK] = LLRFInterlock();
	all_trace_interlocks[LLRF_CH5_INTERLOCK] = LLRFInterlock();
	all_trace_interlocks[LLRF_CH6_INTERLOCK] = LLRFInterlock();
	all_trace_interlocks[LLRF_CH7_INTERLOCK] = LLRFInterlock();
	all_trace_interlocks[LLRF_CH8_INTERLOCK] = LLRFInterlock();

	// now we set the TYPE (phase or POWER) 
	//messenger.printDebugMessage("Setting interlock Trace TYPE");
	//const std::vector<std::string> channels{ "CH1","CH2","CH3","CH4","CH5","CH6","CH7","CH8" };
	//// loop over every channel
	//for (auto&& it : channels)
	//{
	//	if (GlobalFunctions::entryExists(channel_to_tracesource_map, it))
	//	{
	//		// if a channe exists in all_trace_interlocks keys
	//		for (auto& it2 : all_trace_interlocks)
	//		{
	//			// if string CHX exist sin 
	//			if (GlobalFunctions::stringIsSubString(it2.first, it))
	//			{
	//				if (GlobalFunctions::stringIsSubString(it2.first, "POWER"))
	//				{
	//					it2.second.interlock_type = TYPE::POWER;
	//				}
	//				else if (GlobalFunctions::stringIsSubString(it2.first, "PHASE"))
	//				{
	//					it2.second.interlock_type = TYPE::PHASE;
	//				}
	//				else
	//				{

	//				}
	//				messenger.printDebugMessage(it2.first, " ", channel_to_tracesource_map[it], " ", ENUM_TO_STRING(it2.second.interlock_type));
	//			}

	//		}
	//	}
	//	else
	//	{
	//		messenger.printDebugMessage("Can't find ", it, " inchannel_to_tracesource_map");
	//	}
	//}
	//
	//for (auto&& channel : channels)
	//{
	//	if (GlobalFunctions::entryExists(specificHardwareParameters, channel))
	//	{
	//		channel_to_tracesource_map[channel] = specificHardwareParameters.find(channel)->second;
	//		messenger.printDebugMessage("Found " + channel + ", connected to " + channel_to_tracesource_map[channel]);
	//	}
	//	else
	//	{
	//		messenger.printDebugMessage(channel + " is NOT in the config");
	//		channel_to_tracesource_map[channel] = "NONE";
	//	}
	//}



	messenger.printDebugMessage("setupInterlocks added these interlocks:");
	for (auto&& trace : all_trace_interlocks)
	{
		messenger.printDebugMessage(trace.first);
	}
}
void LLRF::initAllTraceSCANandACQM()
{
	messenger.printDebugMessage("setupAllTraceSCAN");
	using namespace LLRFRecords;

	std::pair < epicsTimeStamp, STATE> init_pair = std::make_pair(epicsTimeStamp(), STATE::UNKNOWN);

	all_trace_scan[ONE_TRACE_SCAN] = init_pair;
	

	all_trace_scan[CH1_PWR_REM] = init_pair;
	all_trace_scan[CH2_PWR_REM] = init_pair;
	all_trace_scan[CH3_PWR_REM] = init_pair;
	all_trace_scan[CH4_PWR_REM] = init_pair;
	all_trace_scan[CH5_PWR_REM] = init_pair;
	all_trace_scan[CH6_PWR_REM] = init_pair;
	all_trace_scan[CH7_PWR_REM] = init_pair;
	all_trace_scan[CH8_PWR_REM] = init_pair;

	all_trace_scan[CH1_PWR_LOC] = init_pair;
	all_trace_scan[CH2_PWR_LOC] = init_pair;
	all_trace_scan[CH3_PWR_LOC] = init_pair;
	all_trace_scan[CH4_PWR_LOC] = init_pair;
	all_trace_scan[CH5_PWR_LOC] = init_pair;
	all_trace_scan[CH6_PWR_LOC] = init_pair;
	all_trace_scan[CH7_PWR_LOC] = init_pair;
	all_trace_scan[CH8_PWR_LOC] = init_pair;


	all_trace_scan[CH1_AMP_DER] = init_pair;
	all_trace_scan[CH2_AMP_DER] = init_pair;
	all_trace_scan[CH3_AMP_DER] = init_pair;
	all_trace_scan[CH4_AMP_DER] = init_pair;
	all_trace_scan[CH5_AMP_DER] = init_pair;
	all_trace_scan[CH6_AMP_DER] = init_pair;
	all_trace_scan[CH7_AMP_DER] = init_pair;
	all_trace_scan[CH8_AMP_DER] = init_pair;

	all_trace_scan[CH1_PHASE_REM] = init_pair;
	all_trace_scan[CH2_PHASE_REM] = init_pair;
	all_trace_scan[CH3_PHASE_REM] = init_pair;
	all_trace_scan[CH4_PHASE_REM] = init_pair;
	all_trace_scan[CH5_PHASE_REM] = init_pair;
	all_trace_scan[CH6_PHASE_REM] = init_pair;
	all_trace_scan[CH7_PHASE_REM] = init_pair;
	all_trace_scan[CH8_PHASE_REM] = init_pair;


	all_trace_scan[CH1_PHASE_DER] = init_pair;
	all_trace_scan[CH2_PHASE_DER] = init_pair;
	all_trace_scan[CH3_PHASE_DER] = init_pair;
	all_trace_scan[CH4_PHASE_DER] = init_pair;
	all_trace_scan[CH5_PHASE_DER] = init_pair;
	all_trace_scan[CH6_PHASE_DER] = init_pair;
	all_trace_scan[CH7_PHASE_DER] = init_pair;
	all_trace_scan[CH8_PHASE_DER] = init_pair;



	all_trace_acqm[ONE_TRACE_ACQM] = init_pair;

	all_trace_acqm[CH1_PWR_REM] = init_pair;
	all_trace_acqm[CH2_PWR_REM] = init_pair;
	all_trace_acqm[CH3_PWR_REM] = init_pair;
	all_trace_acqm[CH4_PWR_REM] = init_pair;
	all_trace_acqm[CH5_PWR_REM] = init_pair;
	all_trace_acqm[CH6_PWR_REM] = init_pair;
	all_trace_acqm[CH7_PWR_REM] = init_pair;
	all_trace_acqm[CH8_PWR_REM] = init_pair;

	all_trace_acqm[CH1_PWR_LOC] = init_pair;
	all_trace_acqm[CH2_PWR_LOC] = init_pair;
	all_trace_acqm[CH3_PWR_LOC] = init_pair;
	all_trace_acqm[CH4_PWR_LOC] = init_pair;
	all_trace_acqm[CH5_PWR_LOC] = init_pair;
	all_trace_acqm[CH6_PWR_LOC] = init_pair;
	all_trace_acqm[CH7_PWR_LOC] = init_pair;
	all_trace_acqm[CH8_PWR_LOC] = init_pair;

	all_trace_acqm[CH1_AMP_DER] = init_pair;
	all_trace_acqm[CH2_AMP_DER] = init_pair;
	all_trace_acqm[CH3_AMP_DER] = init_pair;
	all_trace_acqm[CH4_AMP_DER] = init_pair;
	all_trace_acqm[CH5_AMP_DER] = init_pair;
	all_trace_acqm[CH6_AMP_DER] = init_pair;
	all_trace_acqm[CH7_AMP_DER] = init_pair;
	all_trace_acqm[CH8_AMP_DER] = init_pair;

	all_trace_acqm[CH1_PHASE_REM] = init_pair;
	all_trace_acqm[CH2_PHASE_REM] = init_pair;
	all_trace_acqm[CH3_PHASE_REM] = init_pair;
	all_trace_acqm[CH4_PHASE_REM] = init_pair;
	all_trace_acqm[CH5_PHASE_REM] = init_pair;
	all_trace_acqm[CH6_PHASE_REM] = init_pair;
	all_trace_acqm[CH7_PHASE_REM] = init_pair;
	all_trace_acqm[CH8_PHASE_REM] = init_pair;

	all_trace_acqm[CH1_PHASE_DER] = init_pair;
	all_trace_acqm[CH2_PHASE_DER] = init_pair;
	all_trace_acqm[CH3_PHASE_DER] = init_pair;
	all_trace_acqm[CH4_PHASE_DER] = init_pair;
	all_trace_acqm[CH5_PHASE_DER] = init_pair;
	all_trace_acqm[CH6_PHASE_DER] = init_pair;
	all_trace_acqm[CH7_PHASE_DER] = init_pair;
	all_trace_acqm[CH8_PHASE_DER] = init_pair;

	//messenger.printDebugMessage("setupAllTraceSCAN added these traces:");
	//for (auto&& trace : all_trace_scan)
	//{
	//	messenger.printDebugMessage(trace.first);
	//}
}
std::map<std::string, STATE> LLRF::getAllTraceSCAN()const
{
	messenger.printDebugMessage("getAllTraceSCAN");
	std::map<std::string, STATE> r;
	for (auto&& it : all_trace_scan)
	{
		r[it.first] = it.second.second;
	}
	for (auto&& it : r)
	{
		messenger.printDebugMessage(it.first, " = ", ENUM_TO_STRING(it.second));
	}
	return r;
}
#ifdef BUILD_PYTHON
boost::python::dict LLRF::getAllTraceSCAN_Py()const
{
	return to_py_dict<std::string, STATE>(getAllTraceSCAN());
}

boost::python::dict LLRF::getAllTraceACQM_Py()const
{
	return to_py_dict<std::string, STATE>(getAllTraceACQM());
}
#endif //BUILD_PYTHON

std::map<std::string, STATE> LLRF::getAllTraceACQM()const
{
	messenger.printDebugMessage("getAllTraceACQM");
	std::map<std::string, STATE> r;
	for (auto&& it : all_trace_acqm)
	{
		r[it.first] = it.second.second;
	}
	for (auto&& it : r)
	{
		messenger.printDebugMessage(it.first, " = ", ENUM_TO_STRING(it.second));
	}	
	return r;
}





bool LLRF::getTraceFromChannelData(const std::string& channel, std::string& string_to_put_trace_name_in )
{
	if (GlobalFunctions::entryExists(channel_to_tracesource_map, channel))
	{
		string_to_put_trace_name_in = channel_to_tracesource_map.at(channel);
		return true;
	}
	return false;
}


std::string LLRF::getTraceFromChannelData(const std::string& channel_data) const
{
	// first we check if we have a PWR_REM or a PHASE_REM in the string 

	std::string pwr_rem_or_phase_rem = "";

	if (GlobalFunctions::stringIsSubString(channel_data, "PWR_REM"))
	{
		pwr_rem_or_phase_rem = "POWER";
	}
	else if (GlobalFunctions::stringIsSubString(channel_data, "PHASE_REM"))
	{
		pwr_rem_or_phase_rem = "PHASE";

	}

	if (!pwr_rem_or_phase_rem.empty())
	{
		// WE KNOW THE TYPE, NOW GET THE SOURCE 
		// this gets the trace SOURCE (kf, kr, cf, cr, cp)
		std::string channel = channel_data.substr(0, 3);
		if (GlobalFunctions::entryExists(channel_to_tracesource_map, channel))
		{
			std::string source = channel_to_tracesource_map.at(channel);

			return channel_to_tracesource_map.at(channel) + "_" + pwr_rem_or_phase_rem;
		}
	}
	
	return "";
}

std::map<std::string, std::string> LLRF::getLLRFStateMap()const
{
	std::map<std::string, std::string> r;
	std::map<std::string, std::string> temp;


	//for (auto&& trace : trace_data_map)
	//{
	//	getMeanStartIndex(trace);
		//}
		return r;
}




//--------------------------------------------------------------------------------------------------
bool LLRF::applyPulseShape()
{
	return epicsInterface->putValue2(pvStructs.at(LLRFRecords::PULSE_SHAPE_APPLY), GlobalConstants::EPICS_ACTIVATE);
}
bool LLRF::setPulseShape(const std::vector<double>& values)
{
	return epicsInterface->putArrayValue(pvStructs.at(LLRFRecords::PULSE_SHAPE_APPLY), values);
}
#ifdef BUILD_PYTHON
bool LLRF::setPulseShape_Py(const boost::python::list& values)
{
	return setPulseShape(to_std_vector<double>(values));
}
bool LLRF::setAndApplyPulseShape(const boost::python::list& values)
{
	return setAndApplyPulseShape(to_std_vector<double>(values));
}
#endif //BUILD_PYTHON
bool LLRF::setAndApplyPulseShape(const std::vector<double>& values)
{
	if (setPulseShape(values))
	{
		GlobalFunctions::pause_50();
		return applyPulseShape();
	}
	return false;
}







//---------------------------------------------------------------------------------------------------------
bool LLRF::trigOff()
{
	return setTrig(STATE::OFF);
}
//____________________________________________________________________________________________
bool LLRF::trigInt()
{
	return setTrig(STATE::INTERNAL);
}
//____________________________________________________________________________________________
bool LLRF::trigExt()
{
	return setTrig(STATE::EXTERNAL);
}

bool LLRF::setTrig(const STATE new_state)
{
	unsigned short v;
	const pvStruct& pvs = pvStructs.at(LLRFRecords::TRIG_SOURCE);
	switch (new_state)
	{
		case STATE::OFF:	  v = 0; break;
		case STATE::EXTERNAL: v = 1; break;
		case STATE::INTERNAL: v = 2; break;
		default: return false;
	}
	return epicsInterface->putValue2(pvStructs.at(LLRFRecords::TRIG_SOURCE), v);
}
STATE LLRF::getTrigSource()const
{
	return trig_source.second;
}




//
//
//std::string LLRF::getTraceFromTracePVKey(const std::string& PVKEY)const
//{
//	/*  THIS MAY SEEM A BIT WEIRD, BUT I THINK IT WORKS
//	The config defines the chanel PV such as:
//	CH1_PWR_REM_ACQM CH1_PHASE_REM_SCAN CH1_PHASE_REM_ACQM CH1_PHASE_REM CH1_AMP_DER_SCAN  
//    CH1_PHASE_DER_SCAN CH1_PWR_LOC_SCAN CH1_INTERLOCK_STATUS CH1_INTERLOCK_ENABLE CH1_INTERLOCK_U 
//	CH1_INTERLOCK_P CH1_INTERLOCK_PDBM ... the same for CH2, etc
// 
//	in the epics callbacks functions we need to put data from these PVs into the correct trace object
//	the correct trace object is defeind in the config, parsed in buildChannelToTraceSourcerMap,
//	*/
//
//	// get the trace_source (i.e, KFP,pow, KFpha, PRPow, etc etc. 
//	std::string trace_source = channel_to_tracesource_map.at(PVKEY.substr(0, 3));
//
//
//
//	using namespace GlobalFunctions;
//	// 
//	if(stringIsSubString(PVKEY, "PWR_REM"))
//	{
//
//	}
//	else if(stringIsSubString(PVKEY, "PWR_REM"))
//	{
//
//	}
//
//
//	//CH1_PWR_REM_ACQM 
//	//CH1_PHASE_REM_SCAN 
//	//CH1_PHASE_REM_ACQM 
//	//CH1_PHASE_REM 
//	//CH1_AMP_DER_SCAN
//	//CH1_PHASE_DER_SCAN 
//	//CH1_PWR_LOC_SCAN 
//	//CH1_INTERLOCK_STATUS 
//	//CH1_INTERLOCK_ENABLE 
//	//CH1_INTERLOCK_U
//	//CH1_INTERLOCK_P 
//	//CH1_INTERLOCK_PDBM
//
//
//}


/*
		Getting image data:

		What I want to achieve:

		Get camera image data without any extra copying
		Have the data in a container that can (easily) be seen in python without copying
		Get time stamped data

		It seems out that acheiving all 3 at once is not obvious
		Proposed solution (which is not perfect and has flaws)

		Function to get time stamp (fast as possible)
		Function to update data into a vector
		Vector exposed to python for access without copying
		Function to get data will copy data into a python list
*/
/* memory shenanigans so its only used for large image arrays when requested */



//
//bool Camera::vector_resize(std::vector<long>& vec)
//{
//	vec.resize(array_data_pixel_count);
//	return vec.size() == array_data_pixel_count;
//}
//void Camera::malloc_imagedata()
//{
//	/*	allocate memory for dbr_image_data, array pointer 	*/
//	unsigned nBytes = dbr_size_n(DBR_TIME_LONG, array_data_pixel_count);
//	dbr_image_data = (struct dbr_time_long*)malloc(nBytes);
//	messenger.printDebugMessage(hardwareName, " dbr_image_data pointer allocated ", nBytes, " BYTES ");
//	image_data_has_not_malloced = false;
//}
//void Camera::malloc_roidata()
//{
//	/*		allocate memory for dbr_roi_data, array pointer  	*/
//	unsigned nBytes = dbr_size_n(DBR_TIME_LONG, array_data_pixel_count);
//	dbr_roi_data = (struct dbr_time_long*)malloc(nBytes);
//	messenger.printDebugMessage(hardwareName, " dbr_roi_data pointer allocated ", nBytes, " BYTES ");
//	roi_data_has_not_malloced = false;
//}
//bool Camera::updateImageData()
//{
//	if (image_data_has_not_vector_resized)
//	{
//		messenger.printDebugMessage("vector_resize for image_data ");
//		vector_resize(image_data.second);
//		if (image_data.second.size() == array_data_pixel_count)
//		{
//			image_data_has_not_vector_resized = false;
//		}
//		else
//		{
//			image_data_has_not_vector_resized = true;
//		}
//	}
//	if (!image_data_has_not_vector_resized)
//	{
//		auto start = std::chrono::high_resolution_clock::now();
//		//bool got_stamp = getArrayTimeStamp(dbr_image_data, pvStructs.at(CameraRecords::CAM2_ArrayData)
//		//	, image_data.first);
//		bool got_value = getArrayValue(image_data.second, pvStructs.at(CameraRecords::CAM2_ArrayData)
//			, image_data.second.size());
//		auto stop = std::chrono::high_resolution_clock::now();
//		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//		messenger.printDebugMessage("updateImageData Time taken: ", duration.count(), " us");
//		return got_value;
//	}
//	else {
//		messenger.printDebugMessage("!!ERROR!! image_data_has_not_vector_resized = ", image_data_has_not_vector_resized);
//	}
//	return false;
//}
//bool Camera::updateImageDataWithTimeStamp()
//{
//	if (image_data_has_not_malloced)
//	{
//		messenger.printDebugMessage("calling malloc_imagedata");
//		malloc_imagedata();
//	}
//	if (image_data_has_not_vector_resized)
//	{
//		messenger.printDebugMessage("vector_resize for image_data ");
//		image_data_has_not_vector_resized = !vector_resize(image_data.second);
//	}
//	if (!image_data_has_not_vector_resized && !image_data_has_not_malloced)
//	{
//		auto start = std::chrono::high_resolution_clock::now();
//		bool got_stamp = getArrayTimeStamp(dbr_image_data, pvStructs.at(CameraRecords::CAM2_ArrayData)
//			, image_data.first);
//		bool got_value = getArrayValue(image_data.second, pvStructs.at(CameraRecords::CAM2_ArrayData)
//			, image_data.second.size());
//		auto stop = std::chrono::high_resolution_clock::now();
//		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//		messenger.printDebugMessage("updateImageDataWithTimeStamp Time taken: ", duration.count(), " us");
//		return got_stamp && got_value;
//	}
//	else {
//		messenger.printDebugMessage("!!ERROR!! image_data_has_not_vector_resized = ", image_data_has_not_vector_resized);
//		messenger.printDebugMessage("!!ERROR!! image_data_has_not_malloced = ", image_data_has_not_malloced);
//	}
//
//	return false;
//}
//
//



//____________________________________________________________________________________________
std::string LLRF::fullLLRFTraceName(const std::string& name_in)const
{
	using namespace LLRFRecords;
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
	if (machine_area == TYPE::HRRG)
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
	else if (machine_area == TYPE::LRRG)
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
