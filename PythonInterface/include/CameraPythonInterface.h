#ifndef CAMERA_PYTHON_INTERFACE_H_
#define CAMERA_PYTHON_INTERFACE_H_

#include <Camera.h>
#include <CameraFactory.h>
#include <PythonTypeConversions.h>
#include <RunningStats.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace BOOST_PYTHON_CAMERA_INCLUDE
{
	void expose_std_vector_long_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::vector<long>>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<std::vector<long>, boost::noncopyable>("std_vector_long", boost::python::no_init)
		.def(boost::python::vector_indexing_suite<std::vector<long>>())
		;
	}
	void expose_camera_object() 
	{
		// function pointers for overloads we want to expose
		bool(Camera::*setMaskandROI_4PARAM)(long, long, long, long) = &Camera::setMaskandROI;
		bool(Camera::*setROI_4PARAM)(long, long, long, long) = &Camera::setROI;
		bool(Camera::*setMask_4PARAM)(long, long, long, long) = &Camera::setMask;

		//boost::python::dict(Camera::*getRunningStats_str)(const std::string&)const = &Camera::getRunningStats;
		//boost::python::dict(Camera::*getRunningStats_type)(TYPE)const = &Camera::getRunningStats;


		bool(Camera::*captureAndSave_with_shotcount)(size_t) = &Camera::captureAndSave;
		bool(Camera::*captureAndSave_without_shotcount)() = &Camera::captureAndSave;


		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Camera>())->to_python_target_type());
		if (is_registered) return;
		// we need to define this here
		boost::python::class_<std::vector<long>, boost::noncopyable>("std_vector_long", boost::python::no_init)
			.def(boost::python::vector_indexing_suite<std::vector<long>>())
			;
		boost::python::class_<Camera, boost::python::bases<Hardware>, boost::noncopyable>("Camera", boost::python::no_init)

		//                   ___  __  
		//  |\ |  /\   |\/| |__  /__` 
		//  | \| /~~\  |  | |___ .__/ 
		//
		.def("getAliases", &Camera::getAliases_Py)
		.def("getScreenNames", &Camera::getScreenNames_Py)
		//   __         ___         ___  __                 
		//  |__) | \_/ |__  |        |  /  \     |\/|  |\/| 
		//  |    | / \ |___ |___     |  \__/     |  |  |  | 
		//  
		.def("getPixelToMM", &Camera::getPixelToMM)
		.def("setPixelToMM", &Camera::setPixelToMM)
		.def("getPix2mm", &Camera::getPixelToMM) // bkwards comp.
		.def("pix2mmX", &Camera::pix2mmX)
		.def("pix2mmY", &Camera::pix2mmY)
		.def("mm2pixX", &Camera::mm2pixX)
		.def("mm2pixY", &Camera::mm2pixY)
		.def("getpix2mmX", &Camera::getpix2mmX)
		.def("getpix2mmY", &Camera::getpix2mmY)
		.def("setpix2mmX", &Camera::setpix2mmX)
		.def("setpix2mmY", &Camera::setpix2mmY)
		//    __   ___      ___  ___  __      __   __      __                      __      __   __      __   __   __  
		//   /  ` |__  |\ |  |  |__  |__)    /  \ |__)    /  \ |\ | __  /\  \_/ | /__`    /  ` /  \ __ /  \ |__) |  \	
		//   \__, |___ | \|  |  |___ |  \    \__/ |  \    \__/ | \|    /~~\ / \ | .__/    \__, \__/    \__/ |  \ |__/ 
		// 
		.def("getCentreXPixel", &Camera::getCentreXPixel)
		.def("getCentreYPixel", &Camera::getCentreYPixel)
		.def("setCentreXPixel", &Camera::setCentreXPixel)
		.def("setCentreYPixel", &Camera::setCentreYPixel)
		.def("setCentrePixels", &Camera::setCentrePixels)
		.def("setMechanicalCentre", &Camera::setMechanicalCentre)
		.def("setOperatingCenter", &Camera::setOperatingCenter)
		.def("getOperatingCentreXPixel", &Camera::getOperatingCentreXPixel)
		.def("getOperatingCentreYPixel", &Camera::getOperatingCentreYPixel)
		.def("getMechanicalCentreXPixel", &Camera::getMechanicalCentreXPixel)
		.def("getMechanicalCentreYPixel", &Camera::getMechanicalCentreYPixel)
		//  			  __   ___ __    __  ___
		//  |  |\/|  /\  / _` |__ /__` |  / |__
		//  |  |  | /~~\ \__> |___.__/ | /_ |___
		// 
		.def("getPixelWidth", &Camera::getPixelWidth)
		.def("getPixelHeight", &Camera::getPixelHeight)
		.def("getArrayDataPixelCountX", &Camera::getArrayDataPixelCountX)
		.def("getArrayDataPixelCountY", &Camera::getArrayDataPixelCountY)
		.def("getBinaryDataPixelCountX", &Camera::getBinaryDataPixelCountX)
		.def("getBinaryDataPixelCountY", &Camera::getBinaryDataPixelCountY)
		//	 __   __                __        __  
		//	/  ` |__) |  |    |    /  \  /\  |  \ 
		//	\__, |    \__/    |___ \__/ /~~\ |__/ 
		//	  
		.def("getCPUTotal", &Camera::getCPUTotal)
		.def("getCPUCropSubMask", &Camera::getCPUCropSubMask)
		.def("getCPUNPoint", &Camera::getCPUNPoint)
		.def("getCPUDot", &Camera::getCPUDot)
		//	              __   ___     __        ___  __                 __  
		//	|  |\/|  /\  / _` |__     /  \ \  / |__  |__) |     /\  \ / /__` 
		//	|  |  | /~~\ \__> |___    \__/  \/  |___ |  \ |___ /~~\  |  .__/ 
		//	
		.def("enableOverlayCross", &Camera::enableOverlayCross)
		.def("disableOverlayCross", &Camera::disableOverlayCross)
		.def("getOverlayCrossState", &Camera::getOverlayCrossState)
		.def("isOverlayCrossEnabled", &Camera::isOverlayCrossEnabled)
		.def("isOverlayCrossDisabled", &Camera::isOverlayCrossDisabled)
		.def("enableOverlayMask", &Camera::enableOverlayMask)
		.def("disableOverlayMask", &Camera::disableOverlayMask)
		.def("getOverlayMaskState", &Camera::getOverlayMaskState)
		.def("isOverlayMaskEnabled", &Camera::isOverlayMaskEnabled)
		.def("isOverlayMaskDisabled", &Camera::isOverlayMaskDisabled)
		.def("enableOverlayResult", &Camera::enableOverlayResult)
		.def("disableOverlayResult", &Camera::disableOverlayResult)
		.def("getOverlayResultState", &Camera::getOverlayResultState)
		.def("isOverlayResultEnabled", &Camera::isOverlayResultEnabled)
		.def("isOverlayResultDisabled", &Camera::isOverlayResultDisabled)
		.def("disableAllOverlay", &Camera::disableAllOverlay)
		//	              __   ___     __        ___  ___  ___  __  
		//	|  |\/|  /\  / _` |__     |__) |  | |__  |__  |__  |__) 
		//	|  |  | /~~\ \__> |___    |__) \__/ |    |    |___ |  \ 
		//	
		.def("getImageBufferTrigger", &Camera::getImageBufferTrigger)
		.def("getImageBufferFilePath", &Camera::getImageBufferFilePath)
		.def("getImageBufferFileName", &Camera::getImageBufferFileName)
		.def("getImageBufferFileNumber", &Camera::getImageBufferFileNumber)
		//.def("triggerImageBuffer", &Camera::triggerImageBuffer)
		.def("getLastImageBufferDirectoryandFileName", &Camera::getLastImageBufferDirectoryandFileName)
		//.def("getLastestBufferDirectoryandFileName", &Camera::getLastImageBufferDirectoryandFileName)
		.def("getLastImageBufferDirectory", &Camera::getLastImageBufferDirectory)
		//.def("getLastestBufferDirectory", &Camera::getLastImageBufferDirectory)
		.def("getLastImageBufferFileName", &Camera::getLastImageBufferFileName)
		//.def("getLastestBufferFileName", &Camera::getLastImageBufferFileName)
		//              __   ___     __        __  ___       __   ___               __      __             _ 
		//|  |\/|  /\  / _` |__     /  `  /\  |__)  |  |  | |__) |__      /\  |\ | |  \    /__`  /\  \  / |__  
		//|  |  | /~~\ \__> |___    \__, /~~\ |     |  \__/ |  \ |___    /~~\ | \| |__/    .__/ /~~\  \/  |___ 
		//
		.def("getLastDirectoryandFileName", &Camera::getLastDirectoryandFileName)
		.def("getLastestDirectoryandFileName", &Camera::getLastDirectoryandFileName)
		.def("getLastDirectory", &Camera::getLastDirectory)
		.def("getLatestDirectory", &Camera::getLastDirectory)
		.def("getLastestFileName", &Camera::getLastFileName)
		.def("getLastFileName", &Camera::getLastFileName)
		.def("getCaptureState", &Camera::getCaptureState)
		.def("isCapturing", &Camera::isCapturing)
		.def("isNotCapturing", &Camera::isNotCapturing)
		.def("isWriting", &Camera::isWriting)
		.def("isNotWriting", &Camera::isNotWriting)
		.def("isCapturingOrSaving", &Camera::isCapturingOrSaving)
		.def("isNotCapturingOrSaving", &Camera::isNotCapturingOrSaving)
		.def("isSaving", &Camera::isWriting)
		.def("isNotSaving", &Camera::isNotWriting)
		// image captur e/ save 
		.def("captureAndSave", captureAndSave_with_shotcount)
		.def("collectAndSave", captureAndSave_without_shotcount)
		.def("isCollectingOrSaving", &Camera::isCapturingOrSaving) // VCC backwards compatability 
		.def("isCollecting", &Camera::isCapturing)
		.def("isNotCollecting", &Camera::isNotCapturing)
		.def("didLastCaptureAndSaveSucceed", &Camera::didLastCaptureAndSaveSucceed)
		//	 __  ___       __  ___   /  __  ___  __   __           __   __          __          __  
		//	/__`  |   /\  |__)  |   /  /__`  |  /  \ |__)     /\  /  ` /  \ |  | | |__) | |\ | / _` 
		//	.__/  |  /~~\ |  \  |  /   .__/  |  \__/ |       /~~\ \__, \__X \__/ | |  \ | | \| \__> 
		//
		.def("startAcquiring", &Camera::startAcquiring)
		.def("stopAcquiring", &Camera::stopAcquiring)
		.def("toggleAcquiring", &Camera::toggleAcquiring)
		.def("isAcquiring", &Camera::isAcquiring)
		.def("isNotAcquiring", &Camera::isNotAcquiring)
		.def("getAcquireState", &Camera::getAcquireState)
		.def("getAcquireTime", &Camera::getAcquireTime)
		.def("getAcquirePeriod", &Camera::getAcquirePeriod)
		.def("getArrayRate", &Camera::getArrayRate)
		//	              __   ___                             __     __  
		//	|  |\/|  /\  / _` |__      /\  |\ |  /\  |    \ / /__` | /__` 
		//	|  |  | /~~\ \__> |___    /~~\ | \| /~~\ |___  |  .__/ | .__/ 
		//
		.def("startAnalysing", &Camera::startAnalysing)
		.def("stopAnalysing", &Camera::stopAnalysing)
		.def("toggleAnalysing", &Camera::toggleAnalysing)
		.def("isAnalysing", &Camera::isAnalysing)
		.def("isNotAnalysing", &Camera::isNotAnalysing)
		.def("getAnalysisState", &Camera::getAnalysisState)
		//	                         __     __           __   __         ___     __   __                    __  
		//	 /\  |\ | |     /\  \ / /__` | /__`    |\ | |__) /  \ | |\ |  |     /__` /  `  /\  |    | |\ | / _` 
		//	/~~\ | \| |___ /~~\  |  .__/ | .__/    | \| |    \__/ | | \|  |     .__/ \__, /~~\ |___ | | \| \__> 
		//	                                                                                                    
		.def("setUseNPointScaling", &Camera::setUseNPointScaling)
		.def("setDoNotUseNPointScaling", &Camera::setDoNotUseNPointScaling)
		.def("toggleUseNPointScaling", &Camera::toggleUseNPointScaling)
		.def("getNPointScalingState", &Camera::getNPointScalingState)
		.def("isUsingNPointScaling", &Camera::isUsingNPointScaling)
		.def("isNotUsingNPointScaling", &Camera::isNotUsingNPointScaling)
		.def("getNpointScalingStepSize", &Camera::getNpointScalingStepSize)
		.def("setNpointScalingStepSize", &Camera::setNpointScalingStepSize)
		//	                         __     __           __     __   ___     ___       __   __   __  
		//	 /\  |\ |  /\  |    \ / /__` | /__`    |\ | /  \ | /__` |__     |__  |    /  \ /  \ |__) 
		//	/~~\ | \| /~~\ |___  |  .__/ | .__/    | \| \__/ | .__/ |___    |    |___ \__/ \__/ |  \ 
		//	 
		.def("setUseFloor", &Camera::setUseFloor)
		.def("setDoNotUseFloor", &Camera::setDoNotUseFloor)
		.def("toggleUseFloor", &Camera::toggleUseFloor)
		.def("setFloorLevel", &Camera::setFloorLevel)
		.def("getUseFloorState", &Camera::getUseFloorState)
		.def("isUsingFloor", &Camera::isUsingFloor)
		.def("isNotUsingFloor", &Camera::isNotUsingFloor)
		.def("getFloorLevel", &Camera::getFloorLevel)
		.def("getFlooredPtsCount", &Camera::getFlooredPtsCount)
		.def("getFlooredPtsPercent", &Camera::getFlooredPtsPercent)
		//                         __     __      __        __        __   __   __             __     
		// /\  |\ | |     /\  \ / /__` | /__`    |__)  /\  /  ` |__/ / _` |__) /  \ |  | |\ | |  \    
		///~~\ | \| |___ /~~\  |  .__/ | .__/    |__) /~~\ \__, |  \ \__> |  \ \__/ \__/ | \| |__/    
		//  
		.def("setNewBackgroundImage", &Camera::setNewBackgroundImage)
		.def("setUseBackgroundImage", &Camera::setUseBackgroundImage)
		.def("setDoNotUseBackgroundImage", &Camera::setDoNotUseBackgroundImage)
		.def("toggleUseBackgroundImage", &Camera::toggleUseBackgroundImage)
		.def("getUsingBackgroundImageState", &Camera::getUsingBackgroundImageState)
		.def("isUsingBackgroundImage", &Camera::isUsingBackgroundImage)
		.def("isNotUsingBackgroundImage", &Camera::isNotUsingBackgroundImage)

		.def("getCamType", &Camera::getCamType)


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
		.def("getXPixScaleFactor", &Camera::getXPixScaleFactor)
		.def("getYPix", &Camera::getYPix		)
		.def("getYPixScaleFactor", &Camera::getYPixScaleFactor)
		.def("getSigXPix", &Camera::getSigXPix		)
		.def("getSigYPix", &Camera::getSigYPix		)
		.def("getSigXYPix", &Camera::getSigXYPix	)
		//.def("getNumXPixFromArrayData", &Camera::getNumXPixFromArrayData)
		//.def("getNumYPixFromArrayData", &Camera::getNumYPixFromArrayData)
		//.def("getFullNumXPix", &Camera::getFullNumXPix)
		//.def("getFullNumYPix", &Camera::getFullNumYPix)
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
		.def("isBusy", &Camera::isBusy)
		.def("isNotBusy", &Camera::isNotBusy)
		.def("getANAPixelResults", &Camera::getPixelResults_Py)
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
		.def("getMaskAndROIxMax", &Camera::getMaskAndROIxMax)
		.def("getMaskAndROIyMax", &Camera::getMaskAndROIyMax)
		.def("getMaskAndROIxSize", &Camera::getMaskAndROIxSize)
		.def("getMaskAndROIySize", &Camera::getMaskAndROIySize)
		.def("getMaskandROI", &Camera::getMaskandROI_Py)
		.def("updateImageData", &Camera::updateImageData)
		.def("updateImageDataWithTimeStamp", &Camera::updateImageDataWithTimeStamp)
		.def("updateROIData", &Camera::updateROIData)
		.def("updateROIDataWithTimeStamp", &Camera::updateROIDataWithTimeStamp)
		.def("getImageDataConstRef", &Camera::getImageDataConstRef, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getROIDataConstRef", &Camera::getROIDataConstRef, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getImageData", &Camera::getImageData_Py)
		.def("getROIData", &Camera::getROIData_Py)
		.def("setMaskAndROIxMax", &Camera::setMaskAndROIxMax) 			// use these for setting mask AND ROI 
		.def("setMaskAndROIyMax", &Camera::setMaskAndROIyMax)
		.def("setMaskAndROIxSize", &Camera::setMaskAndROIxSize)
		.def("setMaskAndROIySize", &Camera::setMaskAndROIySize)
		.def("setMaskandROI_4PARAM", setMaskandROI_4PARAM)
		.def("setMaskandROI", &Camera::setMaskandROI_Py)
		.def("setMaskXCenter", &Camera::setMaskXCenter) 			// we don:t want people to use these!! 
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

			
		.def("getSumIntensity", &Camera::getSumIntensity)
		.def("getAvgIntensity", &Camera::getAvgIntensity)
		.def("setSumIntensity", &Camera::setSumIntensity)
		.def("setAvgIntensity", &Camera::setAvgIntensity)
		.def("setAvgPixValueForBeam", &Camera::setAveragePixelValueForBeam)
		.def("getAvgPixValueForBeam", &Camera::getAveragePixelValueForBeam)
		.def("hasBeam", &Camera::hasBeam)
		.def("hasNoBeam", &Camera::hasNoBeam)
		.def("getAnalysisData", &Camera::getAnalayisData_Py)

		
		.def("getTemperature", &Camera::getTemperature)
		.def("hasLED", &Camera::hasLED)
		.def("setLEDOn", &Camera::setLEDOn)
		.def("setLEDOff", &Camera::setLEDOff)
		.def("isLEDOn", &Camera::isLEDOn)
		.def("isLEDOff", &Camera::isLEDOff)
		.def("getLEDState", &Camera::getLEDState)
		.def("getMMResults", &Camera::getMMResults_Py)

		.def("getMMResults", &Camera::getMMResults_Py)
				
		.def("getActiveCameraLimit", &Camera::getActiveCameraLimit)
		.def("getActiveCameraCount", &Camera::getActiveCameraCount)
		.def("canStartCamera", &Camera::canStartCamera)
				
		.def("areAllRunningStatsFull", &Camera::areAllRunningStatsFull)
		.def("saveImageBuffer", &Camera::saveImageBuffer)
		.def("resetCaptureAndSaveError", &Camera::resetCaptureAndSaveError)
		.def("stopAcquiringAndWait", &Camera::stopAcquiringAndWait)
			
		// TODO the runnings atsts and buffersd a re being updated 

		.def("getBufferSize", &Camera::getBufferSize)
		.def("setBufferSize", &Camera::setAllRunningStatBufferSizes)
		.def("setAllRunningStatBufferSizes", &Camera::setAllRunningStatBufferSizes)
		.def("clearBuffers", &Camera::clearAllRunningStatBuffers)
		.def("clearAllRunningStatBuffers", &Camera::clearAllRunningStatBuffers)
		.def("getRunningStats", &Camera::getAllRunningStats)

		.def("clearAllRunningStats", &Camera::clearAllRunningStats)
		.def("setAllRunningStatSizes", &Camera::setAllRunningStatSizes)

		.def("getXPixRunningStats", &Camera::getXPixRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getYPixRunningStats", &Camera::getYPixRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getSigmaXPixRunningStats", &Camera::getSigmaXPixRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getSigmaYPixRunningStats", &Camera::getSigmaYPixRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getSigmaXYPixRunningStats", &Camera::getSigmaXYPixRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getXmmRunningStats", &Camera::getXmmRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getYmmRunningStats", &Camera::getYmmRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getSigmaXmmRunningStats", &Camera::getSigmaXmmRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getSigmaYmmRunningStats", &Camera::getSigmaYmmRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getSigmaXYmmRunningStats", &Camera::getSigmaXYmmRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getAvgIntensityRunningStats", &Camera::getAvgIntensityRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getSumIntensityRunningStats", &Camera::getSumIntensityRunningStats, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getScreen", &Camera::getScreen)
			
		.def("setGain", &Camera::setGain)
		.def("getGain", &Camera::getGain)
		.def("setBlackLevel", &Camera::setBlackLevel)
		.def("getBlackLevel", &Camera::getBlackLevel)
			
				
		.def("getAnalysisResultsPixels", &Camera::getAnalysisResultsPixels_Py)



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

		bool(CameraFactory::*setMaskandROI_4PARAM)(const std::string&, long, long, long, long) = &CameraFactory::setMaskandROI;
		bool(CameraFactory::*setROI_4PARAM)(const std::string&, long, long, long, long) = &CameraFactory::setROI;
		bool(CameraFactory::*setMask_4PARAM)(const std::string&, long, long, long, long) = &CameraFactory::setMask;

		bool(CameraFactory::*captureAndSave_with_shotcount)(const std::string&, size_t) = &CameraFactory::captureAndSave;
		bool(CameraFactory::*captureAndSave_without_shotcount)(const std::string & ) = &CameraFactory::captureAndSave;


		STATE(CameraFactory:: * saveSnapshot_nofile)(const std::string&) = &CameraFactory::saveSnapshot;
		STATE(CameraFactory:: * saveSnapshot_withfile)(const std::string&, const std::string&, const std::string&) = &CameraFactory::saveSnapshot;
		STATE(CameraFactory:: * applySnaphot_withDict)(const boost::python::dict&, TYPE) = &CameraFactory::applySnaphot;
		STATE(CameraFactory:: * applySnaphot_withfile)(const std::string&, const std::string&, TYPE) = &CameraFactory::applySnaphot;

		boost::python::class_<std::vector<long>, boost::noncopyable>("std_vector_long", boost::python::no_init)
			.def(boost::python::vector_indexing_suite<std::vector<long>>())
			;

		boost::python::class_<CameraFactory, boost::noncopyable>("CameraFactory", boost::python::no_init)

			//                   ___  __  
			//  |\ |  /\   |\/| |__  /__` 
			//  | \| /~~\  |  | |___ .__/ 
			// 
			.def("getCameraNames", &CameraFactory::getCameraNames_Py)
			.def("getAliases", &CameraFactory::getAliases_Py)
			.def("getScreenNames", &CameraFactory::getScreenNames_Py)
			//   __         ___         ___  __                 
			//  |__) | \_/ |__  |        |  /  \     |\/|  |\/| 
			//  |    | / \ |___ |___     |  \__/     |  |  |  | 
			//  
			.def("getPix2mm", &CameraFactory::getPixelToMM)
			.def("pix2mmX", &CameraFactory::pix2mmX)
			.def("pix2mmY", &CameraFactory::pix2mmY)
			.def("mm2pixX", &CameraFactory::mm2pixX)
			.def("mm2pixY", &CameraFactory::mm2pixY)
			.def("getpix2mmX", &CameraFactory::getpix2mmX)
			.def("getpix2mmY", &CameraFactory::getpix2mmY)
			.def("setpix2mmX", &CameraFactory::setpix2mmX)
			.def("setpix2mmY", &CameraFactory::setpix2mmY)
			//    __   ___      ___  ___  __      __   __      __                      __      __   __      __   __   __  
			//   /  ` |__  |\ |  |  |__  |__)    /  \ |__)    /  \ |\ | __  /\  \_/ | /__`    /  ` /  \ __ /  \ |__) |  \	
			//   \__, |___ | \|  |  |___ |  \    \__/ |  \    \__/ | \|    /~~\ / \ | .__/    \__, \__/    \__/ |  \ |__/ 
			// 
			.def("getCentreXPixel", &CameraFactory::getCentreXPixel)
			.def("getCentreYPixel", &CameraFactory::getCentreYPixel)
			.def("setCentreXPixel", &CameraFactory::setCentreXPixel)
			.def("setCentreYPixel", &CameraFactory::setCentreYPixel)
			.def("setCentrePixels", &CameraFactory::setCentrePixels)
			.def("setMechanicalCentre", &CameraFactory::setMechanicalCentre)
			.def("setOperatingCenter",  &CameraFactory::setOperatingCenter)
			.def("getOperatingCentreXPixel", &CameraFactory::getOperatingCentreXPixel)
			.def("getOperatingCentreYPixel", &CameraFactory::getOperatingCentreYPixel)
			.def("getMechanicalCentreXPixel", &CameraFactory::getMechanicalCentreXPixel)
			.def("getMechanicalCentreYPixel", &CameraFactory::getMechanicalCentreYPixel)
			//  			  __   ___ __    __  ___
			//  |  |\/|  /\  / _` |__ /__` |  / |__
			//  |  |  | /~~\ \__> |___.__/ | /_ |___
			// 
			.def("getPixelWidth", &CameraFactory::getPixelWidth)
			.def("getPixelHeight", &CameraFactory::getPixelHeight)
			.def("getArrayDataPixelCountX", &CameraFactory::getArrayDataPixelCountX)
			.def("getArrayDataPixelCountY", &CameraFactory::getArrayDataPixelCountY)
			.def("getBinaryDataPixelCountX", &CameraFactory::getBinaryDataPixelCountX)
			.def("getBinaryDataPixelCountY", &CameraFactory::getBinaryDataPixelCountY)
			//	 __   __                __        __  
			//	/  ` |__) |  |    |    /  \  /\  |  \ 
			//	\__, |    \__/    |___ \__/ /~~\ |__/ 
			//	  
			.def("getCPUTotal", &CameraFactory::getCPUTotal)
			.def("getCPUCropSubMask", &CameraFactory::getCPUCropSubMask)
			.def("getCPUNPoint", &CameraFactory::getCPUNPoint)
			.def("getCPUDot", &CameraFactory::getCPUDot)
		//	              __   ___     __        ___  ___  ___  __  
		//	|  |\/|  /\  / _` |__     |__) |  | |__  |__  |__  |__) 
		//	|  |  | /~~\ \__> |___    |__) \__/ |    |    |___ |  \ 
		//	
		//.def("triggerImageBuffer", &CameraFactory::triggerImageBuffer)
		.def("getImageBufferTrigger", &CameraFactory::getImageBufferTrigger)
		.def("getImageBufferFilePath", &CameraFactory::getImageBufferFilePath)
		.def("getImageBufferFileName", &CameraFactory::getImageBufferFileName)
		.def("getImageBufferFileNumber", &CameraFactory::getImageBufferFileNumber)
		.def("getLastImageBufferDirectoryandFileName", &CameraFactory::getLastImageBufferDirectoryandFileName)
		//.def("getLastestBufferDirectoryandFileName", &CameraFactory::getLastImageBufferDirectoryandFileName)

		.def("getLastImageBufferDirectory", &CameraFactory::getLastImageBufferDirectory)
		//.def("getLastestBufferDirectory", &CameraFactory::getLastImageBufferDirectory)
		.def("getLastImageBufferFileName", &CameraFactory::getLastImageBufferFileName)
		//.def("getLastestBufferFileName", &CameraFactory::getLastImageBufferFileName)
		//	              __   ___     __        __  ___       __   ___               __      __             _ 
		//	|  |\/|  /\  / _` |__     /  `  /\  |__)  |  |  | |__) |__      /\  |\ | |  \    /__`  /\  \  / |__  
		//	|  |  | /~~\ \__> |___    \__, /~~\ |     |  \__/ |  \ |___    /~~\ | \| |__/    .__/ /~~\  \/  |___ 
		//	 
			.def("getLastDirectoryandFileName", &CameraFactory::getLastDirectoryandFileName)
			.def("getLastestDirectoryandFileName", &CameraFactory::getLastDirectoryandFileName)
			.def("getLastDirectory", &CameraFactory::getLastDirectory)
			.def("getLatestDirectory", &CameraFactory::getLastDirectory)
			.def("getLastestFileName", &CameraFactory::getLastFileName)
			.def("getLastFileName", &CameraFactory::getLastFileName)
			.def("captureAndSave", captureAndSave_with_shotcount)
			.def("collectAndSave", captureAndSave_with_shotcount)
			.def("captureAndSave", captureAndSave_without_shotcount)
			.def("collectAndSave", captureAndSave_without_shotcount)
			.def("getCaptureState", &CameraFactory::getCaptureState)
			.def("isCapturing", &CameraFactory::isCapturing)
			.def("isNotCapturing", &CameraFactory::isNotCapturing)
			.def("isWriting", &CameraFactory::isWriting)
			.def("isNotWriting", &CameraFactory::isNotWriting)
			.def("isCapturingOrSaving", &CameraFactory::isCapturingOrSaving)
			.def("isSaving", &CameraFactory::isWriting)
			.def("isNotSaving", &CameraFactory::isNotWriting)
			.def("isCollectingOrSaving", &CameraFactory::isCapturingOrSaving)
			.def("isCollecting", &CameraFactory::isCapturing)
			.def("isNotCollecting", &CameraFactory::isNotCapturing)
	//	 __        __      
	//	|__) |  | /__` \ / 
	//	|__) \__/ .__/  |  
	//


