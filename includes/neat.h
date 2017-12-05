/*
 * neat.h
 *
 *  Created on: Dec 2, 2017
 *      Author: daniel
 */

#ifndef INCLUDES_NEAT_H_
#define INCLUDES_NEAT_H_

#include <const.h>

namespace FastEcslent {

	class NEAT {

	public:

        float input[NEATSegments*2*2]; // input is number of segments (NEATSegments) * 2 (for each side, friendly/enemy) * 2 (one for units, other average distance)
        float output[2]; // output: desiredHeading, desiredSpeed

        void NEATProcess(); // assumes input has been set, sets output

	};
}


#endif /* INCLUDES_NEAT_H_ */
