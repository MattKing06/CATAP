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
			.def("attachContext", &LaserMirror::attachToInitialContext)
			.def("detachContext", &LaserMirror::detachFromInitialContext)
			.def("getHorizontalPosition", &LaserMirror::getCurrentHorizontalPosition)
			.def("getVerticalPosition", &LaserMirror::getCurrentVerticalPosition)
			.def("moveLeft", &LaserMirror::moveLeft)
			.def("moveRight", &LaserMirror::moveRight)
			.def("moveUp", &LaserMirror::moveUp)
			.def("moveDown", &LaserMirror::moveDown)

			.def("setHStep", &LaserMirror::setHStep)
			.def("setVStep", &LaserMirror::setVStep)
			.def("getHStep", &LaserMirror::getHStep)
			.def("getVStep", &LaserMirror::getVStep)

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
		void(LaserMirrorFactory:: * attachContext_single)(const std::string&) = &LaserMirrorFactory::attachContext;
		void(LaserMirrorFactory:: * attachContext_all)(void) = &LaserMirrorFactory::attachContext;
		void(LaserMirrorFactory:: * detachContext_single)(const std::string&) = &LaserMirrorFactory::detachContext;
		void(LaserMirrorFactory:: * detachContext_all)(void) = &LaserMirrorFactory::detachContext;
		boost::python::class_<LaserMirrorFactory, boost::noncopyable>("LaserMirrorFactory", boost::python::no_init)
			.def(boost::python::init<STATE, const std::string>())
			.def(boost::python::init<STATE>())
			.def("setup", &LaserMirrorFactory::setup)
			.def("attachContext", &LaserMirrorFactory::attachContext_Py)
			.def("attachContext", attachContext_single)
			.def("attachContext", attachContext_all)
			.def("detachContext", &LaserMirrorFactory::detachContext_Py)
			.def("detachContext", detachContext_single)
			.def("detachContext", detachContext_all)
			.def("getLaserMirror", &LaserMirrorFactory::getLaserMirror, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("debugMessagesOn", &LaserMirrorFactory::debugMessagesOn)
			.def("debugMessagesOff", &LaserMirrorFactory::debugMessagesOff)
			.def("messagesOn", &LaserMirrorFactory::messagesOn)
			.def("messagesOff", &LaserMirrorFactory::messagesOff)
			.def("isDebugOn", &LaserMirrorFactory::isDebugOn)
			.def("isMessagingOn", &LaserMirrorFactory::isMessagingOn);
	}



}

#endif //LASER_MIRROR_PYTHON_INTERFACE_H_