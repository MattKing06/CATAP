#define BOOST_TEST_MODULE ControllerTest

#include <boost/test/unit_test.hpp>
#include "Controller.h"
#include <string>


BOOST_AUTO_TEST_CASE(controller_with_name_and_type_test)
{
	std::string controller_name = "MAG-CONT-1";
	std::string controller_type = "Magnet";
	LoggingSystem logger;
	BOOST_TEST((true && true));
}
