#ifndef RF_HEARTBEAT_PYTHON_INTERFACE_H_
#define RF_HEARTBEAT_PYTHON_INTERFACE_H_

#include <RFHeartbeat.h>
#include <RFHeartbeatFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_RF_HEARTBEAT_INCLUDE
{

	void expose_rf_heartbeat_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFHeartbeat>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<RFHeartbeat, boost::python::bases<Hardware>, boost::noncopyable>("RFHeartbeat", boost::python::no_init)
			.def("debugMessagesOn", &RFHeartbeat::debugMessagesOn)
			.def("debugMessagesOff", &RFHeartbeat::debugMessagesOff)
			.def("attachContext", &RFHeartbeat::attachToInitialContext)
			.def("getValue", &RFHeartbeat::getValue)
			.def("setValue", &RFHeartbeat::setValue)
			.def("pulse", &RFHeartbeat::pulse)

			.def("messagesOn", &RFHeartbeat::messagesOn)
			.def("messagesOff", &RFHeartbeat::messagesOff)
			.def("isDebugOn", &RFHeartbeat::isDebugOn)
			.def("isMessagingOn", &RFHeartbeat::isMessagingOn);
	}
	void expose_rf_heartbeat_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFHeartbeatFactory>())->to_python_target_type());
		if (is_registered) return;
		void(RFHeartbeatFactory:: * attachContext_single)(const std::string&) = &RFHeartbeatFactory::attachContext;
		void(RFHeartbeatFactory:: * attachContext_all)(void) = &RFHeartbeatFactory::attachContext;
		boost::python::class_<RFHeartbeatFactory, boost::noncopyable>("RFHeartbeatFactory", boost::python::no_init)
			.def(boost::python::init<STATE>(boost::python::arg("mode")))
			.def(boost::python::init<STATE, const std::string>())
			.def("setup", &RFHeartbeatFactory::setup, (boost::python::arg("self"), boost::python::arg("version")))
			.def("attachContext", &RFHeartbeatFactory::attachContext_Py)
			.def("attachContext", attachContext_single)
			.def("attachContext", attachContext_all)
			.def("getValue", &RFHeartbeatFactory::getValue)
			.def("setValue", &RFHeartbeatFactory::setValue)
			.def("pulse", &RFHeartbeatFactory::pulse)
			.def("getHeartbeatNames", &RFHeartbeatFactory::getHeartbeatNames_Py)
			.def("getFullName", &RFHeartbeatFactory::getFullName)
	
			.def("getRFHeartbeat", &RFHeartbeatFactory::getRFHeartbeat, (boost::python::arg("self"), boost::python::arg("name")), boost::python::return_value_policy<boost::python::reference_existing_object>())

			
			.def("debugMessagesOn", &RFHeartbeatFactory::debugMessagesOn)
			.def("debugMessagesOff", &RFHeartbeatFactory::debugMessagesOff)
			.def("messagesOn", &RFHeartbeatFactory::messagesOn)
			.def("messagesOff", &RFHeartbeatFactory::messagesOff)
			.def("isDebugOn", &RFHeartbeatFactory::isDebugOn)
			.def("isMessagingOn", &RFHeartbeatFactory::isMessagingOn);
	}



}

#endif //RF_HEARTBEAT_PYTHON_INTERFACE_H_