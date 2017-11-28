/* 
 * File:   Rect.h
 * Author: chris
 *
 * Created on April 28, 2013, 6:06 PM
 */

#ifndef RECT_H
#define	 RECT_H
#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreMaterial.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTechnique.h>

/*! A basic class for drawing a simple rectangle filled with a single color.*/
class Rect
{
public:
    /*!
     * Constructor
     */
    Rect(float length, float height, Ogre::ColourValue colour, Ogre::ManualObject* mObj);
    Rect(const Rect& orig);
    virtual ~Rect();

    /*!
     * Takes a position and uses width+height to calculate the resulting vertices.
     * Adds these vertices to a manual object to be drawn.
     * @see Hide
     * @see ClearVirtices
     * @param pos The position at which to draw the health bar.
     */
    void Draw(Ogre::Vector3 pos);

        /*!
     * Makes the rectangle invisible, without actually destroying the vertices used to draw the rectangles.
     * @see Draw
     * @see ClearVirtices
     */
    void Hide()
    {
    };
    
        /*!
     * Deletes all the rectangle vertices in the manual object
     * @see Hide
     * @see Draw
     */
    void ClearVertices();

    float length; /*! The length of the rectangle */
    float height; /*! The height of the rectangle */
    Ogre::ColourValue colour; /*! The color of the rectangle */
    Ogre::ManualObject* mObj; /*! Pointer to the object containing the vertices used to draw the rectangle. */

private:

};

#endif	/* RECT_H */

