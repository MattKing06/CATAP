#include <SnapshotFileManager.h>
#include <GlobalConstants.h>
#include <GlobalFunctions.h>
#include <MagnetPVRecords.h>
#include <iostream>
namespace SnapshotFileManager
{
	// HERE THE EXPECTTATION IS IT WILL ALWAYS RETURN FILE DATA AS A YAML::NODE 
	// AND DATA TO WRITE TO A FILE SHOUDL BE PASSED AS A YAML::NODE 
	// AND THAT SHOULD BE THE END OF ANY DISCUSSION ;)
	//
	// EVERY CLASS TO DO WITH LOADING AND SAVING SNAPSHOT FILES SHOULD USE THIESE FUNCTIONS !! 
	//
	// TODO 
	// have this address as the IP and domain name and test if they work  ???
	std::string const magnet_snapshot_default_path = "\\\\claraserv3\\claranet\\Snapshots";
	std::string const defaultMachineSnapshotLocation = "\\\\claraserv3\\claranet\\Snapshots";
	const std::string snapshot_file_reference = "SNAPSHOT_FILE_REFERENCE";
	const std::vector<std::string> extensions = { ".yml", ".yaml", ".YML", ".YAML" };

	bool isFormatValid(const std::string& fileExtension)
	{
		if (std::find(SnapshotFileManager::extensions.begin(), SnapshotFileManager::extensions.end(),
			fileExtension) != SnapshotFileManager::extensions.end())
		{
			return true;
		}
		else
		{
			std::cout << "!!WARNING!! isFormatValid invalid format, passed fileExtension =  " << fileExtension << std::endl;
			return false;
		}
	}

	bool writeSnapshotToYAML(const std::string& location, const std::string& filename, const YAML::Node& outputNode, const STATE& mode)
	{
		const std::string fullpath = location + "/" + filename;
		std::ofstream outFile(fullpath.c_str());
		if (!outFile) { return false; }
		// COULD GET THE HARDWARE TYPE FROM OutputNode, NEED TO ENFORCE HARDWARE TYPE AS FIRST ELEMENT.
		outFile << "# YAML VELA/CLARA VALVE SETTINGS SAVE FILE: VERSION 1" << std::endl;
		outFile << "# THIS SNAPSHOT WAS CREATED IN: " << ENUM_TO_STRING(mode) << " MODE." << std::endl;
		outFile << outputNode << std::endl;
		return true;
	}

	boost::system::error_code createMachineSnapshotDirectory()
	{
		return boost::system::error_code();
	}

	std::vector<std::string> getAllFilesInDirectory(const std::string& dirPath, const std::vector<std::string> skipList)
	{
		std::vector<std::string> fileList;
		boost::filesystem::path path(dirPath);
		try
		{
			if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path))
			{
				boost::filesystem::directory_iterator iter(path);
				boost::filesystem::directory_iterator end;
				while (iter != end)
				{
					if (boost::filesystem::is_directory(iter->path()) &&
						(std::find(skipList.begin(), skipList.end(), iter->path().filename()) != skipList.end()))
					{
						boost::system::error_code ec;
						iter.increment(ec);
						if (ec)
						{
							std::cout << " COULD NOT ACCESS : " << iter->path().string() << std::endl;
						}
					}
					else if (isFormatValid(iter->path().extension().string()))
					{
						fileList.push_back(iter->path().string());
						boost::system::error_code ec;
						iter.increment(ec);
						if (ec)
						{
							std::cout << " COULD NOT ACCESS : " << iter->path().string() << std::endl;
						}
					}
					else
					{
						boost::system::error_code ec;
						iter.increment(ec);
						std::cout << iter->path().string() << " is in the wrong format (no yaml files found...)" << std::endl;
					}
				}
			}
		}
		catch (std::system_error& e)
		{
			std::cout << e.what() << std::endl;
		}
		return fileList;
	}

	YAML::Node readSnapshotFile(const std::string& location, const std::string& filename)
	{
		const boost::filesystem::path directory(location);
		const boost::filesystem::path file(filename);
		const boost::filesystem::path full_path = directory / file;

		if (containsDBURTExtension(full_path.extension().string()))
		{
			return DBURT2YAMLNode(full_path);
		}
		else
		{
			if ( isFormatValid(full_path.extension().string()) )
			{
				std::cout << " readSnapshotFile  LoadFile " << full_path.string() << std::endl;
				return YAML::LoadFile(full_path.string());
			}
			else
			{
				std::cout << " readSnapshotFile  isFormatValid returned false " << std::endl;
			}
		}
		YAML::Node empty_node;
		return empty_node;
	}
