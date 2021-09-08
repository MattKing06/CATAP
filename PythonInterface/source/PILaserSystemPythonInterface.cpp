#include <PILaserSystemPythonInterface.h>
#include <GlobalStatesPythonInterface.h>
#include <GlobalTypesPythonInterface.h>
#include <RunningStatsPythonInterface.h>
#include <CameraPythonInterface.h>
#include <ChargePythonInterface.h>
#include <LaserEnergyMeterPythonInterface.h>
#include <LaserHWPPythonInterface.h>
#include <LaserMirrorPythonInterface.h>
#include <ShutterPythonInterface.h>
#include <HardwarePythonInterface.h>

BOOST_PYTHON_MODULE(_PILaserSystem)
{
	boost::python::docstring_options CATAP_docstring_options;
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_TYPES_INCLUDE::expose_global_type_enums();
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
	BOOST_PYTHON_LASER_FEEDBACK_INCLUDE::exposePILaserSystemObject();
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	// EXPOSE CAMERA/SHUTTER/MIRROR/HWP/ENERGY_METER/CHARGE
	BOOST_PYTHON_RUNNING_STATS_INCLUDE::expose_running_stats_object();
	BOOST_PYTHON_CAMERA_INCLUDE::expose_camera_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_object();
	BOOST_PYTHON_LASER_ENERGY_METER_INCLUDE::expose_laser_energy_meter_object();
	BOOST_PYTHON_LASER_HWP_INCLUDE::expose_laser_hwp_object();
	BOOST_PYTHON_LASER_MIRROR_INCLUDE::expose_laser_mirror_object();
	BOOST_PYTHON_SHUTTER_INCLUDE::expose_shutter_object();
	
}