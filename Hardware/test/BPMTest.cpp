#define BOOST_TEST_MODULE BPMTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include "BPM.h"
#include <vector>

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
	std::vector< double > dataToSet(9);
	for(auto& it : dataToSet)
	{
		it = rand() % 10 + 1.0;
	}
	bpm.setData(dataToSet);
	BOOST_CHECK_EQUAL_COLLECTIONS(bpm.getData().begin(), bpm.getData().end(), dataToSet.begin(), dataToSet.end());
}

BOOST_AUTO_TEST_CASE(bpm_set_and_check_data_vector_test)
{
	BPM bpm = BPM();
	srand(time(NULL));
	std::vector< std::vector< double > > dataToSet(2);
	bpm.setVectorSize(2);
	for (auto&& it : dataToSet)
	{
		it.resize(9);
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

BOOST_AUTO_TEST_SUITE_END()