//	 __  ___       __  ___   /  __  ___  __   __           __   __          __          __  
//	/__`  |   /\  |__)  |   /  /__`  |  /  \ |__)     /\  /  ` /  \ |  | | |__) | |\ | / _` 
//	.__/  |  /~~\ |  \  |  /   .__/  |  \__/ |       /~~\ \__, \__X \__/ | |  \ | | \| \__> 
//
		.def("stopAllAcquiring", &CameraFactory::stopAllAcquiring)

//	              __   ___                             __     __  
//	|  |\/|  /\  / _` |__      /\  |\ |  /\  |    \ / /__` | /__` 
//	|  |  | /~~\ \__> |___    /~~\ | \| /~~\ |___  |  .__/ | .__/ 
// 
//	                         __     __           __     __   ___     ___       __   __   __  
//	 /\  |\ |  /\  |    \ / /__` | /__`    |\ | /  \ | /__` |__     |__  |    /  \ /  \ |__) 
//	/~~\ | \| /~~\ |___  |  .__/ | .__/    | \| \__/ | .__/ |___    |    |___ \__/ \__/ |  \ 
//
	.def("setUseFloor", &CameraFactory::setUseFloor)
	.def("setDoNotUseFloor", &CameraFactory::setDoNotUseFloor)
	.def("toggleUseFloor", &CameraFactory::toggleUseFloor)
	.def("setFloorLevel", &CameraFactory::setFloorLevel)
	.def("getUseFloorState", &CameraFactory::getUseFloorState)
	.def("isUsingFloor", &CameraFactory::isUsingFloor)
	.def("isNotUsingFloor", &CameraFactory::isNotUsingFloor)
	.def("getFloorLevel", &CameraFactory::getFloorLevel)
	.def("getFlooredPtsCount", &CameraFactory::getFlooredPtsCount)
	.def("getFlooredPtsPercent", &CameraFactory::getFlooredPtsPercent)
	//	                         __     __           __   __         ___     __   __                    __  
	//	 /\  |\ | |     /\  \ / /__` | /__`    |\ | |__) /  \ | |\ |  |     /__` /  `  /\  |    | |\ | / _` 
	//	/~~\ | \| |___ /~~\  |  .__/ | .__/    | \| |    \__/ | | \|  |     .__/ \__, /~~\ |___ | | \| \__> 
	//	
	.def("setUseNPointScaling", &CameraFactory::setUseNPointScaling)
	.def("setDoNotUseNPointScaling", &CameraFactory::setDoNotUseNPointScaling)
	.def("toggleUseNPointScaling", &CameraFactory::toggleUseNPointScaling)
	.def("getNPointScalingState", &CameraFactory::getNPointScalingState)
	.def("isUsingNPointScaling", &CameraFactory::isUsingNPointScaling)
	.def("isNotUsingNPointScaling", &CameraFactory::isNotUsingNPointScaling)
	.def("setNpointScalingStepSize", &CameraFactory::setNpointScalingStepSize)
	.def("getNpointScalingStepSize", &CameraFactory::getNpointScalingStepSize)


