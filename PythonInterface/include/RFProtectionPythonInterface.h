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
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFProtection>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<RFProtection, boost::python::bases<Hardware>, boost::noncopyable>("RFProtection", boost::python::no_init)
			
			.def("attachContext", &RFProtection::attachToInitialContext)
			.def("getCmi", &RFProtection::getCmi)
			.def("getStatus", &RFProtection::getStatus)
			.def("isGood", &RFProtection::isGood)
			.def("isNotGood", &RFProtection::isNotGood)
			.def("isBad", &RFProtection::isBad)
			.def("reset", &RFProtection::reset)
			.def("enable", &RFProtection::enable)
			.def("disable", &RFProtection::disable)





			

			.def("debugMessagesOff", &RFProtection::debugMessagesOff)
			.def("messagesOn", &RFProtection::messagesOn)
			.def("messagesOff", &RFProtection::messagesOff)
			.def("isDebugOn", &RFProtection::isDebugOn)
			.def("isMessagingOn", &RFProtection::isMessagingOn);
	}
	void expose_rf_protection_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFProtectionFactory>())->to_python_target_type());
		if (is_registered) return;
		void(RFProtectionFactory:: * attachContext_single)(const std::string&) = &RFProtectionFactory::attachContext;
		void(RFProtectionFactory:: * attachContext_all)(void) = &RFProtectionFactory::attachContext;
		boost::python::class_<RFProtectionFactory, boost::noncopyable>("RFProtectionFactory", boost::python::no_init)
			
			.def(boost::python::init<STATE>())
			.def(boost::python::init<STATE, const std::string>())
			.def("attachContext", &RFProtectionFactory::attachContext_Py)
			.def("attachContext", attachContext_single)
			.def("attachContext", attachContext_all)
			.def("resetGun", &RFProtectionFactory::resetGun)
			.def("resetL01", &RFProtectionFactory::resetL01)
			.def("enableGun", &RFProtectionFactory::enableGun)
			.def("enableL01", &RFProtectionFactory::enableL01)
			.def("disableGun", &RFProtectionFactory::disableGun)
			.def("disableL01", &RFProtectionFactory::disableL01)
			
			.def("debugMessagesOn", &RFProtectionFactory::debugMessagesOn)
			.def("debugMessagesOff", &RFProtectionFactory::debugMessagesOff)
			.def("messagesOn", &RFProtectionFactory::messagesOn)
			.def("messagesOff", &RFProtectionFactory::messagesOff)
			.def("isDebugOn", &RFProtectionFactory::isDebugOn)
			.def("isMessagingOn", &RFProtectionFactory::isMessagingOn);
	}



}

#endif //RF_PROTECTION_PYTHON_INTERFACE_H_