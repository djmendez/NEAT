/*
 * uiMgr.h
 *
 *  Created on: Feb 18, 2012
 *      Author: sushil
 */

#ifndef UIMGR_H_
#define UIMGR_H_

#include <list>
#include <set>
#include <map>


#include <OgreCamera.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreSceneQuery.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <gfxNode.h>
#include <gfxMgr.h>
#include <cameraMgr.h>

#include <selectionBox.h>

#include <inputHandlers.h>
#include <inputEvents.h>
#include <controlGroupsHandler.h>
#include <minimap.h>

#include <group.h>

#include <const.h>
#include <enums.h>
#include "HealthBar.h"
#include "widgets.h"

namespace OgreGFX {
class GraphicsInteractionManager;
class FEButton;

/*! A singleton class that acts as the primary MouseListener and KeyListener for OIS.
 * This class manages all the mouse and key events, allowing the user to interact
 * with the game by moving the camera, handling mouse clicks on the UI/Buttons,
 * handling mouse clicks on units or in the game world, and using keyboard key
 * presses for macros. Also acts as an Ogre FramListener, for drawing visual
 * representations of the users actions, such as highlighting selected units or
 * buttons under the mouse.
 */
class UIMgr : public GFXMgr, public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener//, public OgreBites::SdkTrayListener
{
public:
    HealthBar bars; /*! The set of health bars to display over units  */

    Ogre::RenderWindow* renderWindow; /*! Pointer to the window used for rendering all graphics */
    Ogre::SceneManager* sceneManager; /*! Pointer to SceneManager that handles all Ogre graphic nodes used for drawing */
	    Ogre::Root *ogreRoot;
    Ogre::Camera* camera; /*! Pointer to camera object, in uiMgr used for getting camera information needed to calculate mouses position in world space. */

    CameraMgr* cameraMgr; /*! Pointer to camera manager, used for handling camera movement and view */

    OIS::InputManager* inputManager; /*! Used for creating and managing input devices, i.e. the mouse and keyboard */
    OIS::Mouse* mouse; /*! Used for getting current information about the mouse state (Buttons up/down, position) */
    OIS::Keyboard* keyboard; /*! Used for getting current information about the keyboard state (Buttons up/down) */
    //OgreBites::SdkTrayManager* mTrayMgr; /* Used for managing default UI elements such as buttons, provided by the OGRE library */

    bool ctrlDown; /*! stores if the ctrl button was pressed since the last frame */
    bool shiftDown; /*! stores if the shift button was pressed since the last frame */
    bool altDown; /*! stores if the alt button was pressed since the last frame */
    OIS::MouseState ms; /*! Stores the mouse state obtained from mouse since previous frame. */
	    Ogre::Vector3 posUnderMouse;
            FEButton* buttonActive; /*! The button that has been pressed and is waiting for a target location/entity from a mouse-click */
            
    bool shutDown; /*! Stores if a shut down request was made in the last frame, so program can exit gracefully as soon as possible. */

		Ogre::RaySceneQuery* raySceneQuery;
    float selectionDistanceSquaredThreshold; /*! Distance limit for detecting if a mouse event should select a nearby unit */
    std::list <OgreGFX::GFXNode*> currentSelections; /*! List of GFXNodes corresponding to currently selected entities */
		boost::mutex selectionUpdateLock;

    SelectionBox* selectionBox; /*! Selection box that is visible when user clicks and drags the mouse */
    Ogre::Vector2 startPos, stopPos; /*! Starting and stopping positions for the selection box */
    Ogre::PlaneBoundedVolumeListSceneQuery* volQuery; /*! Involved in mouse-drag unit selection somehow, unclear. */
    bool selectingNow; /* Switch for detecting if the user in the process of a mouse click and drag, for unit mass selection */
    bool volumeSelectingNow /* Switch for detecting if the user in the process of a mouse click and drag, for unit mass selection */;

