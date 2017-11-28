#include "micro/MicroAspect.h"
#include <cmath>
#include <map>
#include "engine.h"
#include "micro/MicroAspect.h"
#include "micro/SquadMgr.h"

using namespace FastEcslent;
using namespace std;
bool hpcompare(Entity* i, Entity* j){
	return i->hitpoints < j->hitpoints;
}

MicroAspect::MicroAspect(Entity* u, SquadMgr* s, Side side){
	this->unit = u;
	this->squad = s;
	this->side = side;

	if(side == RED){
		params = GA::getInstance()->getRedParams();
	}else{
		params = GA::getInstance()->getBlueParams();
	}

	microparam.freeze = params.m_freeze * (0.125f / 1.0f);
	microparam.kitingRange = params.m_kitingRange * 32;
	microparam.kitingDist = params.m_kitingDist;   //IM cell distance
	microparam.targetNearby = params.m_targetNearby*10;
	microparam.hpfocus = params.m_hpfocus*0.14;
	microparam.hpkiting = params.m_hpkiting*0.14; // change from 0.1 to get upper limit closer to 1.0
	return;

	//ignore for now.
//	if(u->entityType == FastEcslent::SC_MARINE){
//		microparam.freeze = 8;
//		microparam.kitingRange = 200;
//		microparam.kitingDist = 100;
//		microparam.targetNearby = 30;
//		microparam.hpfocus = 0.3;
//	}else if(u->entityType == FastEcslent::SC_MARINE){
//		microparam.freeze = 8;
//		microparam.kitingRange = 300;
//		microparam.kitingDist = 100;
//		microparam.targetNearby = 30;
//		microparam.hpfocus = 0.3;
//	}else if(u->entityType == FastEcslent::SC_VULTURE){
//		microparam.freeze = 0;
//		microparam.kitingRange = 30;
//		microparam.kitingDist = 250;//3;   //IM cell distance
//		microparam.targetNearby = 80;
//		microparam.hpfocus = 0.3;
//		microparam.hpkiting = 0.5;
//	}
};

//Target selection
Entity* MicroAspect::getTarget(std::set<Entity*> &enemies){
	Entity* t = getNearestUnit(enemies);//nearest enemy
	std::set<Entity*> nearbyunits;//enemies nearby that enemy?
	if(t == NULL)
		return NULL;

	for(int i=0;i<t->engine->entityMgr->nEnts;i++){
		if(t->engine->entityMgr->ents[i]->entityId.side == this->side) continue; //are they us?
		else if(t->engine->entityMgr->ents[i]->entityState != ALIVE) continue; //are they alive?
		else if(t->engine->distanceMgr->distance[t->entityId.id][i] < this->microparam.targetNearby) continue; //are they near that enemy?
		else{
			nearbyunits.insert(t->engine->entityMgr->ents[i]);
		}
	}

	Entity* low = NULL;   //check nearby enemy units, choose the low health target
	Entity* hf = NULL; 	//focus fire on target units.

	if(nearbyunits.size() > 0 ){
		low = getLowHPUnit(nearbyunits);
		hf  = getHighFocusUnit(nearbyunits);
	}

	if(low && this->getUnitHPPercent(low) < this->microparam.hpfocus){ //health < 0.3
		return low;
	}else if(hf && this->getNumberOfAttackers(hf) > 0){
		return hf;
	}else{
		return t;
	}
}


Entity* MicroAspect::getNearestUnit(std::set<Entity*> &enemies){
	double minDist = 100000;
	Entity* t = NULL;
	//calculate all the distances
	for (set<Entity*>::iterator i = enemies.begin();i!= enemies.end();i++){
		double dist = unit->pos.distance((*i)->pos);
		if(dist< minDist){
			t = (*i);
			minDist= dist;
		}
	}

	return t;
}

Entity* MicroAspect::getLowHPUnit(set<Entity*> &nearbyunits){
	vector<Entity*> unitVec;
	copy(nearbyunits.begin(), nearbyunits.end(), std::back_inserter(unitVec));
	sort(unitVec.begin(), unitVec.end(), hpcompare);
	
	return *(unitVec.begin());
}

Entity* MicroAspect::getHighFocusUnit(set<Entity*> &nearbyunits){
	int max = 0;
	Entity* hf;

	for (set<Entity*>::iterator i = nearbyunits.begin();i!= nearbyunits.end();i++){
		int n = this->getNumberOfAttackers(*i);
		if (n >= max ){
			hf = *i;
			max = n;
		}
	}
	
	return hf;
}

