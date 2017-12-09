/*
 * gameMgr.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: sushil
 */
#include <assert.h>

#include <const.h>
#include <utils.h>
#include <engine.h>
#include <ent.h>
#include <aspect.h>
#include <ai/gather.h>
#include <commandHelp.h>
#include <unitAI.h>

#include <groupAI.h>
#include <flock.h>

#include <gameMgr.h>

#include <levelMgr.h>
#include <OgreVector3.h>
#include <cfloat>
#include "DEBUG.h"

#include <time.h>

//FastEcslent::GameMgr::GameMgr() {
//	gameNumber = 0;
	//reset();
//}

FastEcslent::GameMgr::GameMgr(Engine* engine, Options opts): Mgr(engine) {
	//reset();
	options = opts;

}

void FastEcslent::GameMgr::init() {
}

void FastEcslent::GameMgr::loadLevel(){
	switch (engine->options.gameNumber) {
	case 0:
		game0();
		break;
	case 1:
		WaterCraft();
		break;
	case 2:
		tester();
		break;
    case 3:
        testLevel();
        break;
	case 4:
		StarCraft();
		break;
		
	default:
		tester();
	}
}

bool FastEcslent::GameMgr::notExceedPopCap(Identity entId) {
	Entity *ent = this->engine->entityMgr->ents[entId.id];
	//return (this->pop[ent->entityId.player] + this->entTypeData[ent->entityType].supply <= this->currentPopCap[ent->entityId.player]);
	return true;
}

bool FastEcslent::GameMgr::preReqExists(Player playerId, EntityType entType) {
    if(preReqResources(playerId, entType) && preReqEntities(playerId, entType))
	return true;
    else
        return false;
}

bool FastEcslent::GameMgr::preReqResources(Player playerId, EntityType entType) {
    if(this->resources[playerId].minerals >= this->entTypeData[entType].minerals)
	return true;
    else
        return false;
}

bool FastEcslent::GameMgr::preReqEntities(Player playerId, EntityType entType) {
    //return true; //stubbed out for now
    std::set<EntityType> reqEntTypes;
    for(int i = 0; i < NENTITYTYPES; i++)
    {
        if(this->entTypeData[entType].neededTypes[i])
        {
            reqEntTypes.insert((EntityType)i);
        }
    }
    
    //loop until all required entities have been found, or all living entities have been checked
    for(int i = 0; i < this->engine->entityMgr->nEnts && reqEntTypes.size() > 0; i++)
    {
        if(reqEntTypes.find(this->engine->entityMgr->ents[i]->entityType) != reqEntTypes.end())
            reqEntTypes.erase(reqEntTypes.find(this->engine->entityMgr->ents[i]->entityType)); //found the entity we required, no longer check for it
    }
    
    if(reqEntTypes.size() == 0) //found all the required ents
        return true;
    else
        return false;
}

    bool FastEcslent::GameMgr::consumeResources(Player playerId, EntityType entType) {
        if(this->preReqResources(playerId, entType))
        {
            this->resources[playerId].minerals -= this->entTypeData[entType].minerals;
            this->resources[playerId].gas -= this->entTypeData[entType].gas;
            return true;
        }
        return false;
        
    }
    
    bool FastEcslent::GameMgr::returnResources(Player playerId, EntityType entType) {
        this->resources[playerId].minerals += this->entTypeData[entType].minerals;
        this->resources[playerId].gas += this->entTypeData[entType].gas;
        return true;
    }


