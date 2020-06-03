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
			.def("debugMessagesOn", &StageFactory::debugMessagesOn)
			.def("debugMessagesOff", &StageFactory::debugMessagesOff)
			.def("messagesOn", &StageFactory::messagesOn)
			.def("messagesOff", &StageFactory::messagesOff)
			.def("isDebugOn", &StageFactory::isDebugOn)
			.def("isMessagingOn", &StageFactory::isMessagingOn);
	}



}

#endif //STAGE_PYTHON_INTERFACE_H_