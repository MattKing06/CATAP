#ifndef LASER_MIRROR_PYTHON_INTERFACE_H_
#define LASER_MIRROR_PYTHON_INTERFACE_H_

#include <LaserMirror.h>
#include <LaserMirrorFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_LASER_MIRROR_INCLUDE
{

	void expose_laser_mirror_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LaserMirror>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LaserMirror, boost::python::bases<Hardware>, boost::noncopyable>("LaserMirror", boost::python::no_init)
			.def("debugMessagesOn", &LaserMirror::debugMessagesOn)
			.def("debugMessagesOff", &LaserMirror::debugMessagesOff)
			.def("messagesOn", &LaserMirror::messagesOn)
			.def("messagesOff", &LaserMirror::messagesOff)
			.def("isDebugOn", &LaserMirror::isDebugOn)
			.def("isMessagingOn", &LaserMirror::isMessagingOn);
	}
	void expose_laser_mirror_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LaserMirrorFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LaserMirrorFactory, boost::noncopyable>("LaserMirrorFactory", boost::python::no_init)
			.def("debugMessagesOn", &LaserMirrorFactory::debugMessagesOn)
			.def("debugMessagesOff", &LaserMirrorFactory::debugMessagesOff)
			.def("messagesOn", &LaserMirrorFactory::messagesOn)
			.def("messagesOff", &LaserMirrorFactory::messagesOff)
			.def("isDebugOn", &LaserMirrorFactory::isDebugOn)
			.def("isMessagingOn", &LaserMirrorFactory::isMessagingOn);
	}



}

#endif //LASER_MIRROR_PYTHON_INTERFACE_H_