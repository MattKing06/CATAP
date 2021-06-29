#include <SnapshotFileManager.h>
#include <GlobalConstants.h>
#include <GlobalFunctions.h>
#include <MagnetPVRecords.h>
#include <iostream>
namespace SnapshotFileManager
{
	std::string const defaultMachineSnapshotLocation = "\\\\claraserv3\\claranet\\MachineSnapshots";
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



//		 __   __        __  ___     __  ___       ___  ___ 
//		|  \ |__) |  | |__)  |     /__`  |  |  | |__  |__  
//		|__/ |__) \__/ |  \  |     .__/  |  \__/ |    |    
//		
	const std::vector<std::string> dburt_extensions = { GlobalConstants::dotdburt, GlobalConstants::dotDBURT};
	bool containsDBURTExtension(const std::string& str)
	{
		for (auto&& item : dburt_extensions)
		{
			if (GlobalFunctions::stringIsSubString(str, item))
			{
				return true;
			}

		}
		return false;
	}

	std::vector<std::string> getDBURTKeyVal(const std::string& trimmedLine, const char delim)
	{
		std::stringstream ss(trimmedLine);
		std::string item;
		std::vector<std::string> entry;
		while (std::getline(ss, item, delim))
			entry.push_back(item);
		return entry;
	}

	std::pair<bool, std::string> isDBURTFileAlias(const std::string& full_path)
	{
		//std::pair<bool, std::string> MagnetFactory::isDBURTFileAlias(const std::string& full_path)const
		//{
			//messenger.printDebugMessage("isDBURTFileAlias checking file: ", full_path);
			//YAML::Parser parser(fileInput);
			//YAML::Node file_data = YAML::LoadFile(full_path);

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
		std::pair<bool, std::string> alias_file = isDBURTFileAlias(full_path.string());
		if (alias_file.first)
		{
			return DBURT2YAMLNode(alias_file.second);
		}

		YAML::Node node;

		node["MAGNET"] = YAML::Null;

		std::string line;
		std::string trimmedLine;
		std::ifstream inputFile;
		int configVersion = -1;
		inputFile.open(full_path.string(), std::ios::in);
		if (inputFile)
		{
			std::cout << "File Opened from " << full_path.string();
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
				else if (GlobalFunctions::stringIsSubString(line, GlobalConstants::VELA_CLARA_DBURT_ALIAS_V1))
				{
					//std::getline(inputFile, line);
					//trimmedLine = GlobalFunctions::trimAllWhiteSpace(trimToDelimiter(line, GlobalConstants::END_OF_LINE));
					//std::vector<std::string> keyvalue = getKeyVal(trimmedLine, GlobalConstants::EQUALS_SIGN_C);
					//pathandfile = getFilePathFromINputs(trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[0]),
					//	trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[1]));
					//message(pathandfile);
					//configVersion = 4;
				}
			}
		}
		std::cout << "Finished preprocessing file" << std::endl;
		inputFile.close();

		std::vector<std::string> keyvalue;
		bool readingParameters ;
		int  linenumber;

//		magnetStructs::magnetStateStruct magState;
		switch (configVersion)
		{
		case -1:
			std::cout << "NO DBURT VERSION FOUND, EXIT" << std::endl;
			break;

		case 1:
			std::cout << "VERSION 1 DBURT FOUND, FILE TOO OLD NOT SUPPORTED ANYMORE" << std::endl;
			break;
		case 2:
			std::cout << "VERSION 2 DBURT FOUND, FILE TOO OLD NOT SUPPORTED ANYMORE" << std::endl;
			break;
		case 3:
			std::cout << "VERSION 3 DBURT FOUND, FILE TOO OLD NOT SUPPORTED ANYMORE" << std::endl;
			break;
		case 4:
		{
			std::cout << "VERSION 4 DBURT FOUND" << std::endl;

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
					if (GlobalFunctions::stringIsSubString(iss.str(), GlobalConstants::DBURT_EOF_V4))
					{
						std::cout << "FOUND END OF FILE" << std::endl;
						readingParameters = false;
						break;
					}
					if (readingParameters)
					{
						trimmedLine = GlobalFunctions::trimAllWhiteSpace(trimToDelimiter(line, GlobalConstants::END_OF_LINE));

						std::vector<std::string> keyvalue = getDBURTKeyVal(trimmedLine, GlobalConstants::COLON_C);

						if (keyvalue.size() == 4)
						{

							std::cout << "FOUND " << keyvalue[0] << ", psu state = " << keyvalue[1] <<
								", SI = " << keyvalue[2] <<
								", RI = " << keyvalue[3] << std::endl;

							YAML::Node mag_snap_node;

							mag_snap_node[MagnetRecords::RPOWER] = keyvalue[1];
							mag_snap_node[MagnetRecords::SETI] = keyvalue[2];
							mag_snap_node[MagnetRecords::READI] = keyvalue[3];


							node["MAGNET"][keyvalue[0]].push_back(mag_snap_node);

						}
					} // if(readingParameters)_END
				} // while
			//magState = readDBURTv4(pathandfile);
				break;

			}
		}
		default:
			std::cout << "UNEXPECTED DBURT VERSION, " << std::endl;
		}
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