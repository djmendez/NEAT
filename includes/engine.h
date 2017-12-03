/*
 * engine.h
 *
 *  Created on: November 20, 2011
 *      Author: sushil j louis
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <options.h>
#include <entityMgr.h>
#include <selectionMgr.h>
#include <weaponMgr.h>
#include <gameMgr.h>
#include <distanceMgr.h>
#include <levelMgr.h>

#include <groupMgr.h>
#include <micro/infoMgr.h>


#include <gfxThread.h>
#include <netThread.h>
#include <timer.h>
#include <neat.h>

//#include<params.h>

namespace FastEcslent {

	enum GameState{
		LOBBY = 0,
		GAME  = 1,

		NGAMESTATES
	};

	class Engine  {

	private:
		void tickAll();

		//test vars/methods
		FastEcslent::MilliSecondTimer* selectionTimer;
		FastEcslent::MilliSecondTimer* distanceTimer;
		void runTests();

		//Main thread sleep while waiting for lobby setting
		boost::posix_time::milliseconds *sleepInterval;

	public:

		Options options;
		int     instanceId;
		//Params  flockParameters;
		int     rands[1000];
		ptime startTime;
		bool   quit;
		unsigned long int frames;

		//State, indicate if game is in lobby state. Could be changed by remote end.
		volatile GameState   gameState;

		boost::mutex quitLock;

		EntityMgr*    entityMgr;
		SelectionMgr* selectionMgr;
		WeaponMgr*    weaponMgr;
		GameMgr*      gameMgr;
		DistanceMgr*  distanceMgr;
		GroupMgr*     groupMgr;
        LevelMgr*     levelMgr;
        InfoMgr*      infoMgr;

        NEAT* engineNEATNet;

		//threads. optional Features

		NetThread *net;
		GfxThread *gfx;

		Engine(int instId, Options opts, NEAT *neatNet);

		~Engine();
		//Managers
		void showMenu();
		void constructManagers();
		void init();
		void loadLevel();
		void releaseLevel();
		void run();
		void stop();

	} ;

	typedef Engine * EnginePtr;
}



#endif /* ENGINE_H_ */
