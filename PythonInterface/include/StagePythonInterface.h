#ifndef STAGE_PYTHON_INTERFACE_H_
#define STAGE_PYTHON_INTERFACE_H_

#include <Stage.h>
#include <StageFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_STAGE_INCLUDE
{

	void expose_stage_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Stage>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Stage, boost::python::bases<Hardware>, boost::noncopyable>("Stage", boost::python::no_init)
			.def("moveToDevice", &Stage::moveToDevice, boost::python::args("self", "device_name"), "Moves the stage to the position associated with device (found in config files)")
			.def("clearForBeam", &Stage::clearForBeam, boost::python::args("self"), "Moves the stage to the \"CLEAR FOR BEAM\" position (found in the config files)")
			.def("isReadPosEqualToSetPos", &Stage::isReadPositionEqualToSetPosition, boost::python::args("self"), "Checks that the set position is equal to the read position.")
			.def("debugMessagesOn", &Stage::debugMessagesOn)
			.def("debugMessagesOff", &Stage::debugMessagesOff)
			.def("messagesOn", &Stage::messagesOn)
			.def("messagesOff", &Stage::messagesOff)
			.def("isDebugOn", &Stage::isDebugOn)
			.def("isMessagingOn", &Stage::isMessagingOn);
	}
	void expose_stage_factory_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<StageFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<StageFactory, boost::noncopyable>("StageFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def(boost::python::init<STATE, const std::string>())
			.def("setup", &StageFactory::setup, boost::python::args("self", "version"), "Sets up the EPICS connections and loads configuration files for stages (version=\"nominal\")")
			.def("getStage", &StageFactory::getStage, boost::python::return_value_policy<boost::python::reference_existing_object>(), boost::python::args("self", "name"), "returns the stage object using the name provided.")
			.def("moveStageToDevice", &StageFactory::moveStageToDevice, boost::python::args("self", "name", "device"), "Moves the stage to the position associated with device (found in config files) using the stage and device name provided")
			.def("isReadPosEqualToSetPos", &StageFactory::isReadPositionEqualToSetPosition, boost::python::args("self", "name"), "Checks that the set position is equal to the read position for the stage name provided.")
			.def("clearAllForBeam", &StageFactory::clearAllForBeam_Py, "Clears all stages for beam, returns a dictionary (key=stage_name, value=clear_for_beam_status.")
			.def("debugMessagesOn", &StageFactory::debugMessagesOn)
			.def("debugMessagesOff", &StageFactory::debugMessagesOff)
			.def("messagesOn", &StageFactory::messagesOn)
			.def("messagesOff", &StageFactory::messagesOff)
			.def("isDebugOn", &StageFactory::isDebugOn)
			.def("isMessagingOn", &StageFactory::isMessagingOn);
	}



}

#endif //STAGE_PYTHON_INTERFACE_H_