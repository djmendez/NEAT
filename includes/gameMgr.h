/*
 * gameMgr.h
 *
 *  Created on: Jan 9, 2012
 *      Author: sushil
 */

#ifndef GAMEMGR_H_
#define GAMEMGR_H_
#include <const.h>
#include <enums.h>
#include <buildings.h>

namespace FastEcslent {

	typedef struct { //SC2
		int gas;
		int minerals;
	} Resources;

	typedef struct { //SC2
		int minerals;
		int gas;
		int supply; //pop
		int buildTime;
		int HotKey;
		int transportSize;
		int sightRange;


		int hitPoints;
		float armor;
		float damage;
		int   targetPriority;
		float cooldown;
		float range;


		float speed;
		float acc;
		float dec;
		float rotSpeed;
		float collisionRadius;

		bool neededTypes[NENTITYTYPES]; //for (int i=0; i < nNeededTypes; i++) needTypes[i] = BARRACKS;
                bool buildableEntities[NENTITYTYPES];
		int nNeededTypes; // if nNeededTypes == 0 , no prereqs

	} GameEntTypeProperties;


	class Engine;

	class GameMgr : public Mgr {

	private:
		int mineralPatchID;
		bool mineralPatchInit;
		bool initMining;
		std::vector<MineralPatch*> mineralPatches;

	public:
		//hack for search fitness function
		int startingNumberOfTurrets;
		int startingNumberOfDrones;
		double droneCost;
		double turretCost;

		// add NEAT fitness function parameters
		double originalSideDistances;

		Options options;
		//game
		int popCap[NPLAYERS];
		int pop[NPLAYERS];
		int currentPopCap[NPLAYERS];
		int currentEntityCounts[NPLAYERS][NENTITYTYPES];
		Entity *playerEnts[NPLAYERS][PerPlayerMaxEnts];
		int     playerNEnts[NPLAYERS];

		GameEntTypeProperties entTypeData[NENTITYTYPES];
		Resources resources[NPLAYERS];

		GameMgr();
		GameMgr(Engine *eng, Options opts);

		void init();
		virtual void loadLevel();
		void tick();

		bool preReqExists(Player playerId, EntityType entType);
                bool preReqResources(Player playerId, EntityType entType);
                bool preReqEntities(Player playerId, EntityType entType);
                bool consumeResources(Player playerId, EntityType entityType);
                bool returnResources(Player playerId, EntityType entityType);
		bool notExceedPopCap(Identity entId);
		void setupEntityBuildTimes();
		void setupEntitySupply();
                void setupEntityBuildables();

		//FastWater
		void WaterCraft();
		void StarCraft();
		void makeBaseForSidePlayer(Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset);
		void makeArmyForSidePlayer(Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset);
		void makeTerrain();
		std::vector<Entity*> createNEntitiesRadial(EntityType entType, int n, Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset, int yawOffsetMultiplier = 1);
		std::vector<Entity*> createNEntitiesCircle(EntityType entType, int n, Side side, Player player, Ogre::Vector3 center, int gap);

		void createMineralPatch(int n, Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset, int yawOffsetMultiplier = 1);
		void startMining(Side side, Player player);
		Entity* findClosestEntityOfTypeWithinDistance(EntityType entityType, Ogre::Vector3 pos, float distance, Side side, Player player);


	//games
		void game0();
		void tester();
		void clearClient();
                
                void testLevel();

		MineralPatch* getMineralPatch(int id);
		MineralPatch* initMineralPatch();
	};

}


#endif /* GAMEMGR_H_ */
