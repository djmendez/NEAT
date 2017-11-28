/*
 * uiMgr.cpp
 *
 *  Created on: Feb 18, 2012
 *      Author: sushil
 */

//using namespace OgreGFX;

#include<cfloat>

#include "DEBUG.h"
#include <engine.h>
#include <uiMgr.h>
#include <GraphicsInteractionManager.h>

#include <DebugDrawer.h>

#include <groupAI.h>
#include <unitAI.h>
#include <flock.h>
#include <target.h>
#include <command.h>

#include <commandHelp.h>
#include <const.h>
#include <enums.h>

#include <ent.h>

#include <levelMgr.h>

#include <creationMouseHandler.h>
#include <controlGroupsHandler.h>
#include "Rect.h"
#include "HealthBar.h"
#include <OgreEntity.h>
#include <OGRE/OgreSubEntity.h>

#include <micro/infoMgr.h>

#include <micro/IMEnemy.h>

using namespace FastEcslent;

OgreGFX::UIMgr::UIMgr(OgreGFX::GraphicsInteractionManager *gim) : GFXMgr(gim), OIS::KeyListener(), OIS::MouseListener(), bars(HealthBar(gim->mSceneMgr->createManualObject("manual"), 100.0f, true)), buttonActive(NULL), ghostUnderMouse(NULL)
{//Engine* eng,  Ogre::RenderWindow *win, Ogre::SceneManager *sm, Ogre::Camera* cam): OgreGFX::Mgr(eng), trayMgr(0), inputManager(0), mouse(0), keyboard(0) {
    shutDown = false;
    selectionDistanceSquaredThreshold = 10000;
    camera = gfx->mCamera;
    renderWindow = gfx->mWindow;
    sceneManager = gfx->mSceneMgr;
    createInputSystem();
    //then
    cameraMgr = new OgreGFX::CameraMgr(gfx); //, camera);

    selectionBox = new OgreGFX::SelectionBox("SelectionBox");
    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(selectionBox);
    volQuery = sceneManager->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
    rayDistanceForVolume = 10;
    selectingNow = false;
    volumeSelectingNow = false;

    clearModifiersDown();
    //currentSelection = gim->gfxNodes[0].node;

    new DebugDrawer(gfx->mSceneMgr, 0.5f);
    this->posUnderMouse = Ogre::Vector3::NEGATIVE_UNIT_Y;
    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(bars.emptyBar->mObj);

}

void OgreGFX::UIMgr::initialize()
{
    DEBUG(std::cout << "Calling uiMgr initializer" << std::endl;)

	mouse->setEventCallback(this);
	keyboard->setEventCallback(this);

    std::set<OIS::KeyCode> *creationMods = new std::set<OIS::KeyCode > ();
    creationMods->insert(OIS::KC_LCONTROL);
    CreationMouseHandler *cmh = new CreationMouseHandler(this->gfx, creationMods);
    this->registerMouseHandler(creationMods, OIS::MB_Left, cmh);
    DEBUG(std::cout << "Registered creation mouse handler" << std::endl;)

    controlGroupsHandler = new ControlGroupsHandler(gfx);

    minimap = new Minimap(gfx);
    //minimap->init();

}

void OgreGFX::UIMgr::clearModifiersDown()
{
    shiftDown = false;
    ctrlDown = false;
    altDown = false;
}

void OgreGFX::UIMgr::createInputSystem()
{
    //Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    DEBUG(std::cout << "*** Initializing OIS ***" << std::endl;)
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    renderWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));


#if defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#elif defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#endif


    inputManager = OIS::InputManager::createInputSystem(pl);

    keyboard = static_cast<OIS::Keyboard*> (inputManager->createInputObject(OIS::OISKeyboard, true));
    mouse = static_cast<OIS::Mouse*> (inputManager->createInputObject(OIS::OISMouse, true));
    mouse->capture();
    ms = mouse->getMouseState();
    ms.width = gfx->mWindow->getWidth();
    ms.height = gfx->mWindow->getHeight();

    //Set initial mouse clipping size
    windowResized(renderWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(renderWindow, this);

    //mRoot->addFrameListener(this); Added in createInputSystem in GraphicsInteractionManager
}
void OgreGFX::UIMgr::CreateGhostUnderMouse(std::string meshName)
{
    if (meshName != "")
    {
                Ogre::Entity* ent = this->gfx->mSceneMgr->createEntity(meshName);
            ent->setVisibilityFlags(OgreGFX::ENT_VISIBILITY_FLAG);
            for(int  i = ent->getNumSubEntities()-1; i >= 0; i--)
            {
                //ent->getSubEntity(i)->getMaterial()->setDepthCheckEnabled(0);
                ent->getSubEntity(i)->setMaterial(ent->getSubEntity(i)->getMaterial()->clone(ent->getSubEntity(i)->getMaterialName() + ent->getName())); //creates a clone of the material, and sets it name to the old material name (non-unique) + the ent name (unique), for independent material alpha oscillation
                ent->getSubEntity(i)->getMaterial()->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
                ent->getSubEntity(i)->getMaterial()->setAmbient(0.5,0.5,0.5);
                ent->getSubEntity(i)->getMaterial()->setDepthWriteEnabled(false);
                ent->getSubEntity(i)->getMaterial()->setDiffuse(0.0,1.0,0.0,0.4);
            }
            ghostUnderMouse = this->gfx->mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));
            ghostUnderMouse->attachObject(ent);
    }
    
}
    void OgreGFX::UIMgr::DestroyGhostUnderMouse(Ogre::SceneNode* node)
    {
        if(node != NULL)
        {
                this->gfx->mSceneMgr->destroyEntity(node->getAttachedObject(0)->getName());
                this->gfx->mSceneMgr->destroySceneNode(node);
                node = NULL;
        }
    }


