/*
 * neat.cpp
 *
 *  Created on: Dec 2, 2017
 *      Author: daniel
 */


/*
 * neat.c
 *
 *  Created on: Dec 2, 2017
 *      Author: daniel
 */


#include <blackboxneat.h>


bool FastEcslent::BlackBoxNEAT::loadFromFile(const char *filename){
	const std::pair<bool, NEAT::Network *> &p = importformfile(filename);
	if (!p.first) {
		std::cout << "error loading neat network" <<std::endl;
		return false;
	}
	net = p.second;
	return true;
}


void FastEcslent::BlackBoxNEAT::NEATProcess() {
	//net->flush();
	if(!activate(net, input, output)){
		std::cout << "error calculating neat network" <<std::endl;
	}
}

