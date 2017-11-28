/*
 * weapon.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: sushil
 */
#include <ent.h>
#include <engine.h>
#include <distanceMgr.h>
#include "unitWeapons.h"
#include "DEBUG.h"

using namespace Ogre;

FastEcslent::Weapon::Weapon(Entity* ent, UnitAspectType ast,const WeaponType *wtype): UnitAspect(ent, ast) {
	weaponType = wtype;
        init();
}

FastEcslent::Weapon::Weapon(Entity* ent, UnitAspectType ast): UnitAspect(ent, ast) {
	init();
}

void FastEcslent::Weapon::init(){
	distanceMgr = this->entity->engine->distanceMgr;
	weaponMgr   = this->entity->engine->weaponMgr;
	weaponType = weaponMgr->weaponType[this->entity->entityType];

	this->entity->hitpoints = weaponMgr->maxHitpoints[this->entity->entityType];
	armor     = weaponMgr->initArmor[this->entity->entityType];
	damageMultiplier =  weaponMgr->initDamageMultiplier[this->entity->entityType];

	this->target.entity = 0;

	m_cooldown = 0;
	if(this->entity->entityType == FastEcslent::SC_ZEALOT) {
		STANDSTILL = 0.75f;
	} else if(this->entity->entityType == FastEcslent::DRONE) {
		STANDSTILL = 1;
	}else{
		STANDSTILL = 0;
	}
	m_onfire = 0;
	BEINGATTACKED = 100;
	m_beingAttacked = 0;
}


void FastEcslent::Weapon::tick() {
	if(m_beingAttacked > 0){
		m_beingAttacked -=1;
	}

	//onfile stand still
	if(m_onfire > 0){
		m_onfire -= 1;
		this->entity->speed = 0;
		return;
	}

	//This is only attack the closest enemy
	if(this->entity->isAttacking && m_cooldown <= 0){
		if(this->target.entity == 0 &&  distanceMgr->closestEnemyDistance[this->entity->entityId.id] <= this->weaponType->maxRange()){
			target.entity = this->entity->engine->entityMgr->ents[distanceMgr->closestEnemy[this->entity->entityId.id]];
		}

		if (this->target.entity !=0 && target.entity->entityState == FastEcslent::ALIVE && this->target.entity->pos.distance(this->entity->pos) <= this->weaponType->maxRange()){
			if(this->weaponType->explosionType() == FastEcslent::ExplosionTypes::Enemy_Splash){
				dealEnemySplashDamageToTarget(target,this->weaponType->outerSplashRadius());  //radius splash damage
			}else if(this->weaponType->explosionType() == FastEcslent::ExplosionTypes::Enemy_Line_Splash){
				dealLineSplashDamageToTarget(target, this->weaponType->outerSplashRadius(), this->weaponType->innerSplashRadius());                //line splash damage
			}else {
				dealDamageToTarget(target);
			}
		}

		//face enemy when attacking
		if(this->target.entity!=0){
			Ogre::Vector3 direct = this->target.entity->pos - this->entity->pos;
			double yaw = -atan2(direct.z,direct.x);
			this->entity->desiredHeading = yaw;
		}

		this->target.entity = 0;
	}else if(m_cooldown >=0){
		if(m_onfire <= 0)
			m_cooldown -= 1;
	}

	//if the target is dead, reset the target
	if(this->target.entity !=0 and target.entity->entityState != FastEcslent::ALIVE){
		target.entity = 0;
	}

	if(entity->entityId.id == 2){
		//std::cout<<"Entity 2 cooldown inside is: "<<m_cooldown<<std::endl;
	}
}

void FastEcslent::Weapon::dealDamageToTarget(Target tgt){
	//double damage = weaponMgr->damageMap[entity->entityType][tgt.entity->entityType] * damageMultiplier;
	double damage = this->weaponType->damageAmount();//weaponMgr->damageMap[tgt.entity->entityType][weaponType] * damageMultiplier*WeaponMgr::DAMAGESCALER;
	tgt.entity->weapon->takeDamage(damage);

	m_cooldown = this->weaponType->damageCooldown();//weaponMgr->cooldown[this->entity->entityType] * WeaponMgr::COOLDOWNSCALER;
	this->m_onfire = STANDSTILL;
}

void FastEcslent::Weapon::dealEnemySplashDamageToTarget(Target tgt, double radius){
	//double damage = weaponMgr->damageMap[entity->entityType][tgt.entity->entityType] * damageMultiplier;
	double damage = this->weaponType->damageAmount();//weaponMgr->damageMap[tgt.entity->entityType][weaponType] * damageMultiplier*WeaponMgr::DAMAGESCALER;
	std::vector<FastEcslent::Entity*> ents = this->getUnitsInRadius(tgt.entity, radius, true);

	for(unsigned int i=0;i<ents.size();i++){
		ents[i]->weapon->takeDamage(damage);
	}

	DEBUG(std::cout<<"Deal enemy radius splash damage: hits: "<<ents.size()<< " units."<<std::endl;)

	m_cooldown = this->weaponType->damageCooldown();
	this->m_onfire = STANDSTILL;
}

