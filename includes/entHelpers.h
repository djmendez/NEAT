/*
 * entHelpers.h
 *
 *  Created on: Jan 24, 2012
 *      Author: sushil
 */

#ifndef ENTHELPERS_H_
#define ENTHELPERS_H_

#include <ent.h>

namespace FastEcslent {

	struct EntityLessThan { // for entity map (dictionary) in (selectionMgr)
		inline bool operator() (const Identity& lhs, const Identity& rhs) const {
			return lhs.id < rhs.id;
		}
	};
}

#endif /* ENTHELPERS_H_ */
