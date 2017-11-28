/*
 * options.h
 *
 *  Created on: Dec 4, 2011
 *      Author: sushil
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <enums.h>

typedef struct  {
	bool runAsEvaluator;
	bool enableGfx;
	bool enableInteraction;
	bool tacticalAI;

	bool enableNetworking;
	int  networkPort;
	bool isServer;

	long int instanceId;

	bool runDebugTests;
	double speedup;

	FastEcslent::Player player;
	FastEcslent::Side side;

	int gameNumber; //which game?
        
    FastEcslent::LevelType levelType;
    unsigned long int maxFrames;

} Options;


#endif /* OPTIONS_H_ */
