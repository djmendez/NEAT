/*
 * weaponMgr.h
 *
 *  Created on: Feb 28, 2012
 *      Author: sushil
 */

#ifndef WEAPONMGR_H_
#define WEAPONMGR_H_

#include <mgr.h>
#include <options.h>
#include <enums.h>
#include <ExplosionType.h>
#include <WeaponType.h>
#include <map>

namespace FastEcslent {

	class Engine;
	class WeaponMgr: public Mgr {

	public:
		Options options;
		const WeaponType * weaponType[NENTITYTYPES];
		double   maxHitpoints[NENTITYTYPES];
		double   initArmor[NENTITYTYPES];
		double   initDamageMultiplier[NENTITYTYPES];

		WeaponMgr(Engine *eng, Options opts);
		void init();
		void tick();

	private:
		void mapWeaponTypeToEntityType();
		void fixFightingProperties();

	};


}


#endif /* WEAPONMGR_H_ */
