/*
* netCommon.cpp
*
*  Created on: Sep 22, 2013
*      Author: simingl
*/

#include "netCommon.h"
#include "utils.h"

using namespace boost::posix_time;
using boost::asio::ip::udp;

namespace FastEcslent{

NetCommon::NetCommon(){
	this->startTime = getCurrentTime();
}
std::pair<boost::asio::ip::udp::endpoint, std::string> NetCommon::getMyIP(){
	udp::endpoint me;
	std::string ipString;
	try {
		boost::asio::io_service netService;
		udp::resolver   resolver(netService);
		udp::resolver::query query(udp::v4(), boost::asio::ip::host_name(), "");
		udp::resolver::iterator endpoints = resolver.resolve(query);
		std::cout << "Got endPoint:" << boost::asio::ip::host_name() << std::endl;
		udp::endpoint ep = *endpoints;
		std::cout << "Address: " << ep.address() << std::endl;
		std::cout << "IP Address:" << boost::asio::ip::address_v4().to_string() << std::endl;

		if (ep.address().to_string().substr(0,3) != "127"){
			me = ep;
			ipString = boost::asio::ip::address_v4().to_string();
		}
	} catch (std::exception& e){
		std::cerr << "getMyIP could not deal with socket. Exception: " << e.what() << std::endl;
	}
	try {
		boost::asio::io_service netService;
		udp::resolver   resolver(netService);
		udp::resolver::query query(udp::v4(), "google.com", "");
		udp::resolver::iterator endpoints = resolver.resolve(query);
		udp::endpoint ep = *endpoints;
		udp::socket socket(netService);
		socket.connect(ep);
		boost::asio::ip::address addr = socket.local_endpoint().address();
		std::cout << "My IP according to google is: " << addr.to_string() << std::endl;
		me =  ep;
		ipString = addr.to_string();
	} catch (std::exception& e){
		std::cerr << "getMyIP could not deal with socket. Exception: " << e.what() << std::endl;
	}
	return std::pair<boost::asio::ip::udp::endpoint, std::string>(me, ipString);
}

long NetCommon::getCurrentTimeLong(){
	return (getCurrentTime() - startTime).total_milliseconds();
}

NetCommon::~NetCommon() {
}

}

