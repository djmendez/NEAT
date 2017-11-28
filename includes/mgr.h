/*
 * mgr.h
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#ifndef MGR_H_
#define MGR_H_



namespace FastEcslent {
	class Engine;

	class Mgr {

		public:
			Engine *engine;
			//constructor
			Mgr(Engine *eng);
			~Mgr();


			//default methods
			virtual void init();
			virtual void crosslink();
			virtual void loadLevel();
			virtual void tick();
			virtual void releaseLevel();
			virtual void cleanup ();

	};
}


#endif /* MGR_H_ */
