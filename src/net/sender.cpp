/*
 * sender.cpp
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#include <messages.h>
#include <sender.h>
#include <socket.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include<iostream>

using namespace FastEcslent;
using namespace boost::asio::ip;

void FastEcslent::Sender::init() { //initialize sockets, public
	std::cout << "Initializing Sender before starting sender thread. port: " << port << std::endl;
	quit = false;
	SERVER_SLEEP_TIME = 470;
	CLIENT_SLEEP_TIME = 70;
	if(isServer)
		sleepTime = new boost::posix_time::milliseconds(SERVER_SLEEP_TIME);
	else
		sleepTime = new boost::posix_time::milliseconds(CLIENT_SLEEP_TIME);
}


void FastEcslent::Sender::run() { //start thread, public
	senderThread = boost::thread(&FastEcslent::Sender::runThread, this);
	std::cout << "Started Sender thread's run method" << senderThread.get_id() << std::endl;
}

void FastEcslent::Sender::addMessage(Message *m) {
	//std::cout << "Sender Added message of Type: " << (int) m->head.msgType << std::endl;
	sendBuffer.push_back(m);
}

void FastEcslent::Sender::addPriorityMessage(Message *m) {
	//std::cout << "Sender Added message of Type: " << (int) m->head.msgType << std::endl;
	sendBuffer.push_front(m);
}

Message *Sender::dequeMessage(){

	if (sendBuffer.empty()) {
		//std::cout << "Sender Dequeueing NULL message" << std::endl;
		return NULL;
	} else {
		//std::cout << "Sender Dequeueing FULL message" << std::endl;
		Message *m = sendBuffer.front();
		sendBuffer.pop_front();
		return m;
	}
}

void FastEcslent::Sender::runThread(){ // run the netManaager thread, private (each tick)

	try {
		socket = makeUDPBroadcastSocket();
		//socket->bind(udp::endpoint(boost::asio::ip::address_v4::broadcast( ), port));
		std::cout << "Sender Opened Broadcast Socket" << std::endl;
		//udp::endpoint broadcastEndpoint(udp::v4(), port);
		udp::endpoint broadcastEndpoint(udp::endpoint(boost::asio::ip::address_v4::broadcast( ), port));
		//socket->bind(broadcastEndpoint);
		int size;
		while(!quit){

			size_t len;
			Message *m = dequeMessage();
			//std::cout << "Sender sending message at time: " << m->head.millisecondsFromStart << std::endl;
			while (m) {
				size = (m->head.numberOfStructs * m->head.sizeOfStruct) + HeaderSize;

				len = socket->send_to(boost::asio::buffer((const void *) m, size), broadcastEndpoint);

				//std::cout << "Sender sent message with len: " << len << " and with size: " << size <<std::endl;
				delete m;
				m = dequeMessage();
			}
			boost::this_thread::sleep(*sleepTime);
		}//end while
		} catch (std::exception& e) {
			std::cerr << "Sender exception: " << e.what() << std::endl;
			quit = true;
		}
		std::cout << "..........................................Sender Dead......................................" << std::endl;
}

void FastEcslent::Sender::stop(){ // end thread, public
	std::cout << "Sender: Stopping.....:" << senderThread.get_id() << std::endl;
	boost::mutex::scoped_lock scoped_lock(quitLock);
	quit = true;
}

void FastEcslent::Sender::kill(){ // end thread, public
	std::cout << "Sender: Killed.....:" << std::endl;
	quit = true;
	senderThread.interrupt();
}

void FastEcslent::Sender::join(){ //wait for thread to die, called by thread creator, public
	std::cout << "Sender: Shutting down socket" << std::endl;
	boost::system::error_code ec;
	try {
		//socket->shutdown(udp::socket::shutdown_both);
		socket->close();
		//socket->close(ec);
		std::cout << "Sender STOPPING, Sender socket closing" << std::endl;
		if (ec){
			std::cerr << "Error in closing Sender socket:" << ec.message() << std::endl;
		}
	} catch (std::exception& e) {
		std::cerr << "Exception: in closing Sender socket:" << e.what() << std::endl;
	}
	std::cout << "Sender: Socket has been shutdown...waiting for join().....:" << std::endl;
	std::flush(std::cout);
	senderThread.join();

}