    ControlGroupsHandler *controlGroupsHandler; /*! Handles macro for selecting groups of entities */
    Minimap *minimap; /*! Minimap object showing a miniature, to-scale map of the current level the unit positions */
		//std::list<Ogre::MovableObject*> selected;
    Ogre::SceneNode* ghostUnderMouse;
    std::list<std::pair<std::deque<FastEcslent::Command*>, Ogre::SceneNode*> > waitingGhosts;
    
    void CreateGhostUnderMouse(std::string meshName);
    void DestroyGhostUnderMouse(Ogre::SceneNode* node);

    //HealthBar bar;

    /*!
     * Setups up window, mouse and keyboard for handling input. Some configuration
     * information is platform dependant, and currently only detects information
     * for linux.
     */
	    void createInputSystem();

    /*!
     * Updates the mouse state so the mouse is aware of the new clickable
     * area in the window.
     * @param win Pointer to the window to that was resized.
     * @see windowClosed().
     */
	    void windowResized(Ogre::RenderWindow* win);
    /*!
     * Starts the cleanup processes for closing a window.
     * @param win Pointer to the window to that was closed.
     * @see kill().
     */
	    void windowClosed(Ogre::RenderWindow* win);

    /*!
     * Called when a frame event is queued, during this time the game captures
     * the state of the mouse and keyboard inputs.
     * @param evt Reference to the current frame event.
     * @see frameStarted
     * @see frameEnded
     * @return True if the function completed successfully.
     */
	    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    /*!
     * Called when its time for the frame to be drawn. Handles the drawing
     * of entity decorations such has circle when mouseover and filled circle
     * around entity when selected.
     * @param evt Reference to the current frame event.
     * @see frameRenderingQueued
     * @see frameEnded
     * @return True if the function completed successfully.
     */
	    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    /*!
     * Called after a frame as been drawn. Cleans up and deletes all the entity
     * decorations that were drawn in the previous frame, so the next frame can
     * set up what to draw.
     * @see frameStarted
     * @see frameRenderingQueued
     * @param evt Reference to the current frame event.
     * @return True if the function completed successfully.
     */
	    virtual bool frameEnded(const Ogre::FrameEvent& evt);

    /*!
     * Used for testing the Debug Drawer, currently not in use.
     */
	    void DebugDrawTest();

	    // OIS::KeyListener
    /*!
     * OIS::KeyListener, called when a keyboard key is pressed down. Passes the
     * event onto the camera manager as well to handle camera movements.
     * All the keyhandlers registered will be checked against the event for matching
     * key configurations to the current keys down. Functions with matching
     * key configurations will be called.
     * @see keyReleased
     * @param arg Reference to the keyboard event.
     * @return True if the function completed successfully.
     */
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    /*!
     * OIS::KeyListener, called when a keyboard key is released. Passes the
     * event onto the camera manager as well to handle camera movements.
     * Updates variables that store modifier keys to reflect which keys are no longer
     * being held down.
     * @see keyPressed
     * @param arg Reference to the keyboard event.
     * @return True if the function completed successfully.
     */
    virtual bool keyReleased(const OIS::KeyEvent &arg);
	    // OIS::MouseListener
    /*!
     * OIS::MouseListener, called when the mouse is moved. Passes the event to the Tray manager
     * for build-in OGRE buttons and the camera manager for camera movement.
     * Implements the functionality for updating the size of a click-drag selection box.
     * @see mousePressed
     * @see mouseReleased
     * @param arg Reference to the mouse event.
     * @return True if the function completed successfully.
     */
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    /*!
     * OIS::MouseListener, called when the mouse is pressed. Passes the event to the Tray manager
     * for build-in OGRE buttons and the widget manager to detect if the event occurred in the
     * unit command box, to prevent the click from passing to game entities.
     * Calls functions for selecting single/multiple entities or movement/attack,
     * depending on button.
     * @see mouseMoved
     * @see mouseReleased
     * @param arg Reference to the mouse event.
     * @return True if the function completed successfully.
     */
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    /*!
     * OIS::MouseListener, called when the mouse is pressed. Passes the event to the Tray manager
     * for build-in OGRE buttons. Checks all mouse handler functions for a mouse configuration
     * that matches the current mouse state. Functions that match the current mouse state will be called.
     * @see mouseMoved
     * @see mousePressed
     * @param arg Reference to the mouse event.
     * @return True if the function completed successfully.
     */
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    
    virtual void ChangeGhostTextureRBGA(Ogre::SceneNode* ghost,Ogre::ColourValue color);
    virtual Ogre::ColourValue GetGhostTextureRBGA(Ogre::SceneNode* ghost);
    virtual void IncreaseGhostAlpha(Ogre::SceneNode* ghost,int percentIncrease);


