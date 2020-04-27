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


#endif //GLOBAL_TYPES_PYTHON_INTERFACE_H_