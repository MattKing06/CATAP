#include <boost/test/unit_test.hpp>
#include <GlobalConstants.h>
#include <IMGFactory.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>

struct fixture {
	fixture()
	{
		imgFac = IMGFactory(STATE::VIRTUAL);
		imgFac.messagesOn();
		imgFac.debugMessagesOff();
		std::string testIMGName = "EBT-INJ-VAC-IMG-01";
		BOOST_TEST_MESSAGE("setup fixture");
		status = imgFac.setup("nominal");
	}
	~fixture() { BOOST_TEST_MESSAGE("teardown fixture"); }

	IMGFactory imgFac;
	std::string testIMGName;
	bool status;
	pvStruct setPV;

};

BOOST_FIXTURE_TEST_SUITE(IMGFactoryTestSuite, fixture)
BOOST_AUTO_TEST_CASE(start_test)
{
	BOOST_TEST_MESSAGE("------	RUNNING HARDWARE FACTORY TESTS	------");
}
BOOST_AUTO_TEST_CASE(img_factory_check_values_on_pressure_test)
{
	BOOST_TEST_MESSAGE("------	IMG FACTORY: CHECK VALUES ON PRESSURE	------");
	if (status)
	{
		if (ca_state(imgFac.getIMG(testIMGName).pvStructs.at("P").CHID) == cs_conn)
		{
			BOOST_CHECK(isnan(imgFac.getIMGPressure(testIMGName)) != true);
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

BOOST_AUTO_TEST_CASE(img_factory_get_all_pressure_test_several_times)
{
	BOOST_TEST_MESSAGE("------	IMG FACTORY: GET ALL OF PRESSURES OF THE MAP	------");
	std::map<std::string, double> allPressures = imgFac.getAllIMGPressure();
	int i = 1;
	if (status)
	{
		if (ca_state(imgFac.getIMG(testIMGName).pvStructs.at("P").CHID) == cs_conn)
		{
			while (i < 1000)
			{
				auto namesMap = imgFac.getAllIMGNames();
				for (auto&& item : namesMap)
				{
					BOOST_CHECK(allPressures.at(item) != GlobalConstants::double_min);
					BOOST_CHECK(imgFac.getIMGPressure(item) != GlobalConstants::double_min);
				}
				i++;
			}
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
