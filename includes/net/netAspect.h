/*
 * netAspect.h
 *
 */

#ifndef NETASPECT_H_
#define NETASPECT_H_

#include <messages.h>
#include <aspect.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <utils.h>

namespace FastEcslent {
	class Entity;

	class NetAspect : public UnitAspect {
		private:
			int id;

			Ogre::Vector3 remoteVel;
			Ogre::Vector3 remotePos;
			Ogre::Vector3 nextRemotePos;
			Ogre::Vector3 nextPos;
			Ogre::Vector3 diffPos;
			Ogre::Vector3 diffPosFrac;

			Ogre::Quaternion remoteQuat;
			Ogre::Quaternion destOrie;
			Ogre::Quaternion srcOrie;
			Ogre::Quaternion finalDestOrie;
			float rotFactor;
			float rotProgress;

			int oldTime;
			float latency;
			float nSteps;

			Ogre::Quaternion qDelta;
			float deltaYaw;

			bool firstTick;

			State statusData;

			int pendingEntityId;

			void initNetworking();
			void updateServer(float newDS, float newDH);
		    bool withinSpeedTolerance(float a, float b);
		    bool withinHeadingTolerance(float a, float b);
		    void squalchOtherClients();
		    bool isValid(State *state);
		    void clearQueue(std::deque<State*> * queue, int size);
		    void setEntityBeingBuilt(Entity * ent);
		    bool checkPendingEntityId();
		    void requestMineralPatchID();
		protected:

		public:
		    NetAspect (Entity* ent, UnitAspectType at): UnitAspect(ent, at) {};

			void init();
			void tick(double dt);
			void lerpPos();
			void lerpRot();
			void updateEcslent(double dt);
			void createEntity(EntityType entType, Ogre::Vector3 pos);
			void squalch();

			void setEntityBeingBuilt(int entID);

			float ds;
			float dh;

			float SPEED_TOLERANCE;
			float HEADING_TOLERANCE;

			std::deque<State *> updateQueue;
	};

}

#endif /* NETASPECT_H_ */
