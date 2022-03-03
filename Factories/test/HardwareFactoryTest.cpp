#include <boost/test/unit_test.hpp>
#include <HardwareFactory.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

/*********************************************************/
/**** TEST WARNING ** TEST WARNING ** TEST WARNING ** ****/
/*********************************************************/
/*I DOn'T THINK SETTING ENV VARIABLES FROM HERE WORKS WITHOUT
OWNING AN ADMIN ACCOUNT, THEY OnLY SET ACCOUNT VARIABLES, NOT
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
//	//std::string VERSION = "PHYSICAL";
//	//hardwareFactory->setup(hardwareType, VERSION);
//	MagnetFactory *magFactory = hardwareFactory->getMagnetFactory();
//	bool status;
//	status = magFactory->setup("nominal");
//	BOOST_CHECK(status);
//	double current = magFactory->getCurrent("CLA-C2V-MAG-HCOR-01");
//	BOOST_CHECK_EQUAL(current, -0.00190269);
//}


BOOST_AUTO_TEST_SUITE(HardwareFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_test)
{
	BOOST_TEST_MESSAGE("------	RUNNING HARDWARE FACTORY TESTS	------");
}
BOOST_AUTO_TEST_CASE(hardware_factory_setup_virtual_magnets)
{
	HardwareFactory hardwareFactory(STATE::VIRTUAL);
	//status = hardwareFactory.setup("Magnet", "nominal");
	MagnetFactory& magFactory = hardwareFactory.getMagnetFactory();
	BOOST_TEST_MESSAGE("RETRIEVED MAGNET FACTORY");
	Magnet& HCOR = magFactory.getMagnet("CLA-C2V-MAG-HCOR-01");
	BOOST_TEST_MESSAGE("RETRIEVED MAGNET " + HCOR.getHardwareName());
	BOOST_CHECK(HCOR.getHardwareName() == "CLA-C2V-MAG-HCOR-01");
}

BOOST_AUTO_TEST_CASE(hardware_factory_messenger_cascade)
{
	HardwareFactory hardwareFactory(STATE::VIRTUAL);
	hardwareFactory.makeSilent();
	CameraFactory camFac = hardwareFactory.getCameraFactory();
	hardwareFactory.debugMessagesOn();
}

BOOST_AUTO_TEST_CASE(setting_up_camera_factory_print_names)
{
	HardwareFactory hardwareFactory(STATE::PHYSICAL);
	hardwareFactory.debugMessagesOff();
	hardwareFactory.messagesOff();
	CameraFactory& camFactory = hardwareFactory.getCameraFactory();

	for (auto&& item : camFactory.getCameraNames())
	{
		camFactory.messenger.printDebugMessage("Camera Object name : " + item);
	}
}


BOOST_AUTO_TEST_CASE(load_machine_snapshot_from_hardware_factory_test)
{
	//HardwareFactory HF = HardwareFactory(STATE::VIRTUAL);
	//bool status = HF.setup("Valve", "nominal");
	//if (status)
	//{
	//	bool saved = HF.saveMachineSnapshot(".\\MachineSnapshots\\");
	//	bool loaded = HF.loadMachineSnapshot(".\\MachineSnapshots\\10-05-2021 13-05-00");
	//	BOOST_CHECK(loaded);
	//}
}

//BOOST_AUTO_TEST_CASE(setting_up_screen_factory_test)
//{
//	//  HardwareFactoryTest/setting_up_screen_factory_test
//	HardwareFactory hardwareFactory(STATE::PHYSICAL);
//	hardwareFactory.debugMessagesOff();
//	hardwareFactory.messagesOff();
//	ScreenFactory fac = hardwareFactory.getScreenFactory();
//}


BOOST_AUTO_TEST_SUITE_END();