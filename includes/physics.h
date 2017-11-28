/*
 * physics.h
 *
 *  Created on: Mar 3, 2012
 *      Author: sushil
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include<aspect.h>

namespace FastEcslent {


	class Entity;

	class Physics: public UnitAspect {

	public:
		float angleDiff;
		float dHeading;
		float timeScaledAcceleration;
		float timeScaledRotationalSpeed;
		float cosYaw;
		float sinYaw;
		void doHelmsman();

	public:
		Physics(Entity *ent, UnitAspectType at): UnitAspect(ent, at){};
		virtual void init() {};//= 0;
		virtual void tick(){};// = 0;
	};


	class Physics2D1: public Physics {

	private:
		void doHelmsman();

	public:
		Physics2D1(Entity* ent, UnitAspectType at): Physics(ent, at) {};
		virtual void init();
		virtual void tick();

	};

	class Physics2D2: public Physics {

	private:
		void doHelmsman();

	public:
		Physics2D2(Entity* ent, UnitAspectType at): Physics(ent, at) {};
		virtual void init();
		virtual void tick();

	};

	class Physics3D: public Physics {
		private:
			void doHelmsman();

		public:
			Physics3D(Entity* ent, UnitAspectType at): Physics(ent, at) {};

			virtual void init();
			virtual void tick();
	};

	class Physics3DR: public Physics {
		private:
			void doHelmsman();

		public:
			Physics3DR(Entity* ent, UnitAspectType at): Physics(ent, at) {};

			virtual void init();
			virtual void tick();
	};
}


#endif /* PHYSICS_H_ */
