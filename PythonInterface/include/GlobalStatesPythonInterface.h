#ifndef GLOBAL_STATES_PYTHON_INTERFACE_H_
#define GLOBAL_STATES_PYTHON_INTERFACE_H_
#include <boost/python.hpp>
#include <GlobalStateEnums.h>


namespace BOOST_PYTHON_STATES_INCLUDE
{
	void expose_global_state_enums()
	{
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
			;

	}
}
#endif //GLOBAL_STATES_PYTHON_INTERFACE_H_