void FastEcslent::Weapon::dealLineSplashDamageToTarget(Target tgt, double length, double width){
	double damage = this->weaponType->damageAmount();//weaponMgr->damageMap[tgt.entity->entityType][weaponType] * damageMultiplier*WeaponMgr::DAMAGESCALER;

	std::vector<FastEcslent::Entity*> ents = this->getUnitsInRectangle(this->entity, tgt.entity, length, width, true);
	for(unsigned int i=0;i<ents.size();i++){
		ents[i]->weapon->takeDamage(damage);
	}

	DEBUG(std::cout<<"Deal enemy line splash damage: hits: "<<ents.size()<< " units."<<std::endl;)

	m_cooldown = this->weaponType->damageCooldown();
	this->m_onfire = STANDSTILL;
}

void FastEcslent::Weapon::takeDamage(double amt) {
	if (this->entity->entityState == FastEcslent::ALIVE) {
		m_beingAttacked = BEINGATTACKED;
		this->entity->hitpoints -= amt * armor;
		if (this->entity->hitpoints <= 0){
			this->entity->switchState(DYING);
			this->entity->hitpoints = 0;
		}
	}
}

std::vector<FastEcslent::Entity*> FastEcslent::Weapon::getUnitsInRadius(Entity* ent, double radius, bool enemyOnly){
	std::vector<FastEcslent::Entity*> ents;
	for (int i = 0; i < this->entity->engine->entityMgr->nEnts; i++){
		//ignore dead units
		if(this->entity->engine->entityMgr->ents[i]->entityState != ALIVE){
			continue;
		}
		//ignore own units if we only do damage to enemy units
		if(enemyOnly && this->entity->engine->entityMgr->ents[i]->entityId.side == this->entity->entityId.side){
			continue;
		}
		double distance = (ent->pos - this->entity->engine->entityMgr->ents[i]->pos).length();
		if(distance <= radius){
			ents.push_back(this->entity->engine->entityMgr->ents[i]);
		}
	}
	return ents;
}

/**
 * Get entities in Rectangle.
 */
std::vector<FastEcslent::Entity*> FastEcslent::Weapon::getUnitsInRectangle(Entity* src, Entity* tgt, double length, double width, bool enemyOnly){
	Vector3 vec = tgt->pos - src->pos;
	Vector3 v1,v2,v3,v4;

	double xdelta = width/2 * sin(atan2(vec.z, vec.x));
	double zdelta = width/2 * cos(atan2(vec.z, vec.x));
	v1.x = src->pos.x - xdelta;
	v2.x = src->pos.x + xdelta;

	v1.z = src->pos.z + zdelta;
	v2.z = src->pos.z - zdelta;

	v3.x = v2.x + length* cos(atan2(vec.z, vec.x));
	v3.z = v2.z + length* sin(atan2(vec.z, vec.x));

	v4.x = v1.x + length* cos(atan2(vec.z, vec.x));
	v4.z = v1.z + length* sin(atan2(vec.z, vec.x));

	std::vector<FastEcslent::Entity*> ents;
	for (int i = 0; i < this->entity->engine->entityMgr->nEnts; i++){
		if(this->entity->engine->entityMgr->ents[i]->entityState != ALIVE){
			continue;
		}
		//ignore own units if we only do damage to enemy units
		if(enemyOnly && this->entity->engine->entityMgr->ents[i]->entityId.side == this->entity->entityId.side){
			continue;
		}

		if(this->isPointInRectangle(v1,v2,v3,v4, this->entity->engine->entityMgr->ents[i]->pos)){
			ents.push_back(this->entity->engine->entityMgr->ents[i]);
		}
	}
	return ents;
}

/**
 * We only consider x & z
 */
bool FastEcslent::Weapon::isPointInRectangle(Ogre::Vector3 sp1, Ogre::Vector3 sp2, Ogre::Vector3 sp3, Ogre::Vector3 sp4, Ogre::Vector3 point){
	double areaRectangle = (sp1-sp2).length() * (sp2-sp3).length();
	double area1 = this->getAreaTriangle(sp1,sp2,point);
	double area2 = this->getAreaTriangle(sp2,sp3,point);
	double area3 = this->getAreaTriangle(sp3,sp4,point);
	double area4 = this->getAreaTriangle(sp4,sp1,point);
	double sum = area1+area2+area3+area4;
	if(sum - areaRectangle < 50){   // 5 is the deviation
		return true;
	}else {
		return false;
	}
}

/**
 * Using Heron's formula to calculate area of the triangle.
 */
double FastEcslent::Weapon::getAreaTriangle(Ogre::Vector3 tp1, Ogre::Vector3 tp2, Ogre::Vector3 tp3){
	double a = (tp1-tp2).length();
	double b = (tp2-tp3).length();
	double c = (tp3-tp1).length();
	double p = (a + b + c)/2;
	return sqrt(p*(p-a)*(p-b)*(p-c));
}

bool FastEcslent::Weapon::isEntityAttacking(){
	if(this->m_onfire > 0 || this->m_cooldown > 0){
		return true;
	}
	return false;
}
