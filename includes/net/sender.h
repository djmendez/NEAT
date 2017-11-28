/*
 * sender.h
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
using boost::asio::ip::udp;

#include <deque>

#include <messages.h>

namespace FastEcslent {

	class Engine;

	class Sender { //receive update
	private:

		boost::thread senderThread;
		udp::socket   *socket;//(netService);
		udp::endpoint myIP;;
		int           port;
		bool          isServer;
		void runThread();
		bool quit;

		int SERVER_SLEEP_TIME;
		int CLIENT_SLEEP_TIME;
		boost::posix_time::milliseconds *sleepTime;

		boost::mutex                     quitLock;
		// helpers

		//ThreadTester *gimPtr;

	public:
		std::deque<Message *> sendBuffer;
		Sender(udp::endpoint ip, int prt, bool isserv){
			myIP = ip;
			port = prt;
			isServer = isserv;
		}
		void init();
		void run();
		void join();
		void stop();
		void kill();

		//helpers
		//threadsafe helpers
		void addMessage(Message *);
		void addPriorityMessage(Message *);
		Message* dequeMessage();
	};

}

#endif /* SENDER_H_ */
