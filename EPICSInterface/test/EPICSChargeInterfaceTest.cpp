#define BOOST_TEST_MODULE EPICSChargeInterfaceTest

#include <boost/test/unit_test.hpp>
#include <string>
#include <EPICSChargeInterface.h>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif

LoggingSystem TEST_LOGGER(true, true);

// THERE ARE NO CHARGE PVS IN VM, SO SKIP THIS FOR NOW