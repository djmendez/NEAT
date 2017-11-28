/*
 * ThreadTester.h
 *
 *  Created on: Dec 21, 2011
 *      Author: sushil
 */

#ifndef THREADTESTER_H_
#define THREADTESTER_H_

#include "DEBUG.h"
#include <boost/thread.hpp>
#include <engine.h>

class ThreadTester {
public:

	FastEcslent::Engine *engine;

	ThreadTester(FastEcslent::Engine *eng){
		engine = eng;
	}

	void go(){
		for (int i = 0; i < 1000000; i++) {
			DEBUG(std::cout << "Number of ents: " << engine->entityMgr->nEnts << std::endl;)
			engine->entityMgr->ents[0]->print();

		}
	}

};


#endif /* THREADTESTER_H_ */
