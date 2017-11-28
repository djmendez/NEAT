/* 
 * File:   Rect.cpp
 * Author: chris
 * 
 * Created on April 28, 2013, 6:06 PM
 */

#include "Rect.h"

Rect::Rect(float length, float height, Ogre::ColourValue colour, Ogre::ManualObject* mObj) :
length(length), height(height), colour(colour), mObj(mObj)
{
}

Rect::Rect(const Rect& orig)
{
}

Rect::~Rect()
{
}

void Rect::Draw(Ogre::Vector3 pos)
{
    Ogre::Vector3 quad[4];
    quad[0] = Ogre::Vector3(pos.x - (length / 2.0), pos.y, pos.z - (height / 2.0)); // ll
    quad[1] = Ogre::Vector3(pos.x - (length / 2.0), pos.y, pos.z + (height / 2.0)); // ul
    quad[2] = Ogre::Vector3(pos.x + (length / 2.0), pos.y, pos.z + (height / 2.0)); // ur
    quad[3] = Ogre::Vector3(pos.x + (length / 2.0), pos.y, pos.z - (height / 2.0)); // lr

    Ogre::MaterialPtr matptr = Ogre::MaterialManager::getSingleton().create("BaseColoured", "General");
    matptr->load();
    matptr->getBestTechnique()->getPass(0)->setVertexColourTracking(Ogre::TVC_DIFFUSE);
    matptr->getBestTechnique()->getPass(0)->setLightingEnabled(false);

    mObj->begin("BaseColoured", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    mObj->position(quad[0]);
    mObj->colour(colour);
    
    //mObj->textureCoord(1,1);
    mObj->position(quad[1]);
    mObj->colour(colour);
    //mObj->textureCoord(1,0);
    mObj->position(quad[2]);
    mObj->colour(colour);
    //mObj->textureCoord(0,0);
    mObj->position(quad[3]);
    mObj->colour(colour);
    //mObj->textureCoord(0,1);

    mObj->quad(0,1,2,3);
    mObj->end();
}

void Rect::ClearVertices()
{
    mObj->clear();
}