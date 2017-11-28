/* 
 * File:   HealthBar.h
 * Author: chris
 *
 * Created on April 29, 2013, 10:34 AM
 */

#ifndef HEALTHBAR_H
#define	HEALTHBAR_H
#include "Rect.h"

/*! Health bar displays a bar above selected entities, showing how much damage the
 * entity has taken. The health bar is composed of two rectangle, a green rectangle for the health
 * remaining, and a red bar for the health depleted. These two rectangles are connected
 * to form a single rectangle. An object of this class may contain multiple healthbars,
 * one for each time the Draw() function is called. */
class HealthBar
{
public:
    /*!
     * Constructs an object for displaying a set of health bars
     * @param mObj A pointer to an object containing all the vertices of all health bars to draw.
     * @param percent the default percent at which the health bar shoul be "filled"
     * @param startOnLeft switch that determines if the health bar fills from the right or left
     */
    HealthBar(Ogre::ManualObject* mObj, float percent, bool startOnLeft);
    HealthBar(const HealthBar& orig);
    virtual ~HealthBar();

    Rect* fullBar; /*! A green rectangle that represents the "full" part of the bar. */
    Rect* emptyBar; /*! A red rectangle connected to the green rectangle, representing the "empty" part of the bar */
    float percent; /*! Percent of the bar that should be shown as full. */
    bool startOnLeft; /* Switch for if the bar "fills/empties" from left-to-right, or right-to-left. */

    /*!
     * Adds a new HealthBar to be drawn at the given position. The proportion of the red/green 
     * bars is determined by percent.
     * @see Hide
     * @see ClearVirtices
     * @param pos The position at which to draw the health bar.
     */
    void Draw(Ogre::Vector3 pos);

    /*!
     * Makes the health bar objects invisible, without actually destroying the vertices used to draw the rectangles.
     * @see Draw
     * @see ClearVirtices
     */
    void Hide()
    {
        fullBar->Hide();
        emptyBar->Hide();
    };

    /*!
     * Deletes all health bars drawn in an object.
     * @see Hide
     * @see Draw
     */
    void ClearVertices();
private:

};

#endif	/* HEALTHBAR_H */

