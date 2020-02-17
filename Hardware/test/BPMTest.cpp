#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include "BPM.h"
#include <vector>
//#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(BPMTestSuite)

BOOST_AUTO_TEST_CASE(creating_empty_bpm_test)
{
	BPM bpm = BPM();
	BOOST_CHECK_EQUAL(bpm.getHardwareName().c_str(), "");
	auto pvStructMap = bpm.getPVStructs();
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_CASE(bpm_set_and_check_x_test)
{
	BPM bpm = BPM();
	srand(time(NULL));
	double xToSet = rand() % 10 + 1.0;
	bpm.setXPV(xToSet);
	BOOST_CHECK_EQUAL(bpm.getXFromPV(), xToSet);
}

BOOST_AUTO_TEST_CASE(bpm_set_and_check_data_test)
{
	BPM bpm = BPM();
	srand(time(NULL));
	std::vector< double > dataToSet(13);
	for(auto& it : dataToSet)
	{
		it = rand() % 10 + 1.0;
	}
	bpm.setData(dataToSet);
	std::vector< double > get = bpm.getData();
	std::vector< double > set = dataToSet;
	BOOST_CHECK_EQUAL_COLLECTIONS(get.begin(), get.end(), set.begin(), set.end());
}

BOOST_AUTO_TEST_CASE(bpm_set_and_check_data_vector_test)
{
	BPM bpm = BPM();
	srand(time(NULL));
	std::vector< std::vector< double > > dataToSet(2);
	bpm.setVectorSize(2);
	for (auto&& it : dataToSet)
	{
		it.resize(13);
		for (auto&& it2 : it)
		{
			it2 = rand() % 10 + 1.0;
		}
	}
	bpm.setData(dataToSet.back());
	std::vector< double > get = bpm.getData();
	std::vector< double > set = dataToSet.back();
	BOOST_CHECK_EQUAL_COLLECTIONS(get.begin(), get.end(), set.begin(), set.end());
}

BOOST_AUTO_TEST_CASE(bpm_monitor_test)
{
	BPM bpm = BPM();
	size_t numShots = 2;
	srand(time(NULL));
	bpm.monitorForNShots(numShots);
	double xPVToSet1 = rand() % 10 + 1.0;
	BOOST_CHECK_EQUAL(bpm.ismonitoringXPV(), true);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	bpm.setXPV(xPVToSet1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	double xPVToSet2 = rand() % 10 + 1.0;
	bpm.setXPV(xPVToSet2);
	std::vector< double > xPVVector(2);
	xPVVector[0] = xPVToSet1;
	xPVVector[1] = xPVToSet2;
	std::vector< double > get = bpm.getXPVVector();
	BOOST_TEST(get == xPVVector, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_SUITE_END()
