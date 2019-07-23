#define BOOST_TEST_MODULE ConfigReaderTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "ConfigReader.h"
#include <vector>

#if defined(__unix__) ||  defined(_unix)
	static const std::string HOME = getenv("HOME");
	static const std::string MASTER_LATTICE_FILE_LOCATION = HOME + "/MasterLattice";
	static const std::string SEPARATOR = "/";
#endif
#ifdef _WIN32
	static const std::string MASTER_LATTICE_FILE_LOCATION = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	static const std::string SEPARATOR = "\\";
#endif

BOOST_AUTO_TEST_SUITE(ConfigReaderTestSuite)
//BOOST_AUTO_TEST_CASE(config_reader_parsing_not_found_yaml_document)
//{
//	ConfigReader reader("C2V_FILE_DOESNT_EXIST.yaml");
//	//YAML lib will throw a BadFile exception, but we catch it and deal with it by printing a user-friendly message
//	BOOST_CHECK_NO_THROW(std::string result = reader.parseYamlFile());
//}
////BOOST_AUTO_TEST_CASE(config_reader_parsing_yaml_document_for_shutter_pv)
////{
//	//ConfigReader reader("C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\CLA_BA1.yaml");
//	//std::vector<std::string> *expectedShutterPVs = new std::vector<std::string>();
//	//expectedShutterPVs->push_back("EBT-LSR-SHUT-02:Sta");
//	//expectedShutterPVs->push_back("EBT-LSR-SHUT-02:On");
//	//expectedShutterPVs->push_back("EBT-LSR-SHUT-02:Off");
//	//reader.parseYamlFile();
//	//BOOST_CHECK(expectedShutterPVs->size() > 0);
//	//BOOST_TEST_MESSAGE("Expected Shutter PV vector initialised");
//	////BOOST_TEST_MESSAGE("Contents: " << expectedShutterPVs->at(0) << "	" << expectedShutterPVs->at(1) << "		" << expectedShutterPVs->at(2));
//	//BOOST_CHECK_EQUAL_COLLECTIONS(reader.PVs->begin(), reader.PVs->end(), expectedShutterPVs->begin(), expectedShutterPVs->end());
////}
//
//BOOST_AUTO_TEST_CASE(config_reader_parsing_empty_yaml_document)
//{
//	ConfigReader reader("C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\EmptyYamlFile.yaml");
//	//We will throw a std::length_error exception, but we catch it and deal with it by printing a user-friendly message
//	BOOST_CHECK_NO_THROW(std::string result = reader.parseYamlFile());
//}

	BOOST_AUTO_TEST_CASE(config_reader_parsing_file_not_found_test)
	{
		std::string undefinedHardwareType = "UNDEFINED";
		std::string undefinedFilename = "UNDEFINED";
		ConfigReader reader(undefinedFilename, false);
		std::map<std::string, std::string> returnedParams = reader.parseYamlFile();
		BOOST_CHECK_EQUAL(returnedParams.size(), 0);
	}

	BOOST_AUTO_TEST_CASE(config_reader_parsing_master_lattice_yaml_document_test)
	{
		std::string hardwareType = "Magnet";
		std::string filename = "CLA-C2V-MAG-HCOR-01";
		ConfigReader reader(filename,false);
		std::map<std::string, std::string> parameters = reader.parseYamlFile();
		std::cout << "map name: " << parameters.find("name")->second << std::endl;
		std::cout << "filename: " << filename << std::endl;
		BOOST_CHECK(parameters.find("name")->second.data() == filename);
	
	}

//BOOST_AUTO_TEST_CASE(config_reader_parsing_yaml_document)
//{
//	ConfigReader reader("C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\C2V.yaml");
//	BOOST_CHECK(reader.parseYamlFile());
//}
BOOST_AUTO_TEST_SUITE_END()