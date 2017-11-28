/*
 * mgr.cpp
 *
 *  Created on: Mar 3, 2013
 *      Author: sushil
 */

#include <mgr.h>
#include <engine.h>

namespace FastEcslent {

	Mgr::Mgr(Engine *eng) {
		engine = eng;
	}
	Mgr::~Mgr(){
		engine = 0;
	}

	void Mgr::init() {};
	void Mgr::crosslink() {};
	void Mgr::loadLevel() {};
	void Mgr::tick() {};
	void Mgr::releaseLevel() {}
	void Mgr::cleanup () {};

}
