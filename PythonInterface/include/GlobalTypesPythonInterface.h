#ifndef GLOBAL_TYPES_PYTHON_INTERFACE_H_
#define GLOBAL_TYPES_PYTHON_INTERFACE_H_

#include <GlobalTypeEnums.h>
#include <boost/python.hpp>
#include <boost/python/converter/registry.hpp>

namespace BOOST_PYTHON_TYPES_INCLUDE
{
	void expose_global_type_enums()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<TYPE>())->to_python_target_type());
		if (is_registered) return;
		boost::python::enum_<TYPE>("TYPE")
			//hardware types 
			.value("MAGNET", TYPE::MAGNET)
			.value("CHARGE", TYPE::CHARGE)
			.value("BPM_TYPE", TYPE::BPM_TYPE)
			.value("VALVE", TYPE::VALVE)
			.value("IMG_TYPE", TYPE::IMG_TYPE)
			.value("LLRF_TYPE", TYPE::LLRF_TYPE)
			.value("SCREEN", TYPE::SCREEN)
			.value("CAMERA_TYPE", TYPE::CAMERA_TYPE)
			// screen stuff 
			.value("VELA_PNEUMATIC", TYPE::VELA_PNEUMATIC)
			.value("VELA_HV_MOVER", TYPE::VELA_HV_MOVER)
			.value("CLARA_HV_MOVER", TYPE::CLARA_HV_MOVER)
			.value("CLARA_V_MOVER", TYPE::CLARA_V_MOVER)
			.value("CLARA_PNEUMATIC", TYPE::CLARA_PNEUMATIC)
			.value("UNKNOWN_SCREEN_TYPE", TYPE::UNKNOWN_SCREEN_TYPE)
			.value("HORIZONTAL", TYPE::HORIZONTAL)
			.value("VERTICAL", TYPE::VERTICAL)
			.value("PNEUMATIC", TYPE::PNEUMATIC)
			.value("UNKNOWN_TYPE", TYPE::UNKNOWN_TYPE)
			
			// magnet stuff
			.value("QUADRUPOLE", TYPE::QUADRUPOLE)
			.value("DIPOLE", TYPE::DIPOLE)
			//.value("HVCOR", TYPE::HVCOR)
			.value("VERTICAL_CORRECTOR", TYPE::VERTICAL_CORRECTOR)
			.value("HORIZONTAL_CORRECTOR", TYPE::HORIZONTAL_CORRECTOR)
			.value("SOLENOID", TYPE::SOLENOID)
			.value("BUCKING_SOLENOID", TYPE::BUCKING_SOLENOID)
			.value("NR", TYPE::NR)
			.value("BIPOLAR", TYPE::BIPOLAR)
			.value("NR_GANGED", TYPE::NR_GANGED)
			.value("POSITIVE_ONLY", TYPE::POSITIVE_ONLY)
			.value("UNKNOWN_AREA", TYPE::UNKNOWN_AREA)
			.value("ALL_VELA_CLARA", TYPE::ALL_VELA_CLARA)
			.value("VELA_LASER", TYPE::VELA_LASER)
			.value("CLARA_LASER", TYPE::CLARA_LASER)
			.value("BA1", TYPE::BA1)
			.value("BA2", TYPE::BA2)
			.value("VELA", TYPE::VELA)
			.value("INJ", TYPE::INJ)
			.value("GUN", TYPE::GUN)
			.value("S01", TYPE::S01)
			.value("S02", TYPE::S02)
			.value("C2V", TYPE::C2V)
			.value("L01", TYPE::L01)
			.value("VELA_2_BA1", TYPE::VELA_2_BA1)
			.value("VELA_2_BA1_BA2", TYPE::VELA_2_BA1_BA2)
			.value("VELA_INJ", TYPE::VELA_INJ)
			.value("VELA_GUN", TYPE::VELA_GUN)
			.value("CLARA_GUN", TYPE::CLARA_GUN)
			.value("HRRG_GUN", TYPE::HRRG) // Typo get rid
			.value("LRRG_GUN", TYPE::LRRG) // Typo get rid
			.value("HRRG", TYPE::HRRG)
			.value("LRRG", TYPE::LRRG)
			.value("CLARA_PH1", TYPE::CLARA_PH1)
			.value("CLARA_2_BA1", TYPE::CLARA_2_BA1)
			.value("CLARA_2_BA1_BA2", TYPE::CLARA_2_BA1_BA2)
			.value("CAMERA_TYPE", TYPE::CAMERA_TYPE)
			.value("CLARA_CAMERA", TYPE::CLARA_CAMERA)
			.value("VELA_CAMERA", TYPE::VELA_CAMERA)
			.value("BPM_TYPE", TYPE::BPM_TYPE)
			.value("CHARGE", TYPE::CHARGE)
			.value("HWP", TYPE::HWP)
			.value("SHUTTER", TYPE::SHUTTER)
			.value("ENERGYMETER", TYPE::ENERGYMETER)
			.value("VALVE", TYPE::VALVE)
			.value("CHARGE", TYPE::CHARGE)
			;
	}
}


#endif //GLOBAL_TYPES_PYTHON_INTERFACE_H_