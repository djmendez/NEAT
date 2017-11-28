/*
 * server.h
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <netThread.h>
#include <deque>

namespace FastEcslent {

	class Server : public NetThread {
	private:
		// deque to hold received data, processed by netMgr
		std::deque<StateMessage> sendQueue;

		// deque to hold send data - filled by netMgr
		std::deque<InputMessage> recQueue;


	public:
		void init(); // set up sender socket, set up command receiver socket
		void start(); // start thread running





	};




}

#endif /* SERVER_H_ */
