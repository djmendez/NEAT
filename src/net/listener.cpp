/*
 * listener.cpp
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#include <iostream>

#include <messages.h>
#include <listener.h>
#include <socket.h>
//#include <utils.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace FastEcslent;
using namespace boost::asio::ip;

void Listener::init() { //initialize sockets, public
	std::cout << "Initializing Listener before starting listener thread. port: " << port << std::endl;
	quit = false;
/*
	boost::asio::socket_base::broadcast option(true);
	socket.set_option(option);

	, udp::endpoint(udp::v4(), port));
*/
}

void Listener::init(std::string ip) {
	IPAddress = ip;
	init();
}

/*
void Listener::start() { //start thread, public
	//listenerThread = boost::thread(&Listener::run, this);
	//std::cout << "Started Net thread's run method" << listenerThread.get_id() << std::endl;
}
*/
void Listener::addMessage(Message *m) {
	//std::cout << "Listener Added message of Type: " << (int) m->head.msgType << std::endl;
	//printMessageHeader(m->head);
	recBuffer.push_back(m);
}

Message *Listener::dequeMessage(){
//	std::cout << "Listener Dequeueing message" << std::endl;
	if (recBuffer.empty()) {
		return NULL;
	} else {
		Message *m = recBuffer.front();
		recBuffer.pop_front();
		return m;
	}
}

void FastEcslent::Listener::run() { //start thread, public
	listenerThread = boost::thread(&FastEcslent::Listener::runThread, this);
	std::cout << "Running Listener thread: " << listenerThread.get_id() << std::endl;
}


void FastEcslent::Listener::runThread(){ // run the netManaager thread, private (each tick)
	std::cout << "..........................................Running listener......................................" << std::endl;
	try {

		socket = makeUDPBroadcastSocket();
		//boost::asio::socket_base::non_blocking_io command(true);
		//socket->io_control(command);

		//udp::endpoint anyEndpoint(udp::v4(), (unsigned short) port);
		//udp::endpoint any(boost::asio::ip::address_v4::any(), port);
		//std::cout << "Listener Endpoint Address: " << anyEndpoint.address().to_string() << " Port: " << anyEndpoint.port() << std::endl;
		//std::cout << "Listener Any Address: " << any.address().to_string() << " Port: " << anyEndpoint.port() << std::endl;
		//socket->bind(boost::asio::ip::address_v4::any());
		socket->bind(udp::endpoint(boost::asio::ip::address_v4::broadcast( ), port));
		std::cout << "Listener Bound Socket" << std::endl;

		udp::endpoint recFrom;
		while(!quit){
			//std::cout << "Running Listener with id: " << listenerThread.get_id() << std::endl;
			size_t len;
			Message *m = new Message;
			len = socket->receive_from(boost::asio::buffer((char *) m, MaxMessageSize), recFrom);
			if (recFrom.address().to_string() != IPAddress ) {
//				std::cout << "Listener got message with length: " << len << " from " << recFrom.address() << ",,,"<< IPAddress <<std::endl;

				//std::cout << "Type: " << (int) m->head.msgType << std::endl;
				addMessage((Message *) m);
			}else{
				delete m;
			}
		}//end while
		} catch (std::exception& e) {
			std::cerr << "Listener Exception: " << e.what() << std::endl;
			//quit = true;
			return;
		}

		std::cout << "..........................................Listener Dead......................................" << std::endl;
}

void FastEcslent::Listener::stop(){ // end thread, public
	std::cout << "Listener stopping..." << std::endl;
	boost::mutex::scoped_lock scoped_lock(quitLock);
	quit = true;
}

void FastEcslent::Listener::kill(){ // end thread, public
	std::cout << "Listener killed..." << std::endl;
	quit = true;
	listenerThread.interrupt();
}

void FastEcslent::Listener::join(){ // end thread, public
	std::cout << "Listener shutting down socket..." << quit << std::endl;
	std::flush(std::cout);
	listenerThread.join();
	try {
			boost::system::error_code ec;			//socket->cancel(ec);
			//socket->shutdown(boost::asio::ip::udp::socket::shutdown_receive, ec);
			if(socket->is_open())
				socket->close(ec);
			//socket->close();
			if (ec){
				std::cerr << "Error in closing listener socket:" << ec.message() << std::endl;
			}
		} catch (std::exception& e) {
			std::cerr << "Exception: in closing listener socket:" << e.what() << std::endl;
		}
	std::cout << "Listener Socket shutdown." << std::endl;
}


