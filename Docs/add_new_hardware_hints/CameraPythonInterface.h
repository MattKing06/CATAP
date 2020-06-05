#ifndef CAMERA_PYTHON_INTERFACE_H_
#define CAMERA_PYTHON_INTERFACE_H_

#include <Camera.h>
#include <CameraFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_CAMERA_INCLUDE
{

	void expose_camera_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Camera>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Camera, boost::python::bases<Hardware>, boost::noncopyable>("Camera", boost::python::no_init)

			.def("getAliases", &Camera::getAliases_Py)
			.def("debugMessagesOff", &Camera::debugMessagesOff)
			.def("messagesOn", &Camera::messagesOn)
			.def("messagesOff", &Camera::messagesOff)
			.def("isDebugOn", &Camera::isDebugOn)
			.def("isMessagingOn", &Camera::isMessagingOn);
	}
	void expose_camera_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<CameraFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<CameraFactory, boost::noncopyable>("CameraFactory", boost::python::no_init)
			.def("getCameraNames", &CameraFactory::getCameraNames_Py)
			.def("debugMessagesOn", &CameraFactory::debugMessagesOn)
			.def("debugMessagesOff", &CameraFactory::debugMessagesOff)
			.def("messagesOn", &CameraFactory::messagesOn)
			.def("messagesOff", &CameraFactory::messagesOff)
			.def("isDebugOn", &CameraFactory::isDebugOn)
			.def("isMessagingOn", &CameraFactory::isMessagingOn);
	}



}

#endif //CAMERA_PYTHON_INTERFACE_H_