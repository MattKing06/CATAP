#include <LLRF.h>
#include "boost/algorithm/string/split.hpp"
#include "LLRFPVRecords.h"
#include "GlobalFunctions.h"




TraceData::TraceData():
name("unknown"),
trace_max(0.0),
mean_start_index(0),
mean_stop_index(0),
trace_cut_mean(0)
{

}
TraceData::TraceData(const TraceData& copy_trace_data)
{

}
TraceData::~TraceData()
{

}





LLRF::LLRF()
{
}

LLRF::LLRF(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
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

LLRF::LLRF(const LLRF& copyLLRF)
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
bool LLRF::setAmpMVM(double value)
{
	amp_MVM.second = value;
	
	// atm we are going to fake power traces
	scaleDummyTraces();
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
double LLRF::getAmpMVM()const
{
	return amp_MVM.second;
}
double LLRF::getPhiDEG()const
{
	return phi_degrees.second;
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





void  LLRF::scaleDummyTraces()
{
	double scale_factor = amp_MVM.second * 1000000.0;
	for (auto&& it : kfpow_dummy_trace)
	{

	}


	//std::transform(myv1.begin(), myv1.end(), myv1.begin(),
	//	std::bind(std::multiplies<T>(), std::placeholders::_1, 3));

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
	if (GlobalFunctions::entryExists(paramMap, "cfpow_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("cfpow_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			cfpow_dummy_trace.push_back(std::stof(value));
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "crpow_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("crpow_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			crpow_dummy_trace.push_back(std::stof(value));
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
	if (GlobalFunctions::entryExists(paramMap, "cfpha_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("cfpha_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			cfpha_dummy_trace.push_back(std::stof(value));
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "crpha_dummy_trace"))
	{
		boost::split(dummy_trace_string, paramMap.find("crpha_dummy_trace")->second, [](char c) {return c == ','; });
		for (auto value : dummy_trace_string)
		{
			//messenger.printDebugMessage(value);
			crpha_dummy_trace.push_back(std::stof(value));
		}
	}
}
