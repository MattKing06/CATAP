#ifndef GLOBAL_STATES_PYTHON_INTERFACE_H_
#define GLOBAL_STATES_PYTHON_INTERFACE_H_
#include <boost/python.hpp>
#include <boost/python/converter/registry.hpp>
#include <GlobalStateEnums.h>


namespace BOOST_PYTHON_STATES_INCLUDE
{
	void expose_global_state_enums()
	{
		//Global State Enum exposure
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<STATE>())->to_python_target_type());
		if (is_registered) return;
		boost::python::enum_<STATE>("STATE", "Global Enums used for displaying States")
			.value("On", STATE::ON)
			.value("OFF", STATE::OFF)
			.value("ERROR", STATE::ERR)
			.value("UNKNOWN_NAME", STATE::UNKNOWN_NAME)
			.value("SUCCESS", STATE::SUCCESS)
			.value("FAIL", STATE::FAIL)
			.value("OFFLINE", STATE::OFFLINE)
			.value("PHYSICAL", STATE::PHYSICAL)
			.value("VIRTUAL", STATE::VIRTUAL)
			.value("GOOD", STATE::GOOD)
			.value("BAD", STATE::BAD)
			.value("OK", STATE::OK)
			.value("OPEN", STATE::OPEN)
			.value("CLOSED", STATE::CLOSED)
			.value("NONLINEAR", STATE::NONLINEAR)
			.value("UNKNOWN", STATE::UNKNOWN)
			.value("ENABLED", STATE::ENABLED)
			.value("DISABLED", STATE::DISABLED)
			.value("HRETRACTED", STATE::HRETRACTED)
			.value("HMAX", STATE::HMAX)
			.value("HSLIT1", STATE::HSLIT1)
			.value("HSLIT2", STATE::HSLIT2)
			.value("HSLIT3", STATE::HSLIT3)
			.value("HAPT1", STATE::HAPT1)
			.value("HAPT2", STATE::HAPT2)
			.value("HAPT3", STATE::HAPT3)
			.value("VRETRACTED", STATE::VRETRACTED)
			.value("VMAX", STATE::VMAX)
			.value("VRF", STATE::VRF)
			.value("VMIRROR", STATE::VMIRROR)
			.value("VYAG", STATE::VYAG)
			.value("VGRAT", STATE::VGRAT)
			.value("VCOL", STATE::VCOL)
			.value("VSLIT1", STATE::VSLIT1)
			.value("RETRACTED", STATE::RETRACTED)
			.value("YAG", STATE::YAG)
			.value("RF", STATE::RF)
			.value("ACQUIRING", STATE::ACQUIRING)
			.value("NOT_ACQUIRING", STATE::NOT_ACQUIRING)
			.value("ANALYSING", STATE::ANALYSING)
			.value("NOT_ANALYSING", STATE::NOT_ANALYSING)
			.value("CAPTURING", STATE::CAPTURING)
			.value("NOT_CAPTURING", STATE::NOT_CAPTURING)
			.value("WRITING", STATE::WRITING)
			.value("NOT_WRITING", STATE::NOT_WRITING)
			.value("SAVING", STATE::SAVING)
			.value("NOT_SAVING", STATE::NOT_SAVING)
			;
	}
}
#endif //GLOBAL_STATES_PYTHON_INTERFACE_H_