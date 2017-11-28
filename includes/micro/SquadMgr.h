#ifndef SQUADMANAGER_H_
#define SQUADMANAGER_H_

#include "mgr.h"
#include "GA.h"
#include <set>
#include "IMParams.h"
#include <OgreVector3.h>
#include "enums.h"

namespace FastEcslent{
class MicroAspect;
class Engine;
class InfoMgr;
class Entity;

class SquadMgr
{
public:
	enum SquadState
	{
		Idle,
		Approaching,
		Attacking,
		Fleeing
	};
	SquadMgr(FastEcslent::Engine *eng, InfoMgr *infoMgr, FastEcslent::Side side);

	void loadUnits();
	void removeUnits();
	void addUnit(FastEcslent::Entity* unit);
	void addEnemy(FastEcslent::Entity* unit);
	void onUnitDestroy(FastEcslent::Entity* unit);

    void onFrame();  //check every frame
	void onFire();   //in a combat
	
	bool isSquadInCombat();
	bool isSquadMoving();
	void switchState(SquadState ns){
		state = ns;
	}
	std::set<FastEcslent::Entity*> getSquadUnits(){
		return unitSet;
	}
	std::set<FastEcslent::Entity*> getEnemies(){
		return enemySet;
	}

	void potentialMove(FastEcslent::Entity* unit, Ogre::Vector3* target, int dist);

	SquadState state;
	FastEcslent::InfoMgr *im;
	Engine *engine;

	~SquadMgr(){};
	FastEcslent::Entity* getTarget(){return this->target;};
	double fightScore();
	double moveScore();
	double getSquadScore();
	int getSquadScore2();
	int getSquadDestroyScore();
	FastEcslent::Entity* setTarget();

	double getPotentialA();
	double getPotentialB();
	double getPotentialM();
	double getPotentialN();
private:
	void regroup();
	void refreshGroupPosition();
	

	std::set<Entity*> unitSet;
	std::set<MicroAspect*> microAspectSet;
	std::map<Entity*, MicroAspect*> unitToMicroAspectMap;

	std::set<FastEcslent::Entity*> enemySet;
	
	bool needsregroup;

	FastEcslent::Entity* target;

	int centerX;
	int centerY;
	int centerZ;
	int radius;
	int threashold;

	IMParams params;
	//potential args
	double U, A, B, C, n, m, l, d, repulsionThreshold, repulsionThresholdEnemy;

	Side side;
	
};

}
#endif





