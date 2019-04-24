#define BOOST_TEST_MODULE MagnetTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include "Magnet.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(MagnetTestSuite)

BOOST_AUTO_TEST_CASE(magnet_with_fullname_test)
{
	Magnet *mag = new Magnet("CLA-C2V-MAG-HCOR-01");
	std::cout << "MAGNET NAME:" << mag->fullPVName << std::endl;
	BOOST_CHECK_EQUAL(mag->fullPVName, "CLA-C2V-MAG-HCOR-01");
	for (auto record : mag->pvStructs)
	{
		std::cout << record.fullPVName << ":" << record.pvRecord << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()