void FastEcslent::GameMgr::setupEntityBuildTimes(){
	this->entTypeData[SCV].buildTime = 17;
	this->entTypeData[MARINE].buildTime = 25;
	this->entTypeData[REAPER].buildTime = 45;
	this->entTypeData[TANK].buildTime = 45;
	this->entTypeData[THOR].buildTime = 60;
	this->entTypeData[MARAUDER].buildTime = 30;
	this->entTypeData[HELLION].buildTime = 30;

	this->entTypeData[DRONE].buildTime = 1;
	this->entTypeData[TURRET].buildTime = 1;

	this->entTypeData[COMMANDCENTER].buildTime = 100;
	this->entTypeData[BARRACKS].buildTime = 65;
	this->entTypeData[FACTORY].buildTime = 60;
	this->entTypeData[ARMORY].buildTime = 65;
	this->entTypeData[ENGINEERINGBAY].buildTime = 35;

	this->entTypeData[REFINERY].buildTime = 30;
	this->entTypeData[SUPPLYDEPOT].buildTime = 30;

	this->entTypeData[MINERALS].buildTime = 0;
	this->entTypeData[GAS].buildTime      = 0;
        
        this->entTypeData[SCV].minerals = 50;
	this->entTypeData[MARINE].minerals = 50;
	this->entTypeData[REAPER].minerals = 150;
	this->entTypeData[TANK].minerals = 150;
	this->entTypeData[THOR].minerals = 150;
	this->entTypeData[MARAUDER].minerals = 150;
	this->entTypeData[HELLION].minerals = 150;

	this->entTypeData[DRONE].minerals = 150;
	this->entTypeData[TURRET].minerals = 200;

	this->entTypeData[COMMANDCENTER].minerals = 500;
	this->entTypeData[BARRACKS].minerals = 30;
	this->entTypeData[FACTORY].minerals = 30;
	this->entTypeData[ARMORY].minerals = 30;
	this->entTypeData[ENGINEERINGBAY].minerals = 30;

	this->entTypeData[REFINERY].minerals = 75;
	this->entTypeData[SUPPLYDEPOT].minerals = 75;
}

void FastEcslent::GameMgr::setupEntityBuildables(){
    //init all to false
    for (int i = 0; i < NENTITYTYPES; i++)
    {
        for (int j = 0; j < NENTITYTYPES; j++)
        {
		this->entTypeData[i].buildableEntities[j] = false;
                this->entTypeData[i].neededTypes[j] = false;
        }        
    }
        //enable buildable units
	this->entTypeData[SCV].buildableEntities[BARRACKS] = true;
        this->entTypeData[SCV].buildableEntities[REFINERY] = true;
        this->entTypeData[SCV].buildableEntities[FACTORY] = true;
        this->entTypeData[COMMANDCENTER].buildableEntities[SCV] = true;
        this->entTypeData[BARRACKS].buildableEntities[MARINE] = true;
        this->entTypeData[FACTORY].buildableEntities[HELLION] = true;
        
        this->entTypeData[FACTORY].neededTypes[BARRACKS] = true;
}

void FastEcslent::GameMgr::setupEntitySupply(){
	for (int i = 0; i < NENTITYTYPES; i++){
		this->entTypeData[i].supply = 0;
	}
	this->entTypeData[SCV].supply      = 1;
	this->entTypeData[MARINE].supply   = 1;
	this->entTypeData[REAPER].supply   = 1;
	this->entTypeData[TANK].supply     = 3;
	this->entTypeData[THOR].supply     = 5;
	this->entTypeData[MARAUDER].supply = 2;
	this->entTypeData[HELLION].supply  = 2;

	this->entTypeData[DRONE].supply = 1;
	this->entTypeData[TURRET].supply = 1;
}

void FastEcslent::GameMgr::game0(){
	for(int i = 0; i < NPLAYERS; i++){
		this->popCap[i] = 200;
		this->currentPopCap[i] = 10;
		this->pop[i] = 0;
	}
	setupEntityBuildTimes();

	tester();
}

void FastEcslent::GameMgr::WaterCraft(){
	//Initialize before any entities being created.
	setupEntityBuildTimes();
	setupEntitySupply();
    setupEntityBuildables();

	for(int i = 0; i < NPLAYERS; i++){
		this->popCap[i] = 200;
		this->pop[i]    = 0;
		this->currentPopCap[i] = 10;
		this->resources[i].gas      = 0;
		this->resources[i].minerals = 50;
		this->playerNEnts[i] = 0;
	}

	if(engine->options.isServer){
		this->mineralPatchID = 1;
		this->mineralPatchInit = false;
		this->initMining = false;

		float offset = 3500.0f;
		makeBaseForSidePlayer(RED, ONE, Ogre::Vector3(-offset, 0, -offset), 550.0f, 0.06f);
		makeBaseForSidePlayer(BLUE, THREE, Ogre::Vector3(offset, 0, offset), 550.0f, 0.06f);
		//makeBaseForSidePlayer(YELLOW, THREE, Ogre::Vector3(-offset, 0, offset), 550.0f, 0.06f);
		//makeBaseForSidePlayer(GREEN, FOUR, Ogre::Vector3(offset, 0, -offset), 550.0f, 0.06f);

	}else{
		clearClient();
	}
//  setupEntityBuildTimes();
//	setupEntitySupply();

//	float offset = 3500.0f;
//	makeBaseForSidePlayer(RED, ONE, Ogre::Vector3(-offset, 0, -offset), 550.0f, 0.06f);
//	//makeBaseForSidePlayer(BLUE, TWO, Ogre::Vector3(offset, 0, offset), 550.0f, 0.06f);
//	//makeBaseForSidePlayer(YELLOW, THREE, Ogre::Vector3(-offset, 0, offset), 550.0f, 0.06f);
//	//makeBaseForSidePlayer(RED, FOUR, Ogre::Vector3(offset, 0, -offset), 550.0f, 0.06f);
//	startMining(RED, ONE);
//	startMining(engine->options.side, engine->options.player);
//	startMining(BLUE, THREE);
}