//                         __     __      __        __        __   __   __             __     
// /\  |\ | |     /\  \ / /__` | /__`    |__)  /\  /  ` |__/ / _` |__) /  \ |  | |\ | |  \    
///~~\ | \| |___ /~~\  |  .__/ | .__/    |__) /~~\ \__, |  \ \__> |  \ \__/ \__/ | \| |__/    
//                                                                                            
//	                         __     __                 __       
//	 /\  |\ |  /\  |    \ / /__` | /__`     |\/|  /\  /__` |__/ 
//	/~~\ | \| /~~\ |___  |  .__/ | .__/     |  | /~~\ .__/ |  \ 
//	                                                            
//	 __   __    
//	|__) /  \ | 
//	|  \ \__/ | 
//	 

//	 __                            __      __             __                ___       ___      
//	/ _`  /\  | |\ |     /\  |\ | |  \    |__) |     /\  /  ` |__/    |    |__  \  / |__  |    
//	\__> /~~\ | | \|    /~~\ | \| |__/    |__) |___ /~~\ \__, |  \    |___ |___  \/  |___ |___ 
//	 

//	 __   __                  __                 __           __   __         __   __  
//	|__) /  \ |     /\  |\ | |  \     |\/|  /\  /__` |__/    /  ` /  \  |\/| |__) /  \ 
//	|  \ \__/ |    /~~\ | \| |__/     |  | /~~\ .__/ |  \    \__, \__/  |  | |__) \__/ 
//	

