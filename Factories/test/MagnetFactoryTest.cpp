#define BOOST_TEST_MODULE MagnetFactoryTest

#include <boost/test/unit_test.hpp>
#include "MagnetFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(MagnetFactoryTestSuite)
LoggingSystem testLogger(true, true);
BOOST_AUTO_TEST_CASE(magnet_factory_turn_on_magnet_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(true);
	LoggingSystem::messagesOn();
	LoggingSystem::debugMessagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		magFac.turnOn(testMagnetName);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		BOOST_CHECK_EQUAL(magFac.getPSUState(testMagnetName), 1);
	}
	LoggingSystem::dumpToFile("MF_TEST_1_OUTPUT.txt");
}

BOOST_AUTO_TEST_CASE(magnet_factory_read_i_magnet_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(true);
	LoggingSystem::messagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		srand(time(NULL));
		double currentToSet = rand() % 10 + 1.0;
		magFac.setCurrent(testMagnetName, currentToSet);
		std::this_thread::sleep_for(std::chrono::seconds(10));
		BOOST_CHECK_EQUAL(magFac.getRICurrent(testMagnetName), currentToSet);
	}
	LoggingSystem::dumpToFile("MF_TEST_2_OUTPUT.txt");
}

BOOST_AUTO_TEST_CASE(magnet_factory_rilk_state_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(true);
	LoggingSystem::messagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		BOOST_CHECK_EQUAL(magFac.getILKState(testMagnetName), 0);
	}
	LoggingSystem::dumpToFile("MF_TEST_3_OUTPUT.txt");
}

BOOST_AUTO_TEST_CASE(magnet_factory_get_all_magnet_currents_test)
{
	MagnetFactory magFac = MagnetFactory(true);
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	LoggingSystem::messagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		std::map<std::string, double> allMagCurrents = magFac.getAllMagnetCurrents();
		BOOST_CHECK_NE(allMagCurrents.at(testMagnetName), std::numeric_limits<double>::min());
	}
	LoggingSystem::dumpToFile("MF_TEST_4_OUTPUT.txt");
}

BOOST_AUTO_TEST_CASE(magnet_factory_logging_system_test)
{
	MagnetFactory magFac = MagnetFactory(true);
	bool status = magFac.setup("nominal");
	magFac.messagesOn();
	BOOST_CHECK_EQUAL(magFac.isMessagingOn(), true);
	BOOST_CHECK_EQUAL(magFac.reader.isMessagingOn(), true);
	if (status)
	{
		for (auto& mag : magFac.getAllMagnets())
		{
			BOOST_CHECK_EQUAL(mag.second.isMessagingOn(), true);
			BOOST_CHECK_EQUAL(mag.second.epicsInterface->isMessagingOn(), true);
		}
	}
	magFac.messagesOff();
	BOOST_CHECK_EQUAL(magFac.isMessagingOn(), false);
	BOOST_CHECK_EQUAL(magFac.reader.isMessagingOn(), false);
	if (status)
	{
		for (auto& mag : magFac.getAllMagnets())
		{
			BOOST_CHECK_EQUAL(mag.second.isMessagingOn(), false);
			BOOST_CHECK_EQUAL(mag.second.epicsInterface->isMessagingOn(), false);
		}
	}
	magFac.debugMessagesOn();
	BOOST_CHECK_EQUAL(magFac.isDebugOn(), true);
	BOOST_CHECK_EQUAL(magFac.reader.isDebugOn(), true);
	if (status)
	{
		for (auto& mag : magFac.getAllMagnets())
		{
			BOOST_CHECK_EQUAL(mag.second.isDebugOn(), true);
			BOOST_CHECK_EQUAL(mag.second.epicsInterface->isDebugOn(), true);
		}
	}
	magFac.debugMessagesOff();
	BOOST_CHECK_EQUAL(magFac.isDebugOn(), false);
	BOOST_CHECK_EQUAL(magFac.reader.isDebugOn(), false);
	if (status)
	{
		for (auto& mag : magFac.getAllMagnets())
		{
			BOOST_CHECK_EQUAL(mag.second.isDebugOn(), false);
			BOOST_CHECK_EQUAL(mag.second.epicsInterface->isDebugOn(), false);
		}
	}
}


BOOST_AUTO_TEST_SUITE_END();