	    //Key and mouse handlers handling
    /*!
     * Registers a KeyHandler function with the KeyLister to be called when the modifier and arg parameters in a key event match
     * @see callKeyHandlers
     * @param modifiers A pointer to a list of modifier keys that need to be pressed for the KeyHandler function to be called.
     * @param arg The keyboard key that must be pressed to trigger the KeyHandler function to be called.
     * @param handler A pointer to a KeyHandler function
     * @return True if the function completed successfully.
     */
    bool registerKeyHandler(const std::set<OIS::KeyCode> *modifiers, const OIS::KeyCode arg, OgreGFX::KeyHandler *handler);
    /*!
     * Registers a MouseHandler function with the MouseLister to be called when the modifier and arg parameters in a key event match
     * @see callMouseHandlers
     * @param modifiers A pointer to a list of modifier keys that need to be pressed for the MouseHandler function to be called.
     * @param arg The mouse button that must be pressed to trigger the MouseHandler function to be called.
     * @param handler A pointer to a MouseHandler function
     * @return True if the function completed successfully.
     */
	    bool registerMouseHandler(const std::set<OIS::KeyCode> *modifiers, const OIS::MouseButtonID arg, OgreGFX::MouseHandler *handler);


    /*!
     * Calls all registered key handlers that match the current key event and modifiers
     * @see registerKeyHandler
     * @param arg The key event that occurred in the last frame.
     * @return True if the function completed successfully.
     */
	    bool callKeyHandlers(const OIS::KeyEvent &arg);
    /*!
     * Calls all registered mouse handlers that match the current mouse event and modifiers
     * @see registerMouseHandler
     * @param arg The mouse event that occurred in the last frame.
     * @param id The ID of the mouse button pressed.
     * @return True if the function completed successfully.
     */
	    bool callMouseHandlers(const OIS::MouseEvent &arg, const OIS::MouseButtonID id);

    std::map <std::string, std::list<OgreGFX::KeyHandler *> > keyHandlers; /*! The list of key handler functions that have been registered */
	    //std::map <std::string, OgreGFX::KeyHandler *> keyHandlers;
	    //std::map <std::string, OgreGFX::MouseHandler *> mouseHandlers;
    std::map <std::string, std::list<OgreGFX::MouseHandler *> > mouseHandlers; /*! The list of mouse handler functions that have been registered */


	    //UIMgr(Engine* eng, Ogre::RenderWindow* win, Ogre::SceneManager *sm, Ogre::Camera* cam);
    /*!
     * Constructor for the UI manager, gets pointers to all the managers relevant to displaying and interacting with the graphics.
     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
     * @see ~UIMgr()
     */
    UIMgr(GraphicsInteractionManager *gim); //Engine* eng, Ogre::RenderWindow* win, Ogre::SceneManager *sm, Ogre::Camera* cam);
	    ~UIMgr();
    /*!
     * Destroys and cleans up all the Mgr objects used. Called when the game is exiting.
     */
	    void kill();

