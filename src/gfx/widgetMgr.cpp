/*
 * widgetMgr.cpp
 *
 *  Created on: Jan 7, 2013
 *      Author: sushil
 */


#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include "DEBUG.h"
#include "command.h"
#include "enums.h"
#include "flock.h"
#include "commandHelp.h"

#include <widgetMgr.h>

#include <GraphicsInteractionManager.h>

OgreGFX::WidgetMgr::WidgetMgr(GraphicsInteractionManager *gim) : GFXMgr(gim), Ogre::FrameListener(), Ogre::WindowEventListener(), OIS::KeyListener(), OIS::MouseListener()
{
    DEBUG(std::cout << "Creating Widget Manager" << std::endl;)
    nextId = 0;

    //Defaults
    this->gfx->mWindow->getWidth();
    POS = Ogre::Vector2(0, 0);
    LABEL_SIZE = Ogre::Vector2(100, 20);
    PANEL_SIZE = Ogre::Vector2(100, 20);
    MENU_ITEM_SIZE = Ogre::Vector2(100, 20);
    BUTTON_SIZE = Ogre::Vector2(0.0417f, 0.0417f);
    COMMANDPANEL_POS = Ogre::Vector2(0.7614583f, 0.825f);
    MENU_SIZE = Ogre::Vector2(0.2083f, 0.125f);
    MID_PANEL_POS = Ogre::Vector2(0.262f, 0.857f);
    MID_PANEL_SIZE = Ogre::Vector2(0.476f,0.019f);

    WIDTH = 100;
    HEIGHT = 20;
    LABEL_X_OFFSET = 5;
    LABEL_Y_OFFSET = 5;

    LABEL_TEXT_COLOR = Ogre::ColourValue(1.0, 1.0, 0.7, 1.0);
    BUTTON_OFF_TEXT_COLOR = Ogre::ColourValue(1.0, 1.0, 0.0, 1.0);
    BUTTON_ON_TEXT_COLOR = Ogre::ColourValue(0.0, 0.0, 0.5, 1.0);
    MENU_DEFAULT_TEXT_COLOR = Ogre::ColourValue(1.0, 1.0, 0.0, 1.0);
    MENU_SELECTED_TEXT_COLOR = Ogre::ColourValue(0.0, 0.0, 0.5, 1.0);


    PANEL_MATERIAL = "ECSLENT/navy/material/shipInfo/overlay";
    LINE_SEP_MATERIAL = "ECSLENT/line";
    MENU_MATERIAL = "ECSLENT/navy/material/rmenu";

    uiBase = 0;
    currMenu = NULL;
    //uiBase->show();



    DEBUG(std::cout << "Finished  Widget Manager: " << getNextId("Test") << std::endl;)
}

OgreGFX::WidgetMgr::~WidgetMgr()
{
}

void OgreGFX::WidgetMgr::activateMenu(FastEcslent::Entity* ent)
{
    this->deactivateMenu();
    switch (ent->entityType)
    {
    case FastEcslent::SCV:
        currMenu = scvMenu;
        break;
    case FastEcslent::HELLION:
            currMenu = multiMenu; //temp menu for testing
            break;
    case FastEcslent::COMMANDCENTER:
        currMenu = cmdcMenu;
        break;
    case FastEcslent::BARRACKS:
        currMenu = barracksMenu;
        break;
    case FastEcslent::FACTORY:
        currMenu = factoryMenu;
        break;
    case FastEcslent::NENTITYTYPES:
        currMenu = multiMenu;
        break;

    //StarCraft Units
    case FastEcslent::SC_MARINE:
    	currMenu = multiMenu;
    	break;
    case FastEcslent::SC_VULTURE:
    	currMenu = multiMenu;
    	std::cout<<"ActivatedMenu for: "<<ent->entityId.id<< std::endl;
    	break;
    case FastEcslent::SC_TANK:
    	currMenu = multiMenu;
    	break;
    default:
        this->deactivateMenu();
        break;
    }
    //assign new menu to curmenu

    if (currMenu)
    {
        currMenu->show();
        unitName->textArea->setCaption(ent->uiname);
        buildProgressPanel->show();
        //unitHealth->textArea->setCaption(boost::to_string(ent->weapon->currentHitpoints) + "/" + boost::to_string(gfx->engine->gameMgr->entTypeData[ent->entityType].hitPoints));
    }

}

