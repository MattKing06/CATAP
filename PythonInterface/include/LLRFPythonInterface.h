#ifndef LLLRF_PYTHON_INTERFACE_H_
#define LLLRF_PYTHON_INTERFACE_H_

#include <LLRF.h>
#include <LLRFFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_LLRF_INCLUDE
{


	void expose_llrf_trace_data_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<TraceData>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<TraceData>
			("TraceData", "TraceData Doc String")
			.add_property("name", &TraceData::name)
			.add_property("trace_max", &TraceData::trace_max)
			.add_property("mean_start_stop", &TraceData::mean_start_stop)
			.add_property("mean_start_stop_time", &TraceData::mean_start_stop_time)
			.add_property("trace_data_size", &TraceData::trace_data_size)
			.add_property("trace_data_buffer_size", &TraceData::trace_data_buffer_size)
			.add_property("one_record_part", &TraceData::one_record_part)
			.add_property("one_record_start_index", &TraceData::one_record_start_index)
			.add_property("one_record_stop_index", &TraceData::one_record_stop_index)
			.add_property("trace_type", &TraceData::trace_type)
			.add_property("trace_type", &TraceData::trace_type)
			;

		//boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> trace_values_buffer;
		//std::vector<double> trace_values;
		//std::pair<size_t, size_t> mean_start_stop;
		//std::pair<double, double> mean_start_stop_time;
		//std::pair<epicsTimeStamp, STATE> scan;
		//std::pair<epicsTimeStamp, STATE> acqm;
		//std::pair<epicsTimeStamp, bool> interlock_state;
		//std::pair<epicsTimeStamp, double > u_level;
		//std::pair<epicsTimeStamp, double > p_level;
		//std::pair<epicsTimeStamp, double > pdbm_level;

	}

	void expose_llrf_interlock_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LLRFInterlock>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LLRFInterlock>
			("LLRFInterlock", "LLRFInterlock Doc String")
			.add_property("name", &LLRFInterlock::name)
			.add_property("interlock_type", &LLRFInterlock::interlock_type)
			.add_property("u_level", &LLRFInterlock::u_level)
			.add_property("p_level", &LLRFInterlock::p_level)
			.add_property("pdbm_level", &LLRFInterlock::pdbm_level)
			.add_property("enable", &LLRFInterlock::enable)
			;


	}

	void expose_llrf_object()
	{
		boost::python::numpy::initialize();

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LLRF>())->to_python_target_type());
		if (is_registered) return;


		// function pointers for overloads that are ambiguous when exposing to Python
		boost::python::list(LLRF::*getRollingAverage_single)(const std::string&)const = &LLRF::getRollingAverage_Py;
		boost::python::dict(LLRF::*getRollingAverage_multiple)()const = &LLRF::getRollingAverage_Py;
		bool(LLRF::*setShouldKeepRollingAverage_single)(const std::string&) = &LLRF::setShouldKeepRollingAverage;
		void(LLRF::*setShouldKeepRollingAverage_multiple)() = &LLRF::setShouldKeepRollingAverage;
		bool(LLRF::*setShouldNotKeepRollingAverage_single)(const std::string&) = &LLRF::setShouldNotKeepRollingAverage;
		void(LLRF::*setShouldNotKeepRollingAverage_multiple)() = &LLRF::setShouldNotKeepRollingAverage;


		boost::python::class_<LLRF, boost::python::bases<Hardware>, boost::noncopyable>("LLRF", boost::python::no_init)
			
			//.def("printSetupData", &LLRF::printSetupData) // for debugging 
		
			.def("setAmp", &LLRF::setAmp)
			.def("setAmpMW", &LLRF::setAmpMW)
			.def("getAmp", &LLRF::getAmp)
			.def("getAmpMW", &LLRF::getAmpMW)


			.def("setPhi", &LLRF::setPhi)
			.def("setPhiDEG", &LLRF::setPhiDEG)
			.def("setCrestPhase", &LLRF::setCrestPhase)
			.def("setOperatingPhase", &LLRF::setOperatingPhase)

			.def("getPhi", &LLRF::getPhi)
			.def("getPhiDEG", &LLRF::getPhiDEG)
			.def("getCrestPhase", &LLRF::getCrestPhase)
			.def("getOperatingPhase", &LLRF::getOperatingPhase)
			
			
			// TRIGGERS AND TRIGGER SOURCE
			.def("trigOff", &LLRF::trigOff)
			.def("trigInt", &LLRF::trigInt)
			.def("trigExt", &LLRF::trigExt)
			.def("getTrigSource", &LLRF::getTrigSource)
			.def("isTrigExternal", &LLRF::isTrigExternal)
			.def("isTrigInternal", &LLRF::isTrigInternal)


			.def("isRFOutput", &LLRF::isRFOutput)
			.def("isNotRFOutput", &LLRF::isNotRFOutput)
			.def("RFOutput", &LLRF::RFOutput)
			.def("enableRFOutput", &LLRF::enableRFOutput)
			.def("disableRFOutput", &LLRF::disableRFOutput)



			.def("isAmpFFconnected", &LLRF::isAmpFFconnected)
			.def("lockAmpFF", &LLRF::lockAmpFF)
			.def("RFOutput", &LLRF::RFOutput)
			.def("lockPhaseFF", &LLRF::lockPhaseFF)
			.def("unlockAmpFF", &LLRF::unlockAmpFF)
			.def("unlockPhaseFF", &LLRF::unlockPhaseFF)
			.def("isFFLocked", &LLRF::isFFLocked)
			.def("isFFNotLocked", &LLRF::isFFNotLocked)
			.def("isAmpFFLocked", &LLRF::isAmpFFLocked)
			.def("isAmpFFNotLocked", &LLRF::isAmpFFNotLocked)
			.def("isPhaseFFLocked", &LLRF::isPhaseFFLocked)
			.def("isPhaseFFNotLocked", &LLRF::isPhaseFFNotLocked)

			.def("enableRFandLock", &LLRF::enableRFandLock)


			.def("setAllSCANTo", &LLRF::setAllSCANTo)

			.def("setAllSCANToPassive", &LLRF::setAllSCANToPassive)
			.def("setAllSCANToIoIntr", &LLRF::setAllSCANToIoIntr) // !!!!!!!!!!!!!!!

			.def("getAllTraceSCAN", &LLRF::getAllTraceSCAN_Py)
			.def("getAllTraceACQM", &LLRF::getAllTraceACQM_Py)
			.def("setTORACQMEvent", &LLRF::setTORACQMEvent)
			.def("getTORACQM", &LLRF::getTORACQM)
			.def("getTORSCAN", &LLRF::getTORSCAN)


			// TODO NEED TO CHECK WORGIN!!! 

			.def("setInterlockActive", &LLRF::setInterlockActive)
			.def("setInterlockNonActive", &LLRF::setInterlockNonActive)
			.def("getInterlock", &LLRF::getInterlock)
			.def("isInterlockActive", &LLRF::isInterlockActive)
			.def("isInterlockNotActive", &LLRF::isInterlockNotActive)

			.def("getAllInterlocks", &LLRF::getAllInterlocks_Py)


					
			.def("startTraceMonitoring", &LLRF::startTraceMonitoring)
			.def("stopTraceMonitoring", &LLRF::stopTraceMonitoring)

						
			.def("getRollingAverage", getRollingAverage_single)
			.def("getRollingAverage", getRollingAverage_multiple)
			.def("setShouldKeepRollingAverage", setShouldKeepRollingAverage_single)
			.def("setShouldKeepRollingAverage", setShouldKeepRollingAverage_multiple)
			.def("setShouldNotKeepRollingAverage", setShouldNotKeepRollingAverage_single)
			.def("setShouldNotKeepRollingAverage", setShouldNotKeepRollingAverage_multiple)
			.def("getRollingAverage", getRollingAverage_single)
			.def("getRollingAverage", getRollingAverage_multiple)


			.def("resetAllRollingAverage", &LLRF::resetAllRollingAverage)
			.def("resetRollingAverage", &LLRF::resetRollingAverage)
			

			.def("resetAllRollingAverage", &LLRF::resetAllRollingAverage)
			.def("resetRollingAverage", &LLRF::resetRollingAverage)
			.def("setKeepRollingAverage", &LLRF::setKeepRollingAverage)
			.def("setRollingAverageSize", &LLRF::setRollingAverageSize)
			.def("setAllRollingAverageSize", &LLRF::setAllRollingAverageSize)
			.def("getRollingAverageSize", &LLRF::getRollingAverageSize)
			.def("getRollingAverageCount", &LLRF::getRollingAverageCount)
			.def("isKeepingRollingAverage", &LLRF::isKeepingRollingAverage)
			.def("hasRollingAverage", &LLRF::hasRollingAverage)
			.def("getRollingAverageTraceBuffer", &LLRF::getRollingAverageTraceBuffer_Py)
			.def("getAllRollingAverageTraceBuffer", &LLRF::getAllRollingAverageTraceBuffer_Py)

			













			
			//.def("setAllTraceBufferSize", &LLRF::setAllTraceBufferSize)


			.def("getAliases", &LLRF::getAliases_Py)

			.def("getIndex", &LLRF::getIndex)
			.def("getTime",  &LLRF::getTime)

			.def("setMeanStartEndIndex", &LLRF::setMeanStartEndIndex)
			.def("setMeanStartEndTime", &LLRF::setMeanStartEndTime)
			.def("getMeanStartEndIndex", &LLRF::getMeanStartEndIndex)
			.def("getMeanStartEndTime", &LLRF::getMeanStartEndTime)

			.def("getTraceMeanIndices", &LLRF::getTraceMeanIndices_Py)
			.def("getTraceMeanTimes", &LLRF::getTraceMeanTimes_Py)
			.def("setTraceMeanIndices", &LLRF::setTraceMeanIndices_Py)
			.def("setTraceMeanTimes", &LLRF::setTraceMeanTimes_Py)


			//.def("getMean", &LLRFFactory::getMeanStartIndex)
			.def("getCavRevPha",&LLRF::getCavRevPha)
			.def("getCutMean",  &LLRF::getCutMean)
			.def("getPowerCutMean",  &LLRF::getPowerCutMean_Py)







			.def("updateTraceValues",  &LLRF::updateTraceValues)

			.def("getActivePulsePowerLimit",  &LLRF::getActivePulsePowerLimit)
			.def("setActivePulsePowerLimit",  &LLRF::setActivePulsePowerLimit)
			.def("getActivePulseCount",  &LLRF::getActivePulseCount)
			.def("setActivePulseCount",  &LLRF::setActivePulseCount)
			.def("addActivePulseCountOffset",  &LLRF::addActivePulseCountOffset)
			.def("getInactivePulseCount",  &LLRF::getInactivePulseCount)
			.def("setInactivePulseCount",  &LLRF::setInactivePulseCount)
			.def("addInactivePulseCountOffset",  &LLRF::addInactivePulseCountOffset)
			.def("getDuplicatePulseCount",  &LLRF::getDuplicatePulseCount)
			.def("setDuplicatePulseCount",  &LLRF::setDuplicatePulseCount)
			.def("addDuplicatePulseCountOffset",  &LLRF::addDuplicatePulseCountOffset)
			.def("getTotalPulseCount",  &LLRF::getTotalPulseCount)
			.def("setTotalPulseCount",  &LLRF::setTotalPulseCount)
			.def("addTotalPulseCountOffset",  &LLRF::addTotalPulseCountOffset)




			.def("setNumTracesToEstimateRepRate",  &LLRF::setNumTracesToEstimateRepRate)
			.def("getNumTracesToEstimateRepRate",  &LLRF::getNumTracesToEstimateRepRate)
			.def("getTraceRepRate",  &LLRF::getTraceRepRate)



			.def("fullLLRFTraceName",  &LLRF::fullLLRFTraceName)
			.def("getAllLLRFTraceNames",  &LLRF::getAllLLRFTraceNames_Py)

				
				

			

			//.def("setTraceDataBufferSize", &LLRF::setTraceDataBufferSize)


			.def("getTraceData", &LLRF::getTraceDataDict)
			.def("getAllTraceData", &LLRF::getAllTraceDataDict)
			.def("getCavRevPwr", &LLRF::getCavRevPwr_Py)
			.def("getCavFwdPwr", &LLRF::getCavFwdPwr_Py)
			.def("getKlyRevPwr", &LLRF::getKlyRevPwr_Py)
			.def("getKlyFwdPwr", &LLRF::getKlyFwdPwr_Py)
			.def("getCavRevPha", &LLRF::getCavRevPha_Py)
			.def("getCavFwdPha", &LLRF::getCavFwdPha_Py)
			.def("getKlyRevPha", &LLRF::getKlyRevPha_Py)
			.def("getKlyFwdPha", &LLRF::getKlyFwdPha_Py)
			.def("getProbePha",  &LLRF::getProbePha_Py)
			.def("getProbePwr",  &LLRF::getProbePwr_Py)
			.def("getCavRevPwrNumPy", &LLRF::getCavRevPwr_NumPy)
			.def("getCavFwdPwrNumPy", &LLRF::getCavFwdPwr_NumPy)
			.def("getKlyRevPwrNumPy", &LLRF::getKlyRevPwr_NumPy)
			.def("getKlyFwdPwrNumPy", &LLRF::getKlyFwdPwr_NumPy)
			.def("getCavRevPhaNumPy", &LLRF::getCavRevPha_NumPy)
			.def("getCavFwdPhaNumPy", &LLRF::getCavFwdPha_NumPy)
			.def("getKlyRevPhaNumPy", &LLRF::getKlyRevPha_NumPy)
			.def("getKlyFwdPhaNumPy", &LLRF::getKlyFwdPha_NumPy)
			.def("getProbePhaNumPy", &LLRF::getProbePha_NumPy)
			.def("getProbePwrNumPy", &LLRF::getProbePwr_NumPy)

			.def("getTraceValues", &LLRF::getTraceValues_Py)
			.def("getTraceTimeStamp", &LLRF::getTraceTimeStamp_Py)

			.def("getCavRevPwr", &LLRF::getCavRevPwr_Py)
			.def("getCavFwdPwr", &LLRF::getCavFwdPwr_Py)
			.def("getKlyRevPwr", &LLRF::getKlyRevPwr_Py)
			.def("getKlyFwdPwr", &LLRF::getKlyFwdPwr_Py)
			.def("getCavRevPha", &LLRF::getCavRevPha_Py)
			.def("getCavFwdPha", &LLRF::getCavFwdPha_Py)
			.def("getKlyRevPha", &LLRF::getKlyRevPha_Py)
			.def("getKlyFwdPha", &LLRF::getKlyFwdPha_Py)
			.def("getProbePha",  &LLRF::getProbePha_Py)
			.def("getProbePwr",  &LLRF::getProbePwr_Py)


				.def("getCavRevPwrTS", &LLRF::getCavRevPwrTS_Py)
				.def("getCavFwdPwrTS", &LLRF::getCavFwdPwrTS_Py)
				.def("getKlyRevPwrTS", &LLRF::getKlyRevPwrTS_Py)
				.def("getKlyFwdPwrTS", &LLRF::getKlyFwdPwrTS_Py)
				.def("getCavRevPhaTS", &LLRF::getCavRevPhaTS_Py)
				.def("getCavFwdPhaTS", &LLRF::getCavFwdPhaTS_Py)
				.def("getKlyRevPhaTS", &LLRF::getKlyRevPhaTS_Py)
				.def("getKlyFwdPhaTS", &LLRF::getKlyFwdPhaTS_Py)
				.def("getProbePhaTS",&LLRF::getProbePhaTS_Py)
				.def("getProbePwrTS",&LLRF::getProbePwrTS_Py)

			.def("debugMessagesOn", &LLRF::debugMessagesOn)
			.def("debugMessagesOff", &LLRF::debugMessagesOff)
			.def("messagesOn", &LLRF::messagesOn)
			.def("messagesOff", &LLRF::messagesOff)
			.def("isDebugOn", &LLRF::isDebugOn)
			.def("isMessagingOn", &LLRF::isMessagingOn);
	}
	void expose_llrf_factory_object()
	{


		// OVERLOADED SETUP FUNCTIONS TO ALLOW USER FULL CONTROL AND "FUTURE PROOVED VERSION PARAMETER" 
		bool (LLRFFactory::* setup_NoArg)() = &LLRFFactory::setup;
		bool (LLRFFactory::* setup_VersionArg)(const std::string&) = &LLRFFactory::setup;
		bool (LLRFFactory::* setup_TypeArg)(TYPE) = &LLRFFactory::setup;
		bool (LLRFFactory::* setup_VersionTypeArg)(const std::string&, TYPE) = &LLRFFactory::setup;
		bool (LLRFFactory::* setup_ListArg)(const boost::python::list&) = &LLRFFactory::setup;
		bool (LLRFFactory::* setup_VersionListArg)(const std::string&, const boost::python::list&) = &LLRFFactory::setup;



		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LLRFFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LLRFFactory, boost::noncopyable>("LLRFFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def(boost::python::init<STATE, const std::string>())
			.def("setup", setup_NoArg)
			.def("setup", setup_VersionArg)
			.def("setup", setup_TypeArg)
			.def("setup", setup_VersionTypeArg)
			.def("setup", setup_ListArg)
			.def("setup", setup_VersionListArg)

			.def("getLLRFNames", &LLRFFactory::getLLRFNames_Py)
			.def("getLLRF", &LLRFFactory::getLLRF , boost::python::return_value_policy<boost::python::reference_existing_object>())

			.def("setPhi", &LLRFFactory::setPhi)
			.def("setAmp", &LLRFFactory::setAmp)
			.def("setAmpMW", &LLRFFactory::setAmpMW)
			.def("setPhiDEG", &LLRFFactory::setPhiDEG)
			.def("getPhi", &LLRFFactory::getPhi)
			.def("getAmp", &LLRFFactory::getAmp)
			.def("getAmpMW", &LLRFFactory::getAmpMW)
			.def("getPhiDEG", &LLRFFactory::getPhiDEG)



			//size_t getIndex(const double time) const;
			//double getTime(const size_t index) const;	
			.def("getIndex", &LLRFFactory::getIndex)
			.def("getTime", &LLRFFactory::getTime)

		//bool setMeanStartEndTime(const double start, const double end, const std::string & name);
		//bool setMeanStartIndex(const std::string & name, size_t  value);
		//bool setMeanStopIndex(const std::string & name, size_t  value);
		//size_t getMeanStartIndex(const std::string & name)const;
		//size_t getMeanStopIndex(const std::string & name)const;
		//double getMean(const std::string & name)const;
		//double getCutMean(const std::string & name)const;
		//double getMeanStartTime(const std::string & name)const;
		//double getMeanStopTime(const std::string & name)const;
		//bool setTraceDataBufferSize(const std::string & name, const size_t new_size);
			.def("setMeanStartEndTime", &LLRFFactory::setMeanStartEndTime)
			.def("setMeanStartIndex", &LLRFFactory::setMeanStartIndex)
			.def("setMeanStopIndex", &LLRFFactory::setMeanStopIndex)
			.def("getMeanStartIndex", &LLRFFactory::getMeanStartIndex)
			//.def("getMean", &LLRFFactory::getMeanStartIndex)
			.def("getCavRevPha", &LLRFFactory::getCavRevPha)
			.def("getCutMean", &LLRFFactory::getCutMean)
			.def("getMeanStartTime", &LLRFFactory::getMeanStartTime)
			.def("getMeanStopTime", &LLRFFactory::getMeanStopTime)

			//.def("setTraceDataBufferSize", &LLRFFactory::setTraceDataBufferSize)


			//.def("getTraceData", &LLRFFactory::getTraceData_Py)
			.def("getCavRevPwr", &LLRFFactory::getCavRevPwr_Py)
			.def("getCavFwdPwr", &LLRFFactory::getCavFwdPwr_Py)
			.def("getKlyRevPwr", &LLRFFactory::getKlyRevPwr_Py)
			.def("getKlyFwdPwr", &LLRFFactory::getKlyFwdPwr_Py)
			.def("getCavRevPha", &LLRFFactory::getCavRevPha_Py)
			.def("getCavFwdPha", &LLRFFactory::getCavFwdPha_Py)
			.def("getKlyRevPha", &LLRFFactory::getKlyRevPha_Py)
			.def("getKlyFwdPha", &LLRFFactory::getKlyFwdPha_Py)
			.def("getProbePha", &LLRFFactory::getProbePha_Py)
			.def("getProbePwr", &LLRFFactory::getProbePwr_Py)


			.def("debugMessagesOn", &LLRFFactory::debugMessagesOn)
			.def("debugMessagesOn", &LLRFFactory::debugMessagesOn)
			.def("debugMessagesOff", &LLRFFactory::debugMessagesOff)
			.def("messagesOn", &LLRFFactory::messagesOn)
			.def("messagesOff", &LLRFFactory::messagesOff)
			.def("isDebugOn", &LLRFFactory::isDebugOn)
			.def("isMessagingOn", &LLRFFactory::isMessagingOn);
	}

}

#endif //LLLRF_PYTHON_INTERFACE_H_