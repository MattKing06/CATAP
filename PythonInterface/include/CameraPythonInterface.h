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
		
		// function pointers for overloads we want to expose
		bool(Camera:: * setMaskandROI_4PARAM)(long, long, long, long) = &Camera::setMaskandROI;
		bool(Camera:: * setROI_4PARAM)(long, long, long, long) = &Camera::setROI;
		bool(Camera:: * setMask_4PARAM)(long, long, long, long) = &Camera::setMask;

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
			.def("getX", &Camera::getX			) // backwards compatibale, prefer to not use
			.def("getY", &Camera::getY			) // backwards compatibale, prefer to not use
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


			.def("getStepSize", &Camera::getStepSize)
			.def("setStepSize", &Camera::setStepSize)

			
			.def("getMaskXCenter", &Camera::getMaskXCenter)
			.def("getMaskYCenter", &Camera::getMaskYCenter)
			.def("getMaskXRadius", &Camera::getMaskXRadius)
			.def("getMaskYRadius", &Camera::getMaskYRadius)
			.def("getMask", &Camera::getMask_Py)
			.def("getROIMinX", &Camera::getROIMinX)
			.def("getROIMinY", &Camera::getROIMinY)
			.def("getROISizeX", &Camera::getROISizeX)
			.def("getROISizeY", &Camera::getROISizeY)
			.def("getROI", &Camera::getROI_Py)
			.def("getMaskAndROIxPos", &Camera::getMaskAndROIxPos)
			.def("getMaskAndROIyPos", &Camera::getMaskAndROIyPos)
			.def("getMaskAndROIxSize", &Camera::getMaskAndROIxSize)
			.def("getMaskAndROIySize", &Camera::getMaskAndROIySize)
			.def("getMaskandROI", &Camera::getMaskandROI_Py)
			
			.def("getImageData", &Camera::getImageData_Py)
			.def("getImageDataRef", &Camera::getImageDataRef_Py, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getROIData_Py", &Camera::getROIData_Py)
			.def("updateImageData", &Camera::updateImageData)
			.def("updateROIData", &Camera::updateROIData)


			// use these for setting mask AND ROI 
			.def("setMaskAndROIxPos", &Camera::setMaskAndROIxPos)
			.def("setMaskAndROIyPos", &Camera::setMaskAndROIyPos)
			.def("setMaskAndROIxSize", &Camera::setMaskAndROIxSize)
			.def("setMaskAndROIySize", &Camera::setMaskAndROIySize)
			.def("setMaskandROI_4PARAM", setMaskandROI_4PARAM)
			.def("setMaskandROI", &Camera::setMaskandROI_Py)
			// we don:t want people to use these!! 
			.def("setMaskXCenter", &Camera::setMaskXCenter)
			.def("setMaskYCenter", &Camera::setMaskYCenter)
			.def("setMaskXRadius", &Camera::setMaskXRadius)
			.def("setMaskYRadius", &Camera::setMaskYRadius)
			.def("setMask", setMask_4PARAM)
			.def("setMask", &Camera::setMask_Py)
			.def("setROIMinX", &Camera::setROIMinX)
			.def("setROIMinY", &Camera::setROIMinY)
			.def("setROISizeX", &Camera::setROISizeX)
			.def("setROIsizeY", &Camera::setROISizeY)
			.def("setROI", setROI_4PARAM)
			.def("setROI", &Camera::setROI_Py)

			
			//.def_readonly("mask_and_roi_keywords", &Camera::mask_and_roi_keywords_Py)
			//.def_readonly("mask_keywords", &Camera::mask_keywords_Py)
			//.def_readonly("roi_keywords", &Camera::roi_keywords_Py)
					
			
			.def("getBufferTrigger", &Camera::getBufferTrigger)
			.def("getBufferFilePath", &Camera::getBufferFilePath)
			.def("getBufferFileName", &Camera::getBufferFileName)
			.def("getBufferFileNumber", &Camera::getBufferFileNumber)
			//.def("getBufferROIminX", &Camera::getBufferROIminX)
			//.def("getBufferROIminY", &Camera::getBufferROIminY)
			//.def("getBufferROIsizeX", &Camera::getBufferROIsizeX)
			//.def("getBufferROIsizeY", &Camera::getBufferROIsizeY)

			


			.def("setUseFloor", &Camera::setUseFloor)
			.def("setDoNotUseFloor", &Camera::setDoNotUseFloor)
			.def("setFloorLevel", &Camera::setFloorLevel)
			.def("getUseFloorState", &Camera::getUseFloorState)
			.def("isUsingFloor", &Camera::isUsingFloor)
			.def("isNotUsingFloor", &Camera::isNotUsingFloor)
			.def("getFloorLevel", &Camera::getFloorLevel)
			.def("getFlooredPtsCount", &Camera::getFlooredPtsCount)
			.def("getFlooredPtsPercent", &Camera::getFlooredPtsPercent)

				
			.def("getCPUCropSubMask", &Camera::getCPUCropSubMask)
			.def("getCPUNPoint", &Camera::getCPUNPoint)
			.def("getCPUDot", &Camera::getCPUDot)

			.def("getPixelWidth", &Camera::getPixelWidth)
			.def("getPixelHeight", &Camera::getPixelHeight)
			.def("setBufferTrigger", &Camera::setBufferTrigger)
			//.def("setBufferROIminX", &Camera::setBufferROIminX)
			//.def("setBufferROIminY", &Camera::setBufferROIminY)
			//.def("setBufferROIsizeX", &Camera::setBufferROIsizeX)
			//.def("setBufferROIsizeY", &Camera::setBufferROIsizeY)



			.def("captureAndSave", &Camera::captureAndSave)
			.def("isCapturingOrSaving", &Camera::isCapturingOrSaving)
			.def("isCapturing", &Camera::isCapturing)
			.def("isNotCapturing", &Camera::isNotCapturing)
			.def("isWriting", &Camera::isWriting)
			.def("isNotWriting", &Camera::isNotWriting)
			.def("isSaving", &Camera::isWriting)
			.def("isNotSaving", &Camera::isNotWriting)

			.def("collectAndSave", &Camera::captureAndSave)
			.def("isCollectingOrSaving", &Camera::isCapturingOrSaving)
			.def("isCollecting", &Camera::isCapturing)
			.def("isNotCollecting", &Camera::isNotCapturing)


				
				
			.def("useBackground", &Camera::useBackground)
			.def("isUsingBackground", &Camera::isUsingBackground)
			.def("isNotUsingBackground", &Camera::isNotUsingBackground)
			.def("getUsingBackgroundState", &Camera::getUsingBackgroundState)

			.def("getNPointState", &Camera::getNPointState)
			.def("useNPoint", &Camera::useNPoint)
			.def("isUsingNPoint", &Camera::isUsingNPoint)
			.def("isNotUsingNPoint", &Camera::isNotUsingNPoint)
				
			.def("startAcquiring", &Camera::startAcquiring)
			.def("stopAcquiring", &Camera::stopAcquiring)
			.def("isAcquiring", &Camera::isAcquiring)
			.def("isNotAcquiring", &Camera::isNotAcquiring)
			.def("getAcquireState", &Camera::getAcquireState)

			.def("startAnalysing", &Camera::startAnalysing)
			.def("stopAnalysing", &Camera::stopAnalysing)
			.def("isAnalysing", &Camera::isAnalysing)
			.def("isNotAnalysing", &Camera::isNotAnalysing)
			.def("getAnalysisState", &Camera::getAnalysisState)


			.def("getAcquireTime", &Camera::getAcquireTime)
			.def("getAcquirePeriod", &Camera::getAcquirePeriod)
			.def("getArrayRate", &Camera::getArrayRate)
			.def("getTemperature", &Camera::getTemperature)
			
			.def("getLastBufferDirectoryandFileName", &Camera::getLastBufferDirectoryandFileName)
			.def("getLastestBufferDirectoryandFileName", &Camera::getLastBufferDirectoryandFileName)
			.def("getLastBufferDirectory", &Camera::getLastBufferDirectory)
			.def("getLastestBufferDirectory", &Camera::getLastBufferDirectory)
			.def("getLastBufferFileName", &Camera::getLastBufferFileName)
			.def("getLastestBufferFileName", &Camera::getLastBufferFileName)

			.def("getLastDirectoryandFileName", &Camera::getLastDirectoryandFileName)
			.def("getLastestDirectoryandFileName", &Camera::getLastDirectoryandFileName)
			.def("getLastDirectory", &Camera::getLastDirectory)
			.def("getLatestDirectory", &Camera::getLastDirectory)
			.def("getLastestFileName", &Camera::getLastFileName)
			.def("getLastFileName", &Camera::getLastFileName)

			.def("setLEDOn", &Camera::setLEDOn)
			.def("setLEDOff", &Camera::setLEDOff)
			.def("isLEDOn", &Camera::isLEDOn)
			.def("isLEDOff", &Camera::isLEDOff)
			.def("getLEDState", &Camera::getLEDState)
			
	
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
			.def("isMessagingOn", &Camera::isMessagingOn)
			;
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

			.def("getStepSize", &CameraFactory::getStepSize)
			.def("setStepSize", &CameraFactory::setStepSize)


			.def("useBackground", &CameraFactory::useBackground)
			.def("isUsingBackground", &CameraFactory::isUsingBackground)
			.def("getUsingBackgroundState", &CameraFactory::getUsingBackgroundState)

			.def("useNPoint", &CameraFactory::useNPoint)
			.def("isUsingNPoint", &CameraFactory::isUsingNPoint)
			.def("getNPointState", &CameraFactory::getNPointState)
		

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