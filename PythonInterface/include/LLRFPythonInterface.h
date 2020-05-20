#ifndef LLLRF_PYTHON_INTERFACE_H_
#define LLLRF_PYTHON_INTERFACE_H_

#include <LLRF.h>
#include <LLRFFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_LLRF_INCLUDE
{

	void expose_llrf_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LLRF>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LLRF, boost::python::bases<Hardware>, boost::noncopyable>("LLRF", boost::python::no_init)
			

			.def("getPhi", &LLRF::setPhi)
			.def("getAmp", &LLRF::setAmp)
			.def("setAmpMW", &LLRF::setAmpMW)
			.def("getPhiDEG", &LLRF::setPhiDEG)

			.def("getAliases", &LLRF::getAliases_Py)

			.def("getIndex", &LLRF::getIndex)
			.def("getTime",  &LLRF::getTime)

			.def("setMeanStartEndTime", &LLRF::setMeanStartEndTime)
			.def("setMeanStartIndex", &LLRF::setMeanStartIndex)
			.def("setMeanStopIndex",  &LLRF::setMeanStopIndex)
			.def("getMeanStartIndex", &LLRF::getMeanStartIndex)
			//.def("getMean", &LLRFFactory::getMeanStartIndex)
			.def("getCavRevPha",&LLRF::getCavRevPha)
			.def("getCutMean",  &LLRF::getCutMean)
			.def("getMeanStartTime",&LLRF::getMeanStartTime)
			.def("getMeanStopTime", &LLRF::getMeanStopTime)

			//.def("setTraceDataBufferSize", &LLRF::setTraceDataBufferSize)


			.def("getTraceData", &LLRF::getTraceData_Py)
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

			.def("getTraceData", &LLRF::getTraceData_Py)
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

			.def("setTraceDataBufferSize", &LLRFFactory::setTraceDataBufferSize)


			.def("getTraceData", &LLRFFactory::getTraceData_Py)
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