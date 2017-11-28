/*
 * commandHelp.cpp
 *
 *  Created on: Feb 26, 2012
 *      Author: sushil
 */

#include <OgreVector3.h>
#include <ai/gather.h>
#include <command.h>
#include <ent.h>
#include <buildings.h>
#include <target.h>
#include <group.h>
#include <unitAI.h>
#include <groupAI.h>

#include "commandFactory.h"
#include <deque>
#include <string>

using namespace FastEcslent;

namespace FastEcslent {

Maintain* createMaintainForEnt(Entity* ent, Entity* enemy) {
	Target* t = new Target;
	t->entity = enemy;
	t->location = enemy->pos;
	t->offset = Ogre::Vector3(0.0f, 0.0f ,0.0f);

	Maintain* maintain = new Maintain(ent, t);
	maintain->init();
	return maintain;
}

void setMaintainForEnt(Entity* ent, Entity* enemy){
	Maintain* mt = createMaintainForEnt(ent, enemy);
	UnitAI* ai = dynamic_cast<UnitAI*> (ent->getAspect(UNITAI));
	ai->setCommand(mt);
}

AttackMove *createAttackForEnt(Entity* ent, Entity* enemy){
	Target* t = new Target;
	t->entity = enemy;
	t->location = enemy->pos;
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	AttackMove* a = new AttackMove(ent, t);
	a->init();
	return a;
}

AttackMove *createAttackForEnt(Entity* ent, Ogre::Vector3 pos){
	Target* t = new Target;
	t->entity = 0;
	t->location = pos;
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	AttackMove* a = new AttackMove(ent, t);
	a->init();
	return a;
}

AttackMove3D *createAttack3DForEnt(Entity* ent, Entity* enemy){
	Target* t = new Target;
	t->entity = enemy;
	t->location = enemy->pos;
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	AttackMove3D* a = new AttackMove3D(ent, t);
	a->init();
	return a;
}

AttackMove3D *createAttack3DForEnt(Entity* ent, Ogre::Vector3 pos){
	Target* t = new Target;
	t->entity = 0;
	t->location = pos;
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	AttackMove3D* a = new AttackMove3D(ent, t);
	a->init();
	return a;
}

Move *createMoveForEnt(Entity* ent, Ogre::Vector3 pos){
	Target* t = new Target;
	t->entity = 0;
	t->location = pos;
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	Move* m = new Move(ent, t);
	m->init();
	return m;
}

Move3D *createMove3DForEnt(Entity* ent, Ogre::Vector3 pos, bool isKiting){
	Target* t = new Target;
	t->entity = 0;
	t->location = pos;
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	Move3D* m = new Move3D(ent, t);
	m->init();
	return m;
}
    
Wait *createWaitForEnt(Entity* ent, double wtime){
	Target* t = new Target;
		t->waitTime = wtime;
	t->entity = 0;
	t->location = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	Wait* w = new Wait(ent, t);
	w->init();
	return w;
}

	Gather *createGatherForEntAndMineral(SCVehicle *scv, Entity *resource){
		Target *t = new Target;
		t->entity = resource;
		t->location = scv->pos;
		t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		Gather *g = new Gather(scv, t);
		g->init();
		return g;
	}

	void setMoveForEnt(Entity* ent, Ogre::Vector3 pos){
		Move *m = createMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		ai->setCommand(m);
	}

	void addMoveForEnt(Entity* ent, Ogre::Vector3 pos){
		Move *m = createMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		ai->addCommand(m);
	}
        
        std::deque<Command*> addCommandForEnt(Entity* ent, CommandFactory* commandFactory, std::pair<int, Ogre::Vector3 > mouseTargets){
		//Move *m = createMoveForEnt(ent, pos);
            if(ent != NULL and ent->getAspect(UNITAI) != NULL and commandFactory != NULL)
            {
                //newCom->entity = ent;
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
             //   if (ai != NULL) //check for an error, i.e. unit does not have a UnitAI aspect and returns a NULL pointer
                ai->setCommandList(commandFactory->MakeCommand(ent,mouseTargets));
                return ai->commands;
            }
            return std::deque<Command*>();
	}

	PotentialMove *createPotentialMoveForEnt(Entity *ent, Ogre::Vector3 pos){
		Target* t = new Target;
		t->entity = 0;
		t->location = pos;
		t->offset   = Ogre::Vector3(0.0f, 200.0f, 0.0f);
		PotentialMove *pm = new PotentialMove(ent, t);
		pm->init();
		return pm;
	}

	Potential3DMove* createPotential3DMoveForEnt(Entity* ent, Ogre::Vector3 pos) {
		Target* t = new Target;
		t->entity = 0;
		t->location = pos;
		t->offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);

		Potential3DMove* p3dm = new Potential3DMove(ent, t);
		p3dm->init();
		return p3dm;
	}

	Potential3DMove* createPotential3DMoveForEnt(Entity* ent, Entity* enemy) {
			Target* t = new Target;
			t->entity = enemy;
			t->location = enemy->pos;
			t->offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);

			Potential3DMove* p3dm = new Potential3DMove(ent, t);
			p3dm->init();
			return p3dm;
		}

	void setDirectMoveForEnt(Entity *ent, Ogre::Vector3 pos, bool append) {
		//can only control their own units.
//		if(ent->engine->options.side != ent->entityId.side or ent->engine->options.player != ent->entityId.player )
//			return;

		Move *m = createMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		append?ai->addCommand(m):ai->setCommand(m);;
	}

	void setPotentialMoveForEnt(Entity *ent, Ogre::Vector3 pos, bool append) {
		//can only control their own units.
//		if(ent->engine->options.side != ent->entityId.side or ent->engine->options.player != ent->entityId.player )
//			return;

		PotentialMove *pm = createPotentialMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		append?ai->addCommand(pm):ai->setCommand(pm);;
	}

	void setGatherForEnt(Entity* ent, Entity* resource, bool append){
		if (ent->entityType == SCV ){
			SCVehicle *scv = dynamic_cast<SCVehicle *>(ent);
			Gather *g = createGatherForEntAndMineral(scv, resource);
			UnitAI *ai = dynamic_cast<UnitAI *> (scv->getAspect(UNITAI));
			append?(ai->addCommand(g)):(ai->setCommand(g));
		}
	}
        std::string EntityTypeToIconName(EntityType entType)
        {
            std::string typeString = "";
            switch(entType)
            {
                case SCV:
                    typeString = "Boats/SCV";
                    break;
                case MARINE:
                    typeString = "Boats/MARINE";
                    break;
                case HELLION:
                    typeString = "Boats/VULTURE";
                    break;
                case BARRACKS:
                    typeString = "BarracksPanel";
                    break;
                case FACTORY:
                    typeString = "FactoryPanel";
                    break;
                default:
                    typeString = "Boats/MARINE"; //Placeholder for testing
            }
            return typeString;
        }
}
