#ifndef SHUTTER_PYTHON_INTERFACE_H_
#define SHUTTER_PYTHON_INTERFACE_H_

#include <Shutter.h>
#include <ShutterFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_SHUTTER_INCLUDE
{

	void expose_shutter_object() 
	{
		//bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Magnet>())->to_python_target_type());
		//if (is_registered) return;
		//// magnet exposure
		//boost::python::class_<Magnet, boost::python::bases<Hardware>, boost::noncopyable>("Magnet", boost::python::no_init)

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Shutter>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Shutter, boost::python::bases<Hardware>, boost::noncopyable>("Shutter", boost::python::no_init)
			
			.def("isClosed", &Shutter::isClosed)
			.def("isOpen",   &Shutter::isOpen)
			.def("getState", &Shutter::getState)
			.def("close",    &Shutter::close)
			.def("open",     &Shutter::open)

			.def("getCMI",     &Shutter::getCMI)
			.def("getCMIBitMap",     &Shutter::getCMIBitMap_Py)
			.def("getAliases",     &Shutter::getAliases)
			
			.def("debugMessagesOn", &Shutter::debugMessagesOn)
			.def("debugMessagesOff", &Shutter::debugMessagesOff)
			.def("messagesOn", &Shutter::messagesOn)
			.def("messagesOff", &Shutter::messagesOff)
			.def("isDebugOn", &Shutter::isDebugOn)
			.def("isMessagingOn", &Shutter::isMessagingOn);
	}
	void expose_shutter_factory_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<ShutterFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<ShutterFactory, boost::noncopyable>("ShutterFactory", boost::python::no_init)

			.def("isClosed", &ShutterFactory::isClosed, , boost::python::arg("name"))
			.def("isOpen", &ShutterFactory::isOpen, boost::python::arg("name"))
			.def("getState", &ShutterFactory::getState, boost::python::arg("name"))
			.def("close", &ShutterFactory::close, boost::python::arg("name"))
			.def("open", &ShutterFactory::open, boost::python::arg("name"))
			.def("getCMI", &ShutterFactory::getCMI, boost::python::arg("name"))
			.def("getCMIBitMap", &ShutterFactory::getCMIBitMap_Py, boost::python::arg("name"))
			//.def("getAliases", &ShutterFactory::getAliases)



			.def("debugMessagesOff", &ShutterFactory::debugMessagesOff)
			.def("messagesOn", &ShutterFactory::messagesOn)
			.def("messagesOff", &ShutterFactory::messagesOff)
			.def("isDebugOn", &ShutterFactory::isDebugOn)
			.def("isMessagingOn", &ShutterFactory::isMessagingOn);
	}



}

#endif //SHUTTER_PYTHON_INTERFACE_H_