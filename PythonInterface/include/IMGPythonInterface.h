#ifndef IMG_PYTHON_INTERFACE_H_
#define IMG_PYTHON_INTERFACE_H_

#include <IMG.h>
#include <IMGFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_IMG_INCLUDE
{

	void expose_img_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<IMG>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<IMG, boost::python::bases<Hardware>, boost::noncopyable>("IMG", boost::python::no_init)
			.def("debugMessagesOn", &IMG::debugMessagesOn)
			.def("debugMessagesOff", &IMG::debugMessagesOff)
			.def("messagesOn", &IMG::messagesOn)
			.def("messagesOff", &IMG::messagesOff)
			.def("isDebugOn", &IMG::isDebugOn)
			.def("isMessagingOn", &IMG::isMessagingOn);
	}
	void expose_img_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<IMGFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<IMGFactory, boost::noncopyable>("IMGFactory", boost::python::no_init)
			.def("debugMessagesOn", &IMGFactory::debugMessagesOn)
			.def("debugMessagesOff", &IMGFactory::debugMessagesOff)
			.def("messagesOn", &IMGFactory::messagesOn)
			.def("messagesOff", &IMGFactory::messagesOff)
			.def("isDebugOn", &IMGFactory::isDebugOn)
			.def("isMessagingOn", &IMGFactory::isMessagingOn);
	}


}

#endif //IMG_PYTHON_INTERFACE_H_