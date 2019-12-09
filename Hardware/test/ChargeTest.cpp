#define BOOST_TEST_MODULE ChargeTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include "Charge.h"
#include <vector>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(ChargeTestSuite)

BOOST_AUTO_TEST_CASE(creating_empty_charge_test)
{
	Charge charge = Charge();
	BOOST_CHECK_EQUAL(charge.getHardwareName().c_str(), "");
	auto pvStructMap = charge.getPVStructs();
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_CASE(charge_set_and_check_q_test)
{
	Charge charge = Charge();
	srand(time(NULL));
	double qToSet = rand() % 10 + 1.0;
	charge.setQ(qToSet);
	BOOST_CHECK_EQUAL(charge.getQ(), qToSet);
}

BOOST_AUTO_TEST_CASE(charge_set_and_check_buffer_test)
{
	Charge charge = Charge();
	srand(time(NULL));
	charge.setBufferSize(2);
	boost::circular_buffer< double > dataToSet(2);
	double set1 = rand() % 10 + 1.0;
	dataToSet.push_back(set1);
	charge.setQ(set1);
	double set2 = rand() % 10 + 1.0;
	dataToSet.push_back(set2);
	charge.setQ(set2);
	boost::circular_buffer< double > get = charge.getQBuffer();
	BOOST_TEST(get == dataToSet, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(charge_monitor_test)
{
	Charge charge = Charge();
	size_t numShots = 2;
	srand(time(NULL));
	charge.monitorForNShots(numShots);
	double qToSet1 = rand() % 10 + 1.0;
	BOOST_CHECK_EQUAL(charge.isMonitoring(), true);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	charge.setQ(qToSet1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	double qToSet2 = rand() % 10 + 1.0;
	charge.setQ(qToSet2);
	std::vector< double > qVector(2);
	qVector[0] = qToSet1;
	qVector[1] = qToSet2;
	std::vector< double > get = charge.getQVector();
	BOOST_TEST(get == qVector, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_SUITE_END()
