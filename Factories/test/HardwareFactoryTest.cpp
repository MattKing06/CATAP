#define BOOST_TEST_MODULE HardwareFactoryTest

#include <boost/test/unit_test.hpp>
#include "HardwareFactory.h"
//#include "Magnet.h"
#include <string>
#include <vector>

BOOST_AUTO_TEST_CASE(hardware_factory_setup_magnets)
{
	HardwareFactory *hardwareFactory = new HardwareFactory();
	//std::string hardwareType = "MAGNETS"; 
	//std::string version = "PHYSICAL";
	//hardwareFactory->setup(hardwareType, version);
	MagnetFactory *magFactory = hardwareFactory->getMagnetFactory();
	bool status;
	status = magFactory->setup("nominal");
	BOOST_CHECK(status);
	double current = magFactory->getCurrent("CLA-C2V-MAG-HCOR-01");
	BOOST_CHECK_EQUAL(current, -0.00190269);
}