void OgreGFX::WidgetMgr::deactivateMenu()
{
    if (currMenu)
    {
        currMenu->hide();
    }
    
    currMenu = NULL;
    //unitName->textArea->setCaption("");
    //unitHealth->textArea->setCaption("");
    buildProgressPanel->hide();
    buildProgressEmpty->percentFull = 0.0;
    
}

void OgreGFX::WidgetMgr::initialize()
{

    uiBase = new UIOutline(this->gfx);
    //uiBase->overlay->setZOrder(0);
    uiBase->show();
    //uiBase->hide();
    Ogre::Vector2 relLabelSize(0.0595f, 0.019f);

    testPanel = new OgreGFX::FEPanel(this->gfx, "TestPanel", (Widget*) 0, POS, PANEL_SIZE, PANEL_MATERIAL, Ogre::GMM_PIXELS);

    buttonInfo = new OgreGFX::FEPanel(this->gfx, "ButtonInfo", (Widget*) 0, Ogre::Vector2(0.7614583f, 0.6666), MENU_SIZE, MENU_MATERIAL, Ogre::GMM_RELATIVE);
    buildProgressPanel  = new OgreGFX::FEPanel(this->gfx, "TestPanel", (Widget*) 0, MID_PANEL_POS, MID_PANEL_SIZE, PANEL_MATERIAL, Ogre::GMM_RELATIVE);
    scvMenu = new OgreGFX::FEPanel(this->gfx, "SCVMenu", (Widget*) 0, COMMANDPANEL_POS, MENU_SIZE, MENU_MATERIAL, Ogre::GMM_RELATIVE);
    cmdcMenu = new OgreGFX::FEPanel(this->gfx, "CmdcMenu", (Widget*) 0, COMMANDPANEL_POS, MENU_SIZE, MENU_MATERIAL, Ogre::GMM_RELATIVE);
    barracksMenu = new OgreGFX::FEPanel(this->gfx, "barracksMenu", (Widget*) 0, COMMANDPANEL_POS, MENU_SIZE, MENU_MATERIAL, Ogre::GMM_RELATIVE);
    factoryMenu = new OgreGFX::FEPanel(this->gfx, "factoryMenu", (Widget*) 0, COMMANDPANEL_POS, MENU_SIZE, MENU_MATERIAL, Ogre::GMM_RELATIVE);
    multiMenu = new OgreGFX::FEPanel(this->gfx, "MultiMenu", (Widget*) 0, COMMANDPANEL_POS, MENU_SIZE, MENU_MATERIAL, Ogre::GMM_RELATIVE);
    std::set<OIS::KeyCode> *creationMods = new std::set<OIS::KeyCode > ();
    OIS::KeyCode attackCode = OIS::KC_T;
    OIS::KeyCode moveCode = OIS::KC_G;
    OIS::KeyCode gatherCode = OIS::KC_G;
    OIS::KeyCode buildBarracksCode = OIS::KC_B;
    OIS::KeyCode buildFactoryCode = OIS::KC_F;
    OIS::KeyCode buildRefineryCode = OIS::KC_R;
    OIS::KeyCode buildSCVCode = OIS::KC_S;
    OIS::KeyCode buildMarineCode = OIS::KC_M;
    OIS::KeyCode buildHellionCode = OIS::KC_H;
    OIS::KeyCode placeholderCode = OIS::KC_UNASSIGNED;
    scvAtk = new OgreGFX::FEButton(this->gfx, attackCode, creationMods, OIS::MB_Left, "scvAtk", NULL,
                                   scvMenu, POS, BUTTON_SIZE, "Attack", "Attack", Ogre::GMM_RELATIVE);
    scvBuildFactory = new OgreGFX::FEButton(this->gfx, buildFactoryCode, creationMods, OIS::MB_Left, "scvFactory", new FastEcslent::BuildStructureFactory(FastEcslent::FACTORY, "Factory.mesh"),
                                   scvMenu, POS, BUTTON_SIZE, "FactoryPanel", "Build a Factory", Ogre::GMM_RELATIVE);
    scvBuildRefinery = new OgreGFX::FEButton(this->gfx, buildRefineryCode, creationMods, OIS::MB_Left, "scvRefinery", new FastEcslent::BuildStructureFactory(FastEcslent::REFINERY, "Refinery.mesh", FastEcslent::TargetEntity, FastEcslent::GAS),
                                   scvMenu, POS, BUTTON_SIZE, "RefineryPanel", "Build a Refinery on an Oil Field", Ogre::GMM_RELATIVE);
    scvBuildBarracks = new OgreGFX::FEButton(this->gfx, buildBarracksCode, creationMods, OIS::MB_Left, "scvBarracks", new FastEcslent::BuildStructureFactory(FastEcslent::BARRACKS, "Barracks.mesh"),
                                   scvMenu, POS, BUTTON_SIZE, "BarracksPanel", "Build a Barracks", Ogre::GMM_RELATIVE);
    scvMove = new OgreGFX::FEButton(this->gfx, moveCode, creationMods, OIS::MB_Left, "scvMove", new FastEcslent::MoveFactory(),
                                   scvMenu, POS, BUTTON_SIZE, "Move", "Move unit", Ogre::GMM_RELATIVE);
    scvGather = new OgreGFX::FEButton(this->gfx, gatherCode, creationMods, OIS::MB_Left, "scvGather", NULL,
                                   scvMenu, POS, BUTTON_SIZE, "Gather", "Gather Resources", Ogre::GMM_RELATIVE);
    cmdcBuildSCV = new OgreGFX::FEButton(this->gfx, buildSCVCode, creationMods, OIS::MB_Left, "cmdcBuildSCV", new FastEcslent::BuildFactory(FastEcslent::SCV),
                                   cmdcMenu, POS, BUTTON_SIZE, "Boats/SCV", "Build SCV", Ogre::GMM_RELATIVE);
    multiAtk = new OgreGFX::FEButton(this->gfx, attackCode, creationMods, OIS::MB_Left, "multiAtk", new FastEcslent::AttackFactory(),
                                   multiMenu, POS, BUTTON_SIZE, "Attack", "Attack", Ogre::GMM_RELATIVE);
    multiMove = new OgreGFX::FEButton(this->gfx, moveCode, creationMods, OIS::MB_Left, "multiMove", new FastEcslent::MoveFactory(),
                                   multiMenu, POS, BUTTON_SIZE, "Move", "Move Units", Ogre::GMM_RELATIVE);
    barracksMarine = new OgreGFX::FEButton(this->gfx,buildMarineCode, creationMods, OIS::MB_Left, "barracksMarine", new FastEcslent::BuildFactory(FastEcslent::MARINE),
                                   barracksMenu, POS, BUTTON_SIZE, "Boats/MARINE", "Build a Marine", Ogre::GMM_RELATIVE);
    factoryBuildVulture = new OgreGFX::FEButton(this->gfx,buildHellionCode, creationMods, OIS::MB_Left, "factoryVulture", new FastEcslent::BuildFactory(FastEcslent::HELLION),
                                   barracksMenu, POS, BUTTON_SIZE, "Boats/VULTURE", "Build a Vulture", Ogre::GMM_RELATIVE);
    buildQueueItem1 = new OgreGFX::FEButton(this->gfx,placeholderCode, creationMods, OIS::MB_Left, "buildQueueItem1", NULL,
                                   buildProgressPanel, POS, BUTTON_SIZE, "Action1", "buildQueueItem1", Ogre::GMM_RELATIVE);
    buildQueueItem2 = new OgreGFX::FEButton(this->gfx,placeholderCode, creationMods, OIS::MB_Left, "buildQueueItem2", NULL,
                                   buildProgressPanel, POS, BUTTON_SIZE, "Action2", "buildQueueItem2", Ogre::GMM_RELATIVE);
    buildQueueItem3 = new OgreGFX::FEButton(this->gfx,placeholderCode, creationMods, OIS::MB_Left, "buildQueueItem3", NULL,
                                   buildProgressPanel, POS, BUTTON_SIZE, "Action3", "buildQueueItem3", Ogre::GMM_RELATIVE);
    buildQueueItem4 = new OgreGFX::FEButton(this->gfx,placeholderCode, creationMods, OIS::MB_Left, "buildQueueItem4", NULL,
                                   buildProgressPanel, POS, BUTTON_SIZE, "Action4", "buildQueueItem4", Ogre::GMM_RELATIVE);
    buildQueueItem5 = new OgreGFX::FEButton(this->gfx,placeholderCode, creationMods, OIS::MB_Left, "buildQueueItem5", NULL,
                                   buildProgressPanel, POS, BUTTON_SIZE, "Action5", "buildQueueItem5", Ogre::GMM_RELATIVE);
    buildProgressEmpty = new OgreGFX::FEProgressBar(this->gfx, "ProgressBar", buildProgressPanel, POS, relLabelSize, "white","green",  Ogre::GMM_RELATIVE);
    buildQueueItems.push_back(buildQueueItem1);
    buildQueueItems.push_back(buildQueueItem2);
    buildQueueItems.push_back(buildQueueItem3);
    buildQueueItems.push_back(buildQueueItem4);
    buildQueueItems.push_back(buildQueueItem5);
    
//    buildSCV = new OgreGFX::FEButton(this->gfx, creationMods, OIS::MB_Left, "NEWB", new FastEcslent::BuildFactory(FastEcslent::SCV),
//                                   cmdcMenu, POS, BUTTON_SIZE, "Attack", "NTEST", Ogre::GMM_RELATIVE);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, scvAtk);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, scvMove);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, scvGather);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, scvBuildFactory);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, scvBuildRefinery);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, scvBuildBarracks);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, cmdcBuildSCV);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, multiAtk);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, multiMove);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, barracksMarine);
    this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, factoryBuildVulture);
    this->gfx->uiMgr->registerKeyHandler(creationMods, buildBarracksCode, scvBuildBarracks);
    this->gfx->uiMgr->registerKeyHandler(creationMods, buildFactoryCode, scvBuildFactory);
    this->gfx->uiMgr->registerKeyHandler(creationMods, buildRefineryCode, scvBuildRefinery);
    this->gfx->uiMgr->registerKeyHandler(creationMods, moveCode, scvMove);
    this->gfx->uiMgr->registerKeyHandler(creationMods, attackCode, scvAtk);
    this->gfx->uiMgr->registerKeyHandler(creationMods, gatherCode, scvGather);
    this->gfx->uiMgr->registerKeyHandler(creationMods, buildSCVCode, cmdcBuildSCV);
    this->gfx->uiMgr->registerKeyHandler(creationMods, attackCode, multiAtk);
    this->gfx->uiMgr->registerKeyHandler(creationMods, moveCode, multiMove);
    this->gfx->uiMgr->registerKeyHandler(creationMods, buildMarineCode, barracksMarine);
    this->gfx->uiMgr->registerKeyHandler(creationMods, buildHellionCode, factoryBuildVulture);
    //this->gfx->uiMgr->registerMouseHandler(creationMods, OIS::MB_Left, buildSCV);

    scvMenu->addItem(scvAtk, Below);
    scvMenu->addItem(scvMove, Right);
    scvMenu->addItem(scvGather, Right);
    scvMenu->addItem(scvBuildFactory, Below);
    scvMenu->addItem(scvBuildBarracks, Right);
    scvMenu->addItem(scvBuildRefinery, Below);
    cmdcMenu->addItem(cmdcBuildSCV, Below);
    //cmdcMenu->addItem(buildSCV, Right);
    multiMenu->addItem(multiAtk, Below);
    multiMenu->addItem(multiMove, Right);
    barracksMenu->addItem(barracksMarine, Below);
    factoryMenu->addItem(factoryBuildVulture, Below);


    unitName = new OgreGFX::FELabel(this->gfx, "unitName", buildProgressPanel, LABEL_TEXT_COLOR, POS, relLabelSize+Ogre::Vector2(0.12f,0), Ogre::GMM_RELATIVE);
    unitHealth = new OgreGFX::FELabel(this->gfx, "unitHealth", buildProgressPanel, LABEL_TEXT_COLOR, POS, relLabelSize, Ogre::GMM_RELATIVE);
    testLabel = new OgreGFX::FELabel(this->gfx, "TestLabel", testPanel, LABEL_TEXT_COLOR, POS, LABEL_SIZE, Ogre::GMM_PIXELS);
    testLabel2 = new OgreGFX::FELabel(this->gfx, "SecondRow", testPanel, LABEL_TEXT_COLOR, POS, LABEL_SIZE, Ogre::GMM_PIXELS);
    buttonDesc = new OgreGFX::FELineWrapLabel(this->gfx, "ButtonDesc", buttonInfo, LABEL_TEXT_COLOR, POS, MENU_SIZE, Ogre::GMM_RELATIVE, 6);
    //buttonDesc->textArea->setCharHeight(buttonDesc->size.y / 4.0);
    buildProgressPanel->addItem(unitName, Below);
    buildProgressPanel->addItem(unitHealth,Right);
    buildProgressPanel->addItem(buildProgressEmpty,Below);
    buildProgressPanel->addItem(buildQueueItem1,Below);
    buildProgressPanel->addItem(buildQueueItem2,Right);
    buildProgressPanel->addItem(buildQueueItem3,Right);
    buildProgressPanel->addItem(buildQueueItem4,Right);
    buildProgressPanel->addItem(buildQueueItem5,Right);
    
    testPanel->addItem(testLabel, Below);
    testPanel->addItem(testLabel2, Below);
    buttonDesc->setCaption("Test");
    buttonInfo->addItem(buttonDesc,Below);
    //buildProgressPanel->addItem(buttonDesc,Below);
    buildProgressPanel->show();
    unitName->show();
    unitHealth->show();
    buildProgressEmpty->show();
    buttonDesc->show();
    testLabel2->show();
    testLabel->show();
    testPanel->show();
    buttonInfo->hide();
    buildProgressPanel->hide();
    scvMenu->hide();
    cmdcMenu->hide();
    multiMenu->hide();
    barracksMenu->hide();
    factoryMenu->hide();

    testLabelPair = new OgreGFX::FELabelPair(this->gfx, "NewKey", "NewValue", testPanel,
                                             LABEL_TEXT_COLOR,
                                             Ogre::Vector2(1000, 0), Ogre::Vector2(100, 200), 20, PANEL_MATERIAL, Ogre::GMM_PIXELS);
    testLabelPair2 = new OgreGFX::FELabelPair(this->gfx, "Name", "Value2", testPanel,
                                              LABEL_TEXT_COLOR,
                                              Ogre::Vector2(1000, 0), Ogre::Vector2(100, 200), 20, PANEL_MATERIAL, Ogre::GMM_PIXELS);
    testPanel->addItem(testLabelPair, Below);
    testPanel->addItem(testLabelPair2, Below);
    testLabelPair->show();
    first = true;
    //creationMods->insert(OIS::KC_LCONTROL);
}

