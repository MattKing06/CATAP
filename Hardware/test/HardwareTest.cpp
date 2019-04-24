#define BOOST_TEST_MODULE HardwareTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "Hardware.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(HardwareTestSuite)

BOOST_AUTO_TEST_CASE(dummy_hardware_test)
{
	Hardware *component = new Hardware();
	std::cout << component->machineArea << std::endl;
	for (auto pv : component->pvStructs)
	{
		std::cout << pv.fullPVName << ":" << pv.pvRecord << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()