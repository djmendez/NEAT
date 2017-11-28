/*
 * unitWeapons.h
 *
 *  Created on: Feb 28, 2012
 *      Author: sushil
 */

#ifndef UNITWEAPONS_H_
#define UNITWEAPONS_H_

#include <aspect.h>
#include <enums.h>

#include <target.h>

#include <weaponMgr.h>
#include <vector>

namespace FastEcslent {

	class DistanceMgr;
	class Entity;

	class Weapon: public UnitAspect {
	private:
		Target target;

		std::vector<Entity*> getUnitsInRadius(Entity* ent, double radius, bool enemy);
		std::vector<FastEcslent::Entity*> getUnitsInRectangle(Entity* src, Entity* tgt, double length, double width, bool enemy);
		bool isPointInRectangle(Ogre::Vector3 sp1, Ogre::Vector3 sp2, Ogre::Vector3 sp3, Ogre::Vector3 sp4, Ogre::Vector3 point);
		double getAreaTriangle(Ogre::Vector3 tp1, Ogre::Vector3 tp2, Ogre::Vector3 tp3);

		int   		 BEINGATTACKED;

		double      m_beingAttacked;

		bool        isAttacking;
	public:
		const WeaponType* weaponType;

		//double*      damageDone;
		double      damageMultiplier;
		double      m_cooldown;
		double      m_onfire;
		double     	 STANDSTILL;

		//double      hitpoints;
                //double*      maxHitpoints;
		double      armor;


		DistanceMgr* distanceMgr;
		WeaponMgr*   weaponMgr;

		Weapon(Entity *ent, UnitAspectType ast, const WeaponType *wType);
		Weapon(Entity *ent, UnitAspectType ast);
		void dealDamageToTarget(Target tgt); //sets target

		/**
		 * deal circle shapedsplash damage in front of the unit, like Hellion in StarCraft 2
		 */
		void dealEnemySplashDamageToTarget(Target tgt, double radius);
		/**
		 * deal line shaped splash damage in front of the unit, like Hellion in StarCraft 2
		 */
		void dealLineSplashDamageToTarget(Target tgt, double length, double width);


		void  setTarget(Entity *ent){
			this->target.entity = ent;
		};
		Target* getTarget(){return &target;};
		void takeDamage(double amt);
		virtual void  tick();
		virtual void  init();

		bool isBeingAttacked(){return m_beingAttacked>0?true:false;};
		bool isEntityAttacking();
		void resetBeingAttack(){m_beingAttacked = 0;};
	};



}


#endif /* UNITWEAPONS_H_ */