void OgreGFX::UIMgr::DebugDrawTest()
{
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                Ogre::AxisAlignedBox box(Ogre::Vector3(i * 10.0f + 2.0f, j * 10.0f + 2.0f, k * 10.0f + 2.0f),
                                         Ogre::Vector3((i + 1) * 10.0f - 2.0f, (j + 1) * 10.0f - 2.0f, (k + 1) * 10.0f - 2.0f));
                DebugDrawer::getSingleton().drawCuboid(box.getAllCorners(),
                                                       Ogre::ColourValue(51.0f * i / 255.0f, 51.0f * j / 255.0f, 51.0f * k / 255.0f), true);
            }
        }
    }
    //DebugDrawer::getSingletonPtr()->build();
}

bool OgreGFX::UIMgr::frameStarted(const Ogre::FrameEvent& evt)
{
    //DebugDrawTest();
    //drawSelectionCircles();
    decorateSelectedEntities();
    drawUnderMouseCircle();

    drawIM();
    drawMicro();

    DebugDrawer::getSingletonPtr()->build(); //One build to draw all the debug objects
    DEBUG(std::cout << renderWindow->getAverageFPS() << std::endl;)
    return true;
}

bool OgreGFX::UIMgr::frameEnded(const Ogre::FrameEvent& evt)
{
    DebugDrawer::getSingletonPtr()->clear();
    bars.ClearVertices();
    //	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
    //		FastEcslent::Entity * ent = gfx->engine->entityMgr->ents[(*it)->id];
    //		if (fabs(ent->attractivePotential) < 10 ) {
    //			std::cout << "Potential vector length, attractive Potential: " << ent->potentialVec.length() << ", "
    //					<< ent->attractivePotential << std::endl;
    //		}
    //	}
    return true;
}

bool mouseDidScroll(int z)
{
    return (z < -1.0 || z > 1.0);
}

bool OgreGFX::UIMgr::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if (renderWindow->isClosed())
        return false;

    //Need to capture/update each device
    keyboard->capture();
    mouse->capture();

    cameraMgr->frameRenderingQueued(evt); // if dialog isn't up, then update the camera

    for(std::list<std::pair<std::deque<FastEcslent::Command*>, Ogre::SceneNode*> >::iterator it = waitingGhosts.begin(); it!= waitingGhosts.end(); it++)
    {
        bool commandWaiting = false;
        for(std::deque<FastEcslent::Command*>::iterator c = it->first.begin(); c != it->first.end() && !commandWaiting; c++)
        {
            for(std::deque<FastEcslent::Command*>::iterator entCommands = (*c)->entity->ai->commands.begin(); entCommands != (*c)->entity->ai->commands.end(); entCommands++)
            {
                if((*c) == (*entCommands)) //command ghost is waiting on still exists in the entity
                {
                    commandWaiting = true;
                    break;
                }
            }
            if(!commandWaiting)
            {
                it->first.pop_front();
                break;
            }
        }
        if(it->first.size() == 0)
        {
            DestroyGhostUnderMouse(it->second);
            waitingGhosts.erase(it); //erasing iterator moves it to the following element
            break; //only remove one element at a time
        }
//        if (it->first->ai == NULL || it->first->ai->commands.size() == 0)
//        {
//            DestroyGhostUnderMouse(it->second);
//            waitingGhosts.erase(it); //erasing iterator moves it to the following element
//            break; //only remove one element at a time
//            //it--; //move to prev element, so we will be on the next element when the loop begins
//        }
    }
    
    if(ghostUnderMouse != NULL)
    {
//        Ogre::Entity* ent = this->gfx->mSceneMgr->getEntity(ghostUnderMouse->getAttachedObject(0)->getName());
//    
//        for(int  i = ent->getNumSubEntities()-1; i >= 0; i--)
//                {
//            Ogre::ColourValue color = ent->getSubEntity(i)->getMaterial()->getTechnique(0)->getPass(0)->getDiffuse();
//            color.a = ((Ogre::Real)(( ((int)(color.a * 100)) + 5) % 100)) / 100.0;
//                    ent->getSubEntity(i)->getMaterial()->setDiffuse(color);
//            }
        IncreaseGhostAlpha(ghostUnderMouse,5);
    }
    

    return true;
}

void OgreGFX::UIMgr::ChangeGhostTextureRBGA(Ogre::SceneNode* ghost, Ogre::ColourValue color)
{
    if(ghost != NULL)
    {
    Ogre::Entity* ent = this->gfx->mSceneMgr->getEntity(ghost->getAttachedObject(0)->getName());
    for(int  i = ent->getNumSubEntities()-1; i >= 0; i--)
            {
            //Ogre::ColourValue color = ent->getSubEntity(i)->getMaterial()->getTechnique(0)->getPass(0)->getDiffuse();
            //color.a = ((Ogre::Real)(( ((int)(color.a * 100)) + 5) % 100)) / 100.0;
                    ent->getSubEntity(i)->getMaterial()->setDiffuse(color);
            }
    }
    
}

Ogre::ColourValue OgreGFX::UIMgr::GetGhostTextureRBGA(Ogre::SceneNode* ghost)
{
    if(ghost != NULL)
    {
        return this->gfx->mSceneMgr->getEntity(ghostUnderMouse->getAttachedObject(0)->getName())->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getDiffuse();
    }
    
}

void OgreGFX::UIMgr::IncreaseGhostAlpha(Ogre::SceneNode* ghost, int percentIncrease)
{
    if(ghost != NULL)
    {
    Ogre::ColourValue color = GetGhostTextureRBGA(ghost);
    color.a = ((Ogre::Real)(( ((int)(color.a * 100)) + percentIncrease) % 100)) / 100.0;
    ChangeGhostTextureRBGA(ghost, color);
    }
    
}

