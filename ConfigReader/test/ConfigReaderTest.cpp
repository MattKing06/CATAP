#define BOOST_TEST_MODULE ConfigReaderTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "ConfigReader.h"

BOOST_AUTO_TEST_SUITE(ConfigReaderTestSuite)
BOOST_AUTO_TEST_CASE(config_reader_parsing_not_found_yaml_document)
{
	ConfigReader reader("C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\C2V_FILE_DOESNT_EXIST.yaml");
	//YAML lib will throw a BadFile exception, but we catch it and deal with it by printing a user-friendly message
	BOOST_CHECK_NO_THROW(reader.parseYamlFile());
}
BOOST_AUTO_TEST_CASE(config_reader_parsing_empty_yaml_document)
{
	ConfigReader reader("C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\EmptyYamlFile.yaml");
	//We will throw a std::length_error exception, but we catch it and deal with it by printing a user-friendly message
	BOOST_CHECK_NO_THROW(reader.parseYamlFile());
}
BOOST_AUTO_TEST_CASE(config_reader_parsing_yaml_document)
{
	ConfigReader reader("C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\C2V.yaml");
	BOOST_CHECK(reader.parseYamlFile());
}
BOOST_AUTO_TEST_SUITE_END()