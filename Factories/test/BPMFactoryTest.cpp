#include <boost/test/unit_test.hpp>
#include "BPMFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(BPMFactoryTestSuite)
BOOST_AUTO_TEST_CASE(start_test)
{
	BOOST_TEST_MESSAGE("------	RUNNING BPM FACTORY TESTS	------");
}


BOOST_AUTO_TEST_CASE(bpm_factory_set_virtual_x_and_y_test)
{
	BPMFactory bpmFac = BPMFactory(STATE::VIRTUAL);
	bool status = bpmFac.setup("nominal");

	if (status)
	{
		std::vector<std::string> bpm_names = bpmFac.getAllBPMNames();
		for (auto& name : bpm_names)
		{
			BPM bpm = bpmFac.getBPM(name);
			bpm.setXPV(1.0);
			bpm.setYPV(1.0);
		}
	}

}

BOOST_AUTO_TEST_CASE(bpm_factory_set_and_check_sa1_test)
{
	std::string testBPMName = "CLA-S01-DIA-BPM-01";
	BPMFactory bpmfac = BPMFactory(STATE::VIRTUAL);
	bpmfac.messagesOn();
	bool status = bpmfac.setup("nominal");
	if (status)
	{
		if (ca_state(bpmfac.getBPM(testBPMName).pvStructs.at("SA1").CHID) == cs_conn)
		{
			srand(time(NULL));
			long sa1ToSet = rand() % 10;
			bpmfac.setSA1(testBPMName, sa1ToSet);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			BOOST_CHECK_EQUAL(bpmfac.getSA1(testBPMName), sa1ToSet);
		}
		else
		{
			bpmfac.messenger.printMessage("CANNOT CONNECT TO EPICS");
		}
	}

}

BOOST_AUTO_TEST_SUITE_END();