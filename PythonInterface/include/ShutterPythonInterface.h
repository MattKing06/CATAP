#ifndef SHUTTER_PYTHON_INTERFACE_H_
#define SHUTTER_PYTHON_INTERFACE_H_

#include <Shutter.h>
#include <ShutterFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_SHUTTER_INCLUDE
{
	void expose_shutter_state_struct() {

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<ShutterState>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<ShutterState>
			("ShutterState", "ShutterState Doc String")
			.def_readonly("name", &ShutterState::name)
			.def_readonly("state", &ShutterState::state)
			.def_readonly("Cmi", &ShutterState::Cmi)
			.def_readonly("interlock_states", &ShutterState::interlock_states_Py)
			;
		//std::string name;
		//STATE state;
		//int Cmi;
		//std::map<std::string, bool>  interlock_states;
		//boost::python::dict interlock_states_Py;
	};
	void expose_shutter_object() 
	{
		//bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Magnet>())->to_python_target_type());
		//if (is_registered) return;
		//// magnet exposure
		//boost::python::class_<Magnet, boost::python::bases<Hardware>, boost::noncopyable>("Magnet", boost::python::no_init)

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Shutter>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Shutter, boost::python::bases<Hardware>, boost::noncopyable>("Shutter", boost::python::no_init)
			
			.def("isEnergyInterlockGood", &Shutter::isEnergyInterlockGood)
			.def("isEnergyInterlockBad", &Shutter::isEnergyInterlockBad)
			.def("isChargeInterlockGood", &Shutter::isChargeInterlockGood)
			.def("isChargeInterlockBad", &Shutter::isChargeInterlockBad)
			.def("isPSInterlockGood", &Shutter::isPSInterlockGood)
			.def("isPSInterlockBad", &Shutter::isPSInterlockBad)
			.def("isClosed", &Shutter::isClosed)
			.def("isOpen",   &Shutter::isOpen)
			.def("getState", &Shutter::getState)
			.def("close",    &Shutter::close)
			.def("open",     &Shutter::open)
			.def("getCMI",     &Shutter::getCMI)
			.def("getCMIBitMap",     &Shutter::getCMIBitMap_Py)
			.def("getAliases",     &Shutter::getAliases_Py)
			.def("getShutterState",     &Shutter::getShutterState)
			.def("getShutterStateDictionary",     &Shutter::getShutterStateDictionary)
			.def("debugMessagesOn", &Shutter::debugMessagesOn)
			.def("debugMessagesOff", &Shutter::debugMessagesOff)
			.def("messagesOn", &Shutter::messagesOn)
			.def("messagesOff", &Shutter::messagesOff)
			.def("isDebugOn", &Shutter::isDebugOn)
			.def("isMessagingOn", &Shutter::isMessagingOn);
	}
	void expose_shutter_factory_object()
	{
		boost::python::arg name = boost::python::arg("name");
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<ShutterFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<ShutterFactory, boost::noncopyable>("ShutterFactory", boost::python::no_init)
			
		
			.def("isEnergyInterlockGood", &ShutterFactory::isEnergyInterlockGood, name)
			.def("isEnergyInterlockBad", &ShutterFactory::isEnergyInterlockBad, name)
			.def("isChargeInterlockGood", &ShutterFactory::isChargeInterlockGood, name)
			.def("isChargeInterlockBad", &ShutterFactory::isChargeInterlockBad, name)
			.def("isPSInterlockGood", &ShutterFactory::isPSInterlockGood, name)
			.def("isPSInterlockBad", &ShutterFactory::isPSInterlockBad, name)

			.def("isClosed", &ShutterFactory::isClosed, name)
			.def("isOpen", &ShutterFactory::isOpen, name)
			.def("getState", &ShutterFactory::getState, name)
			.def("close", &ShutterFactory::close, boost::python::arg("name"))
			.def("open", &ShutterFactory::open, name)
			.def("getCMI", &ShutterFactory::getCMI, name)
			.def("getCMIBitMap", &ShutterFactory::getCMIBitMap_Py, name)
			.def("getFullName", &ShutterFactory::getFullName)
			.def("getAllShutterNames", &ShutterFactory::getAllShutterNames_Py)
			.def("getShutterState", &ShutterFactory::getShutterState)
			.def("getShutterStates", &ShutterFactory::getShutterStates_Py)
			.def("getShutterStateDictionary", &ShutterFactory::getShutterStateDictionary, name)
			.def("getShutterStateDictionaries", &ShutterFactory::getShutterStateDictionaries)
			.def("getShutter", &ShutterFactory::getShutter, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("debugMessagesOn", &ShutterFactory::debugMessagesOn)
			.def("debugMessagesOff", &ShutterFactory::debugMessagesOff)
			.def("messagesOn", &ShutterFactory::messagesOn)
			.def("messagesOff", &ShutterFactory::messagesOff)
			.def("isDebugOn", &ShutterFactory::isDebugOn)
			.def("isMessagingOn", &ShutterFactory::isMessagingOn);
			;
	}



}

#endif //SHUTTER_PYTHON_INTERFACE_H_