void OgreGFX::UIMgr::checkSetModifiers(const OIS::KeyEvent &arg, bool value)
{
    if (arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT)
    {
        shiftDown = value;

    }
    else if (arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL)
    {
        ctrlDown = value;

    }
    else if (arg.key == OIS::KC_LMENU || arg.key == OIS::KC_RMENU)
    {
        altDown = value;

    }

}

bool OgreGFX::UIMgr::keyPressed(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_ESCAPE)
    {
        shutDown = true;
        //return false;
    }
    DEBUG(std::cout << "Key pressed: " << arg.key << std::endl;)
    checkSetModifiers(arg, true);

    //	else if (arg.key == OIS::KC_F12) {
    //buildCubes();
    //}

    // Will need to check if camera related keys were pressed.
    // if a camera related key is pressed:
    cameraMgr->injectKeyDown(arg);
    // else if a registered key combo is pressed:
    //call key handler
    //else
    // do nothing

    callKeyHandlers(arg);
    DEBUG(std::cout << "Control down is: " << ctrlDown << std::endl;)
    controlGroupsHandler->checkHandleControlGroupKeys(ctrlDown, arg.key);

    return true;
}

bool OgreGFX::UIMgr::keyReleased(const OIS::KeyEvent &arg)
{
    checkSetModifiers(arg, false);
    cameraMgr->injectKeyUp(arg);
    return true;
}

bool OgreGFX::UIMgr::mouseMoved(const OIS::MouseEvent &arg)
{
    if (selectingNow)
    {
        volumeSelectingNow = true;
        ms = mouse->getMouseState();
        stopPos.x = ms.X.abs / (float) ms.width;
        stopPos.y = ms.Y.abs / (float) ms.height;
        selectionBox->setCorners(startPos, stopPos);
    }
    else if (mouseDidScroll(arg.state.Z.rel) && !arg.state.buttonDown(OIS::MB_Left) && !arg.state.buttonDown(OIS::MB_Right) && !arg.state.buttonDown(OIS::MB_Middle))
    {
        cameraMgr->handleMouseScrollCameraZoom(arg);
    }
    else if (ghostUnderMouse != NULL)
    {
        Ogre::Vector3 ghostPos = getMouseWorldPos(mouse->getMouseState()).second;
        Ogre::ColourValue ghostColour = GetGhostTextureRBGA(ghostUnderMouse);
        if(this->buttonActive != NULL && this->buttonActive->commandFactory!= NULL && this->buttonActive->commandFactory->requiredTarget == FastEcslent::TargetEntity)
        {
            std::pair<int, Ogre::Vector3> entUnderMouse = this->entIndexUnderMouse();
            if(entUnderMouse.first != -1 && (this->buttonActive->commandFactory->targetEntityType == FastEcslent::NENTITYTYPES || this->buttonActive->commandFactory->targetEntityType == this->gfx->gfxNodes[entUnderMouse.first].entType))
            {
                ghostPos = this->gfx->gfxNodes[entUnderMouse.first].node->getPosition();
                ghostColour = Ogre::ColourValue(0.0,1.0,0.0,ghostColour.a);
            }
            else
            {
                ghostColour = Ogre::ColourValue(1.0,0.0,0.0,ghostColour.a);
            }
        }
        ghostUnderMouse->setPosition(ghostPos);
        ChangeGhostTextureRBGA(ghostUnderMouse, ghostColour);
    }
    //else if ()
    this->gfx->widgetMgr->mouseOverButton(arg);
    return true;
}

//-------------------------Manage Selection ---------------------------------------------

void OgreGFX::UIMgr::decorateSelectedEntities()
{
    Ogre::Vector3 pos;
    Ogre::Vector3 start;
    Ogre::Vector3 end;
    float selectionCircleRadius = 100.0f;
    FastEcslent::Entity* ent;
    int entId;
    for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
    {
        pos = (*it)->node->getPosition();
        Ogre::Vector3 cpos = Ogre::Vector3(pos.x, pos.y + 1, pos.z);
        selectionCircleRadius = gfx->engine->entityMgr->ents[(*it)->id]->length;
        DebugDrawer::getSingleton().drawCircle(cpos, selectionCircleRadius, FastEcslent::NCircleSegments, Ogre::ColourValue(0.5f, 0.5f, 0.9f, 0.2f), true);

        entId = (*it)->id;
        ent = gfx->engine->entityMgr->ents[entId];
        start = ent->pos;

        if (ent->entityClass != FastEcslent::STATIC)
        {
            float lineLength = ent->length * (1.0 + (ent->speed / ent->speedRange));
            DebugDrawer::getSingleton().drawAngleLine(cpos, Ogre::Radian(ent->heading), lineLength, Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0));
            lineLength = ent->length * (1.0 + ent->desiredSpeed / ent->speedRange);
            DebugDrawer::getSingleton().drawAngleLine(cpos, Ogre::Radian(ent->desiredHeading), lineLength, Ogre::ColourValue(1.0f, 1.0f, 1.0f, 0.5));
        }
        Ogre::Vector3 leftPoint = Ogre::Vector3(cpos.x - FastEcslent::healthLineLength / 2, cpos.y, cpos.z - ent->length);
        Ogre::Vector3 rightPoint = Ogre::Vector3(cpos.x + FastEcslent::healthLineLength / 2, cpos.y, cpos.z - ent->length);
        DebugDrawer::getSingleton().drawLine(leftPoint, rightPoint, Ogre::ColourValue(1.0f, 0.0f, 0.0f, 0.5f));
        DEBUG(std::cout << "Hit points: " << ent->hitpoints << " Max Hit Points: " << gfx->engine->weaponMgr->maxHitpoints[ent->entityType] << std::endl;)
        float healthNow = FastEcslent::healthLineLength * ent->hitpoints / gfx->engine->weaponMgr->maxHitpoints[ent->entityType];
        rightPoint.x = leftPoint.x + healthNow;
        DebugDrawer::getSingleton().drawLine(leftPoint, rightPoint, Ogre::ColourValue(0.0f, 1.0f, 0.0f, 0.5f));


        bars.percent = 100.0 * (ent->hitpoints / gfx->engine->weaponMgr->maxHitpoints[ent->entityType]);
        bars.Draw(Ogre::Vector3(pos.x, pos.y + 1, pos.z - selectionCircleRadius));


        // Debug potential fields
        DebugDrawer::getSingleton().drawAngleLine(cpos, Ogre::Radian(ent->potentialVec.angleBetween(Ogre::Vector3::UNIT_X)), ent->potentialVec.length() * 100, Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0));
        DebugDrawer::getSingleton().drawLine(cpos, cpos + (ent->potentialVec * 100), Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0));
    }

}


