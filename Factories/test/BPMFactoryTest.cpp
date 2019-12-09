#define BOOST_TEST_MODULE BPMFactoryTest

#include <boost/test/unit_test.hpp>
#include "BPMFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(BPMFactoryTestSuite)
LoggingSystem testLogger(true, true);
BOOST_AUTO_TEST_CASE(bpm_factory_set_and_check_sa1_test)
{
	std::string testBPMName = "VM-CLA-S01-DIA-BPM-01";
	BPMFactory bpmfac = BPMFactory(true);
	//bpmfac.messagesOn();
	bool status = bpmfac.setup("nominal");
	//if (status)
	//{
	//	srand(time(NULL));
	//	long sa1ToSet = rand() % 10;
	//	bpmfac.setSA1(testBPMName, sa1ToSet);
	//	std::this_thread::sleep_for(std::chrono::seconds(1));
	//	BOOST_CHECK_EQUAL(bpmfac.getSA1(testBPMName), sa1ToSet);
	//}
	//else
	//{
	//	testLogger.printMessage("CANNOT CONNECT TO EPICS");
	//}
}

BOOST_AUTO_TEST_SUITE_END();