#define BOOST_TEST_MODULE HardwareFactoryTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "HardwareFactory.h"


#include <string>
BOOST_AUTO_TEST_SUITE(HardwareFactoryTestSuite)
BOOST_AUTO_TEST_CASE(hardware_factory_setup_magnets)
{
	HardwareFactory *hardwareFactory = new HardwareFactory();
	std::string hardwareType = "MAGNETS"; 
	std::string version = "PHYSICAL";
	hardwareFactory->setup(hardwareType, version);
	Magnet *mag = hardwareFactory->getMagnet("CLA-C2V-MAG-HCOR-01");
	std::cout << mag->getFullPSUName() << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()