void FastEcslent::GameMgr::StarCraft(){

	droneCost = 150.0;
	turretCost = 150.0;


	setupEntityBuildTimes();
	setupEntitySupply();
    setupEntityBuildables();
	for(int i = 0; i < NPLAYERS; i++){
		this->popCap[i] = 200;
		this->pop[i]    = 0;
		this->currentPopCap[i] = 10;
		this->resources[i].gas      = 0;
		this->resources[i].minerals = 50;
		this->playerNEnts[i] = 0;
	}


	// FOR NEAT GAME:
	// Set up blue side alway at X = Z = 1000
	// Then spread out red (NEAT) side at a radius on diffPoints around the circumference
	int redX, redY = 100, redZ;
	int blueX  = 1000, blueY = 100, blueZ = 1000;
	int diffPoints = 8;
	float radius = 800.0f;

	// find a random starting position
	// DISCUSS WITH AAVAAS -- THIS IS TO MAKE STARTING POSITION TRULY P-RANDOM BUT MEANS WONT BE REPEATABLE
	int randCornerRed;
	srandom(time(NULL));
	randCornerRed = random() % diffPoints;
	//int randCornerBlue = 0;//random() % 8; //0


	float angle;
	angle = ((float)randCornerRed / diffPoints)* (2 * M_PI);

	redX =  blueX + (int) (radius * cos(angle));
	redZ =  blueZ + (int) (radius * sin(angle));

//	float offset = 400.0f;
//	makeArmyForSidePlayer(RED, ONE, Ogre::Vector3(offset, 0, offset), 250.0f, 0.06f);
//	makeArmyForSidePlayer(BLUE, THREE, Ogre::Vector3(offset*4, 0, offset*4), 250.0f, 0.ecto06f);

	originalSideDistances = radius;
	std::cout << "Starting Friendly(RED) side positions at (" << redX << "," <<redZ << ")" << std::endl;
	makeArmyForSidePlayer(RED, ONE, Ogre::Vector3(redX,redY,redZ), 250.0f, 0.06f);
	makeArmyForSidePlayer(BLUE, THREE, Ogre::Vector3(blueX,blueY,blueZ) , 250.0f, 0.06f);

	//makeTerrain();
	//makeBaseForSidePlayer(YELLOW, THREE, Ogre::Vector3(-offset, 0, offset), 550.0f, 0.06f);
	//makeBaseForSidePlayer(GREEN, FOUR, Ogre::Vector3(offset, 0, -offset), 550.0f, 0.06f);

}

void FastEcslent::GameMgr::clearClient(){

}

void FastEcslent::GameMgr::makeBaseForSidePlayer(Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset){
	//create command center
	Entity *ent;
	ent = engine->entityMgr->createEntityForPlayerAndSide(COMMANDCENTER, location, 0.0f, side, player);
	ent->pos = location;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)
	this->currentEntityCounts[player][ent->entityType]++;
	this->playerEnts[player][this->playerNEnts[player]++] = ent;
	ent->entityState = ALIVE;

	//create Minerals
	createMineralPatch(8, NEUTRAL, player, location, offset, yawOffset);
//	createNEntitiesRadial(MINERALS, 8, side, player, location, offset, yawOffset);
	// create Gas
	createNEntitiesRadial(GAS, 2, NEUTRAL, player, location, offset, yawOffset * 5, 2.0f);
	//create SCVS
	createNEntitiesRadial(SCV, 5, side, player, location, offset/3.0f, yawOffset/2.0f);
        
}

