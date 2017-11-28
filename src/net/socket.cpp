/*
 * socket.cpp
 *
 *  Created on: Jan 12, 2012
 *      Author: sushil
 */
#include <iostream>
#include <boost/asio.hpp>

#include <socket.h>
#include <DEBUG.h>

using namespace FastEcslent;


boost::asio::ip::udp::socket* FastEcslent::makeUDPBroadcastSocket (){

		boost::asio::io_service netService;
		/*
		udp::endpoint anyEndpoint(udp::v4(), (unsigned short) port);
		udp::endpoint any(boost::asio::ip::address_v4::any(), port);
		udp::endpoint recFrom;
		//std::cout << "Listener Endpoint Address: " << anyEndpoint.address().to_string() << " Port: " << anyEndpoint.port() << std::endl;
		//std::cout << "Listener Any Address: " << any.address().to_string() << " Port: " << anyEndpoint.port() << std::endl;

		 */

		boost::asio::ip::udp::socket* socket;
		try {
			socket = new boost::asio::ip::udp::socket(netService);
			socket->open(boost::asio::ip::udp::v4());
			DEBUG(std::cout << "Opened Socket" << std::endl;)

			boost::asio::socket_base::reuse_address option1(true);
			socket->set_option(option1);
			boost::asio::socket_base::broadcast option2(true);
			socket->set_option(option2);

			boost::asio::socket_base::receive_buffer_size option(65536);
			socket->set_option(option);
			boost::asio::socket_base::receive_buffer_size option3;
			socket->get_option(option3);


			DEBUG(std::cout << "Socket receive buffer size: " << option3.value() << std::endl;)
		} catch (std::exception &e) {
			DEBUG(std::cerr << "Exception in creating socket: " << e.what() << std::endl;)
		}
		return socket;
}


