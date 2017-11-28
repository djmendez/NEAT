/* 
 * File:   HealthBar.cpp
 * Author: chris
 * 
 * Created on April 29, 2013, 10:34 AM
 */

#include "HealthBar.h"
//test commit
HealthBar::HealthBar(Ogre::ManualObject* mObj, float percent = 100.0f, bool startOnLeft = true):
percent(percent), startOnLeft(startOnLeft), fullBar(new Rect(0, 20, Ogre::ColourValue(0,1,0),mObj)),
        emptyBar(new Rect(0, 20, Ogre::ColourValue(1,0,0),mObj))
{
}

HealthBar::HealthBar(const HealthBar& orig)
{
}

HealthBar::~HealthBar()
{
}

void HealthBar::Draw(Ogre::Vector3 pos)
{
    const float length = 200;
    const float edge = pos.x - (length / 2.0);
    //ClearVertices();
    fullBar->length = length * (percent / 100.0f);
    emptyBar->length = length - fullBar->length;
    //float fullStart = pos.x - length + (fullBar->length);
    //float emptyStart = fullStart + (fullBar->length / 2.0) + (emptyBar->length / 2.0);
    float fullStart = edge + (fullBar->length / 2.0);
    float emptyStart = fullStart + (fullBar->length / 2.0) + (emptyBar->length / 2.0);
    fullBar->Draw(Ogre::Vector3(fullStart, pos.y, pos.z));
    emptyBar->Draw(Ogre::Vector3(emptyStart, pos.y, pos.z));    
    
}
//if the mObj is shared between multiple healthbars, clear() resets them all
void HealthBar::ClearVertices()
{
    fullBar->ClearVertices();
    emptyBar->ClearVertices();
}