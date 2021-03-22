#include <boost/test/unit_test.hpp>
#include <GlobalConstants.h>
#include <IMGFactory.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>

//struct fixture {
//	fixture()
//	{
//		imgFac = IMGFactory(STATE::VIRTUAL);
//		imgFac.messagesOn();
//		imgFac.debugMessagesOff();
//	}
//	~fixture() { BOOST_TEST_MESSAGE("teardown fixture"); }
//
//	IMGFactory imgFac;
//	pvStruct setPV;
//
//};

BOOST_AUTO_TEST_SUITE(IMGFactoryTestSuite)

	BOOST_AUTO_TEST_CASE(start_test)
	{
		BOOST_TEST_MESSAGE("------	RUNNING HARDWARE FACTORY TESTS	------");
	}
	BOOST_AUTO_TEST_CASE(img_factory_check_values_on_pressure_test)
	{
		BOOST_TEST_MESSAGE("------	IMG FACTORY: CHECK VALUES ON PRESSURE	------");
		std::string testIMGName = "EBT-INJ-VAC-IMG-03";
		IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
		imgFac.messagesOn();
		imgFac.debugMessagesOff();
		bool status = imgFac.setup("nominal");
		if (status)
		{
			IMG test_IMG = imgFac.getIMG(testIMGName);
			if (ca_state(test_IMG.pvStructs.at("P").CHID) == cs_conn)
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
		IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
		imgFac.messagesOn();
		imgFac.debugMessagesOff();
		auto namesMap = imgFac.getAllIMGNames();
		bool status = imgFac.setup("nominal");
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
		imgFac.debugMessagesOff();
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
		std::string testIMGName = "EBT-INJ-VAC-IMG-03";
		IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
		imgFac.messagesOn();
		imgFac.debugMessagesOff();
		bool status = imgFac.setup("nominal");
		if (status)
		{
			IMG testIMG = imgFac.getIMG(testIMGName);
			if (ca_state(testIMG.pvStructs.at("P").CHID) == cs_conn)
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
		IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
		imgFac.messagesOn();
		imgFac.debugMessagesOff();
		bool status = imgFac.setup("nominal");
		if (status)
		{
			auto allIMGNames = imgFac.getAllIMGNames();

			for (auto img : imgFac.IMGMap)
			{
				int i = 1;
					while (i < 100)
					{

						if (ca_state(img.second.pvStructs.at("P").CHID) == cs_conn)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(5));
							BOOST_CHECK(imgFac.getIMGPressure(img.first) != GlobalConstants::double_min);
							i++;
						}
						else
						{
							imgFac.messenger.printDebugMessage(img.first, " NOT CONNECTED TO EPICS");
						}
					}

			}
		}
		imgFac.messenger.dumpToFile("MF_TEST_OUTPUT.txt");
	}

	BOOST_AUTO_TEST_CASE(img_factory_retrieve_monitor_test)
	{
		BOOST_TEST_MESSAGE("------	IMG FACTORY: RETRIEVE MONITOR STATUS	------");
		IMGFactory imgFac = IMGFactory(STATE::VIRTUAL);
		imgFac.messagesOn();
		imgFac.debugMessagesOff();
		pvStruct setPV;
		setPV.fullPVName = "VM-EBT-INJ-VAC-IMG-03";
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