//void OgreGFX::UIMgr::drawSelectionCircles(){
//	Ogre::Vector3 pos;
//
//	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
//		pos = (*it)->node->getPosition();
//		Ogre::Vector3 cpos = Ogre::Vector3(pos.x, pos.y+1, pos.z);
//		//DebugDrawer::getSingleton().drawCircle((*it)->node->getPosition(), 50.0f, 8, Ogre::ColourValue(255.0f, 255.0f, 255.0f), true );
//		//DebugDrawer::getSingleton().drawCircle(cpos, 50.0f, 8, Ogre::ColourValue(255.0f, 255.0f, 255.0f), true );
//		float radius = gfx->engine->entityMgr->ents[(*it)->id]->length;
//		DebugDrawer::getSingleton().drawCircle(cpos, radius, FastEcslent::NCircleSegments, Ogre::ColourValue(0.0f, 1.0f, 0.0f, 0.5f), true );
//	}
//}

void OgreGFX::UIMgr::drawUnderMouseCircle()
{
    Ogre::Vector3 pos;
    Ogre::Vector3 newPos;
    std::pair<int, Ogre::Vector3> underMouse;
    float radius;
    int id;
    underMouse = entIndexUnderMouse();
    if (underMouse.first >= 0 && underMouse.first < gfx->nGFXNodes)
    {
        DEBUG(std::cout << "Found an ent under mouse" << "\n";)
        id = gfx->gfxNodes[underMouse.first].id;
        pos = gfx->engine->entityMgr->ents[id]->pos;
        newPos = Ogre::Vector3(pos.x, pos.y + 1, pos.z);
        radius = gfx->engine->entityMgr->ents[id]->length;
        DebugDrawer::getSingleton().drawCircle(newPos, radius * 1.5, FastEcslent::NCircleSegments, Ogre::ColourValue(0.0f, 0.0f, 1.0f, 1.0f), false);
    }
}

//TODO turn to 3D representation
void OgreGFX::UIMgr::drawIM()
{
	FastEcslent::IMEnemy3D *im = this->gfx->engine->infoMgr->getIMEnemy(RED);

	/*//draw the grid
	for (int i=0; i< im->m_dataSizeX; i++)
	{
		OgreGFX::DebugDrawer::getSingleton().drawLine(Ogre::Vector3(i*im->m_celResX, 0, 0), Ogre::Vector3(i*im->m_celResX,0, im->m_worldSizeY), Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f));
	}
	for (int i=0; i< im->m_dataSizeY; i++)
	{
		OgreGFX::DebugDrawer::getSingleton().drawLine(Ogre::Vector3(0, 0, i*im->m_celResY), Ogre::Vector3(im->m_worldSizeX,0, i*im->m_celResY), Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f));
	}*/

	//draw grid color
	for(int i=0;i<im->m_numCels;i++)
	{
		//int gridX = i%im->m_dataSizeX;
		//int gridY = i/im->m_dataSizeX;
		int gridX = i / (im->m_dataSizeY * im->m_dataSizeZ) % im->m_dataSizeX;
		int gridY = i / (im->m_dataSizeZ) % im->m_dataSizeX;
		int gridZ = i % im->m_dataSizeZ;

		if(gridY != 1)
			continue;

		/*if(gridY > 3) {
			continue;
		}*/

		int x = gridX*im->m_celResX+im->m_celResX/2;
		int y = gridY*im->m_celResY+im->m_celResY/2;
		//int y = 0;
		int z = gridZ*im->m_celResZ+im->m_celResZ/2;

		if (im->m_map[i] == 0){
			continue;
		}
		Ogre::Vector3 * verties=new Ogre::Vector3[4];
		verties[0] = Ogre::Vector3(x, y, z);
		verties[1] = Ogre::Vector3(x, y, z+im->m_celResZ-10);
		verties[2] = Ogre::Vector3(x+im->m_celResX-10, y, z+im->m_celResZ-10);
		verties[3] = Ogre::Vector3(x+im->m_celResX-10, y, z);

		float cv = (float)(im->m_map[i])/180.0f;
		/*if(cv > 0) {
			std::cout << "Red" << std::endl;
		}*/

		Ogre::ColourValue color = Ogre::ColourValue(cv,0,0,0.25);
		OgreGFX::DebugDrawer::getSingleton().drawQuad(verties, color, true);
		delete verties;
	}

	//draw unit
	std::map<FastEcslent::Entity*, FastEcslent::RegObj> imobjs = im->registeredObjects;
	for (std::map<FastEcslent::Entity*, FastEcslent::RegObj>::iterator i= imobjs.begin(); i!= imobjs.end();i++){
		if(i->first->isSelected){
			OgreGFX::DebugDrawer::getSingleton().drawCircle(i->first->pos, i->first->seekRange, FastEcslent::NCircleSegments*10, Ogre::ColourValue(0.0f, 0.0f, 1.0f, 1.0f), false);
		}
	}
}

