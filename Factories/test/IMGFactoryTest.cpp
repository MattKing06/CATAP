#include <boost/test/unit_test.hpp>
#include <IMGFactory.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(IMGFactoryTestSuite)
BOOST_AUTO_TEST_CASE(start_test)
{
	BOOST_TEST_MESSAGE("------	RUNNING IMG FACTORY TESTS	------");
}

BOOST_AUTO_TEST_CASE(img_factory_check_values_on_pressure_test)
{
	BOOST_TEST_MESSAGE("------	IMG FACTORY: CHECK VALUES ON PRESSURE	------");
	std::string testIMGName = "EBT-INJ-VAC-IMG-01";
	IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
	imgFac.messagesOn();
	imgFac.debugMessagesOn();
	bool status = imgFac.setup("nominal");
	if (status)
	{
		if (ca_state(imgFac.getIMG(testIMGName).pvStructs.at("P").CHID) == cs_conn)
		{
			BOOST_CHECK(isnan(imgFac.getIMGPressure(testIMGName)) == true);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else
		{
			imgFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
}

BOOST_AUTO_TEST_CASE(setting_up_img_factory_and_get_names_pressure_test)
{
	BOOST_TEST_MESSAGE("------	IMG FACTORY: GET MAP OF NAMES AND PRESSURES	------");
	IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
	bool status = imgFac.setup("nominal");
	auto namesMap = imgFac.getAllIMGNames();
		
	if (status)
	{
		for (auto&& item : namesMap)
		{
			if (ca_state(imgFac.getIMG(item).pvStructs.at("P").CHID) == cs_conn)
			{
				BOOST_CHECK(isnan(imgFac.getIMGPressure(item)) != true);
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			else
			{
				imgFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
			}
		}
	}	
}

BOOST_AUTO_TEST_CASE(img_factory_check_all_names_test)
{
	BOOST_TEST_MESSAGE("------	IMG FACTORY: GET ALL NAMES	------");
	IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
	imgFac.messagesOn();
	bool status = imgFac.setup("nominal");
	
	if (status) {
		auto allIMGNames = imgFac.getAllIMGNames();
		std::map<std::string, double> pressureMap = imgFac.getAllIMGPressure();
		for (auto imgName : allIMGNames)
		{
			if (ca_state(imgFac.getIMG(imgName).pvStructs.at("P").CHID) == cs_conn)
			{
				BOOST_CHECK(pressureMap.at(imgName) != NULL);
			}
			else
			{
				imgFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(img_factory_get_all_pressure_test)
{
	BOOST_TEST_MESSAGE("------	IMG FACTORY: GET ALL OF PRESSURES OF THE MAP	------");
	IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
	std::string testIMGName = "EBT-INJ-VAC-IMG-01";
	imgFac.messagesOn();
	bool status = imgFac.setup("nominal");
	if (status)
	{
		if (ca_state(imgFac.getIMG(testIMGName).pvStructs.at("P").CHID) == cs_conn)
		{
			std::map<std::string, double> allPressures = imgFac.getAllIMGPressure();
			BOOST_CHECK(isnan(allPressures.at(testIMGName)) != true);

		}
		else
		{
			imgFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
	imgFac.messenger.dumpToFile("MF_TEST_OUTPUT.txt");
}

BOOST_AUTO_TEST_CASE(img_factory_retrieve_monitor_test)
{
	BOOST_TEST_MESSAGE("------	IMG FACTORY: RETRIEVE MONITOR STATUS	------");
	IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
	pvStruct setPV;
	setPV.fullPVName = "VM-EBT-INJ-VAC-IMG-01";
	setPV.pvRecord = "P";
	imgFac.messagesOn();
	bool status = imgFac.setup("nominal");
	imgFac.retrieveMonitorStatus(setPV);
	BOOST_CHECK(setPV.monitor == true);
	setPV.pvRecord = 'Sta';
	imgFac.retrieveMonitorStatus(setPV);
	BOOST_CHECK(setPV.monitor == true);
	
}

BOOST_AUTO_TEST_SUITE_END()