#include <EPICSArchiverHandler.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(EPICSArchiverHandlerTests)

BOOST_AUTO_TEST_CASE(test_http_connection)
{
	std::string url = "192.168.83.13";
	std::string port = "17668";
	EPICSArchiverHandler handl = EPICSArchiverHandler(url, port);
	handl.getHTTPRequest();
}


BOOST_AUTO_TEST_SUITE_END()