void OgreGFX::UIMgr::drawMicro()
{
	//draw my units.
	for(int i=0;i<gfx->engine->entityMgr->nEnts;i++){
		Entity* ent = gfx->engine->entityMgr->ents[i];
		if(ent->entityType == TURRET)
			continue;

		//if(ent->entityId.player != gfx->engine->options.player) continue;
		UnitAI* ai     = static_cast<UnitAI*>(ent->getAspect(UNITAI));
		if(ai->commands.size() > 0){
			Command* comm = ai->commands.front();
			if(comm && comm->commandType == MoveCommand){
				UnitCommand* uc = static_cast<UnitCommand*>(comm);
				Target *target = uc->target;
				DebugDrawer::getSingleton().drawLine(ent->pos, target->location, Ogre::ColourValue(0.0f, 0.0f, 1.0f, 0.5f));
				DebugDrawer::getSingleton().drawCircle(target->location, 5, FastEcslent::NCircleSegments*10, Ogre::ColourValue(0.0f, 0.0f, 1.0f, 1.0f), true);
			}else if(comm && comm->commandType == AttackCommand){
				UnitCommand* uc = static_cast<UnitCommand*>(comm);
				Target *target = uc->target;
				DebugDrawer::getSingleton().drawLine(ent->pos, target->location, Ogre::ColourValue(1.0f, 0.0f, 0.0f, 0.5f));
				DebugDrawer::getSingleton().drawCircle(target->location, 10, FastEcslent::NCircleSegments*10, Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0f), true);
			}
		}
	}
}

std::pair<int, Ogre::Vector3> OgreGFX::UIMgr::entIndexUnderMouse()
{
    OIS::MouseState ms;
    ms = mouse->getMouseState();
    return entIndexUnderMouse(ms);
}

std::pair<int, Ogre::Vector3> OgreGFX::UIMgr::entIndexUnderMouse(const OIS::MouseEvent &arg)
{
    OIS::MouseState ms;
    ms = mouse->getMouseState();
    return entIndexUnderMouse(ms);
}

std::pair<bool, Ogre::Vector3> OgreGFX::UIMgr::getMouseWorldPos(const OIS::MouseState &arg)
{
    Ogre::Vector3 pos = Ogre::Vector3::NEGATIVE_UNIT_Y;
    Ogre::Ray mouseRay = camera->getCameraToViewportRay(arg.X.abs / (float) ms.width, arg.Y.abs / (float) ms.height);

    std::pair<bool, float> result = mouseRay.intersects(gfx->oceanSurface);
    if (result.first)
    {
        pos = mouseRay.getPoint(result.second);
    }
    //result.second = mouseRay.getPoint(result.second);
    return std::pair<bool, Ogre::Vector3 > (result.first, pos);

}

std::pair<int, Ogre::Vector3> OgreGFX::UIMgr::entIndexUnderMouse(const OIS::MouseState &arg)
{

	int index = -1;
    std::pair<bool, Ogre::Vector3> mousePos = getMouseWorldPos(arg);
	//Ogre::Vector3 pos = Ogre::Vector3::NEGATIVE_UNIT_Y;
    Ogre::Ray mouseRay = camera->getCameraToViewportRay(arg.X.abs / (float) ms.width, arg.Y.abs / (float) ms.height);
	//this->raySceneQuery->setRay(mouseRay);
	std::pair<bool, float> result = mouseRay.intersects(gfx->oceanSurface);
    if (result.first)
    {
		this->posUnderMouse = mouseRay.getPoint(result.second);
		//this->posUnderMouse = pos;
		float minDistanceSquared = FLT_MAX;
		float distanceSquared;
        // find ent that is 1. Selectable && 2. Within threshold distance && 3. Nearest to mouse cursor
        for (int i = 0; i < gfx->nGFXNodes; i++)
                {
            if (gfx->gfxNodes[i].selectable)
            {
				distanceSquared = this->posUnderMouse.squaredDistance(gfx->gfxNodes[i].node->getPosition());
                if (distanceSquared < selectionDistanceSquaredThreshold)
                {
                    if (distanceSquared < minDistanceSquared)
                    {
						index = i;
						minDistanceSquared = distanceSquared;
					}
				}
			}
		}
	}
    return std::pair<int, Ogre::Vector3 > (index, this->posUnderMouse);
}

void OgreGFX::UIMgr::selectEntity(int index)
{
    currentSelections.push_back(&gfx->gfxNodes[index]); // add node to empty or non-empty list
    //gfx->gfxNodes[index].node->showBoundingBox(true);   // and show boundingbox
    //gfx->engine->entityMgr->ens[gfxNode.id] is entity
}

void OgreGFX::UIMgr::handleSingleSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

    if (volumeSelectingNow) return;
    
    std::pair<int, Ogre::Vector3> result = entIndexUnderMouse(arg);
    
    if(buttonActive != NULL)
    {
        buttonActive->handleMouseEvent(arg);        
    }
    else
    {
        int index = result.first;
        // Found an ent or none
        if (index >= 0 && index < gfx->nGFXNodes)
        { //if something is near enough the cursor to be selected
            if (!shiftDown)
            { // if it is not to be added to the selection
                clearSelectionsAndUpdateFEEngine(); //clear currently selected
            }
            selectEntity(index);

        }
        else
        { //None: nothing close enough to be selected, so clear selections
            clearSelectionsAndUpdateFEEngine();
        }
        updateFEEngineWithSelections();
    }
}

