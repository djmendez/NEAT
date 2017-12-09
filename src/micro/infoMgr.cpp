
#include<fstream>

#include "micro/infoMgr.h"
#include "engine.h"
#include "micro/InfluenceMap3D.h"
#include "DEBUG.h"

using namespace FastEcslent;
using namespace std;
InfoMgr::InfoMgr(Engine *eng):Mgr(eng) {
	squadmgr_red  = new SquadMgr(engine, this, RED);
	squadmgr_blue = new SquadMgr(engine, this, BLUE);

	redParams  = GA::getInstance()->getRedParams();
	blueParams = GA::getInstance()->getBlueParams();
	//this->maxFrames = 300000;
}

InfoMgr::~InfoMgr(){
	delete eIM_red;
	delete tIM_red;
	delete eIM_blue;
	delete tIM_blue;
	delete squadmgr_red;
	delete squadmgr_blue;
}

IMEnemy3D* InfoMgr::getIMEnemy(Side s){
	if(s == RED) {
		return this->eIM_red;
	}else{
		return this->eIM_blue;
	}
}

void InfoMgr::init(){
	this->frame = 0;
	this->maxFrames = 6000;
	this->positiveFitnessFactor = 1000.0;
	this->dt = 0;

	this->eIM_red  = new IMEnemy3D(RED);
	this->tIM_red  = new IMTerrain(RED);

	this->eIM_blue = new IMEnemy3D(BLUE);
	this->tIM_blue = new IMTerrain(BLUE);

	int sizeX = 64;
	int sizeY = 64;
	int sizeZ = 64;
	int unitSize = 32;

	this->tIM_red->Init(sizeX, sizeZ, sizeX* unitSize, sizeZ*unitSize);
	//this->eIM_red->setTerrainIM(tIM_red->m_map);
	this->eIM_red->Init(sizeX, sizeY, sizeZ, sizeX* unitSize, sizeY*unitSize, sizeZ*unitSize);

	this->tIM_blue->Init(sizeX, sizeZ, sizeX* unitSize, sizeZ*unitSize);
	//this->eIM_blue->setTerrainIM(tIM_blue->m_map);
	this->eIM_blue->Init(sizeX, sizeY, sizeZ, sizeX* unitSize, sizeY*unitSize, sizeZ*unitSize);

}


void InfoMgr::registerEntity(Entity *ent){
	this->tIM_red->RegisterGameObj(ent);
	this->eIM_red->RegisterGameObj(ent);

	this->tIM_blue->RegisterGameObj(ent);
	this->eIM_blue->RegisterGameObj(ent);
}


void InfoMgr::tick(){
	if(this->engine->frames == 7){ // ASK: FIND OUT WHAT THIS DOES, WHAT IS 7?
		//this->tIM_red->Update(dtime);
		//this->eIM_red->setTerrainIM(tIM_red->m_map);
		//this->tIM_blue->Update(dtime);
		//this->eIM_blue->setTerrainIM(tIM_blue->m_map);
	}
	this->eIM_red->Update();
	this->eIM_blue->Update();
	squadmgr_red->onFrame();
	squadmgr_blue->onFrame();

	if(this->engine->frames > 10){
		//check the end criteria
		//		if(this->squadmgr_red->getEnemies().size() <= 0 || this->squadmgr_blue->getEnemies().size() <= 0 || this->getFrameCount() > 300000){
		if(this->squadmgr_red->getEnemies().size() <= 0 || this->squadmgr_blue->getEnemies().size() <= 0 || this->engine->frames > this->maxFrames){
			ptime endTime = getCurrentTime();
			ptime startTime = this->engine->startTime;
			long duration = (endTime - startTime).total_seconds();

			// double score = this->squadmgr_red->getSquadScore();
			// std::cout << score << std::endl;

			cout<< "Game duration: " << duration << " seconds" << endl;

			// if graphics force exit so as not to wait for thread -- else will popup and terminate nicely - eeeeww: yes, i know
			if (this->engine->options.enableGfx)
				exit(0);
			else
				this->engine->quit = true;
			// exit(0);
		}
	}
}