void FastEcslent::GameMgr::makeTerrain(){
	for(int i=1;i<13;i++){
		Ogre::Vector3 locfr(i*160, 0, 100);
		Entity* frent = engine->entityMgr->createEntityForPlayerAndSide(MINERALS, locfr, 0, NEUTRAL, FOUR);

		Ogre::Vector3 loclr(i*160, 0, 2000);
		Entity* lrent = engine->entityMgr->createEntityForPlayerAndSide(MINERALS, loclr, 0, NEUTRAL, FOUR);

		Ogre::Vector3 locfc(100, 0, 160*i);
		Entity* fcent = engine->entityMgr->createEntityForPlayerAndSide(MINERALS, locfc, 0, NEUTRAL, FOUR);

		Ogre::Vector3 loclc(2000, 0, 160*i);
		Entity* lcent = engine->entityMgr->createEntityForPlayerAndSide(MINERALS, loclc, 0, NEUTRAL, FOUR);
	}
}

void FastEcslent::GameMgr::makeArmyForSidePlayer(Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset){

	// WILL NEED TO REPLACE THESE HARDCODED VALUES by params
	//startingNumberOfDrones = 80;
	//startingNumberOfTurrets = 5;

	//create Marine
	if(side==RED){
		/*for(int i = 0; i < startingNumberOfDrones; i++)
		{
			location.x = random() % 2048;
			location.y = random() % 2048;
			location.z = random() % 2048;
			//std::cout << location.x << ", " << location.z<<std::endl;

			createNEntitiesCircle(DRONE, 1, side, player, location, 1);
			//createNEntitiesCircle(SC_ZEALOT, 1, side, player, location, 1);
		}*/

		//location.x = random() % 2048;
		//location.y = random() % 2048;
		//location.z = random() % 2048;

		//createNEntitiesCircle(SC_ZEALOT, startingNumberOfTurrets, side, player, location, 5);

		//location.x = 1948;
		//location.z = 1948;

		createNEntitiesCircle(DRONE, engine->options.numUnitsA, side, player, location, 5);
		//createNEntitiesCircle(SC_VULTURE, engine->options.numUnitsA, side, player, location, 5);

//				location.x = 1948;
//				location.z = 1948;
//				createNEntitiesCircle(DRONE, startingNumberOfDrones/2, side, player, location, 5);
//				location.x = 100;
//				location.z = 100;
//				createNEntitiesCircle(DRONE, startingNumberOfDrones/2, side, player, location, 5);
//				location.x = 1848;
//				location.z = 200;
//				createNEntitiesCircle(DRONE, startingNumberOfDrones/4, side, player, location, 5);
//				location.x = 200;
//				location.z = 1848;
//				createNEntitiesCircle(DRONE, startingNumberOfDrones/4, side, player, location, 5);



	}else{
		/*for(int i = 0; i < startingNumberOfTurrets; i++)
		{
			location.x = random() % 2048;
			location.y = random() % 2048;
			location.z = random() % 2048;
			//std::cout << location.x << ", " << location.z<<std::endl;

			//createNEntitiesCircle(SC_ZEALOT, 1, side, player, location, 1);
			createNEntitiesCircle(DRONE, 1, side, player, location, 1);
		}*/

		//location.x = random() % 2048;
		//location.y = random() % 2048;
		//location.z = random() % 2048;

		//createNEntitiesCircle(DRONE, startingNumberOfDrones, side, player, location, 5);

//				location.x = 1024;
//				location.z = 1024;

		createNEntitiesCircle(SC_ZEALOT, engine->options.numUnitsB, side, player, location, 5);

				//		location.x = 1648;
				//		location.z = 1648;
				//		createNEntitiesCircle(SC_ZEALOT, 12, side, player, location, 5);
				//		location.x = 400;
				//		location.z = 400;
				//		createNEntitiesCircle(SC_ZEALOT, 12, side, player, location, 5);
				//		location.x = 1648;
				//		location.z = 400;
				//		createNEntitiesCircle(SC_ZEALOT, 13, side, player, location, 5);
				//		location.x = 400;
				//		location.z = 1648;
				//		createNEntitiesCircle(SC_ZEALOT, 13, side, player, location, 5);

		//createNEntitiesCircle(SC_VULTURE, 1, side, player, location, 5);
		//createNEntitiesCircle(SC_VULTURE, 6, side, player, location,5);
	}
//	createNEntitiesRadial(SC_TANK,    5, side, player, location, offset/1.0f, yawOffset/2.0f);

}


