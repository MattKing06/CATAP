#ifndef GLOBAL_STATES_PYTHON_INTERFACE_H_
#define GLOBAL_STATES_PYTHON_INTERFACE_H_
#include <boost/python.hpp>
#include <boost/python/converter/registry.hpp>
#include <GlobalStateEnums.h>



/*
  ___ ___   _   ___  __  __ ___ 
 | _ \ __| /_\ |   \|  \/  | __|
 |   / _| / _ \| |) | |\/| | _| 
 |_|_\___/_/ \_\___/|_|  |_|___|
                                
	THE ENTRIES IN HERE SHOULD BE THE SAME AS IN GlobalStateEnums.h
*/
namespace BOOST_PYTHON_STATES_INCLUDE
{
	void expose_global_state_enums()
	{
		//Global State Enum exposure
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<STATE>())->to_python_target_type());
		if (is_registered) return;



		boost::python::enum_<STATE>("STATE", "Global Enums used for displaying States")
			.value("On", STATE::ON) // Err ???  
			.value("OFF", STATE::OFF)
			.value("ON", STATE::ON)
			.value("ERR", STATE::ERR)
			.value("ON", STATE::ON) // TODO should we just overload upper and lower case versions of these ?? 
			.value("OFF", STATE::OFF)
			.value("UNKNOWN_NAME", STATE::UNKNOWN_NAME)
			.value("SUCCESS", STATE::SUCCESS)
			.value("FAIL", STATE::FAIL)
			.value("TIMING", STATE::TIMING)
			.value("OFFLINE", STATE::OFFLINE)
			.value("PHYSICAL", STATE::PHYSICAL)
			.value("VIRTUAL", STATE::VIRTUAL)
			.value("ERROR", STATE::ERR)
			.value("ERR", STATE::ERR)
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
			//.value("SAVING", STATE::SAVING)
			//.value("NOT_SAVING", STATE::NOT_SAVING)
			.value("WRITE_CHECK_OK", STATE::WRITE_CHECK_OK)
			.value("WRITE_CHECK_ERROR", STATE::WRITE_CHECK_ERROR)
			.value("TIMEOUT", STATE::TIMEOUT)
			.value("NOT_USING_FLOOR", STATE::NOT_USING_FLOOR)
			.value("USING_FLOOR", STATE::USING_FLOOR)
			.value("USING_NPOINT", STATE::USING_NPOINT)
			.value("NOT_USING_NPOINT", STATE::NOT_USING_NPOINT)
			.value("USING_BACKGROUND", STATE::USING_BACKGROUND)
			.value("NOT_USING_BACKGROUND", STATE::NOT_USING_BACKGROUND)
			// LLRF trace SCAN STATEs
			.value("PASSIVE", STATE::PASSIVE)
			.value("EVENT", STATE::EVENT)
			.value("IO_INTR", STATE::IO_INTR)
			.value("TEN", STATE::TEN)    
			.value("FIVE", STATE::FIVE)
			.value("TWO", STATE::TWO)
			.value("ONE", STATE::ONE)
			.value("ZERO_POINT_FIVE", STATE::ZERO_POINT_FIVE)
			.value("ZERO_POINT_TWO", STATE::ZERO_POINT_TWO)   
			.value("ZERO_POINT_ONE", STATE::ZERO_POINT_ONE)    
			.value("ZERO_POINT_ZERO_FIVE", STATE::ZERO_POINT_ZERO_FIVE)
			.value("NOW", STATE::NOW)		    
			// LLRF trigger STATEs
			.value("EXTERNAL", STATE::EXTERNAL)
			.value("INTERNAL", STATE::INTERNAL)
			.value("NOT_ACTIVE", STATE::NOT_ACTIVE)
			.value("ACTIVE", STATE::ACTIVE)
			// LLRF trace ACQM STATEs
			.value("UNKNOWN_ACQM", STATE::UNKNOWN_ACQM)
			.value("ACQM_NOW", STATE::ACQM_NOW)
			.value("ACQM_EVENT", STATE::ACQM_EVENT)
		;
	}
}
#endif //GLOBAL_STATES_PYTHON_INTERFACE_H_