/*
 * ent.h
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#ifndef ENT_SC_H_
#define ENT_SC_H_

#include <ent.h>


namespace FastEcslent {

	class Engine ;
	class UnitAspect ;

	class SC_Marine : public Entity { //marine

	public:
		SC_Marine(Engine *eng) : Entity(eng, SC_MARINE) {
			meshName = "cigarette.mesh";
			uiname = "Marine.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(20.0f);
			width  = meters(4.0f);
			height = meters(6.0f);

			maxSpeed = knots(30.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;
			maxAcceleration = feetPerSecondSquared(35.0f);
			maxRotationalSpeed = degreesPerSecond(70.0f);
			turningRadius = 100;
			mass = pounds(100);

			selectable = true;

			entityId.side = RED;

			hitpoints = 200;
			hitpointsmax = 200;
			seekRange = 600;
		}
	};

	class SC_Tank : public Entity { //tank

	public:
		SC_Tank(Engine *eng) : Entity(eng, SC_TANK) {
			meshName = "cvn68.mesh";
			uiname = "Tank.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = feet(466.0f);
			width  = feet(59.0f);
			height = feet(100.0f);
			maxSpeed = knots(30.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;
			maxAcceleration = feetPerSecondSquared(35.0f);
			maxRotationalSpeed = degreesPerSecond(70.0f);
			turningRadius = 100;
			mass = tons(400);

			selectable = true;

			entityId.side = BLUE;

			hitpoints = 160;
			hitpointsmax = 160;
			seekRange = 600;
		}
	};

	class SC_SCVehicle : public Entity {

	public:

		float mineralCount;
		float mineralOccupyTime;
		float maxMineralCount;
		float mineralGatherRate;

		float gasCount;
		float gasOccupyTime;
		float maxGasCount;
		float gasGatherRate;

		SC_SCVehicle(Engine *eng) : Entity(eng, SC_SCV) {
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
			turningRadius = 750;
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
			hitpointsmax = 50;
			seekRange = 600;
		}
	};

	class SC_Vulture : public Entity {

	public:
		SC_Vulture(Engine *eng) : Entity(eng, SC_VULTURE) {
			meshName = "cigarette.mesh";
			//meshName = "ddg51.mesh";
			uiname = "Vulture.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(45.0f);
			width  = meters(10.0f);
			height = meters(12.0f);

			maxSpeed = knots(64.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;

			maxAcceleration = feetPerSecondSquared(55.0f);
			maxRotationalSpeed = degreesPerSecond(170.0f);
			turningRadius = 180;
			mass = tons(200);

			selectable = true;

			entityId.side = YELLOW;

			hitpoints = 80;
			hitpointsmax = 80;
			seekRange = 256*2;

			isAttacking = false;
		}
	};

    class SC_Zealot : public Entity { //wraith

	public:

		SC_Zealot(Engine *eng) : Entity(eng, SC_ZEALOT) {
			meshName = "banshee.mesh";
			uiname = "Zealot.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(18.0f);
			width  = meters(3.0f);
			height = meters(6.0f);
			maxSpeed = 4.0f;//knots(40.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;
			maxAcceleration = 1;//feetPerSecondSquared(40.0f);
			maxRotationalSpeed =  degreesPerSecond(5.73f);//degreesPerSecond(170.0f);
			turningRadius = 180;
			mass = tons(100);

			selectable = true;

			entityId.side = GREEN;

			hitpoints = 160;
			hitpointsmax = 160;
			seekRange = 10000; //224*2;
		}
	};
}

#endif /* ENT_H_ */
