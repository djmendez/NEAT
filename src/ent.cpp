/*
 * ent.cpp
 *
 *  Created on: Dec 18, 2011
 *      Author: sushil
 */

#include <iostream>
#include <assert.h>
#include <ent.h>
#include <engine.h>
#include <aspect.h>
#include <command.h>
#include "DEBUG.h"

int FastEcslent::Entity::count = 0;

void FastEcslent::Entity::print() {
     std::cout << "Id: " << entityId.id << ", instanceId: " << engine->instanceId << std::endl;
     std::cout << "UIName " << uiname << ", meshName: " << meshName << std::endl;
     std::cout << "Position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
     std::cout << "Velocity: " << vel.x << ", " << vel.y << ", " << vel.z << std::endl;
     std::cout << "Accelert: " << acc.x << ", " << acc.y << ", " << acc.z << std::endl;
     std::cout << "Speed:    " << speed << std::endl;
     std::cout << "Heading:  " << heading << std::endl;
     std::cout << "dsrdSpd:  " << desiredSpeed << std::endl;
     std::cout << "dsrdHdg:  " << desiredHeading << std::endl;
     std::cout << "yaw:      " << yaw << std::endl;
     std::cout << "selected: " << isSelected << std::endl;
     std::cout << "-------------------------------------------------------------------" << std::endl;
}

void FastEcslent::Entity::reset() {
    //FastEcslent::Entity::tick = &FastEcslent::Entity::gestatingTick;
    //this->tick = &FastEcslent::Entity::gestatingTick;
    //tic = &FastEcslent::Entity::gestatingTick;
    pos = Ogre::Vector3(0.0f, 0.0f, 0.0f);
    vel = Ogre::Vector3(0.0f, 0.0f, 0.0f);
    acc = Ogre::Vector3(0.0f, 0.0f, 0.0f);
    maxSpeed = 40.0f;
    minSpeed = -10.0f;
    speedRange = maxSpeed - minSpeed;
    rot = Ogre::Quaternion(Ogre::Radian(0.0f), Ogre::Vector3::UNIT_Y);
    maxAcceleration = 1.0f;

    yaw = 0.0f;
    maxRotationalSpeed = 0.1;
    desiredSpeed = 0.0f;
    desiredHeading = 0.0f;
    speed = 0.0f;
    heading = 0.0f;
    //entityType = CIGARETTE;
    //alive = true;
    selectable = false;

    turningRadius = 10000;

    //selection
    isSelected = false;
    
    //selection sound
    didSelectSoundPlayed = false;
    //newborn sound
    isNewBorn = false;

    entityId.side = BATTLEMASTER;
    entityId.player = ONE;
    entityClass = SURFACE;
    entityState = ALIVE;
    gestationPeriod = 10.0;
    timeLeftToBirth = 10.0;
    timeLeftToDie = 10.0;

    /*
    preReqs.neededResources.gas = 0;
    preReqs.neededResources.minerals = 50;
    preReqs.neededResources.supply = 1;

    preReqs.neededTypes[0] = BARRACKS;
    preReqs.nNeededTypes = 1;
     */

    aspects.clear();

}

FastEcslent::Entity::Entity(Engine *eng, EntityType entType):builder(NULL) {

	engine = eng;
	//entityId.id = id;
	entityType = entType;
	entityClass = SURFACE;
	meshName = "cigarette.mesh";
	uiname = "Cigarette";

	isAttacking = false;
	//uiname.append(boost::lexical_cast<std::string>(count++));
	reset();

	init();
}






// There should only be one aspect of each type

/*int FastEcslent::Entity::addAspect(Aspect *asp) {
        aspects[nAspects++] = asp;
        return nAspects - 1;
}*/
void FastEcslent::Entity::init() {

	for (int aType = 0; aType < NASPECTTYPES; aType++){ // the problem with enums in C++n
		switch (aType){
		case (PHYSICS):
//			if(this->entityType == DRONE || this->entityType ==  SC_ZEALOT)
//				physics = new Physics3D(this, PHYSICS);
//			else
			//physics = new Physics2D2(this, PHYSICS);
		physics = new Physics2D2(this, PHYSICS);
		//physics = new Physics3D(this, PHYSICS);

			physics->init();
			this->addAspect(physics);
			break;
		case (UNITAI):
			ai = new UnitAI(this, UNITAI);
			ai->init();
		    this->addAspect(ai);
			break;
		case (WEAPON):
			weapon = new Weapon(this, WEAPON);
			this->addAspect(weapon);
			break;
		case (NET):
			if(this->engine->options.enableNetworking){
				netAspect = new NetAspect(this, NET);
				netAspect->init();
				this->addAspect(netAspect);
			}
			break;
		}
		//this->addAspect(asp);
	}


    this->gestationPeriod = this->engine->gameMgr->entTypeData[this->entityType].buildTime;

}

