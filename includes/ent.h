/*
 * ent.h
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#ifndef ENT_H_
#define ENT_H_


#include <iostream>
#include <cstring>
#include <assert.h>

#include <boost/lexical_cast.hpp>


#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <const.h>
#include <utils.h>

#include <command.h>
#include <unitWeapons.h>
#include <unitAI.h>
#include <physics.h>

#include <enums.h>
#include <identity.h>
#include "unitBuilder.h"

//#include <aspect.h>
#include <netAspect.h>
#include <boost/thread/mutex.hpp>

namespace FastEcslent {

	class Engine ;
	class UnitAspect ;
//	class GameMgr;

	struct RegObj
	{
	//	BWAPI::Unit* m_pObject;
		Entity* m_pObject;

		int m_objSizeX;
		int m_objSizeY;
		int m_objSizeZ;

		int m_objType;

		int m_lastPosX;
		int m_lastPosY;
		int m_lastPosZ;

		bool m_stamped;
		bool isBuilding;
		int m_objSide;
		std::string m_typeName;
		int currentGrid;
		bool exist;
	};

	class Entity {

	public:
		static int count;
		//physics and AI

		Ogre::Vector3 pos;
		Ogre::Vector3 vel;
		Ogre::Vector3 acc;

		Ogre::Vector3 potentialVec;
		double attractivePotential;

		Ogre::Quaternion rot;
		float yaw;
		float desiredSpeed;
		float desiredHeading;
		float speed;
		float maxSpeed;
		float minSpeed;
		float speedRange;
		float heading;
		float maxAcceleration;
		float maxRotationalSpeed;

		float altitude;
		float desiredAltitude;
		float deltaAltitude;
		float climbrate;
		float minAltitude;
		float maxAltitude;

		float turningRadius;

		float length, width, height, depth;
		float mass;

		std::string uiname;
		std::string meshName;

		Engine   *engine;
		Identity entityId;
		//int      idNumber;
		EntityType entityType;   // cigarette, ddg, ...
		EntityClass entityClass;//surface, air, building

		//Game stuff
		//bool     alive;
		EntityState entityState;
		double      gestationPeriod;
		double      timeLeftToBirth;
		double      timeLeftToDie;

		double      hitpoints;
		double      hitpointsmax;

		boost::mutex entLock;

		std::deque<UnitAspect*> aspects;
		//UnitAspect   *aspects[NASPECTTYPES];
		Weapon*      weapon;
		UnitAI*      ai;
		Physics*     physics;
                Builder*     builder;
                NetAspect*   netAspect;

		// selection
		bool isSelected;
		bool selectable;
                bool didSelectSoundPlayed;
                bool isNewBorn;

		//Distance

		/*Command *commands[MaxCommands];
		int     nCommands;
*/
        int seekRange;
        bool isAttacking;
	private:
		void reset();

	protected:
		virtual void init();

	public:
		Entity(Engine *eng, EntityType entType) ;

		UnitAspect *getAspect(UnitAspectType i);
		void        removeAspect(UnitAspectType i);
		void         addAspect(UnitAspect* asp);
		void		switchState(EntityState newState);

		virtual void print();
		virtual void tick();
		//****************************************************************************************************************
		//void(Entity::*tick)(double); // The tick function called depends on entity's state. One of the next four possibilities