FastEcslent::Group* OgreGFX::UIMgr::groupFromSelections()
{

    FastEcslent::Entity * ents[FastEcslent::MaxEnts];
    int i = 0;
    for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
    {
        ents[i++] = gfx->engine->entityMgr->ents[(*it)->id];
    }
    FastEcslent::Group* group = gfx->engine->groupMgr->createGroup(ents, i);

    //group->addMember(gfx->engine->entityMgr->ents[(*it)->id]);
    return group;
}

void OgreGFX::UIMgr::handleVolumeSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

    startPos.x = arg.state.X.abs / (float) ms.width;
    startPos.y = arg.state.Y.abs / (float) ms.height;
    stopPos = startPos;
    selectingNow = true;
    selectionBox->clear();
    selectionBox->setVisible(true);
}

void OgreGFX::UIMgr::printCurrentSelections()
{
    for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
    {
        DEBUG(std::cout << (*it)->id << ", ";)
    }
    DEBUG(std::cout << std::endl;)
    //	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); it++){
    //		std::cout << (*it)->id << ", ";
    //	}
    DEBUG(std::cout << std::endl;)
}

void OgreGFX::UIMgr::updateFEEngineWithSelections()
{
    boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
    for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
    {
        gfx->engine->selectionMgr->select((*it)->id);
    }

    if (currentSelections.size() == 0 || gfx->engine->selectionMgr->getPrimarySelection()->entityId.player != gfx->engine->options.player || gfx->engine->selectionMgr->getPrimarySelection()->entityId.side != gfx->engine->options.side)
    {
        gfx->widgetMgr->deactivateMenu();
    }
    else
    {
        gfx->widgetMgr->activateMenu(gfx->engine->selectionMgr->getPrimarySelection());
    }
//    else if (currentSelections.size() == 1)
//    {
//        gfx->widgetMgr->activateMenu(gfx->engine->entityMgr->ents[(*currentSelections.begin())->id]->entityType);
//
//    }
//    else
//    {
//        gfx->widgetMgr->activateMenu(FastEcslent::NENTITYTYPES);
//    }
}

void OgreGFX::UIMgr::clearSelectionsAndUpdateFEEngine()
{
    boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
    for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
    {
        //boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
        gfx->engine->selectionMgr->unselect((*it)->id);
    }

    currentSelections.clear();
}

void swap(float &x, float &y)
{
    float tmp = x;
    x = y;
    y = tmp;
}

void OgreGFX::UIMgr::doVolumeSelection(const Ogre::Vector2& first, const Ogre::Vector2& second)
{

    float left = first.x, right = second.x, top = first.y, bottom = second.y;
    if (left > right) swap(left, right);
    if (top > bottom) swap(top, bottom);
    if ((right - left) * (bottom - top) < 0.01) return;

    Ogre::Ray topLeft = camera->getCameraToViewportRay(left, top);
    Ogre::Ray topRight = camera->getCameraToViewportRay(right, top);
    Ogre::Ray bottomLeft = camera->getCameraToViewportRay(left, bottom);
    Ogre::Ray bottomRight = camera->getCameraToViewportRay(right, bottom);

    Ogre::PlaneBoundedVolume vol;
    vol.planes.push_back(Ogre::Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3))); //front (closest to cam) plane
    vol.planes.push_back(Ogre::Plane(topLeft.getOrigin(), topLeft.getPoint(10), topRight.getPoint(10))); //top plane
    vol.planes.push_back(Ogre::Plane(topLeft.getOrigin(), bottomLeft.getPoint(10), topLeft.getPoint(10))); //left plane
    vol.planes.push_back(Ogre::Plane(bottomLeft.getOrigin(), bottomRight.getPoint(10), bottomLeft.getPoint(10))); //bottom Plane
    vol.planes.push_back(Ogre::Plane(topRight.getOrigin(), topRight.getPoint(10), bottomRight.getPoint(10))); //right Plane

    Ogre::PlaneBoundedVolumeList volList;
    volList.push_back(vol);

    volQuery->setVolumes(volList);
    Ogre::SceneQueryResult result = volQuery->execute();
    clearSelectionsAndUpdateFEEngine();

    Ogre::SceneQueryResultMovableList::iterator iter; // for some reason the last sceneNode is always wrong
    int n = result.movables.size(); //have to throw away last item in iterator. I don't know why?
    //	int i = 0;                      //ToDo -> fix iterator, why is it n-1?
    // The issue is std::map with returns an iterator that points to the end of the map if the key is not in the map.
    //Causes all kinds of strange behavior
    DEBUG(std::cout << "Number of movables: " << n << std::endl;)

    for (iter = result.movables.begin(); iter != result.movables.end(); ++iter)
    {
        //for(iter = result.movables.begin(); iter != result.movables.end(); ++iter){
        //std::cout << gfx->sceneNodeToEntIdMap[(*iter)->getParentSceneNode()] << std::endl;
        if (gfx->sceneNodeToEntIdMap.find((*iter)->getParentSceneNode()) != gfx->sceneNodeToEntIdMap.end())
        {
            selectEntity(gfx->sceneNodeToEntIdMap[(*iter)->getParentSceneNode()]); //why parent scene node?
        }
    }
    updateFEEngineWithSelections();
}
//-------------------------End Manage Selection ---------------------------------------------

//------------------------Handle Commands------------------------------------------------

void OgreGFX::UIMgr::CommandMaintainAttack(int i)
{
    DEBUG(std::cout << "MaintainAttacking: (" << gfx->engine->entityMgr->ents[i]->uiname << ")" << std::endl;)
}

