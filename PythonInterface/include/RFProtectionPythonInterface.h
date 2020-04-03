#ifndef RF_PROTECTION_PYTHON_INTERFACE_H_
#define RF_PROTECTION_PYTHON_INTERFACE_H_

#include <RFProtection.h>
#include <RFProtectionFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_RF_PROTECTION_INCLUDE
{

	void expose_rf_protection_object() 
	{
		boost::python::class_<RFProtection, boost::python::bases<Hardware>, boost::noncopyable>("RFProtection", boost::python::no_init)
			.def("debugMessagesOn", &RFProtection::debugMessagesOn)
			.def("debugMessagesOff", &RFProtection::debugMessagesOff)
			.def("messagesOn", &RFProtection::messagesOn)
			.def("messagesOff", &RFProtection::messagesOff)
			.def("isDebugOn", &RFProtection::isDebugOn)
			.def("isMessagingOn", &RFProtection::isMessagingOn);
	}
	void expose_rf_protection_factory_object() 
	{
		boost::python::class_<RFProtectionFactory, boost::noncopyable>("RFProtectionFactory", boost::python::no_init)
			.def("debugMessagesOn", &RFProtectionFactory::debugMessagesOn)
			.def("debugMessagesOff", &RFProtectionFactory::debugMessagesOff)
			.def("messagesOn", &RFProtectionFactory::messagesOn)
			.def("messagesOff", &RFProtectionFactory::messagesOff)
			.def("isDebugOn", &RFProtectionFactory::isDebugOn)
			.def("isMessagingOn", &RFProtectionFactory::isMessagingOn);
	}



}

#endif //RF_PROTECTION_PYTHON_INTERFACE_H_