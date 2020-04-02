#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "Hardware.h"
#include <vector>
#include "GlobalTypeEnums.h"

BOOST_AUTO_TEST_SUITE(HardwareTestSuite)

BOOST_AUTO_TEST_CASE(empty_hardware_test)
{
	Hardware component = Hardware();
	TYPE expectedMachineArea = TYPE::UNKNOWN_TYPE;
	TYPE expectedHardwareType = TYPE::UNKNOWN_TYPE;
	BOOST_CHECK_EQUAL(component.getHardwareType(), expectedHardwareType);
	BOOST_CHECK_EQUAL(component.getMachineArea(), expectedMachineArea);
	BOOST_CHECK_EQUAL(component.getSpecificHardwareParameters().empty(), true);
	auto pvStructMap = component.getPVStructs();	
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