bool OgreGFX::WidgetMgr::frameStarted(const Ogre::FrameEvent& evt)
{
    return true;
}

bool OgreGFX::WidgetMgr::frameEnded(const Ogre::FrameEvent& evt)
{
    return true;
}

bool OgreGFX::WidgetMgr::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    std::string gasLabel = "     Gas: ";
    std::string mineralsLabel = "Minerals: ";
    DEBUG(std::cout << "Setting caption" << std::endl;)
    if (first)
    {
        //first = false;
        //--------------------------------------------------------------------------
        //testLabel->textArea->setCaption(testLabel->caption);
        gasLabel.append(boost::lexical_cast<std::string > (this->gfx->engine->gameMgr->resources[gfx->engine->options.player].gas));
        mineralsLabel.append(boost::lexical_cast<std::string > (this->gfx->engine->gameMgr->resources[gfx->engine->options.player].minerals));
        testLabel->textArea->setCaption(gasLabel);
        testLabel2->textArea->setCaption(mineralsLabel);
        //unitName->textArea->setCaption("TEST NAME");
        //unitHealth->textArea->setCaption("TEST HEALTH");
        testLabelPair->setLeft(testLabelPair->left->caption);
        testLabelPair->setRight(testLabelPair->right->caption);

        testLabelPair2->setLeft(testLabelPair2->left->caption);
        testLabelPair2->setRight(testLabelPair2->right->caption);
        buttonDesc->textArea->setCaption(buttonDesc->caption);
        
        buildProgressEmpty->percentFull = 0;
        buildQueueItem1->material ="Action1";
        buildQueueItem2->material ="Action2";
        buildQueueItem3->material ="Action3";
        buildQueueItem4->material ="Action4";
        buildQueueItem5->material ="Action5";
        std::vector<FEButton*>::iterator buildIcon = buildQueueItems.begin();
        if(gfx->engine->selectionMgr->getPrimarySelection() != NULL)
        {
            unitHealth->textArea->setCaption(boost::to_string(gfx->engine->selectionMgr->getPrimarySelection()->hitpoints) + "/" + boost::to_string(gfx->engine->weaponMgr->maxHitpoints[gfx->engine->selectionMgr->getPrimarySelection()->entityType]));
            if(gfx->engine->selectionMgr->getPrimarySelection()->builder!= NULL && gfx->engine->selectionMgr->getPrimarySelection()->builder->entityBeingBuilt != NULL)
            {
                buildProgressEmpty->percentFull = 1.0f - (gfx->engine->selectionMgr->getPrimarySelection()->builder->entityBeingBuilt->timeLeftToBirth / gfx->engine->gameMgr->entTypeData[gfx->engine->selectionMgr->getPrimarySelection()->builder->entityBeingBuilt->entityType].buildTime);
                for(std::deque<FastEcslent::EntityType>::iterator it = gfx->engine->selectionMgr->getPrimarySelection()->builder->entityQueue.begin(); it != gfx->engine->selectionMgr->getPrimarySelection()->builder->entityQueue.end() && it != gfx->engine->selectionMgr->getPrimarySelection()->builder->entityQueue.begin()+5; it++)
                {
                        (*buildIcon)->material = FastEcslent::EntityTypeToIconName(*it);
                        buildIcon++;
                }
                
            }
        }

        for(std::vector<FEButton*>::iterator it = buildQueueItems.begin(); it != buildQueueItems.end(); it++)
        {
            (*it)->panel->setMaterialName((*it)->material);
        }
        //buildQueueItem1->panel->setMaterialName(buildQueueItem1->material);
        buildProgressEmpty->UpdateFillPercent();
//        scvAtk->textArea->setCaption(scvAtk->caption);
//        scvMove->textArea->setCaption(scvMove->caption);
//        scvGather->textArea->setCaption(scvGather->caption);
//        cmdcBuildSCV->textArea->setCaption(cmdcBuildSCV->caption);
//        multiAtk->textArea->setCaption(multiAtk->caption);
//        multiMove->textArea->setCaption(multiMove->caption);
//        barracksMarine->textArea->setCaption(barracksMarine->caption);
        //--------------------------------------------------------------------------
    }
    return true;
}

