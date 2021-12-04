#include <EPICSArchiverHandler.h>

EPICSArchiverHandler::EPICSArchiverHandler()
{

}

EPICSArchiverHandler::EPICSArchiverHandler(const std::string& hostAddress) :
	address(hostAddress),
	portNumber("80")
{
}

EPICSArchiverHandler::EPICSArchiverHandler(const std::string& hostAddress, const std::string& portNumber) :
	address(hostAddress),
	portNumber(portNumber)
{
	if (portNumber.compare("80") != 0)
	{
		address = hostAddress + ":" + portNumber;
	}
}

std::string EPICSArchiverHandler::getAddress() const
{
	return address;
}

void EPICSArchiverHandler::getHTTPRequest()
{
	std::string queryStr = "/retrieval/data/getData.json?pv=CLA-C2V-MAG-HCOR-01%3AREADI&from=2021-10-26T12%3A17%3A00.000Z&to=2021-10-27T12%3A17%3A00.000Z";
	std::cout << address << std::endl;
	boost::asio::io_service io;
	boost::asio::ip::tcp::resolver rs(io);
	boost::asio::ip::tcp::resolver::query query(address, portNumber);
	std::cout << query.host_name() << std::endl;
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = rs.resolve(query);
	std::cout << endpoint_iterator->host_name() << std::endl;
	boost::asio::ip::tcp::socket socket(io);
	boost::asio::connect(socket, endpoint_iterator);


	// Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << "GET " << queryStr << " HTTP/1.1\r\n";  // note that you can change it if you wish to HTTP/1.0
	request_stream << "Host: " << address << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: close\r\n\r\n";

	boost::asio::write(socket, request);

}