//		/TickType tick;
		//****************************************************************************************************************
		virtual void gestatingTick();
		virtual void aliveTick();
		virtual void dyingTick();
		virtual void deadTick();
		//****************************************************************************************************************
		virtual bool raising();
		virtual bool sinking();

		virtual void setVultureProperties();
		virtual void setZealotProperties();

	};

	class Drone : public Entity {
		public:
			Drone(Engine *eng) : Entity(eng, DRONE) {
				//if (entityId.side == RED)
					meshName = "drone.mesh";
				//else
				//	meshName = "ddg51.mesh";
				uiname = "Drone.";
				uiname.append(boost::lexical_cast<std::string>(count++));

				// properties
				length = meters(45.0f);
				width  = meters(10.0f);
				height = meters(12.0f);

				maxSpeed = 6.4f;//knots(64.0f);

				minSpeed = knots(0.0f);
				speedRange = maxSpeed - minSpeed + EPSILON;

				maxAcceleration = 100; //feetPerSecondSquared(55.0f);

				maxRotationalSpeed = degreesPerSecond(9.167f); //degreesPerSecond(170.0f);
				turningRadius = 180;
				mass = tons(200);

				selectable = true;

				entityId.side = YELLOW;

				hitpoints = 80;
				hitpointsmax = 80;

				seekRange = 8*75;

				isAttacking = false;
			}
	};

	class Turret : public Entity {
		public:
			Turret(Engine* eng) : Entity(eng, TURRET) {
				meshName = "turret.mesh";
				uiname = "Turret.";
				uiname.append(boost::lexical_cast<std::string>(count++));

				length = feet(40.0f);
				width = feet(40.0f);
				height = feet(40.0f);

				maxSpeed = knots(0.0f);
				minSpeed = knots(0.0f);
				speedRange = maxSpeed - minSpeed;
				maxAcceleration = feetPerSecondSquared(0.0f);
				maxRotationalSpeed = degreesPerSecond(0.0f);
				turningRadius = 0;

				mass = tons(50);

				selectable = false;
				entityId.side = YELLOW;

				hitpoints = 200;
				hitpointsmax = 200;
				seekRange = 11*75;

				isAttacking = false;
			}
	};


	class Marine : public Entity { //marine

	public:

		Marine(Engine *eng) : Entity(eng, MARINE) {
			meshName = "cigarette.mesh";
			uiname = "Cigarette.";
			uiname.append(boost::lexical_cast<std::string>(count++));


			// properties
			length = meters(20.0f);
			width  = meters(4.0f);
			height = meters(6.0f);

			maxSpeed = knots(45.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;
			maxAcceleration = feetPerSecondSquared(15.0f);
			maxRotationalSpeed = degreesPerSecond(6.5f);
			turningRadius = 10;
			mass = pounds(1100);

			selectable = true;

			entityId.side = RED;

			hitpoints = 50;

		}
	};

	class Reaper : public Entity { // flamer

	public:

		Reaper(Engine *eng) : Entity(eng, REAPER) {
			meshName = "boat.mesh";
			uiname = "SpeedBoat.";
			uiname.append(boost::lexical_cast<std::string>(count++));


			// properties
			length = 10.0f;
			width  = 4.0f;
			height = 6.0f;
			maxSpeed = knots(40.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;
			maxAcceleration = 20.0f;
			maxRotationalSpeed = 2.5f;
			turningRadius = 10;
			mass = pounds(1100);

			selectable = true;

			entityId.side = RED;

			hitpoints = 60;
		}
	};

	class Tank : public Entity { //tank

	public:

		Tank(Engine *eng) : Entity(eng, TANK) {
			meshName = "ddg51.mesh";
			uiname = "DDG51.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = feet(466.0f);
			width  = feet(59.0f);
			height = feet(100.0f);
			maxSpeed = knots(30.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;
			maxAcceleration = feetPerSecondSquared(10.0f);
			maxRotationalSpeed = degreesPerSecond(1.0f);
			turningRadius = 750;
			mass = tons(842);

			selectable = true;

			entityId.side = BLUE;

			hitpoints = 160;
		}
	};


	class Thor : public Entity {

	public:

		Thor(Engine *eng) : Entity(eng, THOR) {
			meshName = "cvn68.mesh";
			uiname = "CVN68.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(332.0f);
			width  = meters(200.0f);
			height = meters(100.0f);

			maxSpeed = knots(30.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;

			maxAcceleration = feetPerSecondSquared(9.0f);
			maxRotationalSpeed = degreesPerSecond(0.5f);
			turningRadius = 100;
			mass = tons(1100);

			selectable = true;

			entityId.side = BLUE;

			hitpoints = 400;
		}
	};

	class Marauder : public Entity {

	public:

		Marauder(Engine *eng) : Entity(eng, MARAUDER) {
			meshName = "sleek.mesh";
			uiname = "Sleek.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(30.0f);
			width  = meters(20.0f);
			height = meters(10.0f);
			maxSpeed = knots(40.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;

			maxAcceleration = feetPerSecondSquared(20.0f);
			maxRotationalSpeed = degreesPerSecond(1.5f);
			turningRadius = 10;
			mass = tons(75);

			selectable = true;

			entityId.side = BLUE;

			hitpoints = 125;

		}
	};

	class Helion : public Entity {

	public:

		Helion(Engine *eng) : Entity(eng, HELLION) {
			meshName = "alienship.mesh";
			uiname = "AlienShip.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(5.0f);
			width  = meters(2.0f);
			height = meters(5.0f);
			maxSpeed = knots(60.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;

			maxAcceleration = feetPerSecondSquared(20.0f);
			maxRotationalSpeed = degreesPerSecond(2.5f);
			turningRadius = 8;
			mass = tons(0.6);

			selectable = true;

			entityId.side = YELLOW;

			hitpoints = 70;

		}
	};
        

	class SCVehicle : public Entity {

	public:

		float mineralCount;
		float mineralOccupyTime;
		float maxMineralCount;
		float mineralGatherRate;

		float gasCount;
		float gasOccupyTime;
		float maxGasCount;
		float gasGatherRate;

		SCVehicle(Engine *eng) : Entity(eng, SCV) {
			meshName = "scv.mesh";
			uiname = "SpaceConstructionVehicle";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(30.0f);
			width  = meters(2.0f);
			height = meters(5.0f);
			maxSpeed = knots(20.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;
			speed = 0.0f;
			desiredSpeed = 0.0f;

			maxAcceleration = feetPerSecondSquared(20.0f);
			maxRotationalSpeed = degreesPerSecond(5.5f);
			turningRadius = 100;
			mass = tons(0.5);

			selectable = true;

			entityId.side = YELLOW;

			// http://wiki.teamliquid.net/starcraft2/Resources
			mineralCount = 0;
			maxMineralCount = 5; //per trip
			mineralOccupyTime = 2.786;
			mineralGatherRate = maxMineralCount/mineralOccupyTime;

			gasCount = 0;
			maxGasCount = 4; // per trip
			gasOccupyTime = 5.981;//1.981;
			gasGatherRate = maxGasCount/gasOccupyTime;
                        
			hitpoints = 50;

            this->init();
                        
		}
        virtual void init();
	};


}

#endif /* ENT_H_ */
