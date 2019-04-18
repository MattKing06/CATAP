#define BOOST_TEST_MODULE MagnetTest

#include <boost/test/unit_test.hpp>
#include <string>
#include "Magnet.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(MagnetTestSuite)

BOOST_AUTO_TEST_CASE(magnet_with_fullname_test)
{
	Magnet *mag = new Magnet("CLA-C2V-MAG-HCOR-01");
}

BOOST_AUTO_TEST_SUITE_END();