bool FastEcslent::Entity::sinking() {
    this->timeLeftToDie -= 1;
    int dyingPeriod = 5;  //all units dying with the same speed.
    this->pos.y = -(-this->height + (this->height * (dyingPeriod - this->timeLeftToDie) / dyingPeriod));
    if (this->timeLeftToDie <= 0.0) {
        //this->entityState = FastEcslent::DEAD;
        this->switchState(DEAD);
        //this->pos.y = -2 * this->height;
        this->pos.y = -100000.0;
        return false;
    }

    return true;
}

bool FastEcslent::Entity::raising() {
    this->timeLeftToBirth -= 1;
    this->pos.y = -this->height + (this->height * (this->gestationPeriod - this->timeLeftToBirth) / this->gestationPeriod);
    if (this->timeLeftToBirth <= 0.0) {
        this->switchState(ALIVE);
        //this->entityState = FastEcslent::ALIVE;
        //this->engine->entityMgr->addEntity(this);
        this->pos.y = 0.0f;
        return false;
    }
    return true;
}

void FastEcslent::Entity::addAspect(UnitAspect *asp) {
    aspects.push_back(asp);
}

FastEcslent::UnitAspect* FastEcslent::Entity::getAspect(UnitAspectType at) {
    for (std::deque<UnitAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++) {
        if ((*it)->aspectType == at)
            return (*it);
    }
    return 0;
}

void FastEcslent::Entity::removeAspect(UnitAspectType at) {
    int index = -1;
    for (std::deque<UnitAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++) {
        index++;
        if ((*it)->aspectType == at) {
            break;
        }
    }
    if (index >= 0 && index < (int) aspects.size()) {
        aspects.erase(aspects.begin() + index);
    }
}

void FastEcslent::Entity::tick() {
    switch (this->entityState) {
        case GESTATING:
            this->gestatingTick();
            break;

        case ALIVE:
            this->aliveTick();
            break;

	case DYING:
		this->dyingTick();
		break;

        case DEAD:
            //this->deadTick();
            break;

        default:

            break;
    }
}

void FastEcslent::Entity::switchState(EntityState newState) {
    this->entityState = newState;
    if (newState == GESTATING) {
        this->timeLeftToBirth = this->engine->gameMgr->entTypeData[this->entityType].buildTime; //this->gestationPeriod;
    } else if (newState == DYING) {
        this->timeLeftToDie = this->engine->gameMgr->entTypeData[this->entityType].buildTime; //this->gestationPeriod;
        this->engine->selectionMgr->unselect(this);
        this->selectable = false;
    }
}

void FastEcslent::Entity::gestatingTick() {
    boost::mutex::scoped_lock scoped_lock(entLock);
    if (!raising()) { // entity rises up through the surface. Return true when done
        this->switchState(ALIVE);
        //this->init();
        this->isNewBorn = true;
    }
}

void FastEcslent::Entity::aliveTick() {
    DEBUG(std::cout << "Ent: " << this->uiname << " EntityState: " << this->entityState << std::endl;)
    boost::mutex::scoped_lock scoped_lock(entLock);
    for (std::deque<UnitAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++) {
        (*it)->tick();
    }
}

void FastEcslent::Entity::dyingTick() {
    boost::mutex::scoped_lock scoped_lock(entLock);
    if (!sinking()) { //entity sinks. Returns true when sunk below surface of water
        this->switchState(DEAD);
    }
}

void FastEcslent::Entity::deadTick() {
    return;
}

void FastEcslent::SCVehicle::init() {
    builder = new Builder(this,BUILDER, Ogre::Vector3(200,0,200),Ogre::Vector3(200,0,200));
    builder->init();
    this->addAspect(builder);
    //builder->buildableEntities[BARRACKS]   = true;
    
    //Entity::init();

}

void FastEcslent::Entity::setVultureProperties(){

	maxSpeed = knots(64.0f);

	minSpeed = knots(0.0f);
	speedRange = maxSpeed - minSpeed + EPSILON;

	maxAcceleration = feetPerSecondSquared(55.0f);

	maxRotationalSpeed = degreesPerSecond(170.0f);
	turningRadius = 180;
	mass = tons(200);

	hitpoints = 80;

	hitpointsmax = 80;

	seekRange = 256*2;
}

void FastEcslent::Entity::setZealotProperties(){

	maxSpeed = knots(40.0f);

	minSpeed = knots(0.0f);
	speedRange = maxSpeed - minSpeed + EPSILON;

	maxAcceleration = feetPerSecondSquared(40.0f);

	maxRotationalSpeed = degreesPerSecond(170.0f);
	turningRadius = 180;
	mass = tons(200);

	hitpoints = 160;

	hitpointsmax = 160;

	seekRange = 224*2;
}

