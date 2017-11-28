/*
 * widgetMgr.h
 *
 *  Created on: Jan 7, 2013
 *      Author: sushil
 */

#ifndef WIDGETMGR_H_
#define WIDGETMGR_H_

#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreVector2.h>
#include <OgreColourValue.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <gfxMgr.h>
#include <widgets.h>
#include "enums.h"

namespace OgreGFX
{

	class GraphicsInteractionManager;

/*! A singleton class used to manage all the GUI elements displayed to the user. Acts as the mouse listener and key listener,
 * meaning that all mouse and key events are sent directly to this class. This class manages the creation and updating of widgets,
 * and passes mouse events along to widgets such as buttons.
 */
class WidgetMgr : public GFXMgr, public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
	public:
    /*!
     * Constructor for a Label widget, which displays some text.
     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
     * @see ~WidgetMgr()
     */
		WidgetMgr(GraphicsInteractionManager *gim);
		~WidgetMgr();


    /*!
     * OIS::FrameListener method, right only only implements a work around an TextAreaOverlayElement caption not displaying.
     * @param evt Information about the current frame event
     * @see first
     * @return True/False if this method completed successfully with the given FrameEvent.
     */
	    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    /*!
     * OIS::FrameListener method, currently does nothing.
     * @param evt Information about the current frame event
     * @return True/False if this method completed successfully with the given FrameEvent.
     */
	    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    /*!
     * OIS::FrameListener method, currently does nothing.
     * @param evt Information about the current frame event
     * @return True/False if this method completed successfully with the given FrameEvent.
     */
	    virtual bool frameEnded(const Ogre::FrameEvent& evt);


	    // OIS::KeyListener
    /*!
     * OIS::KeyListener method called when any keyboard key is pressed, currently does nothing.
     * @param arg Information about the key event sent.
     * @return True/False if this method completed successfully with the given KeyEvent.
     */
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    // OIS::KeyListener
    /*!
     * OIS::KeyListener method called when any keyboard key is released, currently does nothing.
     * @param arg Information about the key event sent.
     * @return True/False if this method completed successfully with the given KeyEvent.
     */
    virtual bool keyReleased(const OIS::KeyEvent &arg);
	    // OIS::MouseListener
    /*!
     * OIS::MouseListener method called when the mouse moves, currently does nothing.
     * @param arg Information about the mouse event sent.
     * @return True/False if this method completed successfully with the given MouseEvent.
     */
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    /*!
     * OIS::MouseListener method called when a mouse button is pressed, currently does nothing.
     * @param arg Information about the mouse event sent.
     * @param id ID of the mouse button is pressed.
     * @return True/False if this method completed successfully with the given MouseEvent.
     */
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    /*!
     * OIS::MouseListener method called when a mouse button is released, currently does nothing.
     * @param arg Information about the mouse event sent.
     * @param id ID of the mouse button is released.
     * @return True/False if this method completed successfully with the given MouseEvent.
     */
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    /*!
     * Brings up the set of buttons that corresponds to the units currently selected.
     * If no entities are selected or if an invalid entity type is passed, the button menu will be cleared of buttons instead.
     * @param type The type of entity selected. Takes NENTITYTYPE as a value when multiple entities are selected.
     */
    virtual void activateMenu(FastEcslent::Entity* ent);
    /*!
     * Deactivates all buttons in the button menu area. This means that the buttons will not be displayed, and clicking
     * in the button area will not cause the functions of the buttons to occur.
     */
    virtual void deactivateMenu();
    /*!
     * Checks if the mouse is currently in the button menu area.
     * @param arg Information about the mouse event sent.
     * @return True if the mouse event occurred in the button menu area, False otherwise.
     */
    virtual bool mouseOverOverlay(const OIS::MouseEvent &arg);
    /*!
     * Checks if the mouse is currently over a button in the button menu area.
     * @param arg Information about the mouse event sent.
     * @return True if the mouse is hovering over a button in the button menu area, False otherwise.
     */
    virtual bool mouseOverButton(const OIS::MouseEvent &arg);

    int nextId; /*! ID number to give to the next widget object created. Prevents name conflicts. */
	    //std::stringstream idStream; // no need to allocated it every time getNextId is called

    /*!
     * Takes the name of a widget and appends an ID number to the end, to prevent name conflicts. Once the ID number has been appended, increments nextId.
     * @param base The name of the widget.
     * @return Base string + '_' + unique id number
     */
	    std::string getNextId(std::string base);


    /*!
     * Creates and initializes all widget objects. Can't be done in the constructor, since
     * the widgets rely on other Mgr objects to be created first. Initialize() is called
     * after the creation of all Mgr objects.
     */
    virtual void initialize(); // in gfxMgr;


