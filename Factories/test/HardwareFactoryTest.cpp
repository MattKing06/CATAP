#define BOOST_TEST_MODULE HardwareFactoryTest

#include <boost/test/unit_test.hpp>
#include "HardwareFactory.h"
//#include "Magnet.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>
/*********************************************************/
/**** TEST WARNING ** TEST WARNING ** TEST WARNING ** ****/
/*********************************************************/
/*I DON'T THINK SETTING ENV VARIABLES FROM HERE WORKS WITHOUT
OWNING AN ADMIN ACCOUNT, THEY ONLY SET ACCOUNT VARIABLES, NOT
SYSTEM-WIDE VARIABLES. TO RUN TEST WE MUST COMMENT OUT THE
REAL-MACHINE TEST, AND CHANGE OUR IP-ADDRESS AND SERVER PORT
TO THE VALUES DEFINED BELOW. THEN THE TEST SHOULD WORK!*/
/*********************************************************/
/**** TEST WARNING ** TEST WARNING ** TEST WARNING ** ****/
/*********************************************************/


//BOOST_AUTO_TEST_CASE(hardware_factory_setup_magnets)
//{
//	char* EPICS_CA_ADDR_LIST_ENV = "EPICS_CA_ADDR_LIST=192.168.83.255";
//	char* EPICS_CA_SERVER_ENV = "EPICS_CA_SERVER_PORT=";
//	int addrStatus = _putenv(EPICS_CA_ADDR_LIST_ENV);
//	std::cout << "USING IP ADDRESS: " << std::getenv("EPICS_CA_ADDR_LIST") << std::endl;
//	// check to make sure that server port is not set for talking to real machine.
//	char* portPath = std::getenv("EPICS_CA_SERVER_PORT");
//	if (portPath != NULL)
//	{
//		std::cout << "EPICS_CA_SERVER_PORT WAS = " << portPath << std::endl;
//		int envStatus = _putenv(EPICS_CA_SERVER_ENV);
//		if (std::getenv(EPICS_CA_SERVER_ENV) == NULL)
//		{
//			std::cout << "EPICS_CA_SERVER_PORT is now NULL" << std::endl;
//		}
//	}
//	HardwareFactory *hardwareFactory = new HardwareFactory();
//	//std::string hardwareType = "MAGNETS"; 
//	//std::string version = "PHYSICAL";
//	//hardwareFactory->setup(hardwareType, version);
//	MagnetFactory *magFactory = hardwareFactory->getMagnetFactory();
//	bool status;
//	status = magFactory->setup("nominal");
//	BOOST_CHECK(status);
//	double current = magFactory->getCurrent("CLA-C2V-MAG-HCOR-01");
//	BOOST_CHECK_EQUAL(current, -0.00190269);
//}
BOOST_AUTO_TEST_CASE(hardware_factory_setup_virtual_magnets)
{
	char* EPICS_CA_ADDR_LIST_ENV = "EPICS_CA_ADDR_LIST=192.168.83.246";
	char* EPICS_CA_SERVER_ENV = "EPICS_CA_SERVER_PORT=6000";
	int envStatus = putenv(EPICS_CA_ADDR_LIST_ENV);
	envStatus = putenv(EPICS_CA_SERVER_ENV);
	std::cout << "USING IP ADDRESS: " << std::getenv("EPICS_CA_ADDR_LIST") << std::endl;
	std::cout << "USING PORT: " << std::getenv("EPICS_CA_SERVER_PORT") << std::endl;
	HardwareFactory hardwareFactory(true);
	bool status;
	status = hardwareFactory.setup("Magnet","nominal");
	MagnetFactory& magFactory = hardwareFactory.getMagnetFactory();
	BOOST_CHECK(status);
	srand(time(NULL));
	double currentToSet = rand() % 10 + 1.0;
	magFactory.setCurrent("VM-CLA-C2V-MAG-HCOR-01", currentToSet);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	magFactory.setCurrent("VM-CLA-C2V-MAG-VCOR-01", currentToSet);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	magFactory.magnetMap;
	BOOST_CHECK_EQUAL(magFactory.getCurrent("VM-CLA-C2V-MAG-HCOR-01"), currentToSet);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	BOOST_CHECK_EQUAL(magFactory.getCurrent("VM-CLA-C2V-MAG-VCOR-01"), currentToSet);
}

