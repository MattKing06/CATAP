#define BOOST_TEST_MODULE MagnetTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include "Magnet.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(MagnetTestSuite)

BOOST_AUTO_TEST_CASE(creating_empty_magnet_test)
{
	Magnet *mag = new Magnet();
	std::cout << "MAGNET NAME:" << mag->getFullPVName() << std::endl;
	//BOOST_CHECK_EQUAL(mag->getFullPVName().c_str(), "CLA-C2V-MAG-HCOR-01");
	auto pvStructVector = mag->getPVStructs();
	for (auto record = pvStructVector->begin(); record != pvStructVector->end(); record++)
	{
		std::cout << record->fullPVName << ":" << record->pvRecord << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()
