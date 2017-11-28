/*
 * command.h
 *
 *  Created on: Dec 21, 2011
 *      Author: sushil
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "DEBUG.h"
#include <OgreVector3.h>

#include<target.h>
//#include<ent.h>
#include "enums.h"


namespace FastEcslent {

	enum CommandType {
		MoveCommand     = 0,
		AttackCommand   = 1,
		RamCommand      = 2,
		MaintainCommand = 3,
		GatherCommand   = 4,
        ConstructCommand= 5,

		FLOCK
	};


	enum LeadershipType {
		ClosestToTarget     = 0,
		FurthestFromTarget  = 1,
		MostMassive         = 2,
		LeastMassive        = 3,
		Random              = 4
	};

	class Entity;
	class Group;

	class Command {
	public:
		CommandType commandType /*! The type of the command */;
                Entity *entity; /*! The entity that the command is being issued to */
		Command(CommandType ct,Entity *entity = NULL): entity(entity),commandType(ct) {};
                Command(Command& orig): entity(orig.entity), commandType(orig.commandType) {};

		virtual bool done() = 0;// {return false;}
		virtual void init() = 0;
		virtual void tick() = 0;
		virtual void postProcess() = 0;
                virtual Command* clone() {};
	};
        
        class BuildCommand : public Command {

	public:

            EntityType entType; /*! The type of entity the BuildCommand constructs */
            bool isDone; /*! Stores if the command has completed all it's actions */

		BuildCommand(Entity* entity, EntityType entType, CommandType ct): Command(ct), entType(entType), isDone(false) {this->entity = entity;}
                BuildCommand(BuildCommand& orig): Command(orig.commandType), entType(orig.entType) {}
                virtual bool done();
		virtual void init();
		virtual void tick();
                virtual Command* clone() {return new BuildCommand(*this);}
                virtual void postProcess() {};

	};
        
        class GasFieldCommand : public Command {

	public:

		GasFieldCommand(CommandType ct,Entity *entity): Command(ct,entity) {}
                GasFieldCommand(GasFieldCommand& orig): Command(orig.commandType) {}
                virtual bool done();
		virtual void init();
		virtual void tick();
                virtual Command* clone() {return new GasFieldCommand(*this);}
                virtual void postProcess() {};

	};


	class UnitCommand : public Command {

	public:

		Target *target;
		//Entity *entity;

		UnitCommand(Entity *ent, CommandType ct, Target* targ): Command(ct,ent) {
			//entity = ent;
			target = targ;
		}

		// vars
		Ogre::Vector3 relativePos;
		Ogre::Vector3 relativeVel;
		Ogre::Vector3 predictedPos;
		Ogre::Vector3 interceptPos;

		double predictedTimeToClose;
		double relativeSpeed;
	};


	class Tactic: public Command {
	public:
		GroupTarget* target;
		Group* group;
		Tactic(Group* grp, CommandType ct, GroupTarget* trgt): Command(ct){
			group = grp;
			target = trgt;
		}

		int mostMassive(bool);
		int closestToTarget(bool, Ogre::Vector3 tpos);
		void changeLeadership(LeadershipType selector);

	};
        
