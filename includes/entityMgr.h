/*
 * entityMgr.h
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#ifndef ENTITYMGR_H_
#define ENTITYMGR_H_

#include<iostream>
#include<list>

#include "const.h"
#include "enums.h"
#include "options.h"

#include "mgr.h"
#include "ent.h"
#include "ent_sc.h"



namespace FastEcslent {

	class Engine;

	class EntityMgr : public Mgr {

	private:
		void reset(){
			nEnts = 0;

			Meshes[0] = "cigarette.mesh";
			Meshes[1] = "boat.mesh";
			Meshes[2] = "ddg51.mesh";
			Meshes[3] = "cvn68.mesh";
			Meshes[4] = "sleek.mesh";
			Meshes[5] = "alienship.mesh";
			Meshes[6] = "drone.mesh";
			Meshes[7] = "turret.mesh";
		}

		Entity* createEntity(EntityType etype, Ogre::Vector3 pos = Ogre::Vector3(0,0,0), float heading = 0.0f);


	public:
		Entity *ents[MaxEnts]; // MaxEnts in const.h
		std::string Meshes[8];
		Options options;
		int     nEnts;

		EntityMgr(Engine *eng, Options opts);
		EntityMgr();
		void init();
		void tick();


		void dumpOne(int i);
		void dumpAll();

		int addEntityToGame(Entity* ent);
		Entity* createEntityForPlayerAndSide(EntityType etype, Ogre::Vector3 pos = Ogre::Vector3(0,0,0), float heading = 0.0f, Side side = BATTLEMASTER, Player player = ONE);
		Entity* createEntityAfterTime(EntityType etype, Ogre::Vector3 pos = Ogre::Vector3(0,0,0), float heading = 0.0f);
		Entity* createEntityNow(EntityType etype, Ogre::Vector3 pos = Ogre::Vector3(0,0,0), float heading = 0.0f);
		bool cancelGestatingEntity(Entity *ent);

		bool preReqExists(EntityType etype);
		bool notExceedPopCap(Identity entId);

		Entity* getEntityById(int id);
	};


}



#endif /* ENTITYMGR_H_ */
