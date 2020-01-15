#define BOOST_TEST_MODULE EPICSChargeInterfaceTest

#include <boost/test/unit_test.hpp>
#include <string>
#include <EPICSChargeInterface.h>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif


// THERE ARE NO CHARGE PVS IN VM, SO SKIP THIS FOR NOW