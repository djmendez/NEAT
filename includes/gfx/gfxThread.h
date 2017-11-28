/*
 * gfxThread.h
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 */

#ifndef GFXTHREAD_H_
#define GFXTHREAD_H_

#include "GraphicsInteractionManager.h"

namespace FastEcslent {
	class Engine;

	class GfxThread {
	private:
		boost::thread gfx;

		void runThread();

	public:
		Engine* engine;
		OgreGFX::GraphicsInteractionManager *gimPtr;

		GfxThread(Engine* eng){
			engine = eng;
		}

		void init();
		void run();
		void join();
		void stop();
		void kill();


	};

}
#endif /* GFXTHREAD_H_ */
