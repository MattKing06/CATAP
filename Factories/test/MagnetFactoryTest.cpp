#define BOOST_TEST_MODULE MagnetFactoryTest

#include <boost/test/unit_test.hpp>
#include "MagnetFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_CASE(magnet_factory_turn_on_magnet_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(true);
	magFac.setup("nominal");
	magFac.turnOn(testMagnetName);
	BOOST_CHECK_EQUAL(magFac.getPSUState(testMagnetName), 1);
}

BOOST_AUTO_TEST_CASE(magnet_factory_read_i_magnet_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(true);
	magFac.setup("nominal");
	srand(time(NULL));
	double currentToSet = rand() % 10 + 1.0;
	magFac.setCurrent(testMagnetName, currentToSet);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	BOOST_CHECK_CLOSE(magFac.getRICurrent(testMagnetName), currentToSet, 10);
}

BOOST_AUTO_TEST_CASE(magnet_factory_rilk_state_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(true);
	magFac.setup("nominal");
	BOOST_CHECK_EQUAL(magFac.getILKState(testMagnetName), 0);
}