//		 __   __        __  ___     __  ___       ___  ___ 
//		|  \ |__) |  | |__)  |     /__`  |  |  | |__  |__  
//		|__/ |__) \__/ |  \  |     .__/  |  \__/ |    |    
//		
//		DON'T USE THSI APART FROM WHEN WE HAVE TO 
// 
	const std::vector<std::string> dburt_extensions = { GlobalConstants::dotdburt, GlobalConstants::dotDBURT};
	bool containsDBURTExtension(const std::string& fileExtension)
	{
		if (std::find(SnapshotFileManager::dburt_extensions.begin(), SnapshotFileManager::dburt_extensions.end(),
			fileExtension) != SnapshotFileManager::dburt_extensions.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	std::vector<std::string> getDBURTKeyVal(const std::string& trimmedLine, const char delim)
	{
		std::stringstream ss(trimmedLine);
		std::string item;
		std::vector<std::string> entry;
		while (std::getline(ss, item, delim))
		{
			//std::cout << "splitter adding " << item << std::endl;
			entry.push_back(item);
		}
		return entry;
	}
	std::pair<bool, std::string> isDBURTFileAlias(const std::string& full_path)
	{
		std::pair<bool, std::string> r(false, "");

		std::ifstream fileInput;
		fileInput = std::ifstream(full_path);
		if (fileInput.is_open())
		{
			std::string line, trimmedLine;

			while (std::getline(fileInput, line)) /// Go through, reading file line by line
			{
				trimmedLine = GlobalFunctions::trimAllWhiteSpace(trimToDelimiter(line, GlobalConstants::END_OF_LINE));

				if (GlobalFunctions::stringIsSubString(line, GlobalConstants::VELA_CLARA_DBURT_ALIAS_V1))
				{
					std::cout << "FOUND \"VELA_CLARA_DBURT_ALIAS_V1)\"" << std::endl;
					std::getline(fileInput, line);
					trimmedLine = GlobalFunctions::trimAllWhiteSpace(trimToDelimiter(line, GlobalConstants::END_OF_LINE));
					std::vector<std::string> keyvalue = getDBURTKeyVal(trimmedLine, GlobalConstants::EQUALS_SIGN_C);

					std::string ref_path = GlobalFunctions::trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[0]);
					std::string ref_name = GlobalFunctions::trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[1]);

					r.first = true;

					if (ref_path.back() == GlobalConstants::SLASH_SLASH_C)
					{
						r.second = ref_path + ref_name;
					}
					else if (ref_path.back() != GlobalConstants::SLASH_SLASH_C)
					{
						r.second = ref_path + GlobalConstants::SLASH_SLASH + ref_name;
					}
					return r;
				}

			}
		}
		return r;
	}
	YAML::Node DBURT2YAMLNode(const boost::filesystem::path& full_path)
	{
		std::cout << "DBURT2YAMLNode, is checking for an alias file " << std::endl;
		std::pair<bool, std::string> alias_file = isDBURTFileAlias(full_path.string());
		if (alias_file.first)
		{
			std::cout << "Alias file points to: " << alias_file.second << std::endl;
			return DBURT2YAMLNode(alias_file.second);
		}
		std::cout << "Reading file data into yaml node " << std::endl;
		YAML::Node node;
		//node["MAGNET"] = YAML::Null; //TODO what is the canonical name for this hardware??  it should be defined somewhere!
		node["MAGNET"] = YAML::Null; //TODO what is the canonical name for this hardware??  it should be defined somewhere!
		std::string line;
		std::string trimmedLine;
		std::ifstream inputFile;
		int configVersion = -1;
		inputFile.open(full_path.string(), std::ios::in);
		if (inputFile)
		{
			//std::cout << "File Opened from " << full_path.string();
			while (std::getline(inputFile, line)) /// Go through, reading file line by line
			{
				trimmedLine = GlobalFunctions::trimAllWhiteSpace(trimToDelimiter(line, GlobalConstants::END_OF_LINE));

				if (GlobalFunctions::stringIsSubString(line, GlobalConstants::VELA_MAGNET_SAVE_FILE_v1))
				{
					configVersion = 1;
					break;
				}
				else if (GlobalFunctions::stringIsSubString(line, GlobalConstants::DBURT_HEADER_V3))
				{
					configVersion = 3;// version 2 got lost around October 2015
					break;
				}
				else if (GlobalFunctions::stringIsSubString(line, GlobalConstants::DBURT_HEADER_V4))
				{
					configVersion = 4;// version 2 got lost around October 2015
					break;
				}
			}
		}
		std::cout << "Finished Pre-Processing File" << std::endl;
		inputFile.close();

		std::vector<std::string> keyvalue;
		bool readingParameters ;
		int  linenumber;
		size_t file_num_objects = 0;
		size_t found_num_objects = 0;
//		magnetStructs::magnetStateStruct magState;
		switch (configVersion)
		{
		case -1:
			std::cout << "!!FILE ERROR!! NO DBURT VERSION FOUND, EXIT" << std::endl;
			break;

		case 1:
			std::cout << "VERSION 1 DBURT FOUND, FILE TOO OLD, NOT SUPPORTED ANYMORE" << std::endl;
			break;
		case 2:
			std::cout << "VERSION 2 DBURT FOUND, FILE TOO OLD, NOT SUPPORTED ANYMORE" << std::endl;
			break;
		case 3:
			std::cout << "VERSION 3 DBURT FOUND, FILE TOO OLD, NOT SUPPORTED ANYMORE" << std::endl;
			break;
		case 4:
		{
			std::cout << "VERSION 4 DBURT FOUND, CONTINUING" << std::endl;

			std::string line, trimmedLine;
			std::ifstream inputFile;

			std::vector<std::string> keyvalue;



			configVersion = -1;
			inputFile.open(full_path.string(), std::ios::in);
			if (inputFile)
			{
				readingParameters = false;
				linenumber = 0;

				while (std::getline(inputFile, line)) /// Go through line by line
				{
					std::stringstream iss(line); /// make a stream of the line and then do some tests
					++linenumber;

					std::cout << "Next line:" << std::endl;
					std::cout << iss.str() << std::endl;

					if (GlobalFunctions::stringIsSubString(iss.str(), GlobalConstants::DBURT_EOF_V4))
					{
						std::cout << "FOUND END OF FILE" << std::endl;
						readingParameters = false;
						break;
					}
					if (readingParameters)
					{
						trimmedLine = GlobalFunctions::trimAllWhiteSpace(trimToDelimiter(line, GlobalConstants::END_OF_LINE));

						std::cout << "readingParameters: " << trimmedLine << std::endl;

						std::vector<std::string> keyvalue = getDBURTKeyVal(trimmedLine, GlobalConstants::COLON_C);

						std::cout << "keyvalue.size(): " << keyvalue.size() << std::endl;

						if (keyvalue.size() == 4)
						{

							std::cout << "FOUND " << keyvalue[0] << ", PSU state = " << keyvalue[1] <<
								", SETI = " << keyvalue[2] <<
								", READI = " << keyvalue[3] << std::endl;

							//YAML::Node mag_snap_node;
							//mag_snap_node[MagnetRecords::RPOWER] = keyvalue[1];
							//mag_snap_node[MagnetRecords::SETI] = keyvalue[2];
							//mag_snap_node[MagnetRecords::READI] = keyvalue[3];

							node["MAGNET"][keyvalue[0]][MagnetRecords::RPOWER] = keyvalue[1];
							node["MAGNET"][keyvalue[0]][MagnetRecords::SETI] = keyvalue[2];
							node["MAGNET"][keyvalue[0]][MagnetRecords::READI] = keyvalue[3];
								
								
							//	.push_back(mag_snap_node);
							//node["MAGNET"][keyvalue[0]].push_back(mag_snap_node);

							found_num_objects += 1;

						}
						else if (keyvalue.size() == 2)
						{
							if (keyvalue[0] == "NUMBER_OF_OBJECTS")
							{
								file_num_objects = size_t(std::stoi(keyvalue[1]));
							}
							std::cout << "file has " << file_num_objects << " objects " << std::endl;
						}
					}
					if (GlobalFunctions::stringIsSubString(iss.str(), "START_OF_DATA"))
					{
						std::cout << "FOUND START_OF_DATA" << std::endl;
						readingParameters = true;
					}



				} // if(readingParameters)_END
			} // while
			if (file_num_objects != found_num_objects)
			{
				std::cout << "!!ERROR!! Failed Object Count Sanity check: " << file_num_objects << " (expected) != " << found_num_objects << "(found)" << std::endl;
			}
			//magState = readDBURTv4(pathandfile);
			break;

		}

		default:
			std::cout << "UNEXPECTED DBURT VERSION, " << std::endl;
		}
		std::cout << "DBURT2YAMLNode complete returning yaml-node with " << found_num_objects << " objects." << std::endl;


		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << node << std::endl;

		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		std::cout << node["MAGNET"]["S02-VCOR3"] << std::endl;
		std::cout << node["MAGNET"]["S02-VCOR3"]["RPOWER"] << std::endl;

		return node;
	}
	// for legacy DBURTS, don't use execpt for in the DBURT stuff 
	std::string trimToDelimiter(std::string const& str, const std::string& STOPDELIMITER) 
	{
		size_t end1 = str.find_first_of("#");
		std::string str2 = str.substr(0, end1);
		size_t last = str2.find_first_of(";");
		return str2.substr(0, last);
	}

}