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
		boost::python::class_<RFHeartbeat, boost::python::bases<Hardware>, boost::noncopyable>("RFHeartbeat", boost::python::no_init)
			.def("debugMessagesOn", &RFHeartbeat::debugMessagesOn)
			.def("debugMessagesOff", &RFHeartbeat::debugMessagesOff)
			.def("messagesOn", &RFHeartbeat::messagesOn)
			.def("messagesOff", &RFHeartbeat::messagesOff)
			.def("isDebugOn", &RFHeartbeat::isDebugOn)
			.def("isMessagingOn", &RFHeartbeat::isMessagingOn);
	}
	void expose_rf_heartbeat_factory_object() 
	{
		boost::python::class_<RFHeartbeatFactory, boost::noncopyable>("RFHeartbeatFactory", boost::python::no_init)
			.def("debugMessagesOn", &RFHeartbeatFactory::debugMessagesOn)
			.def("debugMessagesOff", &RFHeartbeatFactory::debugMessagesOff)
			.def("messagesOn", &RFHeartbeatFactory::messagesOn)
			.def("messagesOff", &RFHeartbeatFactory::messagesOff)
			.def("isDebugOn", &RFHeartbeatFactory::isDebugOn)
			.def("isMessagingOn", &RFHeartbeatFactory::isMessagingOn);
	}



}

#endif //RF_HEARTBEAT_PYTHON_INTERFACE_H_