/*
 * Listener.h
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
using boost::asio::ip::udp;

#include <deque>


#include <messages.h>

namespace FastEcslent {

	class Engine;

	class Listener { //receive update
	private:
		udp::socket *socket;//(netService);
		udp::endpoint myIP;;
		int port;
		bool isServer;
		bool quit;
		boost::thread listenerThread;
		boost::mutex  quitLock;
		std::string IPAddress;

		void runThread();
		// helpers

	protected:


	public:
		std::deque<Message *> recBuffer; //store received messages


		Listener(udp::endpoint ip, int prt, bool isserv) {
			myIP = ip;
			port = prt;
			isServer = isserv;
		}

		void init();
		void init(std::string ip);
		void stop();
		void join();
		void run();

		void kill();

		//helpers
		//threadsafe helpers
		void addMessage(Message *);
		Message* dequeMessage();

	};

}

#endif /* LISTENER_H_ */