//return
int MicroAspect::getNumberOfAttackers(Entity* u){
	int count = 0;
	set<Entity*> units = this->squad->getSquadUnits();

	for (set<Entity*>::iterator i = units.begin();i!= units.end();i++){
		Weapon* weapon = static_cast<Weapon*>((*i)->getAspect(WEAPON));
		if(weapon->getTarget()->entity == u){
			count ++;
		}
	}

	return count;
}

void MicroAspect::onFire(set<Entity*> &enemies){
	updatePosition();

	//no enemy
	if(enemies.size() <=0 ) {
		this->unit->desiredSpeed = 0.0f;
		return;
	}

	Weapon* weapon = static_cast<Weapon*>(unit->getAspect(WEAPON));
	UnitAI* ai     = static_cast<UnitAI*>(unit->getAspect(UNITAI));

	if(unit->entityType == SC_ZEALOT && unit->entityId.side == BLUE) {
		Entity* ourtarget = NULL;
		double minDist = 100000;

		for(int i = 0; i < unit->engine->entityMgr->nEnts; i++) {
			if(unit->engine->entityMgr->ents[i]->entityId.side == RED) {
				if(ourtarget == NULL) {
					ourtarget = unit->engine->entityMgr->ents[i];
					minDist = unit->pos.distance(ourtarget->pos);
				} else {
					double tempDist = unit->pos.distance(unit->engine->entityMgr->ents[i]->pos);
					if(tempDist < minDist) {
						ourtarget = unit->engine->entityMgr->ents[i];
						minDist = tempDist;
					}
				}
			}
		}

		if(unit->engine->distanceMgr->distance[unit->entityId.id][ourtarget->entityId.id] < unit->seekRange) {
			AttackMove3D* attack= createAttack3DForEnt(this->unit, ourtarget);
			ai->setCommand(attack);
		}

		return;
	}

	Entity* newtarget = this->getTarget(enemies);

	//attack freeze
	//for drone:
	//cooldown starts at damageCooldown() value then increments down by dt
	//
	//if(1.24f - (0 - 1.24f) < 0.125);
	//will never work (1.24 - x) will never be more than 1.375
	/*if(weapon->weaponType->damageCooldown() - weapon->m_cooldown < this->microparam.freeze){
	//if((unit->getType().groundWeapon().damageCooldown()-unit->getGroundWeaponCooldown()) < this->microparam.freeze){
		return;
	}*/
	if(weapon->weaponType->damageCooldown() - weapon->m_cooldown < ((this->microparam.freeze * weapon->weaponType->damageCooldown()) / 2.0f)) {
		return;
	}

	//ready to attack
	if(weapon->m_cooldown<=0){
		IMEnemy3D* myim = this->squad->im->getIMEnemy(this->side);
		Entity* lowimunit = myim->getLowestValueUnit();
		//update target
		bool isSquadMoving = this->squad->isSquadMoving();
		if(this->squad->state == SquadMgr::SquadState::Approaching && !isSquadMoving){
			this->squad->switchState(SquadMgr::SquadState::Attacking);
		}
		


			bool isSquadInCombat = this->squad->isSquadInCombat();
			if(isSquadInCombat){
				this->squad->switchState(SquadMgr::SquadState::Attacking);
			}

			//if it is far from enemy, potential move toward enemy and formation
			if(unit->pos.distance(newtarget->pos) > unit->seekRange && (this->squad->state == SquadMgr::SquadState::Approaching ||this->squad->state == SquadMgr::SquadState::Idle)){
				this->squad->switchState(SquadMgr::SquadState::Approaching);

				Ogre::Vector3 fmtp = this->squad->im->getIMEnemy(this->side)->getLowestNearby(lowimunit, &unit->pos);
				this->squad->potentialMove(this->unit, &fmtp, 0);
				return;
			}

			this->target = newtarget;
			if(this->unit->entityType == DRONE || this->unit->entityType == SC_ZEALOT) {
				AttackMove3D* attack= createAttack3DForEnt(this->unit, target);
				ai->setCommand(attack);
			} else {
				AttackMove* attack = createAttackForEnt(this->unit, target);
				ai->setCommand(attack);
			}


	}else{
		this->target = newtarget;
		kiteMove(enemies);
	}

	//Ogre::Vector3 fmtp = Ogre::Vector3(0, 96, 0);
	//this->squad->potentialMove(this->unit, &fmtp, 80);

	/*UnitAI* ai     = static_cast<UnitAI*>(unit->getAspect(UNITAI));

	if(this->unit->entityType == DRONE) {
		Potential3DMove* attack= createPotential3DMoveForEnt(this->unit, Ogre::Vector3(0, 100, 0));
		ai->setCommand(attack);
	}*/
}

