#include <HardwareFactoryPythonInterface.h>
#include <HardwarePythonInterface.h>
#include <GlobalStatesPythonInterface.h>
#include <GlobalTypesPythonInterface.h>
#include <BPMPythonInterface.h>
#include <ChargePythonInterface.h>
#include <EPICSToolsPythonInterface.h>
#include <IMGPythonInterface.h>
#include <LaserMirrorPythonInterface.h>
#include <LEDPythonInterface.h>
#include <LightingPythonInterface.h>
#include <LinacPIDPythonInterface.h>
#include <LLRFPythonInterface.h>
#include <MagnetPythonInterface.h>
#include <RFHeartbeatPythonInterface.h>
#include <RFModulatorPythonInterface.h>
#include <RFProtectionPythonInterface.h>
#include <CameraPythonInterface.h>
#include <ScreenPythonInterface.h>
#include <ShutterPythonInterface.h>
#include <StagePythonInterface.h>
#include <ValvePythonInterface.h>
#include <LaserEnergyMeterPythonInterface.h>
#include <LaserHWPPythonInterface.h>

BOOST_PYTHON_MODULE(_HardwareFactory)
{
	// IT IS NECESSARY TO EXPOSE EVERYTHING IN THIS MODULE
	// AS THE USER COULD SOLELY IMPORT HARDWARE FACTORY AND
	// NEEDS ACCESS TO ALL POSSIBLE CLASSES.
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_TYPES_INCLUDE::expose_global_type_enums();
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_HARDWARE_FACTORY_INCLUDE::exposeHardwareFactoryObject();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_object();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_factory_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_factory_object();
	BOOST_PYTHON_EPICS_TOOLS_INCLUDE::expose_epics_tools_object();
	BOOST_PYTHON_EPICS_TOOLS_INCLUDE::expose_listener_object();
	BOOST_PYTHON_IMG_INCLUDE::expose_img_object();
	BOOST_PYTHON_IMG_INCLUDE::expose_img_factory_object();
	BOOST_PYTHON_LASER_MIRROR_INCLUDE::expose_laser_mirror_object();
	BOOST_PYTHON_LASER_MIRROR_INCLUDE::expose_laser_mirror_factory_object();
	BOOST_PYTHON_LED_INCLUDE::expose_led_object();
	BOOST_PYTHON_LED_INCLUDE::expose_led_factory_object();
	BOOST_PYTHON_LIGHTING_INCLUDE::expose_lighting_object();
	BOOST_PYTHON_LIGHTING_INCLUDE::expose_lighting_factory_object();
	BOOST_PYTHON_LLRF_INCLUDE::expose_llrf_object();
	BOOST_PYTHON_LLRF_INCLUDE::expose_llrf_factory_object();
	BOOST_PYTHON_CAMERA_INCLUDE::expose_camera_object();
	BOOST_PYTHON_CAMERA_INCLUDE::expose_camera_factory_object();

	BOOST_PYTHON_LINAC_PID_INCLUDE::exposeLinacPIDObject();
	BOOST_PYTHON_LINAC_PID_INCLUDE::exposeLinacPIDFactoryObject();



	BOOST_PYTHON_SHUTTER_INCLUDE::expose_shutter_state_struct();
	BOOST_PYTHON_SHUTTER_INCLUDE::expose_shutter_object();
	BOOST_PYTHON_SHUTTER_INCLUDE::expose_shutter_factory_object();

	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_factory_object();

	BOOST_PYTHON_RF_HEARTBEAT_INCLUDE::expose_rf_heartbeat_object();
	BOOST_PYTHON_RF_HEARTBEAT_INCLUDE::expose_rf_heartbeat_factory_object();
	BOOST_PYTHON_RF_MODULATOR_INCLUDE::expose_rf_modulator_object();
	BOOST_PYTHON_RF_MODULATOR_INCLUDE::expose_rf_modulator_factory_object();
	BOOST_PYTHON_RF_PROTECTION_INCLUDE::expose_rf_protection_object();
	BOOST_PYTHON_RF_PROTECTION_INCLUDE::expose_rf_protection_factory_object();
	BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_object();
	BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_factory_object();
	BOOST_PYTHON_STAGE_INCLUDE::expose_stage_object();
	BOOST_PYTHON_STAGE_INCLUDE::expose_stage_factory_object();
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveObject();
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveFactoryObject();
	BOOST_PYTHON_LASER_ENERGY_METER_INCLUDE::expose_laser_energy_meter_object();
	BOOST_PYTHON_LASER_ENERGY_METER_INCLUDE::expose_laser_energy_meter_factory_object();
	BOOST_PYTHON_LASER_HWP_INCLUDE::expose_laser_hwp_object();
	BOOST_PYTHON_LASER_HWP_INCLUDE::expose_laser_hwp_factory_object();
}