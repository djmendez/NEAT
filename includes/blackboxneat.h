/*
 * neat.h
 *
 *  Created on: Dec 2, 2017
 *      Author: daniel
 */

#ifndef INCLUDES_BLACKBOXNEAT_H_
#define INCLUDES_BLACKBOXNEAT_H_

#include <const.h>
#include <network.h>
#include <utility>
#include <neatmain.h>
#include <ent.h>

namespace FastEcslent {

	class  BlackBoxNEAT {

	public:
        bool loadFromFile(const char *filename);
        bool isCloseEnough(float distance);
        void applyNEATProcessToGetSpeedAndHeading(Entity *entity);
	private:
        // Neural Net
        NEAT::Network *net;

		// input is number of segments (NEATSegments) * 2 (for each side, friendly/enemy) * 2 (one for units, other average distance)
		// Enemies (BLUE) occupy first half of array, FRiendlies (RED) second half
        double input[NEATSegments*2*2];

        // output: desiredHeading, desiredSpeed
        double output[2];

        // NEAT parameter as to whether we are close enough
        const float distanceIsCloseEnough = 50.0;


        void NEATProcess(); // assumes input has been set, sets output
	};
}


#endif /* INCLUDES_BLACKBOXNEAT_H_ */
