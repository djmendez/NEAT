/*
 * socket.h
 *
 *  Created on: Jan 12, 2012
 *      Author: sushil
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <boost/asio.hpp>

namespace FastEcslent {

boost::asio::ip::udp::socket* makeUDPBroadcastSocket();

}


#endif /* SOCKET_H_ */
