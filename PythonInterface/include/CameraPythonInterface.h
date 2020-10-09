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
			.def("pix2mmX", &Camera::pix2mmX        )
			.def("pix2mmY", &Camera::pix2mmY		)
			.def("mm2pixX", &Camera::mm2pixX		)
			.def("mm2pixY", &Camera::mm2pixY		)
			.def("getpix2mmX", &Camera::getpix2mmX		)
			.def("getpix2mmY", &Camera::getpix2mmY		)
			.def("setpix2mmX", &Camera::setpix2mmX		)
			.def("setpix2mmY", &Camera::setpix2mmY		)
			.def("getX", &Camera::getX			)
			.def("getY", &Camera::getY			)
			.def("getXmm", &Camera::getX)
			.def("getYmm", &Camera::getY)
			.def("getSigX", &Camera::getSigX		)
			.def("getSigY", &Camera::getSigY		)
			.def("getSigXY", &Camera::getSigXY		)
			.def("getSigXmm", &Camera::getSigX)
			.def("getSigYmm", &Camera::getSigY)
			.def("getSigXYmm", &Camera::getSigXY)
			.def("getXPix", &Camera::getXPix		)
			.def("getYPix", &Camera::getYPix		)
			.def("getSigXPix", &Camera::getSigXPix		)
			.def("getSigYPix", &Camera::getSigYPix		)
			.def("getSigXYPix", &Camera::getSigXYPix	)
			.def("setX", &Camera::setX			)
			.def("setY", &Camera::setY			)
			.def("setSigX", &Camera::setSigX		)
			.def("setSigY", &Camera::setSigY		)
			.def("setSigXY", &Camera::setSigXY		)
			.def("setXmm", &Camera::setX)
			.def("setYmm", &Camera::setY)
			.def("setSigXmm", &Camera::setSigX)
			.def("setSigYmm", &Camera::setSigY)
			.def("setSigXYmm", &Camera::setSigXY)
			.def("getSumIntensity", &Camera::getSumIntensity)
			.def("getAvgIntensity", &Camera::getAvgIntensity)
			.def("setSumIntensity", &Camera::setSumIntensity)
			.def("setAvgIntensity", &Camera::setAvgIntensity)

			.def("getMaskXCenter", &Camera::getMaskXCenter)
			.def("getMaskYCenter", &Camera::getMaskYCenter)
			.def("getMaskXRadius", &Camera::getMaskXRadius)
			.def("getMaskYRadius", &Camera::getMaskYRadius)


			.def("setMaskXCenter", &Camera::setMaskXCenter)
			.def("setMaskYCenter", &Camera::setMaskYCenter)
			.def("setMaskXRadius", &Camera::setMaskXRadius)
			.def("setMaskYRadius", &Camera::setMaskYRadius)



			.def("captureAndSave", &Camera::captureAndSave)


			.def("startAcquiring", &Camera::startAcquiring)
			.def("stopAcquiring", &Camera::stopAcquiring)
			
			.def("makeANewDirectoryAndName", &Camera::makeANewDirectoryAndName)
	
			.def("getAliases", &Camera::getAliases_Py)
			.def("getScreenNames", &Camera::getScreenNames_Py)



			.def("getBufferSize", &Camera::getBufferSize)
			.def("setBufferSize", &Camera::setBufferSize)
			.def("clearBuffers", &Camera::clearBuffers)
			.def("getRunningStats", &Camera::getRunningStats)


			.def("getScreen", &Camera::getScreen)
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
			.def("getCamera", &CameraFactory::getCamera, boost::python::return_value_policy<boost::python::reference_existing_object>() )

			.def("pix2mmX", &CameraFactory::pix2mmX)
			.def("pix2mmY", &CameraFactory::pix2mmY)
			.def("mm2pixX", &CameraFactory::mm2pixX)
			.def("mm2pixY", &CameraFactory::mm2pixY)
			.def("getpix2mmX", &CameraFactory::getpix2mmX)
			.def("getpix2mmY", &CameraFactory::getpix2mmY)
			.def("setpix2mmX", &CameraFactory::setpix2mmX)
			.def("setpix2mmY", &CameraFactory::setpix2mmY)
			.def("getX", &CameraFactory::getX)
			.def("getY", &CameraFactory::getY)
			.def("getXmm", &CameraFactory::getX)
			.def("getYmm", &CameraFactory::getY)
			.def("getSigX", &CameraFactory::getSigX)
			.def("getSigY", &CameraFactory::getSigY)
			.def("getSigXY", &CameraFactory::getSigXY)
			.def("getSigXmm", &CameraFactory::getSigX)
			.def("getSigYmm", &CameraFactory::getSigY)
			.def("getSigXYmm", &CameraFactory::getSigXY)
			.def("getXPix", &CameraFactory::getXPix)
			.def("getYPix", &CameraFactory::getYPix)
			.def("getSigXPix", &CameraFactory::getSigXPix)
			.def("getSigYPix", &CameraFactory::getSigYPix)
			.def("getSigXYPix", &CameraFactory::getSigXYPix)
			.def("setX", &CameraFactory::setX)
			.def("setY", &CameraFactory::setY)
			.def("setSigX", &CameraFactory::setSigX)
			.def("setSigY", &CameraFactory::setSigY)
			.def("setSigXY", &CameraFactory::setSigXY)
			.def("setXmm", &CameraFactory::setX)
			.def("setYmm", &CameraFactory::setY)
			.def("setSigXmm", &CameraFactory::setSigX)
			.def("setSigYmm", &CameraFactory::setSigY)
			.def("setSigXYmm", &CameraFactory::setSigXY)

			.def("getSumIntensity", &CameraFactory::getSumIntensity)
			.def("getAvgIntensity", &CameraFactory::getAvgIntensity)
			.def("setSumIntensity", &CameraFactory::setSumIntensity)
			.def("setAvgIntensity", &CameraFactory::setAvgIntensity)

			.def("stopAcquiring", &CameraFactory::stopAcquiring)
			.def("stopAllAcquiring", &CameraFactory::stopAllAcquiring)
			.def("stopAllAcquiringExceptVC", &CameraFactory::stopAllAcquiringExceptVC)
			.def("startAcquiring", &CameraFactory::startAcquiring)
			.def("getAllAcquireStates", &CameraFactory::getAllAcquireStates_Py)
			.def("isAcquiring", &CameraFactory::isAcquiring)
			.def("isNotAcquiring", &CameraFactory::isNotAcquiring)
			.def("getAcquireState", &CameraFactory::getAcquireState)



			.def("getRunningStats", &CameraFactory::getRunningStats)
			.def("getAllRunningStats", &CameraFactory::getAllRunningStats)



			.def("getMaskXCenter", &CameraFactory::getMaskXCenter)
			.def("getMaskYCenter", &CameraFactory::getMaskYCenter)
			.def("getMaskXRadius", &CameraFactory::getMaskXRadius)
			.def("getMaskYRadius", &CameraFactory::getMaskYRadius)

			.def("setMaskXCenter", &CameraFactory::setMaskXCenter)
			.def("setMaskYCenter", &CameraFactory::setMaskYCenter)
			.def("setMaskXRadius", &CameraFactory::setMaskXRadius)
			.def("setMaskYRadius", &CameraFactory::setMaskYRadius)

			.def("startAnalysing", &CameraFactory::startAnalysing)
			.def("stopAnalysing", &CameraFactory::stopAnalysing)
			.def("isAnalysing", &CameraFactory::isAnalysing)
			.def("isNotAnalysing", &CameraFactory::isNotAnalysing)

			.def("getAnalysisState", &CameraFactory::getAnalysisState)
			.def("getAllAnalysisState", &CameraFactory::getAllAnalysisState_Py)



			.def("debugMessagesOn", &CameraFactory::debugMessagesOn)
			.def("debugMessagesOff", &CameraFactory::debugMessagesOff)
			.def("messagesOn", &CameraFactory::messagesOn)
			.def("messagesOff", &CameraFactory::messagesOff)
			.def("isDebugOn", &CameraFactory::isDebugOn)
			.def("isMessagingOn", &CameraFactory::isMessagingOn);
	}



}

#endif //CAMERA_PYTHON_INTERFACE_H_