//        class BuildStructure: public UnitCommand {
//	private:
//                EntityType entType; /*! The type of entity the BuildCommand constructs */
//		bool valid(Ogre::Vector3 pos){
//			return true;
//		}
//
//	public:
//		BuildStructure (Entity *ent, Target *tgt): UnitCommand(ent, MoveCommand, tgt) {
//			if(valid(tgt->location)) {
//				DEBUG(std::cout << "Moving to build at: " << tgt->location << std::endl;)
//			}
//		}
//		virtual bool done();
//		virtual void init();
//		virtual void tick();
//		virtual void postProcess(){}; //called when the command is interrupted.
//                virtual Command* clone() {};
//	};



	class Move: public UnitCommand {
	private:
		Ogre::Vector3 repulsor;
		float isCollide(Entity *ent);
		Ogre::Vector3 computeRepulsor();
		bool valid(Ogre::Vector3 pos){
			return true;
		}

	public:
		Move (Entity *ent, Target *tgt): UnitCommand(ent, MoveCommand, tgt) {
			if(valid(tgt->location)) {
				DEBUG(std::cout << "Moving to: " << tgt->location << std::endl;)
			}
		}
		virtual bool done();
		virtual void init();
		virtual void tick();
		virtual void postProcess(){}; //called when the command is interrupted.
                virtual Command* clone() {};
	};

	class Move3D: public UnitCommand {
	private:
		Ogre::Vector3 repulsor;
		float isCollide(Entity *ent);
		Ogre::Vector3 computeRepulsor();
		bool valid(Ogre::Vector3 pos){
			return true;
		}

	public:
		Move3D (Entity *ent, Target *tgt): UnitCommand(ent, MoveCommand, tgt) {
			if(valid(tgt->location)) {
				DEBUG(std::cout << "Moving to 3D location: " << tgt->location << std::endl;)
			}
		}
		virtual bool done();
		virtual void init();
		virtual void tick();
		virtual void postProcess(){}; //called when the command is interrupted.
				virtual Command* clone() {};
	};

	//-----------------Wait--------------------------
	class Wait: public UnitCommand { // For SCVs
	private:
		bool valid(Ogre::Vector3 pos){
			return true;
		}

	public:
		Wait (Entity *ent, Target *tgt): UnitCommand(ent, GatherCommand, tgt) {
			timeLeftToWait = tgt->waitTime;
			DEBUG(std::cout << "Waiting for: " << tgt->waitTime << std::endl;)
		}
		bool finished;
		double timeLeftToWait;
		virtual bool done();
		virtual void init();
		virtual void tick();
                void postProcess(){};
                virtual Command* clone() {};
	};

	//---------------END--Gather--------------------------

	class DistanceMgr;
	//-----------------Seek--------------------------
	class Guard: public UnitCommand { // For SCVs
	private:
		DistanceMgr *distanceMgr;
		Move *move;
	public:
		Guard (Entity *ent, Target *tgt): UnitCommand(ent, RamCommand, tgt) {
		}
		bool isGuarding;
		virtual bool done();
		virtual void init();
		void startGuarding();
		void stopGuarding();
		virtual void tick();
		void postProcess(){};
		virtual Command* clone() {};
	};


	class PotentialMove: public UnitCommand {
	private:
		bool valid(Ogre::Vector3 pos){
			return true;
		}
		double A, B, B2, m, n, RepulsionThresholdDistance;

	public:
		PotentialMove (Entity *ent, Target *tgt): UnitCommand(ent, MoveCommand, tgt) {
			if(valid(tgt->location)) {
				DEBUG(std::cout << "Moving to: " << tgt->location << " using Potential Fields" << std::endl;)
				A = 1000.0;
				B = 80000.0;
				B2 = 3000000.0;
				m = 4.0;
				n = 1;
				RepulsionThresholdDistance = 10000;
			}
		}
		virtual bool done();
		virtual void init();
		virtual void tick();
		virtual void postProcess(){};
                virtual Command* clone() {};

	};

	class Potential3DMove: public UnitCommand {
		private:
			bool valid(Ogre::Vector3 pos) {
				return true;
			}

			double A, B, B2, m, n, RepulsionThresholdDistance, updateTime;

		public:
			Potential3DMove(Entity* ent, Target* tgt) : UnitCommand(ent, MoveCommand, tgt) {
				if(valid(tgt->location)) {
					//DEBUG(std::cout << "Moving to: " << tgt->location << " using 3D Potential Field" << std::endl);
//					A = 1000.0;
//					B = 80000.0;
//					B2 = 3000000.0;
//					m = 4.0;
//					n = 1;
					//RepulsionThresholdDistance = 100;
					init();
				}
			}

			virtual bool done();
			virtual void init();
			virtual void tick();
			virtual void postProcess() {};
		};

	class Maintain: public UnitCommand {
	private:
		bool valid (Entity *ent);

	public:
		Maintain (Entity *ent, Target *tgt);
		virtual bool done();

		virtual void init();
		virtual void tick();
		virtual void postProcess(){};
        virtual Command* clone() {};

	};

	class AttackMove: public UnitCommand {
	private:
		Move *move;
		bool valid(Ogre::Vector3 pos){
			return true;
		}
		bool valid(Entity* ent ){
			return ent != NULL;
		}
	public:
		AttackMove (Entity *ent, Target *tgt): UnitCommand(ent, AttackCommand, tgt) {
			if(valid(tgt->entity)) {
				DEBUG(std::cout << "Attack entity: " << tgt->entity << std::endl;)
			}else if(valid(tgt->location)) {
				DEBUG(std::cout << "Attack to: " << tgt->location << std::endl;)
			}
		}
		virtual bool done();
		virtual void init();
		virtual void tick();
		virtual void postProcess(); //called when the command is interrupted.
        virtual Command* clone() {};
	};

	class AttackMove3D: public UnitCommand {
		private:
			Potential3DMove *move;
			bool valid(Ogre::Vector3 pos){
				return true;
			}
			bool valid(Entity* ent ){
				return ent != NULL;
			}
		public:
			AttackMove3D (Entity *ent, Target *tgt): UnitCommand(ent, AttackCommand, tgt) {
				if(valid(tgt->entity)) {
					DEBUG(std::cout << "Attack entity: " << tgt->entity << std::endl;)
				}else if(valid(tgt->location)) {
					DEBUG(std::cout << "Attack to: " << tgt->location << std::endl;)
				}
			}
			virtual bool done();
			virtual void init();
			virtual void tick();
			virtual void postProcess(); //called when the command is interrupted.
	        virtual Command* clone() {};
		};
}



#endif /* COMMAND_H_ */
