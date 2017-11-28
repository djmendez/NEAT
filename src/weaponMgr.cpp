/*
 * weaponMgr.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: sushil
 */

#include <weaponMgr.h>
#include <engine.h>
#include <enums.h>

FastEcslent::WeaponMgr::WeaponMgr(Engine *eng, Options opts): Mgr(eng) {
	options = opts;
	//establish weapon mapping
}

void FastEcslent::WeaponMgr::fixFightingProperties(){
	for (int entType = 0; entType < NENTITYTYPES; entType++){
		switch(entType){
		case SCV:
			maxHitpoints[SCV] = 50;
			initArmor[SCV]    = 1.0;
			initDamageMultiplier[SCV] = 1.0;
			weaponType[SCV] = &WeaponTypes::Fusion_Cutter;
			break;
		case MARINE:
			maxHitpoints[MARINE] = 50.0;
			initArmor[MARINE]     = 1.0;
			initDamageMultiplier[MARINE] = 1.0;
			weaponType[MARINE] = &WeaponTypes::Rifle;
			break;
		case REAPER:
			maxHitpoints[REAPER] = 60.0;
			initArmor[REAPER]     = 1.0;
			initDamageMultiplier[REAPER] = 1.0;
			weaponType[REAPER] = &WeaponTypes::Rifle;
			break;
		case TANK:
			maxHitpoints[TANK] = 160.0;
			initArmor[TANK]     = 1.0;
			initDamageMultiplier[TANK] = 1.0;
			weaponType[TANK] = &WeaponTypes::Cannon;
			break;
		case THOR:
			maxHitpoints[THOR] = 400.0;
			initArmor[THOR]     = 1.0f;
			initDamageMultiplier[THOR] = 1.0f;
			weaponType[THOR] = &WeaponTypes::Cannon;
			break;
		case MARAUDER:
			maxHitpoints[MARAUDER] = 125;
			initArmor[MARAUDER]     = 1.0;
			initDamageMultiplier[MARAUDER] = 1.0;
			weaponType[MARAUDER] = &WeaponTypes::Cannon;
			break;
		case HELLION:
			maxHitpoints[HELLION] = 70;
			initArmor[HELLION]     = 1.0;
			initDamageMultiplier[HELLION] = 1.0;
			weaponType[HELLION] = &WeaponTypes::Hellfire;
			break;
			//-----------------------------------------------------
		case SC_SCV:
			maxHitpoints[SC_SCV] = 50;
			initArmor[SC_SCV]    = 1.0;
			initDamageMultiplier[SC_SCV] = 1.0;
			weaponType[SC_SCV] = &WeaponTypes::Fusion_Cutter;
			break;
		case SC_MARINE:
			maxHitpoints[SC_MARINE] = 50.0;
			initArmor[SC_MARINE]     = 1.0;
			initDamageMultiplier[SC_MARINE] = 1.0;
			weaponType[SC_MARINE] = &WeaponTypes::Rifle;
			break;
		case SC_VULTURE:
			maxHitpoints[SC_VULTURE] = 60.0;
			initArmor[SC_VULTURE]     = 1.0;
			initDamageMultiplier[SC_VULTURE] = 1.0;
			weaponType[SC_VULTURE] = &WeaponTypes::Rifle;
			break;
		case DRONE:
			maxHitpoints[DRONE] = 80.0;
			initArmor[DRONE]     = 1.0;
			initDamageMultiplier[DRONE] = 1.0;
			weaponType[DRONE] = &WeaponTypes::Rifle;
			break;
		case TURRET:
			maxHitpoints[TURRET] = 200.0;
			initArmor[TURRET] = 1.0;
			initDamageMultiplier[TURRET] = 1.0;
			weaponType[TURRET] = &WeaponTypes::Hellfire;
			break;
		case SC_TANK:
			maxHitpoints[SC_TANK] = 160.0;
			initArmor[SC_TANK]     = 1.0;
			initDamageMultiplier[SC_TANK] = 1.0;
			weaponType[SC_TANK] = &WeaponTypes::Cannon;
			break;
		case SC_ZEALOT:
			maxHitpoints[SC_ZEALOT] = 160.0;
			initArmor[SC_ZEALOT]     = 1.0;
			initDamageMultiplier[SC_ZEALOT] = 1.0;
			weaponType[SC_ZEALOT] = &WeaponTypes::Fusion_Cutter;
			break;

			//-----------------------------------------------------
		case BARRACKS:
			maxHitpoints[BARRACKS] = 1000;
			initArmor[BARRACKS]    = 1.0;
			initDamageMultiplier[BARRACKS] = 1.0;
			weaponType[BARRACKS] = &WeaponTypes::None;
			break;
		case FACTORY:
			maxHitpoints[FACTORY] = 1000;
			initArmor[FACTORY]    = 1.0;
			initDamageMultiplier[FACTORY] = 1.0;
			weaponType[FACTORY] = &WeaponTypes::None;
			break;
		case ARMORY:
			maxHitpoints[ARMORY] = 1000;
			initArmor[ARMORY]    = 1.0;
			initDamageMultiplier[ARMORY] = 1.0;
			weaponType[ARMORY] = &WeaponTypes::None;
			break;
		case ENGINEERINGBAY:
			maxHitpoints[ENGINEERINGBAY] = 1000;
			initArmor[ENGINEERINGBAY]    = 1.0;
			initDamageMultiplier[ENGINEERINGBAY] = 1.0;
			weaponType[ENGINEERINGBAY] = &WeaponTypes::None;
			break;
                case REFINERY:
			maxHitpoints[REFINERY] = 500;
			initArmor[REFINERY]    = 1.0;
			initDamageMultiplier[REFINERY] = 1.0;
			weaponType[REFINERY] = &WeaponTypes::None;
			break;
		case GAS:
			maxHitpoints[GAS] = 10000000;
			initArmor[GAS]    = 1.0;
			initDamageMultiplier[GAS] = 1.0;
			weaponType[GAS] = &WeaponTypes::None;
			break;
		case MINERALS:
			maxHitpoints[MINERALS] = 10000000;
			initArmor[MINERALS]    = 1.0;
			initDamageMultiplier[MINERALS] = 1.0;
			weaponType[MINERALS] = &WeaponTypes::None;
			break;
		case LAND:
			maxHitpoints[LAND] = 10000000;
			initArmor[LAND]    = 1.0;
			initDamageMultiplier[LAND] = 1.0;
			weaponType[LAND] = &WeaponTypes::None;
			break;
		case COMMANDCENTER:
			maxHitpoints[COMMANDCENTER] = 1500;
			initArmor[COMMANDCENTER]    = 1.0;
			initDamageMultiplier[COMMANDCENTER]= 1.0;
			weaponType[COMMANDCENTER] = &WeaponTypes::None;
			break;
		default:
			DEBUG(std::cerr << "Unknown Entity Type in WeaponManager. Need to exit" << std::endl;)
			break;
		}
	}
}




void FastEcslent::WeaponMgr::init(){
	//establish weaponType, entityType damage mapping
	fixFightingProperties();

	FastEcslent::ExplosionTypes::init();
	FastEcslent::WeaponTypes::init();
}


void FastEcslent::WeaponMgr::tick(){

}
