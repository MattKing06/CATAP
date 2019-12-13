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
	bool status = magFac.setup("nominal");
	if (status)
	{
		magFac.turnOn(testMagnetName);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		BOOST_CHECK_EQUAL(magFac.getPSUState(testMagnetName), 1);
	}

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

}


BOOST_AUTO_TEST_SUITE_END();