//	       ___   __   
//	|     |__   |  \  
//	|___ .|___ .|__/ .
//

	//----------------------------------------------------------------------------------------------------------------
	//			 __             __   __        __  ___    
	//			/__` |\ |  /\  |__) /__` |__| /  \  |     
	//			.__/ | \| /~~\ |    .__/ |  | \__/  |     
	//
	.def("saveSnapshot", saveSnapshot_nofile, (boost::python::arg("self"), boost::python::arg("comments") = ""))
	.def("saveSnapshot", saveSnapshot_withfile,
		(boost::python::arg("self"), boost::python::arg("filepath"), boost::python::arg("filename"), boost::python::arg("comments") = ""))
	.def("applySnaphot", applySnaphot_withDict,
		(boost::python::arg("self"), boost::python::arg("snapshot_dict"), boost::python::arg("magnet_type") = TYPE::CAMERA_TYPE))
	.def("applySnaphot", applySnaphot_withfile,
		(boost::python::arg("self"), boost::python::arg("filepath"), boost::python::arg("filename"), boost::python::arg("magnet_type") = TYPE::CAMERA_TYPE))
	.def("getSnapshot", &CameraFactory::getSnapshot_Py, (boost::python::args("self")))
	.def("getSnapshot", &CameraFactory::getSnapshotFromFile_Py, (boost::python::args("self"), boost::python::args("filepath"), boost::python::args("filename")))
	.def("loadSnapshot", &CameraFactory::loadSnapshot, (boost::python::args("self", "filepath", "filename")))
	.def("loadSnapshot", &CameraFactory::loadSnapshot_Py, (boost::python::args("self", "snapshot_dict")))
	.def("checkLastAppliedSnapshot", &CameraFactory::checkLastAppliedSnapshot, (boost::python::args("self")))

	.def("getLastSnapshotFilename", &CameraFactory::getLastSnapshotFilename)
	.def("getLastSnapshotDirectory", &CameraFactory::getLastSnapshotDirectory)

