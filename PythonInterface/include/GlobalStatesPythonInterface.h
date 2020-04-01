#ifndef GLOBAL_STATES_PYTHON_INTERFACE_H_
#define GLOBAL_STATES_PYTHON_INTERFACE_H_
#include <boost/python.hpp>
#include <GlobalStateEnums.h>
#include <GlobalTypeEnums.h>

namespace BOOST_PYTHON_STATES_INCLUDE
{
	void expose_global_state_enums()
	{
		//Global State Enum exposure
		boost::python::enum_<STATE>("STATE", "Global Enums used for displaying States")
			.value("On", STATE::ON)
			.value("OFF", STATE::OFF)
			.value("ERROR", STATE::ERR)
			.value("UNKNOWN_NAME", STATE::UNKNOWN_NAME)
			.value("SUCCESS", STATE::SUCCESS)
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
			;
	}
	void expose_global_type_enums()
	{
		boost::python::enum_<TYPE>("TYPE")
			.value("VELA_PNEUMATIC", TYPE::VELA_PNEUMATIC)
			.value("VELA_HV_MOVER", TYPE::VELA_HV_MOVER)
			.value("CLARA_HV_MOVER", TYPE::CLARA_HV_MOVER)
			.value("CLARA_V_MOVER", TYPE::CLARA_V_MOVER)
			.value("CLARA_PNEUMATIC", TYPE::CLARA_PNEUMATIC)
			.value("UNKNOWN_SCREEN_TYPE", TYPE::UNKNOWN_SCREEN_TYPE)
			.value("HORIZONTAL", TYPE::HORIZONTAL)
			.value("VERTICAL", TYPE::VERTICAL)
			.value("PNEUMATIC", TYPE::PNEUMATIC)
			;
	}
}
#endif //GLOBAL_STATES_PYTHON_INTERFACE_H_