std::vector<FastEcslent::Entity*> FastEcslent::GameMgr::createNEntitiesRadial(EntityType entType, int nEntities, Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset, int yawOffsetMultiplier){
	std::vector<Entity*> ents;
	Entity *ent;
	Ogre::Vector3 entityLocation;
	if(entType == DRONE)
		entityLocation = Ogre::Vector3(location.x, location.y, location.z);
	else
		entityLocation = Ogre::Vector3(location.x, 0, location.z);
	float radius = entityLocation.length();
	radius = radius + fabs(offset);
	float yaw  = atan2(location.z, location.x);
	yaw = yaw - yawOffset*nEntities/2.0f;

	entityLocation.x = cos(yaw) * radius;
	entityLocation.z = sin(yaw) * radius;
	for (int i = 0; i < nEntities; i++) {
		ent = engine->entityMgr->createEntityForPlayerAndSide(entType, entityLocation, yaw, side, player );

		this->pop[player] += this->entTypeData[ent->entityType].supply;
		this->currentEntityCounts[player][ent->entityType]++;
		this->playerEnts[player][this->playerNEnts[player]++] = ent;
		ent->entityState = ALIVE;

		ents.push_back(ent);

		yaw += yawOffset * yawOffsetMultiplier;
//		entityLocation.x = cos(yaw) * radius;
//		entityLocation.z = sin(yaw) * radius;
	}

	return ents;
}

std::vector<FastEcslent::Entity*> FastEcslent::GameMgr::createNEntitiesCircle(EntityType entType, int nEntities, Side side, Player player, Ogre::Vector3 center, int gap){
	std::vector<Entity*> ents;
	Entity *ent;
	Ogre::Vector3 entityLocation(center.x, center.y, center.z);
	float yaw  = atan2(center.z, center.x);
	float radius = 50;
	for (int i = 0; i < nEntities; i++) {
		radius = radius + gap;

		yaw = yaw - 10*nEntities/2.0f;

		entityLocation.x = cos(yaw) * radius+center.x;
		entityLocation.z = sin(yaw) * radius+center.z;

		ent = engine->entityMgr->createEntityForPlayerAndSide(entType, entityLocation, yaw, side, player );

		this->pop[player] += this->entTypeData[ent->entityType].supply;
		this->currentEntityCounts[player][ent->entityType]++;
		this->playerEnts[player][this->playerNEnts[player]++] = ent;
		ent->entityState = ALIVE;

		ents.push_back(ent);

		//yaw += yawOffset * yawOffsetMultiplier;
//		entityLocation.x = cos(yaw) * radius;
//		entityLocation.z = sin(yaw) * radius;
	}

	return ents;
}

void FastEcslent::GameMgr::createMineralPatch(int nEntities, Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset, int yawOffsetMultiplier){
	MineralPatch *mp = new MineralPatch(this->mineralPatchID++);
	mineralPatches.push_back(mp);
	std::vector<Entity*> ents = createNEntitiesRadial(MINERALS, nEntities, side, player, location, offset, yawOffset);
	for(std::vector<Entity*>::iterator i= ents.begin(); i!= ents.end();i++){
		Minerals *m = dynamic_cast<Minerals*>(*i);
		mp->addMineral(m);
		m->mineralPatchId = mp->getID();
	}
}

FastEcslent::Entity *FastEcslent::GameMgr::findClosestEntityOfTypeWithinDistance(EntityType entityType, Ogre::Vector3 pos, float maxDistance, Side side, Player player){
	float minDistance = FLT_MAX;
	float distance;
	Entity *minEnt = 0;
	Entity *ent = 0;
	for (int i = 0; i < this->playerNEnts[player]; i++){
		ent = this->playerEnts[player][i];
		//if (ent->entityType == MINERALS && ent->entityId.side == side && ent->entityId.player == player){
		if (ent->entityType == entityType && ent->entityState == ALIVE){
			DEBUG(std::cout << "Found: " << ent->uiname << ": " << ent->entityState << std::endl;)
			distance = pos.distance(ent->pos);
			if (distance < maxDistance) {
				DEBUG(std::cout << ent->uiname << ", distance: " << distance << std::endl;)
				if (distance < minDistance) {
					minDistance = distance;
					minEnt      = ent;
				}
			}
		}
	}
	return minEnt;
}

