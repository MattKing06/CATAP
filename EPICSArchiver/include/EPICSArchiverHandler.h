#ifndef EPICS_ARCHIVER_HANDLER_H_
#define EPICS_ARCHIVER_HANDLER_H_

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <string>
#include <iostream>
#include <istream>
#include <ostream>

class EPICSArchiverHandler
{
public:
	EPICSArchiverHandler();
	EPICSArchiverHandler(const std::string& hostAddress);
	EPICSArchiverHandler(const std::string& hostAddress, const std::string& portNumber);
	std::string getAddress() const;
	void getHTTPRequest();
private:
	std::string address;
	std::string portNumber;

};


#endif //EPICS_ARCHIVER_HANDLER_H_