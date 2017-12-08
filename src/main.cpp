/*
 * main.cpp
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <getopt.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <engine.h>
#include <entityMgr.h>

#include <options.h>
#include <ctime>
#include "micro/IMParams.h"
#include <sstream>
#include <cstdlib>
#include <blackboxneat.h>

using namespace FastEcslent;
using namespace std;

//extern void gfxManager();
pair<IMParams, IMParams> makeParams(int argc, char*argv[]){
	string s = argv[1];
	string delimiter = ",";
	int array[28];

	size_t pos = 0;
	int index = 0;
	while ((pos = s.find(delimiter)) != std::string::npos) {
	    array[index++] = atoi(s.substr(0, pos).c_str());
	    s.erase(0, pos + delimiter.length());
	}
	array[index] = atoi(s.c_str());


	IMParams param1;
	param1.UnitValue      = array[0];
	param1.UnitRadius     = array[1];
	//param1.TerrainValue   = array[2];
	//param1.TerrainRadius  = array[3];

	param1.A = array[2];
	param1.B = array[3];

	param1.m = array[4];
	param1.n = array[5];

	param1.m_freeze      = array[6];
	param1.m_kitingRange = array[7];
	param1.m_targetNearby= array[8];
	param1.m_kitingDist  = array[9];
	param1.m_hpfocus     = array[10];
	param1.m_hpkiting    = array[11];


	IMParams param2;
	param2.UnitValue      = array[12];
	param2.UnitRadius     = array[13];
	//param2.TerrainValue   = array[14];
	//param2.TerrainRadius  = array[15];

	param2.A = array[14];
	param2.B = array[15];

	param2.m = array[16];
	param2.n = array[17];

	param2.m_freeze      = array[18];
	param2.m_kitingRange = array[19];
	param2.m_targetNearby= array[20];
	param2.m_kitingDist  = array[21];
	param2.m_hpfocus     = array[22];
	param2.m_hpkiting    = array[23];

	return pair<IMParams, IMParams> (param1,param2);
}

Options makeOptions(bool enemyTacticalAI,bool enableGfx, int numUnitsA, int numUnitsB, unsigned long int maxFrames){
	Options options;

	options.enableNetworking = false;
	options.networkPort      = 54321;
	options.isServer         = true;

	options.gameNumber = 4;
	options.side = RED;
	options.player = ONE;


	options.runAsEvaluator = true;
	options.instanceId = random();

	options.tacticalAI = true;
	options.enemyTacticalAI = enemyTacticalAI;

	options.enableInteraction = false;

	options.enableGfx = enableGfx;
	options.runDebugTests = false;

	options.speedup = 5; //300.0; //10.0;
    
    options.levelType = _64x64;
    options.maxFrames = maxFrames;

    options.numUnitsA = numUnitsA;
    options.numUnitsB = numUnitsB;

	return options;
}

//should be network, number_units, tacticalAI
int FEEvaluate(int argc, char *argv[], BlackBoxNEAT *neatNet, int numUnitsA, int numUnitsB, unsigned long int maxFrames, bool enemyTacticalAI, bool enableGfx) {


	srandom(1);

	pair<IMParams, IMParams> microparams = makeParams(argc, argv);
	GA::getInstance()->setParams(microparams.first, microparams.second);

	Options options = makeOptions(enemyTacticalAI,enableGfx,numUnitsA,numUnitsB,maxFrames);

	Engine *engine = new Engine(random(), options, neatNet);

	//Construct all the managers
	engine->constructManagers();
	//Initialize all the managers
	engine->init();
	//Launch game (1. gameMgr load game; 2. Run network)
	engine->loadLevel();
	//Run game
	engine->run();
	//Unload game
	engine->releaseLevel();
	//Stop game
	engine->stop();

	double fitness = engine->infoMgr->squadmgr_red->getSquadScore();

	delete engine;

	return fitness;
}


int* loadgameparams(const char *filename) {
    ifstream infile(filename, ios::in);
    std::string line;
    int a,b,c,d,e;
    if (!std::getline(infile, line)) {
        return nullptr;
    }
    std::istringstream iss(line);
    iss >> a;
    iss >> b;
    iss >> c;
    iss >> d;
    iss >> e;
    int* out = new int[5]{a,b,c,d,e};
    infile.close();
    return out;
}

void savefitnesstofile(const char *filename, int fitness){
	std::ofstream outFile(filename);
	outFile << fitness << std::endl;
	outFile.close();
}

int main(int argc, char *argv[]){
//	for (int i= 0; i <5 ;i++){
//		std::cout << m[i] << "," ;
//	}
//	std::cout<<std::endl;
//	BlackBoxNEAT *myneat = new BlackBoxNEAT();
//	myneat->loadFromFile("networkconf");
//	myneat->input[0]= 1;//bias
//	myneat->input[1]= 0;
//	myneat->input[2]= 0;
//	myneat->NEATProcess();
//	std::cout <<"xorout"<< myneat->output[0] <<std::endl;

	//srandom(time(NULL));
	//srandom(atoi(argv[2]));
	//std::cout << atoi(argv[2]) << std::endl;


	double fitness;
	BlackBoxNEAT *neatNet = new BlackBoxNEAT();
	neatNet->loadFromFile("networkconf");
	unsigned long int maxFrames = 10000;

	printf("Starting Main\n");

	// ARGUMENTS
	// argc, argv -- unused, left for legacy
	// neatNet --- pointer to NEATNeuralNetwork NEAT*
	// numUnits side A 	// A are "friendlies", Side RED
	// numUnits side B 	// B are "Enemies", Side BLUE
	// boolean enable enemyTacticalAI FOR ENEMY SIDE - false just leaves enemy stationary
	// boolean enable Graphics
	int* m = loadgameparams("gameparams");
	fitness = FEEvaluate(argc,argv,neatNet,m[0],m[1],m[2],m[3],m[4]);
	cout << "Ending with fitness: " << fitness << endl;

	savefitnesstofile("outfitness", fitness);
	return 0;
}





/*
	NetThread *net;
	if(options.enableNetworking){
		net = new NetThread(engine);
		net->init();
		net->start();
	}

	GfxThread *gfx;
	if(options.enableGfx){
		gfx = new GfxThread(engine);
		gfx->init();
		gfx->start();
	}
	sleep(2.0); //wait for graphics thread to get going, start gets the thread running



	while (!engine->quit) {
		engine->loadLevel();
		engine->run();
	}

	if (options.enableNetworking){
		net->join();
	}

	if (options.enableGfx) {
		//gfx->stop();
		gfx->join();

	}

*/