void OgreGFX::UIMgr::CommandGatherOrPFMove(int i)
{
    DEBUG(std::cout << "Gathing (" << gfx->engine->entityMgr->ents[i]->uiname << ")" << std::endl;)
	FastEcslent::Entity * ent;
	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
	{
		ent = gfx->engine->entityMgr->ents[(*it)->id];
		if (ent->entityClass == FastEcslent::STATIC)   //buildings, continue
		{
			continue;
		}else if (ent->entityType == FastEcslent::SCV){//scvs, gathering
			if(gfx->engine->entityMgr->ents[i]->entityType == FastEcslent::REFINERY){
				FastEcslent::Refinery* ref = dynamic_cast<FastEcslent::Refinery*>(gfx->engine->entityMgr->ents[i]);
				FastEcslent::setGatherForEnt(ent, ref->associatedOilField, shiftDown);
			}else{
				FastEcslent::setGatherForEnt(ent, gfx->engine->entityMgr->ents[i], shiftDown);
			}
		}else {  //units other than scv, move to the mineral
			FastEcslent::setPotentialMoveForEnt(ent, gfx->engine->entityMgr->ents[i]->pos, shiftDown);
		}
	}
}

void OgreGFX::UIMgr::CommandPotentialFieldsMove(Ogre::Vector3 pos)
{
    if(!gfx->engine->levelMgr->positionWithinLevel(pos))
        return;
    
    DEBUG(std::cout << "Potential Fields: Moving to: (" << pos.x << ", " << pos.z << ")" << std::endl;)
    FastEcslent::Entity * ent;
    for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
    {
        ent = gfx->engine->entityMgr->ents[(*it)->id];
        if (ent->entityClass == FastEcslent::STATIC)
        {
            continue;
        }
        FastEcslent::setPotentialMoveForEnt(ent, pos, shiftDown);
    }
}

void OgreGFX::UIMgr::CommandDirectMove(Ogre::Vector3 pos)
{
	if(!gfx->engine->levelMgr->positionWithinLevel(pos))
	        return;

    DEBUG(std::cout << "Potential Fields: Moving to: (" << pos.x << ", " << pos.z << ")" << std::endl;)
    FastEcslent::Entity * ent;
    for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it)
    {
        ent = gfx->engine->entityMgr->ents[(*it)->id];
        if (ent->entityClass == FastEcslent::STATIC)
        {
            continue;
        }
        FastEcslent::setDirectMoveForEnt(ent, pos, shiftDown);
    }
}

void OgreGFX::UIMgr::CommandMove(Ogre::Vector3 pos)
{
    DEBUG(std::cout << "Moving to: (" << pos.x << ", " << pos.z << ")" << std::endl;)

    if (currentSelections.size() <= 0) return;

    if (currentSelections.size() > 1)
    {
        FastEcslent::Group* group = groupFromSelections();
        for (int i = 0; i < group->nEntitiesInGroup; i++)
        {
            group->members[i]->ai->commands.clear();
        }
        group->setLeaderByIndex(0);
        FastEcslent::GroupAI* gai = new FastEcslent::GroupAI(group, FastEcslent::GROUPAI);
        FastEcslent::GroupTarget* gt = new FastEcslent::GroupTarget;
        gt->target.location = pos;
        FastEcslent::Flock* flock = new FastEcslent::Flock(group, gt);
        //flock->changeLeadership(FastEcslent::MostMassive);
        DEBUG(std::cout << "Left Shift: " << shiftDown << std::endl;)
        if (shiftDown)
        {
            DEBUG(std::cout << "Adding flock" << std::endl;)
            FastEcslent::addMoveForEnt(gfx->engine->entityMgr->ents[currentSelections.front()->id], pos); //commandHelper
            //gai->addCommand(flock);
        }
        else
        {
            FastEcslent::setMoveForEnt(gfx->engine->entityMgr->ents[currentSelections.front()->id], pos); //commandHelper
            //gai->setCommand(flock);
        }
        group->addAspect(gai);
    }
    else
    {
        FastEcslent::Entity * ent = gfx->engine->entityMgr->ents[currentSelections.front()->id];
        boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
        FastEcslent::setPotentialMoveForEnt(ent, pos, shiftDown);
    }
}

void OgreGFX::UIMgr::handleCommand(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    std::pair<int, Ogre::Vector3> result = entIndexUnderMouse(arg);
    int index = result.first;

    if (index >= 0 && index < gfx->nGFXNodes)
    { //do_maintain()
    	if(gfx->engine->entityMgr->ents[index]->entityType == FastEcslent::MINERALS ||
    			gfx->engine->entityMgr->ents[index]->entityType == FastEcslent::REFINERY){
    		CommandGatherOrPFMove(index);
    	}else{
    		CommandMaintainAttack(index);
    	}
    }
    else
    {
        CommandPotentialFieldsMove(result.second);
    	//this->CommandDirectMove(result.second);
    }
    DEBUG(std::cout << "MousePos: " << getMouseWorldPos(arg.state).second << std::endl;)
    DEBUG(std::cout << "Game State: ";)
    for (int i = 0; i < this->gfx->engine->entityMgr->nEnts; i++)
    {
        DEBUG(std::cout << this->gfx->engine->entityMgr->ents[i]->entityType << "," << this->gfx->engine->entityMgr->ents[i]->pos.x << "," << this->gfx->engine->entityMgr->ents[i]->pos.y << "," << this->gfx->engine->entityMgr->ents[i]->pos.z << " || ";)

    }
    DEBUG(std::cout << std::endl;)

}
//------------------------Handle Mouse Commands------------------------------------------------

