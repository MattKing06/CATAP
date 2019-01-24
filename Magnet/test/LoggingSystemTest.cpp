#define BOOST_TEST_MODULE LoggingSystemTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "LoggingSystem.h"
#include <iostream>

BOOST_AUTO_TEST_SUITE(LoggingSystemTestSuite)

struct cout_redirect
{
	cout_redirect(std::streambuf * new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{}

	~cout_redirect()
	{
		std::cout.rdbuf(old);
	}
private:
	std::streambuf* old;
};

BOOST_AUTO_TEST_CASE(cout_messages_divert_to_boost_test_tools_stream_test)
{
	boost::test_tools::output_test_stream output;
	{
		cout_redirect guard(output.rdbuf());
		std::cout << "Testing 1 2 3" << std::endl;
	}
	BOOST_CHECK(output.is_equal("Testing 1 2 3\n"));
}

BOOST_AUTO_TEST_CASE(logging_system_with_no_message_and_no_debug_print_message_test)
{
	LoggingSystem test_logging_system = LoggingSystem(false, false);
	//Since LoggingSystem prints messages to std::cout (generally the command line),
	//we must redirect the text in cout buffer to the output_test_stream so that 
	//boost has access to the messages printed to command line.
	boost::test_tools::output_test_stream output;
	{
		cout_redirect guard(output.rdbuf());
		test_logging_system.printMessage(std::string("Hello"));
	}
	std::string predicted_message_output_from_logging_system = "Messages have been turned off\n";
	BOOST_CHECK(!output.is_empty(false));
	BOOST_CHECK(output.is_equal(predicted_message_output_from_logging_system));
	//output.flush();
}

BOOST_AUTO_TEST_CASE(logging_system_with_message_and_no_debug_print_message_test)
{
	//LoggingSystem test_logging_system = LoggingSystem(false, true);
	//boost::test_tools::output_test_stream output;
	//{
	//	cout_redirect guard(output.rdbuf());
	//	test_logging_system.printMessage(std::string("Test String 1"));
	//}
	//std::string predicted_output_from_logging_system = "Test String 1";
	//BOOST_CHECK(!output.is_empty());
	//BOOST_CHECK(output.is_equal(predicted_output_from_logging_system));
}

BOOST_AUTO_TEST_SUITE_END()