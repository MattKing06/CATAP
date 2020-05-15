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