//	 __         ___          __   __            ___  __  
//	|__) | \_/ |__  |       /  ` /  \ |  | |\ |  |  /__` 
//	|    | / \ |___ |___    \__, \__/ \__/ | \|  |  .__/ 
//	        

/*
				  __   ___                             __     __
	|  |\/|  /\  / _` |__      /\  |\ |  /\  |    \ / /__` | /__`
	|  |  | /~~\ \__> |___    /~~\ | \| /~~\ |___  |  .__/ | .__/

*/
	.def("startAcquiring", &CameraFactory::startAcquiring)
	.def("stopAcquiring", &CameraFactory::stopAcquiring)
	.def("stopAcquiringAndWait", &CameraFactory::stopAcquiringAndWait)
	.def("isAcquiring", &CameraFactory::isAcquiring)
	.def("isNotAcquiring", &CameraFactory::isNotAcquiring)
	.def("getAcquireState", &CameraFactory::getAcquireState)
	.def("startAnalysing", &CameraFactory::startAnalysing)
	.def("stopAnalysing", &CameraFactory::stopAnalysing)
	.def("isAnalysing", &CameraFactory::isAnalysing)
	.def("isNotAnalysing", &CameraFactory::isNotAnalysing)
	.def("getAnalysisState", &CameraFactory::getAnalysisState)
	.def("getAcquireTime", &CameraFactory::getAcquireTime)
	.def("getAcquirePeriod", &CameraFactory::getAcquirePeriod)
	.def("getArrayRate", &CameraFactory::getArrayRate)


	//.def("setUseFloor", &CameraFactory::setUseFloor)
	//.def("toggleUseFloor", &CameraFactory::toggleUseFloor)
	//.def("setDoNotUseFloor", &CameraFactory::setDoNotUseFloor)
	//.def("setFloorLevel", &CameraFactory::setFloorLevel)
	//.def("getUseFloorState", &CameraFactory::getUseFloorState)
	//.def("isUsingFloor", &CameraFactory::isUsingFloor)
	//.def("isNotUsingFloor", &CameraFactory::isNotUsingFloor)
	//.def("getFloorLevel", &CameraFactory::getFloorLevel)
	//.def("getFlooredPtsCount", &CameraFactory::getFlooredPtsCount)
	//.def("getFlooredPtsPercent", &CameraFactory::getFlooredPtsPercent)



	.def(" ", &CameraFactory::setNewBackgroundImage)
	.def("setUseBackgroundImage", &CameraFactory::setUseBackgroundImage)
	.def("toggleUseBackgroundImage", &CameraFactory::toggleUseBackgroundImage)
	.def("setDoNotUseBackgroundImage", &CameraFactory::setDoNotUseBackgroundImage)
	.def("getUsingBackgroundImageState", &CameraFactory::getUsingBackgroundImageState)
	.def("isUsingBackgroundImage", &CameraFactory::isUsingBackgroundImage)
	.def("isNotUsingBackgroundImage", &CameraFactory::isNotUsingBackgroundImage)


