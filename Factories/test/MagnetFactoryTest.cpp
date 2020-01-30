#include <boost/test/unit_test.hpp>
#include "MagnetFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(MagnetFactoryTestSuite)
BOOST_AUTO_TEST_CASE(start_test)
{
	BOOST_TEST_MESSAGE("------	RUNNING MAGNET FACTORY TESTS	------");
}
BOOST_AUTO_TEST_CASE(magnet_factory_turn_on_magnet_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	magFac.messagesOn();
	magFac.debugMessagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("SPOWER").CHID) == cs_conn)
		{
			magFac.switchOn(testMagnetName);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			BOOST_CHECK_EQUAL(magFac.getPSUState(testMagnetName), 1);
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
}

BOOST_AUTO_TEST_CASE(magnet_factory_read_i_magnet_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	magFac.messagesOn();
	magFac.debugMessagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("SETI").CHID) == cs_conn)
		{
			srand(time(NULL));
			double currentToSet = rand() % 10 + 1.0;
			magFac.SETI(testMagnetName, currentToSet);
			std::this_thread::sleep_for(std::chrono::seconds(10));
			BOOST_CHECK_EQUAL(magFac.getSETI(testMagnetName), currentToSet);
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
}

BOOST_AUTO_TEST_CASE(magnet_factory_rilk_state_test)
{
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	magFac.messagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("RILK").CHID) == cs_conn)
		{
			BOOST_CHECK_EQUAL(magFac.getILKState(testMagnetName), STATE::ON);
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}

	}
}

BOOST_AUTO_TEST_CASE(magnet_factory_get_all_magnet_currents_test)
{
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
	magFac.messagesOn();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("GETSETI").CHID) == cs_conn)
		{
			std::map<std::string, double> allMagCurrents = magFac.getAllSETI();
			BOOST_CHECK_NE(allMagCurrents.at(testMagnetName), std::numeric_limits<double>::min());
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
	magFac.messenger.dumpToFile("MF_TEST_OUTPUT.txt");
}



BOOST_AUTO_TEST_SUITE_END();