void MicroAspect::kiteMove(set<Entity*> &enemies){
	float distance = unit->pos.distance(target->pos);
	Ogre::Vector3 kitingpos = this->getKitingPositionByIM(&unit->pos);
	UnitAI* uai     = static_cast<UnitAI*>(unit->getAspect(UNITAI));
	Weapon* uweapon = static_cast<Weapon*>(unit->getAspect(WEAPON));
	Weapon* tweapon = static_cast<Weapon*>(target->getAspect(WEAPON));

	if(distance < tweapon->weaponType->maxRange() + this->microparam.kitingRange){
		if(tweapon->weaponType->maxRange() < uweapon->weaponType->maxRange()){
			if(this->unit->entityType == DRONE || this->unit->entityType == SC_ZEALOT)
			{
				//3D COMMAND
				Potential3DMove* move = createPotential3DMoveForEnt(this->unit, kitingpos);
				move->init();
				uai->setCommand(move);
			}
			else
			{
				Move* move = createMoveForEnt(this->unit, kitingpos);
				move->init();
				uai->setCommand(move);
			}
		}else if(this->isBeingTarget(unit, enemies) && (this->getUnitHPPercent(unit) < this->microparam.hpkiting)){ //kiting only being targeted
			if(this->unit->entityType == DRONE || this->unit->entityType == SC_ZEALOT)
			{
				//3D COMMAND
				Potential3DMove* move = createPotential3DMoveForEnt(this->unit, target->pos);
				move->init();
				uai->setCommand(move);
			}
			else
			{
				Move* move = createMoveForEnt(this->unit, target->pos);
				move->init();
				uai->setCommand(move);
			}
		}
	}
}


Entity* MicroAspect::getEnemyAttackingThisUnit(std::set<Entity*> &enemies){
	for(set<Entity*>::iterator i = enemies.begin(); i!= enemies.end();i++){
		Weapon* weapon = static_cast<Weapon*>((*i)->getAspect(WEAPON));
		if(weapon->getTarget()->entity == this->unit){
			return *i;
		}
	}
	return NULL;
}

Ogre::Vector3 MicroAspect::getKitingPosition(Entity* u, Entity* target){
	double x1 = u->pos.x;
	double y1 = u->pos.y;
	double x2 = target->pos.x;
	double y2 = target->pos.y;

	double theta = std::atan2(y1-y2, x1-x2);
	//0.2 = 23 degree
	int x = x1+ this->microparam.kitingDist*cos(theta);
	int y = y1+ this->microparam.kitingDist*sin(theta);

	if(x<0) x=0;
	if(y<0) y=0;
	
	return Ogre::Vector3(x,0,y);
}

Ogre::Vector3 MicroAspect::getKitingPositionByIM(Ogre::Vector3* pos){
	Ogre::Vector3 enemyPos = Ogre::Vector3(*pos);
	if(this->target != 0)
		enemyPos = this->target->pos;

	Ogre::Vector3 kp = this->squad->im->getIMEnemy(this->side)->getHidingPos(pos, &enemyPos, this->microparam.kitingDist);
	//kp.y = kp.y + 100;//TODO**** remove

	return kp;
}


bool MicroAspect::isBeingTarget(Entity* u, std::set<Entity*> &enemies){
	for(set<Entity*>::iterator i = enemies.begin(); i!= enemies.end();i++){
		Weapon* weapon = static_cast<Weapon*>((*i)->getAspect(WEAPON));
		if(weapon->getTarget()->entity == this->unit){
			return true;
		}
	}
	return false;
}

void MicroAspect::updatePosition(){
	if(this->unit->pos.x != this->position.second.x || this->unit->pos.y != this->position.second.y){
		this->position = pair<int, Ogre::Vector3>(squad->im->getFrameCount(), this->unit->pos);
	}
}
bool MicroAspect::isUnitMoving(){
	if(squad->im->getFrameCount() - this->position.first > 10){
		return false;
	}
	return true;
}

float MicroAspect::getUnitHPPercent(Entity* u){
	float hp = u->hitpoints;
	float max = u->hitpointsmax;
	return hp/max;
}

MicroAspect::~MicroAspect(){
}


