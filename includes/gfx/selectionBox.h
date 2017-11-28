/*
 * selectionBox.h
 *
 *  Created on: Feb 23, 2012
 *      Author: sushil
 */

#ifndef SELECTIONBOX_H_
#define SELECTIONBOX_H_


#include <OgreManualObject.h>
//#include <OgreRenderQueue.h>

namespace OgreGFX {
	class SelectionBox  : public  Ogre::ManualObject	{
	public :
		void convertAndReadyCorners(float left, float top, float right, float bottom);
		void setCorners(const Ogre::Vector2& topLeft, const Ogre::Vector2& bottomRight);

		virtual ~SelectionBox(void);
		SelectionBox(const Ogre::String& name);



	};
}

#endif /* SELECTIONBOX_H_ */