bool OgreGFX::WidgetMgr::keyPressed(const OIS::KeyEvent &arg)
{
    DEBUG(std::cout << "Widget Manager key pressed: " << arg.key << std::endl;)
    return true;
}

bool OgreGFX::WidgetMgr::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}

bool OgreGFX::WidgetMgr::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}

bool OgreGFX::WidgetMgr::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool OgreGFX::WidgetMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

std::string OgreGFX::WidgetMgr::getNextId(std::string base)
{
    std::stringstream idStream;
    this->nextId++;
    idStream << base << "_" << nextId;
    return idStream.str();
}

bool OgreGFX::WidgetMgr::mouseOverOverlay(const OIS::MouseEvent& arg)
{
    if (scvMenu->cursonInMe(Ogre::Vector2(arg.state.X.abs, arg.state.Y.abs)))
        return true;
    return false;
}

bool OgreGFX::WidgetMgr::mouseOverButton(const OIS::MouseEvent& arg)
{
    if (currMenu)
    {
        for(std::list<Widget*>::iterator it = currMenu->items.begin(); it != currMenu->items.end(); it++)
        {
            if((*it)->cursonInMe(Ogre::Vector2(arg.state.X.abs, arg.state.Y.abs)))
            {
                buttonDesc->setCaption((*it)->desc);
                buttonDesc->textArea->setCaption(buttonDesc->caption);
                buttonInfo->show();
                buildProgressPanel->show();
                return true;
            }
        }
    }
    buttonInfo->hide();
    //buildProgressPanel->hide();
    buttonDesc->setCaption("ERROR");
    buttonDesc->textArea->setCaption(buttonDesc->caption);
    return false;
}