void FastEcslent::GameMgr::startMining(Side side, Player player){
	Entity *ent;
	for (int i = 0; i < this->playerNEnts[player]; i++){
		ent = this->playerEnts[player][i];
		if (ent->entityType == SCV ){
			SCVehicle *scv = dynamic_cast<SCVehicle *>(ent);
			Entity *mineral = findClosestEntityOfTypeWithinDistance(MINERALS, scv->pos, FLT_MAX, side, player);
			assert(mineral != 0);
			Gather *g = createGatherForEntAndMineral(scv, mineral);
			UnitAI *ai = dynamic_cast<UnitAI *> (scv->getAspect(UNITAI));
			ai->setCommand(g);
		}
	}
}


void FastEcslent::GameMgr::tester(){
	int x = 0;
	int z = 0;

	int rangex = 2000;
	int rangez = rangex;
	int nEnts  = 50;
	DEBUG(std::cout << "Game2...nEnts: " << nEnts << std::endl;)
	engine->selectionMgr->resetAll();
	//Group *group = engine->groupMgr->createGroup();

	Entity *ent;
	ent = engine->entityMgr->createEntityAfterTime(BARRACKS, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)

	ent = engine->entityMgr->createEntityAfterTime(COMMANDCENTER, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)

	ent = engine->entityMgr->createEntityAfterTime(FACTORY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)

	ent = engine->entityMgr->createEntityAfterTime(REFINERY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)

	ent = engine->entityMgr->createEntityAfterTime(SUPPLYDEPOT, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)

	ent = engine->entityMgr->createEntityAfterTime(ENGINEERINGBAY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)

	ent = engine->entityMgr->createEntityAfterTime(ARMORY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)

	x = 0;
	z = 0;
	for(int i = 0; i < nEnts; i++){
		ent = engine->entityMgr->createEntityAfterTime(static_cast<EntityType>(i%7), Ogre::Vector3(0, 0, 0), 0.0f);
		DEBUG(std::cout << "Game Manager: " << ent->uiname << std::endl;)
		ent->pos = Ogre::Vector3(x, 0.0f, z);
		z = rangez - random() % (2 * rangez);
		x = rangex - random() % (2 * rangex);
		ent->heading = (random()%180) * 0.0174532925;
		ent->yaw = ent->heading;
		ent->desiredHeading = ent->heading;
		ent->desiredSpeed = 0.0f;
	}

}

void FastEcslent::GameMgr::testLevel() {
    setupEntityBuildTimes();
	setupEntitySupply();
    setupEntityBuildables();
    //FastEcslent::Level level = engine->levelMgr->loadLevel(_64x64);
    for(int i = 0; i < NPLAYERS; i++){
		this->popCap[i] = 200;
		this->currentPopCap[i] = 10;
		this->pop[i] = 0;
    }
    
    engine->selectionMgr->resetAll();
    
    Player player = ONE;
    Side side = RED;
    Entity* ent = engine->entityMgr->createEntityForPlayerAndSide(SCV, Ogre::Vector3(0,0,0), 0, side, player);
    ent->entityState = ALIVE;
    
    currentEntityCounts[player][ent->entityType]++;
    playerEnts[player][playerNEnts[player]++] = ent;
    
}

void FastEcslent::GameMgr::tick(){
//	if(!initMining){
//		for(int i = 0;i<engine->entityMgr->nEnts;i++){
//			if(engine->entityMgr->ents[i]->entityType == SCV && engine->entityMgr->ents[i]->speed == 0){
//				initMining = true;
//				startMining(engine->options.side, engine->options.player);
//			}
//		}
//	}
	return;
}

FastEcslent::MineralPatch* FastEcslent::GameMgr:: getMineralPatch(int id){
	if(mineralPatches.size() == 0 && !this->mineralPatchInit){
		this->mineralPatchInit = true;
		initMineralPatch();
	}

	for(std::vector<MineralPatch*>::iterator i = this->mineralPatches.begin(); i != this->mineralPatches.end();i++){
		if((*i)->getID() == id){
			return *i;
		}
	}

	MineralPatch *mp = new MineralPatch(id);
	mineralPatches.push_back(mp);
	return mp;
}


FastEcslent::MineralPatch* FastEcslent::GameMgr:: initMineralPatch(){
	for(int i = 0;i<engine->entityMgr->nEnts;i++){
		if(engine->entityMgr->ents[i]->entityType == MINERALS){
			Minerals * mineral = dynamic_cast<Minerals*>(engine->entityMgr->ents[i]);
			MineralPatch* patch = this->getMineralPatch(mineral->mineralPatchId);
			patch->addMineral(mineral);
		}
	}
}
