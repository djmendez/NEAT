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

		float input[NEATSegments*4];
		float output[2];

		void NEATProcess();
	};
}


#endif /* INCLUDES_NEAT_H_ */