	    //Defaults
    Ogre::Vector2 POS; /*! default position to use for widgets */
    Ogre::Vector2 LABEL_SIZE; /*! Default size of all label widgets */
    Ogre::Vector2 PANEL_SIZE; /*! Default size of all panel widgets */
    Ogre::Vector2 MENU_ITEM_SIZE; /*! Default size of all sub-widgets */
    Ogre::Vector2 BUTTON_SIZE; /*! Default size of all buttons */
    Ogre::Vector2 COMMANDPANEL_POS; /*! Default position of button panel for unit commands */
    Ogre::Vector2 MENU_SIZE; /*! Default size of menu widget */
    Ogre::Vector2 MID_PANEL_SIZE;
    Ogre::Vector2 MID_PANEL_POS;

	    int WIDTH;
	    int HEIGHT;
	    int LABEL_X_OFFSET;
	    int LABEL_Y_OFFSET;

    Ogre::ColourValue LABEL_TEXT_COLOR; /*! Default color or label text */
    Ogre::ColourValue BUTTON_OFF_TEXT_COLOR; /*! Default color for when a button is disabled  */
    Ogre::ColourValue BUTTON_ON_TEXT_COLOR; /*! Default color when a button is enabled  */
	    Ogre::ColourValue MENU_DEFAULT_TEXT_COLOR;
	    Ogre::ColourValue MENU_SELECTED_TEXT_COLOR;


    std::string PANEL_MATERIAL; /*! Path to panel background texture  */
    std::string LINE_SEP_MATERIAL /*! Path to line separator texture  */;
    std::string MENU_MATERIAL; /*! Path to menu background texture  */

    bool first; /*! a bad way to work around an TextAreaOverlayElement caption not displaying */
    UIOutline *uiBase; /*! Displays the background GUI for the minimap, unit info and unit command GUI. */
    FELabel *testLabel; /*! Test able to add to test panel  */
    FELabel *testLabel2; /*! Test able to add to test panel  */
    FELineWrapLabel *buttonDesc; /*! Text Description added to buttonInfo panel  */
    FEPanel *testPanel; /*! Panel used to debug panel class */
    FEPanel *buildProgressPanel; /*! Display info about the current progress of units being built by an entity */
    FEPanel *buttonInfo; /*! Info of button currently under the mouse  */
    FEPanel *scvMenu; /*! The menu to display when only a SCV is selected  */
    FEPanel *cmdcMenu; /*! The menu to display when only a Command Center is selected.  */
    FEPanel *barracksMenu; /*! The menu to display when only a Barracks is selected.  */
    FEPanel *factoryMenu; /*! The menu to display when only a Factory is selected.  */
    FEPanel *multiMenu; /*! The menu to display when multiple entities are selected. */
    FEPanel *currMenu; /*! Pointer to panel currently on display in unit command box */
    FELabelPair *testLabelPair; /*! Pair of labels to test LabelPair class */
    FELabelPair *testLabelPair2; /*! Pair of labels to test LabelPair class */
    FEButton *scvAtk; /*! The button used to Attack in the SCV Menu  */
    FEButton *scvBuildBarracks; /*! The button used to build Barracks in the SCV Menu  */
    FEButton *scvBuildFactory; /*! The button used to build Factories in the SCV Menu  */
    FEButton *scvBuildRefinery; /*! The button used to build Refineries in the SCV Menu  */
    FEButton *scvMove; /*! The button used to Move in the SCV Menu  */
    FEButton *scvGather; /*! The button used to Gather resources in the SCV Menu  */
    FEButton *cmdcBuildSCV; /*! The button used to build SCVs in the Command Center Menu  */
    FEButton *multiAtk; /*! The button used to Attack in the Multi-Menu  */
    FEButton *multiMove; /*! The button used to Move in the Multi-Menu  */
    FEButton *barracksMarine; /*! The button used build Marines in the Barracks Menu */
    FEButton *factoryBuildVulture; /*! The button used build Hellions in the Factory Menu */
    FEButton *selectedUnit; /*! Displays info/img for currently selected unit */
    FELabel *unitName; /*! Displays the name of the currently selected unit  */
    FELabel *unitHealth; /*! Displays the health of the currently selected unit  */
    FEProgressBar* buildProgressEmpty; /*! Display the progress the selected unit has made to build another unit */
    FEButton *buildQueueItem1; /*! Displays info/img for item 1 in the build queue */
    FEButton *buildQueueItem2; /*! Displays info/img for item 2 in the build queue */
    FEButton *buildQueueItem3; /*! Displays info/img for item 3 in the build queue */
    FEButton *buildQueueItem4; /*! Displays info/img for item 4 in the build queue */
    FEButton *buildQueueItem5; /*! Displays info/img for item 5 in the build queue */
    std::vector<FEButton*> buildQueueItems;
    
    //FEButton *buildSCV; /*! New Button to build SCVs in CommandCenter menu  */
	};

}

#endif /* WIDGETMGR_H_ */
