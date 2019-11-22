#define BOOST_TEST_MODULE LoggingSystemTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "LoggingSystem.h"
#include <iostream>

BOOST_AUTO_TEST_SUITE(LoggingSystemTestSuite)
#ifndef TIME_DATE_BUFFER_SIZE
#define TIME_DATE_BUFFER_SIZE 80
#endif // TIME_DATE_BUFFER_SIZE
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

std::string getCurrentDateAndTimeString(){
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[TIME_DATE_BUFFER_SIZE];
#ifdef _WIN32
	localtime_s(&tstruct, &now);
#endif //WIN32

#if defined(__unix__) ||  defined(_unix)
	localtime_r(&now, &tstruct);
#endif //UNIX

	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d-%H%M", &tstruct);
	return buf;
}

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
		test_logging_system.printMessage("HELLO", "WORLD",1,"2",3.0);
	}
	std::string predicted_message_output_from_logging_system = "";
	BOOST_CHECK(output.is_empty(true));
	BOOST_CHECK(output.is_equal(predicted_message_output_from_logging_system));
	//output.flush();
}

BOOST_AUTO_TEST_CASE(logging_system_with_message_and_no_debug_print_message_test)
{
	LoggingSystem test_logging_system = LoggingSystem(false, true);
	//Since LoggingSystem prints messages to std::cout (generally the command line),
	//we must redirect the text in cout buffer to the output_test_stream so that 
	//boost has access to the messages printed to command line.
	boost::test_tools::output_test_stream output;
	{
		cout_redirect guard(output.rdbuf());
		test_logging_system.printMessage("HELLO", "WORLD", 1, "2", 3.6);
	}
	std::string predicted_message_output_from_logging_system = "HELLOWORLD123.6\n";
	BOOST_CHECK(output.is_equal(predicted_message_output_from_logging_system));
	output.flush();
}


BOOST_AUTO_TEST_SUITE_END()