bool OgreGFX::UIMgr::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    DEBUG(std::cout << "Input System: Mouse Pressed (" << arg.state.X.abs << ", " << arg.state.Y.abs << ")" << std::endl;)
    if (this->gfx->widgetMgr->mouseOverOverlay(arg)) return true;
    if (id == OIS::MB_Left)
    { //select
        handleSingleSelection(arg, id);
        handleVolumeSelection(arg, id);
    }
    else if (id == OIS::MB_Right)
    {
        handleCommand(arg, id);
    }
    //callMouseHandlers(arg, id);
    return true;
}

bool OgreGFX::UIMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        doVolumeSelection(startPos, stopPos);
        selectingNow = false;
        volumeSelectingNow = false;
        selectionBox->setVisible(false);
    }
    callMouseHandlers(arg, id);
    return true;
}

//----------------------------------------Handle key registration-----------------------------------

bool OgreGFX::UIMgr::registerKeyHandler(const std::set<OIS::KeyCode> *modifiers, const OIS::KeyCode key, OgreGFX::KeyHandler *handler)
{
    OgreGFX::KeySet keySet = OgreGFX::KeySet(modifiers, key);
    std::string keySetString = keySet.toString(); //convertKeySetToString(keySet);
    //keyHandlers[keySetString] = handler; // Map should make it easier to select handler to run
    keyHandlers[keySetString].push_back(handler); // Map should make it easier to select handler to run
    return true;
}

bool OgreGFX::UIMgr::registerMouseHandler(const std::set<OIS::KeyCode> *modifiers, const OIS::MouseButtonID buttonId, OgreGFX::MouseHandler *handler)
{
    OgreGFX::MouseButtonKeySet mbKeySet = OgreGFX::MouseButtonKeySet(modifiers, buttonId);
    std::string mouseButtonKeySetString = mbKeySet.toString(); //convertMouseButtonKeySetToString(mbKeySet);
    //mouseHandlers[mouseButtonKeySetString] = handler;
    mouseHandlers[mouseButtonKeySetString].push_back(handler);
    return true;
}

// --------------------------------------Call key/mouse Handlers-------------------------------------------

std::set<OIS::KeyCode> *OgreGFX::UIMgr::makeModifiersSet()
{
    std::set<OIS::KeyCode> *mods = new std::set<OIS::KeyCode > ();
    if (shiftDown)
        mods->insert(OIS::KC_LSHIFT);
    if (altDown)
        mods->insert(OIS::KC_LMENU);
    if (ctrlDown)
        mods->insert(OIS::KC_LCONTROL);
    return mods;
}

std::string OgreGFX::UIMgr::modifierKeyHash(const OIS::KeyCode keyCode)
{

    std::set<OIS::KeyCode> *mods = makeModifiersSet();
    OgreGFX::KeySet ks = OgreGFX::KeySet(mods, keyCode);
    return ks.toString();
}

std::string OgreGFX::UIMgr::modifierKeyMouseHash(const OIS::MouseButtonID id)
{

    std::set<OIS::KeyCode> *mods = makeModifiersSet();
    OgreGFX::MouseButtonKeySet mbks = OgreGFX::MouseButtonKeySet(mods, id);
    return mbks.toString();
}

bool OgreGFX::UIMgr::callKeyHandlers(const OIS::KeyEvent &arg)
{

    std::string keySetHash = modifierKeyHash(arg.key);
    if (this->keyHandlers.count(keySetHash) > 0)
    {
        for (std::list<KeyHandler *>::iterator khi = this->keyHandlers[keySetHash].begin(); khi != this->keyHandlers[keySetHash].end(); ++khi)
        {
            (*khi)->handleKeyEvent();
        }
        //this->keyHandlers[keySetHash]->handleKeyEvent();
        return true;
    }
    return false;
}

bool OgreGFX::UIMgr::callMouseHandlers(const OIS::MouseEvent &arg, const OIS::MouseButtonID id)
{

    std::string mouseButtonKeySetHash = modifierKeyMouseHash(id);
    if (this->mouseHandlers.count(mouseButtonKeySetHash) > 0)
    {
        for (std::list<MouseHandler *>::iterator mhi = this->mouseHandlers[mouseButtonKeySetHash].begin(); mhi != this->mouseHandlers[mouseButtonKeySetHash].end(); ++mhi)
        {
            (*mhi)->handleMouseEvent(arg);
        }
        //this->mouseHandlers[mouseButtonKeySetHash]->handleMouseEvent(arg);
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------------------

//void OgreGFX::UIMgr::createNewEntity(FastEcslent::Identity id){
//
//}

//--------------------------------------------------------------------------------------------------

//Adjust mouse clipping area

void OgreGFX::UIMgr::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

OgreGFX::UIMgr::~UIMgr()
{

    kill();
}

void OgreGFX::UIMgr::kill()
{
    //sceneManager->destroyQuery(volQuery);



    if (selectionBox)
    {
        //delete selectionBox;
    }

    delete DebugDrawer::getSingletonPtr();

    if (cameraMgr)
    {
        delete cameraMgr;
        cameraMgr = 0;
    }
    if (inputManager)
    {
        inputManager->destroyInputObject(mouse);
        inputManager->destroyInputObject(keyboard);

        OIS::InputManager::destroyInputSystem(inputManager);
        inputManager = 0;
    }
    if (renderWindow)
    {
        Ogre::WindowEventUtilities::removeWindowEventListener(renderWindow, this);
        //if(renderWindow) windowClosed(renderWindow);
    }


}

void OgreGFX::UIMgr::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if (rw == renderWindow)
    {
        kill();
    }
}

//void OgreGFX::UIMgr::buttonHit(OgreBites::Button* button)
//{
//
//}
