#ifndef MICROASPECT_H_
#define MICROASPECT_H_


#include "microparams.h"
#include "IMParams.h"
#include "OgreVector3.h"
#include "enums.h"

namespace FastEcslent{
class SquadMgr;
class Entity;

class MicroAspect{
public:
	MicroAspect(Entity* u, SquadMgr* s, Side side);

	FastEcslent::Entity* getTarget(std::set<FastEcslent::Entity*> &enemies);
	

	void onFire(std::set<FastEcslent::Entity*> &enemies);
	//void dodge(std::set<FastEcslent::Entity*> &enemies);
	bool isBeingTarget(FastEcslent::Entity* u, std::set<FastEcslent::Entity*> &enemies);
	bool isUnitMoving();
	void kiteMove(std::set<FastEcslent::Entity*> &enemies);
    ~MicroAspect(); 

private:
    FastEcslent::Entity* getEnemyAttackingThisUnit(std::set<FastEcslent::Entity*> &enemies);

    FastEcslent::Entity* unit;
	SquadMgr * squad;
	
	FastEcslent::Entity* target;

	Ogre::Vector3 getKitingPosition(FastEcslent::Entity* u, FastEcslent::Entity* enemy);
	Ogre::Vector3 getKitingPositionByIM(Ogre::Vector3* pos);
	int getNumberOfAttackers(FastEcslent::Entity* u);
	
	FastEcslent::MicroParam microparam;
	IMParams params;

	std::pair<int, Ogre::Vector3> position;
	void updatePosition();

	float getUnitHPPercent(FastEcslent::Entity* u);
	FastEcslent::Entity* getNearestUnit(std::set<FastEcslent::Entity*> &enemies);
	FastEcslent::Entity* getLowHPUnit(std::set<FastEcslent::Entity*> &nearbyunits);
	FastEcslent::Entity* getHighFocusUnit(std::set<FastEcslent::Entity*> &nearbyunits);
	
	Side side;
	int count;
};

}

#endif