    /*!
     * After the game has detected which GFXNodes have been selected, updates the engine
     * by letting the engine know that the corresponding entity objects should also be
     * marked as selected.
     * @see clearSelectionsAndUpdateFEEngine
     */
	    void updateFEEngineWithSelections();
    /*!
     * After the game has detected which GFXNodes have been unselected, updates the engine
     * by letting the engine know that the corresponding entity objects should also be
     * marked as unselected.
     * @see updateFEEngineWithSelections
     */
	    void clearSelectionsAndUpdateFEEngine();

    /*!
     * Used for debugging, prints the list of entity IDs currently selected to the terminal.
     */
	    void printCurrentSelections();

    /*!
     * Determines if a mouse click should select a single GFXNode or no GFXNode, and tells the engine
     * to mark the corresponding entity object as selected.
     * @see handleVolumeSelection
     * @param arg The mouse event that occurred in the last frame.
     * @param id The ID of the mouse button pressed.
     */
    void handleSingleSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    /*!
     * Determines the selection area defined by a mouse being clicked-and-dragged
     * @see handleSingleSelection
     * @see doVolumeSelection
     * @param arg The mouse event that occurred in the last frame.
     * @param id The ID of the mouse button pressed.
     */

    void handleVolumeSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    /*!
     * On a mouse up event, if handleSingleSelect was called in mouse down, 
     * marks GFXNodes within the marked selection as selected, and tells
     * the engine to updated the corresponding entities as selected.
     * @see handleVolumeSelection
     * @param first The start position of the mass-selection mouse click-and-drag.
     * @param second The stop position of the mass-selection mouse click-and-drag.
     */
	    void doVolumeSelection(const Ogre::Vector2 &first, const Ogre::Vector2 &second);
    /*!
     * Adds the GFXNode at the given index to the list of selected GFX nodes
     * @see handleSingleSelection
     * @see handleVolumeSelection
     * @param index Index of the selected GFXNode.
     */
	    void selectEntity(int index);
    /*!
     * Handles the drawing of healthbars and selection circles, according to
     * which GFXNodes are in the selected list.
     * @see drawSelectionCircles
     * @see drawUnderMouseCircle
     */
	    void decorateSelectedEntities();
    /*!
     * Handles the drawing selection circles, according to
     * which GFXNodes are in the selected list.
     * @see decorateSelectedEntities
     * @see drawUnderMouseCircle
     */
	    void drawSelectionCircles();
    /*!
     * Handles the drawing mouse-hover circles, according to
     * which GFXNodes beneath the mouse cursor position.
     * @see decorateSelectedEntities
     * @see drawSelectionCircles
     */
	    void drawUnderMouseCircle();
	    void drawIM();
	    void drawMicro();

	    std::pair<int, Ogre::Vector3> entIndexUnderMouse();
	    std::pair<int, Ogre::Vector3> entIndexUnderMouse(const OIS::MouseEvent &arg);
	    std::pair<int, Ogre::Vector3> entIndexUnderMouse(const OIS::MouseState &arg);

	    void handleCommand(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	    void CommandMaintainAttack(int index);
	    void CommandGatherOrPFMove(int index);
	    void CommandMove(Ogre::Vector3 pos);
	    void CommandDirectMove(Ogre::Vector3 pos);
	    void CommandPotentialFieldsMove(Ogre::Vector3 pos);

	    FastEcslent::Group* groupFromSelections();

	    int rayDistanceForVolume;


	    virtual void initialize();
//    virtual void buttonHit(OgreBites::Button* button);

	private:
	    void checkSetModifiers(const OIS::KeyEvent &arg, bool value);
	    void clearModifiersDown();

    std::pair<bool, Ogre::Vector3> getMouseWorldPos(const OIS::MouseState &arg);

	    std::set<OIS::KeyCode> *makeModifiersSet();
	    std::string modifierKeyHash(const OIS::KeyCode);
	    std::string modifierKeyMouseHash(const OIS::MouseButtonID);


	};

}

#endif /* UIMGR_H_ */
