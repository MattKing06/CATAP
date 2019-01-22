#define BOOST_TEST_MODULE ControllerTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "Controller.h"
#include "LoggingSystem.h"
#include <string>

BOOST_AUTO_TEST_SUITE(ControllerTestSuite)


BOOST_AUTO_TEST_CASE (controller_with_name_and_type_test)
{
	Controller test_controller = Controller("Magnet", "MAG-CONST-1");
	BOOST_CHECK_EQUAL(test_controller.controller_type, "Magnet");
	BOOST_CHECK_EQUAL(test_controller.controller_name, "MAG-CONST-1");
	BOOST_CHECK_EQUAL(test_controller.controller_messenger.isMessagingOn(), true);
	BOOST_CHECK_EQUAL(test_controller.controller_messenger.isDebugOn(), true);
}

BOOST_AUTO_TEST_SUITE_END()
/*
boost::unit_test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
	
	Controller ControllerList[3];
	ControllerList[0] = Controller();
	ControllerList[1] = Controller("Magnet", "MAG-CONST-1");
	ControllerList[2] = Controller("Magnet", "MAG-CONST-1", LoggingSystem(true, true));

	boost::unit_test::test_suite* controller_test_suite = BOOST_TEST_SUITE("Controller Test Suite");
	controller_test_suite->add(BOOST_TEST_CASE(&controller_with_name_and_type_test));
	boost::unit_test::framework::master_test_suite().add(controller_test_suite);
	return 0;
}
*/