#ifndef RF_MODULATOR_PYTHON_INTERFACE_H_
#define RF_MODULATOR_PYTHON_INTERFACE_H_

#include <RFModulator.h>
#include <RFModulatorFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_RF_MODULATOR_INCLUDE
{

	void expose_rf_modulator_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFModulator>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<RFModulator, boost::python::bases<Hardware>, boost::noncopyable>("RFModulator", boost::python::no_init)

			.def("getLowLevelNumericalData", &RFModulator::getLowLevelNumericalData_Py)
			.def("getLowLevelStringData", &RFModulator::getLowLevelStringData_Py)
			.def("getLowLevelData", &RFModulator::getLowLevelData)

			.def("getMainState", &RFModulator::getMainState)
			.def("isInRFOn", &RFModulator::isInRFOn)
			.def("isInHVOn", &RFModulator::isInHVOn)
			.def("isInStandby", &RFModulator::isInStandby)
			.def("isInOff", &RFModulator::isInOff)

			.def("getErrorState", &RFModulator::getErrorState)
			.def("isErrorStateGood", &RFModulator::isErrorStateGood)
			.def("getStateSetRead", &RFModulator::getStateSetRead)

			.def("isWarm", &RFModulator::isWarm)
			.def("isNotWarm", &RFModulator::isNotWarm)

			.def("getAliases", &RFModulator::getAliases_Py)
			.def("isInRFOn", &RFModulator::isInRFOn)
			.def("isInHVOn", &RFModulator::isInHVOn)
			.def("isInStandby", &RFModulator::isInStandby)
			.def("isInOff", &RFModulator::isInOff)

				
				
			.def("debugMessagesOn", &RFModulator::debugMessagesOn)
			.def("debugMessagesOff", &RFModulator::debugMessagesOff)
			.def("messagesOn", &RFModulator::messagesOn)
			.def("messagesOff", &RFModulator::messagesOff)
			.def("isDebugOn", &RFModulator::isDebugOn)
			.def("isMessagingOn", &RFModulator::isMessagingOn);
	}
	void expose_rf_modulator_factory_object() 
	{
		bool(RFModulatorFactory:: *setup_all)(const std::string&) = &RFModulatorFactory::setup;
		bool(RFModulatorFactory:: * setup_by_area)(const std::string&, const std::vector<TYPE>& machine_areas) = &RFModulatorFactory::setup;
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFModulatorFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<RFModulatorFactory, boost::noncopyable>("RFModulatorFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def(boost::python::init<STATE, const std::string>())
			.def("setup", setup_all)
			.def("setup", setup_by_area)
			.def("getModulator", &RFModulatorFactory::getModulator, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getFullName", &RFModulatorFactory::getFullName)
			.def("getAllRFModulatorNames", &RFModulatorFactory::getAllRFModulatorNames_Py)
			.def("getAliases", &RFModulatorFactory::getAliases_Py)

			.def("getMainState", &RFModulatorFactory::getMainState)
			.def("isInRFOn", &RFModulatorFactory::isInRFOn)
			.def("isInHVOn", &RFModulatorFactory::isInHVOn)
			.def("isInStandby", &RFModulatorFactory::isInStandby)
			.def("isInOff", &RFModulatorFactory::isInOff)

			.def("getErrorState", &RFModulatorFactory::getErrorState)
			.def("isErrorStateGood", &RFModulatorFactory::isErrorStateGood)
			.def("getStateSetRead", &RFModulatorFactory::getStateSetRead)

			.def("isWarm", &RFModulatorFactory::isWarm)
			.def("isNotWarm", &RFModulatorFactory::isNotWarm)
			.def("getLowLevelNumericalData", &RFModulatorFactory::getLowLevelNumericalData_Py)
			.def("getLowLevelStringData", &RFModulatorFactory::getLowLevelStringData_Py)
			.def("getLowLevelData", &RFModulatorFactory::getLowLevelData)

				
			.def("debugMessagesOn", &RFModulatorFactory::debugMessagesOn)
			.def("debugMessagesOff", &RFModulatorFactory::debugMessagesOff)
			.def("messagesOn", &RFModulatorFactory::messagesOn)
			.def("messagesOff", &RFModulatorFactory::messagesOff)
			.def("isDebugOn", &RFModulatorFactory::isDebugOn)
			.def("isMessagingOn", &RFModulatorFactory::isMessagingOn);
	}



}

#endif //RF_MODULATOR_PYTHON_INTERFACE_H_