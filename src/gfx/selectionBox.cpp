/*
 * selectionBox.cpp
 *
 *  Created on: Feb 23, 2012
 *      Author: sushil
 */


#include <selectionBox.h>

void OgreGFX::SelectionBox::convertAndReadyCorners(float left, float top, float right, float bottom){

	left = left * 2 - 1;
	right = right * 2 - 1;
	top = 1 - top * 2;
	bottom = 1 - bottom * 2;
	clear();
	begin("", Ogre::RenderOperation::OT_LINE_STRIP);
	position(left, top, -1);
	position(right, top, -1);
	position(right, bottom, -1);
	position(left, bottom, -1);
	position(left, top, -1);
	end();
	setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
}

void OgreGFX::SelectionBox::setCorners(const Ogre::Vector2& topLeft, const Ogre::Vector2& bottomRight)
{
	this->convertAndReadyCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

OgreGFX::SelectionBox::SelectionBox(const Ogre::String& name) : Ogre::ManualObject(name){
	Ogre::ManualObject::setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	Ogre::ManualObject::setUseIdentityProjection(true);
	Ogre::ManualObject::setUseIdentityView(true);
	Ogre::ManualObject::setQueryFlags(0);
}

OgreGFX::SelectionBox::~SelectionBox(void){

}