//			.def("didLastCaptureAndSaveSucceed", &CameraFactory::didLastCaptureAndSaveSucceed)

	.def("getCamera", &CameraFactory::getCamera, boost::python::return_value_policy<boost::python::reference_existing_object>())
	.def("getCamType", &CameraFactory::getCamType)


	.def("getX", &CameraFactory::getX) // backwards compatibale, prefer to not use
	.def("getY", &CameraFactory::getY) // backwards compatibale, prefer to not use
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
	.def("isBusy", &CameraFactory::isBusy)
	.def("isNotBusy", &CameraFactory::isNotBusy)

	.def("getMaskXCenter", &CameraFactory::getMaskXCenter)
	.def("getMaskYCenter", &CameraFactory::getMaskYCenter)
	.def("getMaskXRadius", &CameraFactory::getMaskXRadius)
	.def("getMaskYRadius", &CameraFactory::getMaskYRadius)
	.def("getMask", &CameraFactory::getMask_Py)
	.def("getROIMinX", &CameraFactory::getROIMinX)
	.def("getROIMinY", &CameraFactory::getROIMinY)
	.def("getROISizeX", &CameraFactory::getROISizeX)
	.def("getROISizeY", &CameraFactory::getROISizeY)
	.def("getROI", &CameraFactory::getROI_Py)
	.def("getMaskAndROIxMax", &CameraFactory::getMaskAndROIxMax)
	.def("getMaskAndROIyMax", &CameraFactory::getMaskAndROIyMax)
	.def("getMaskAndROIxSize", &CameraFactory::getMaskAndROIxSize)
	.def("getMaskAndROIySize", &CameraFactory::getMaskAndROIySize)
	.def("getMaskandROI", &CameraFactory::getMaskandROI_Py)
	.def("updateImageData", &CameraFactory::updateImageData)
	.def("updateImageDataWithTimeStamp", &CameraFactory::updateImageDataWithTimeStamp)
	.def("updateROIData", &CameraFactory::updateROIData)
	.def("updateROIDataWithTimeStamp", &CameraFactory::updateROIDataWithTimeStamp)
	.def("getImageDataConstRef", &CameraFactory::getImageDataConstRef, boost::python::return_value_policy<boost::python::reference_existing_object>())
	.def("getROIDataConstRef", &CameraFactory::getROIDataConstRef, boost::python::return_value_policy<boost::python::reference_existing_object>())
	.def("getImageData", &CameraFactory::getImageData_Py)
	.def("getROIData", &CameraFactory::getROIData_Py)
	.def("setMaskAndROIxMax", &CameraFactory::setMaskAndROIxMax) 			// use these for setting mask AND ROI 
	.def("setMaskAndROIyMax", &CameraFactory::setMaskAndROIyMax)
	.def("setMaskAndROIxSize", &CameraFactory::setMaskAndROIxSize)
	.def("setMaskAndROIySize", &CameraFactory::setMaskAndROIySize)
	.def("setMaskandROI_4PARAM", setMaskandROI_4PARAM)
	.def("setMaskandROI", &CameraFactory::setMaskandROI_Py)
	.def("setMaskXCenter", &CameraFactory::setMaskXCenter) 			// we don:t want people to use these!! 
	.def("setMaskYCenter", &CameraFactory::setMaskYCenter)
	.def("setMaskXRadius", &CameraFactory::setMaskXRadius)
	.def("setMaskYRadius", &CameraFactory::setMaskYRadius)
	.def("setMask", setMask_4PARAM)
	.def("setMask", &CameraFactory::setMask_Py)
	.def("setROIMinX", &CameraFactory::setROIMinX)
	.def("setROIMinY", &CameraFactory::setROIMinY)
	.def("setROISizeX", &CameraFactory::setROISizeX)
	.def("setROIsizeY", &CameraFactory::setROISizeY)
	.def("setROI", setROI_4PARAM)
	.def("setROI", &CameraFactory::setROI_Py)

			
				
		.def("getSumIntensity", &CameraFactory::getSumIntensity)
		.def("getAvgIntensity", &CameraFactory::getAvgIntensity)
		.def("setSumIntensity", &CameraFactory::setSumIntensity)
		.def("setAvgIntensity", &CameraFactory::setAvgIntensity)

		.def("getTemperature", &CameraFactory::getTemperature)
		.def("hasLED", &CameraFactory::hasLED)
		.def("setLEDOn", &CameraFactory::setLEDOn)
		.def("setLEDOff", &CameraFactory::setLEDOff)
		.def("isLEDOn", &CameraFactory::isLEDOn)
		.def("isLEDOff", &CameraFactory::isLEDOff)
		.def("getLEDState", &CameraFactory::getLEDState)
		.def("getBufferSize", &CameraFactory::getBufferSize)
		.def("setBufferSize", &CameraFactory::setBufferSize)
		.def("clearBuffers", &CameraFactory::clearBuffers)
		.def("getAllRunningStats", &CameraFactory::getAllRunningStats)
		.def("getScreen", &CameraFactory::getScreen)


		.def("getAnalysisResultsPixels", &CameraFactory::getAnalysisResultsPixels_Py)


		.def("setGain", &CameraFactory::setGain)
		.def("getGain", &CameraFactory::getGain)
		.def("setBlackLevel", &CameraFactory::setBlackLevel)
		.def("getBlackLevel", &CameraFactory::getBlackLevel)

		.def("areAllRunningStatsFull", &CameraFactory::areAllRunningStatsFull)



		.def("getActiveCameraLimit", &CameraFactory::getActiveCameraLimit)
		.def("getActiveCameraCount", &CameraFactory::getActiveCameraCount)
		.def("canStartCamera", &CameraFactory::canStartCamera)

			//	              __   ___     __        ___  __                 __  
			//	|  |\/|  /\  / _` |__     /  \ \  / |__  |__) |     /\  \ / /__` 
			//	|  |  | /~~\ \__> |___    \__/  \/  |___ |  \ |___ /~~\  |  .__/ 
			//	
			//	TODO  
			//	

		.def("enableOverlayCross", &CameraFactory::enableOverlayCross)
		.def("disableOverlayCross", &CameraFactory::disableOverlayCross)
		//.def("getOverlayCrossState", &CameraFactory::getOverlayCrossState)
		//.def("isOverlayCrossEnabled", &CameraFactory::isOverlayCrossEnabled)
		//.def("isOverlayCrossDisabled", &CameraFactory::isOverlayCrossDisabled)
		.def("enableOverlayMask", &CameraFactory::enableOverlayMask)
		.def("disableOverlayMask", &CameraFactory::disableOverlayMask)
		//.def("getOverlayMaskState", &CameraFactory::getOverlayMaskState)
		//.def("isOverlayMaskEnabled", &CameraFactory::isOverlayMaskEnabled)
		//.def("isOverlayMaskDisabled", &CameraFactory::isOverlayMaskDisabled)
		.def("enableOverlayResult", &CameraFactory::enableOverlayResult)
		.def("disableOverlayResult", &CameraFactory::disableOverlayResult)
		//.def("getOverlayResultState", &CameraFactory::getOverlayResultState)
		//.def("isOverlayResultEnabled", &CameraFactory::isOverlayResultEnabled)
		//.def("isOverlayResultDisabled", &CameraFactory::isOverlayResultDisabled)
		.def("disableAllOverlay", &CameraFactory::disableAllOverlay)
		.def("disableAllOverlayForAllCameras", &CameraFactory::disableAllOverlayForAllCameras)

			//.def("pix2mmX", &CameraFactory::pix2mmX)
			//.def("pix2mmY", &CameraFactory::pix2mmY)
			//.def("mm2pixX", &CameraFactory::mm2pixX)
			//.def("mm2pixY", &CameraFactory::mm2pixY)
			//.def("getpix2mmX", &CameraFactory::getpix2mmX)
			//.def("getpix2mmY", &CameraFactory::getpix2mmY)
			//.def("setpix2mmX", &CameraFactory::setpix2mmX)
			//.def("setpix2mmY", &CameraFactory::setpix2mmY)
			//.def("getX", &CameraFactory::getX)
			//.def("getY", &CameraFactory::getY)
			//.def("getXmm", &CameraFactory::getX)
			//.def("getYmm", &CameraFactory::getY)
			//.def("getSigX", &CameraFactory::getSigX)
			//.def("getSigY", &CameraFactory::getSigY)
			//.def("getSigXY", &CameraFactory::getSigXY)
			//.def("getSigXmm", &CameraFactory::getSigX)
			//.def("getSigYmm", &CameraFactory::getSigY)
			//.def("getSigXYmm", &CameraFactory::getSigXY)
			//.def("getXPix", &CameraFactory::getXPix)
			//.def("getYPix", &CameraFactory::getYPix)
			//.def("getSigXPix", &CameraFactory::getSigXPix)
			//.def("getSigYPix", &CameraFactory::getSigYPix)
			//.def("getSigXYPix", &CameraFactory::getSigXYPix)
			//.def("setX", &CameraFactory::setX)
			//.def("setY", &CameraFactory::setY)
			//.def("setSigX", &CameraFactory::setSigX)
			//.def("setSigY", &CameraFactory::setSigY)
			//.def("setSigXY", &CameraFactory::setSigXY)
			//.def("setXmm", &CameraFactory::setX)
			//.def("setYmm", &CameraFactory::setY)
			//.def("setSigXmm", &CameraFactory::setSigX)
			//.def("setSigYmm", &CameraFactory::setSigY)
			//.def("setSigXYmm", &CameraFactory::setSigXY)
			//.def("getStepSize", &CameraFactory::getStepSize)
			//.def("setStepSize", &CameraFactory::setStepSize)
			//.def("useBackground", &CameraFactory::useBackground)
			//.def("isUsingBackground", &CameraFactory::isUsingBackground)
			//.def("getUsingBackgroundState", &CameraFactory::getUsingBackgroundState)
			//.def("useNPoint", &CameraFactory::useNPoint)
			//.def("isUsingNPoint", &CameraFactory::isUsingNPoint)
			//.def("getNPointState", &CameraFactory::getNPointState)
			//.def("getSumIntensity", &CameraFactory::getSumIntensity)
			//.def("getAvgIntensity", &CameraFactory::getAvgIntensity)
			//.def("setSumIntensity", &CameraFactory::setSumIntensity)
			//.def("setAvgIntensity", &CameraFactory::setAvgIntensity)
			//.def("stopAcquiring", &CameraFactory::stopAcquiring)
			//.def("stopAllAcquiring", &CameraFactory::stopAllAcquiring)
			//.def("stopAllAcquiringExceptVC", &CameraFactory::stopAllAcquiringExceptVC)
			//.def("startAcquiring", &CameraFactory::startAcquiring)
			//.def("getAllAcquireStates", &CameraFactory::getAllAcquireStates_Py)
			//.def("isAcquiring", &CameraFactory::isAcquiring)
			//.def("isNotAcquiring", &CameraFactory::isNotAcquiring)
			//.def("getAcquireState", &CameraFactory::getAcquireState)
			//.def("getRunningStats", &CameraFactory::getRunningStats)
			//.def("getAllRunningStats", &CameraFactory::getAllRunningStats)
			//.def("getMaskXCenter", &CameraFactory::getMaskXCenter)
			//.def("getMaskYCenter", &CameraFactory::getMaskYCenter)
			//.def("getMaskXRadius", &CameraFactory::getMaskXRadius)
			//.def("getMaskYRadius", &CameraFactory::getMaskYRadius)
			//.def("setMaskXCenter", &CameraFactory::setMaskXCenter)
			//.def("setMaskYCenter", &CameraFactory::setMaskYCenter)
			//.def("setMaskXRadius", &CameraFactory::setMaskXRadius)
			//.def("setMaskYRadius", &CameraFactory::setMaskYRadius)
			//.def("startAnalysing", &CameraFactory::startAnalysing)
			//.def("stopAnalysing", &CameraFactory::stopAnalysing)
			//.def("isAnalysing", &CameraFactory::isAnalysing)
			//.def("isNotAnalysing", &CameraFactory::isNotAnalysing)
			//.def("getAnalysisState", &CameraFactory::getAnalysisState)
			//.def("getAllAnalysisState", &CameraFactory::getAllAnalysisState_Py)
			.def("debugMessagesOn", &CameraFactory::debugMessagesOn)
			.def("debugMessagesOff", &CameraFactory::debugMessagesOff)
			.def("messagesOn", &CameraFactory::messagesOn)
			.def("messagesOff", &CameraFactory::messagesOff)
			.def("isDebugOn", &CameraFactory::isDebugOn)
			.def("isMessagingOn", &CameraFactory::isMessagingOn);
	}



}

#endif //CAMERA_PYTHON_INTERFACE_H_