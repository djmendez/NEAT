/*
 * identity.h
 *
 *  Created on: Mar 3, 2012
 *      Author: sushil
 */

#ifndef IDENTITY_H_
#define IDENTITY_H_

#include <enums.h>


namespace FastEcslent {

	class Identity {

	public:
		int id;
		int instanceId;
		Side side;
		Player player;

		inline bool operator==(const Identity &x) {
			return ((x.id == id) && (x.instanceId == instanceId));
		}
	};

}


#endif /* IDENTITY_H_ */
