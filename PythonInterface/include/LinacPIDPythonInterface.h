#ifndef LINAC_PID_PYTHON_INTERFACE_H_
#define LINAC_PID_PYTHON_INTERFACE_H_
#include <LinacPID.h>
#include <LinacPIDFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_LINAC_PID_INCLUDE
{
	void exposeLinacPIDObject()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LinacPID>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LinacPID, boost::python::bases<Hardware>, boost::noncopyable>("LinacPID", boost::python::no_init)
			.add_property("name", &LinacPID::getHardwareName)


			.def("setPhase", &LinacPID::setPhase, boost::python::arg("self"))
			.def("getPhase", &LinacPID::getPhase, boost::python::arg("self"))
			.def("setCrestPhase", &LinacPID::setCrestPhase, boost::python::arg("self"))
			.def("getCrestPhase", &LinacPID::getCrestPhase, boost::python::arg("self"))
			.def("setOperatingPhase", &LinacPID::setOperatingPhase, boost::python::arg("self"))
			.def("getOperatingPhase", &LinacPID::getOperatingPhase, boost::python::arg("self"))

			.def("setForwardPhaseWeight", &LinacPID::setForwardPhaseWeight, boost::python::arg("self"))
			.def("getForwardPhaseWeight", &LinacPID::getForwardPhaseWeight, boost::python::arg("self"))
			.def("getForwardPhaseWrapped", &LinacPID::getForwardPhaseWrapped, boost::python::arg("self"))
			.def("setProbePhaseWeight", &LinacPID::setProbePhaseWeight, boost::python::arg("self"))
			.def("getProbePhaseWeight", &LinacPID::getProbePhaseWeight, boost::python::arg("self"))
			.def("getProbePhaseWrapped", &LinacPID::getProbePhaseWrapped, boost::python::arg("self"))
			.def("getOVAL", &LinacPID::getOVAL, boost::python::arg("self"))
			.def("enable", &LinacPID::enable, boost::python::arg("self"))
			.def("disable", &LinacPID::disable, boost::python::arg("self"))
			.def("isEnabled", &LinacPID::isEnabled, boost::python::arg("self"))
			.def("isDisabled", &LinacPID::isDisabled, boost::python::arg("self"))
			.def("getEnabledState", &LinacPID::getEnabledState, boost::python::arg("self"))
			.def("getMaxPhase", &LinacPID::getMaxPhase, boost::python::arg("self"))
			.def("getMinPhase", &LinacPID::getMinPhase, boost::python::arg("self"))
			.def("getMaxPhaseWeight", &LinacPID::getMaxPhaseWeight, boost::python::arg("self"))
			.def("getMinPhaseWeight", &LinacPID::getMinPhaseWeight, boost::python::arg("self"))

			.def("debugMessagesOn", &LinacPID::debugMessagesOn, boost::python::arg("self"))
			.def("debugMessagesOff", &LinacPID::debugMessagesOff, boost::python::arg("self"))
			.def("messagesOn", &LinacPID::messagesOn, boost::python::arg("self"))
			.def("messagesOff", &LinacPID::messagesOff, boost::python::arg("self"))
			;
	}

	void exposeLinacPIDFactoryObject()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LinacPIDFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LinacPIDFactory, boost::noncopyable>("LinacPIDFactory", boost::python::no_init)
			.def(boost::python::init<STATE>(boost::python::arg("mode")))
			.def("setup", &LinacPIDFactory::setup, (boost::python::arg("self"), boost::python::arg("version")))
			.def("getLinacPID", &LinacPIDFactory::getLinacPID, (boost::python::arg("self"), boost::python::arg("name")), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllLinacPIDNames", &LinacPIDFactory::getAllLinacPIDNames_Py, (boost::python::arg("self")))
			.def("debugMessagesOn", &LinacPIDFactory::debugMessagesOn, (boost::python::arg("self")))
			.def("debugMessagesOff", &LinacPIDFactory::debugMessagesOff, (boost::python::arg("self")))
			.def("messagesOn", &LinacPIDFactory::messagesOn, (boost::python::arg("self")))
			.def("messagesOff", &LinacPIDFactory::messagesOff, (boost::python::arg("self")))
			;
	}

}


#endif //!LINAC_PID_PYTHON_INTERFACE_H_