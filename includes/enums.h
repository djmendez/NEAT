/*
 * enums.h
 *
 *  Created on: Jan 27, 2012
 *      Author: sushil
 */

#ifndef ENUMS_H_
#define ENUMS_H_


namespace FastEcslent {

	enum UnitAspectType {

		PHYSICS = 0,
		UNITAI = 1,
		NET    = 2,
		WEAPON = 3,
                BUILDER = 4,
		//DISTANCE = 2,

		NASPECTTYPES
	};

	enum GroupAspectType {

		GROUPAI = 0,
		//LEADER = 1, //A leader aspect to pick current leader

		NGROUPASPECTTYPES
	};
//	enum WeaponType {
//
//		RIFLE  = 0,
//		FLAMER = 1,
//		SHELL  = 2,
//		//DISTANCE = 2,
//
//		NWEAPONTYPES
//	};

	enum EntityType {

		SCV          = 0,
		MARINE       = 1, // Marine
		REAPER       = 2, // Marauder
		TANK         = 3, // Tank
		THOR         = 4, //
		MARAUDER     = 5, //
		HELLION      = 6, // Hellion
		DRONE		 = 19,
		TURRET		 = 20,

		BARRACKS      = 7,
		COMMANDCENTER = 8,
		REFINERY      = 9,
		FACTORY       = 10,
		ARMORY        = 11,
		ENGINEERINGBAY= 12,
		SUPPLYDEPOT   = 13,
//		REACTOR       = 10,
//		TECHLAB       = 11,
//		SPACEPORT     = 12,
//		GHOSTACADEMY  = 13,

		MINERALPATCH  = 14,
		MINERALS      = 15,
		GAS           = 16,

		LAND          = 18,
                
		SC_SCV        = 100,
		SC_MARINE     = 101,
		SC_VULTURE    = 102,
		SC_TANK       = 103,
        SC_ZEALOT     = 104,

		NENTITYTYPES
	};

	enum EntityState {

		GESTATING = 0,
		ALIVE     = 1,
		DYING     = 2,
		DEAD      = 3,

		NENTITYSTATES

	};

	enum EntityClass { // Euphemism for buildings and

		SURFACE    = 0,
		AIR        = 1,
		SUBSURFACE = 2,
		STATIC     = 3, //Buildings, turrets, fixed weapons, OtherNonMoving obstacles

		NENTITYCLASSTYPES

	};

	enum Side {

		BATTLEMASTER = 0,
		RED,
		BLUE,
		GREEN,
		YELLOW,
		NEUTRAL,

		NSIDES
	};

	enum Player {
				ONE = 0,
				TWO,
				THREE,
				FOUR,

				NPLAYERS
	};

	enum ControlType{
		MANUAL = 0,
		NETSLAVE,

		NCONTROLTYPES
	};
        
        
        enum LevelType {
        _64x64,
        _128x128
        };
        
} // end FastEcslent namespace enums



#endif /* ENUMS_H_ */
