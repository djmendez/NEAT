/*
 * widgets.cpp
 *
 *  Created on: Jan 8, 2013
 *      Author: sushil
 */

#include <GraphicsInteractionManager.h>
#include <widgets.h>

#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <iostream>
#include "enums.h"
#include "DEBUG.h"
#include "commandHelp.h"

OgreGFX::Widget::Widget(GraphicsInteractionManager *gim, Widget *aParent, Ogre::Vector2 defaultPos, Ogre::Vector2 defaultSize, Ogre::GuiMetricsMode
defaultMode) {

	if (defaultPos == Ogre::Vector2::NEGATIVE_UNIT_X)
		this->pos = Ogre::Vector2(0, 0);
	if (defaultSize == Ogre::Vector2::NEGATIVE_UNIT_X)
		this->size = Ogre::Vector2(100, 30);
	if (!aParent)
		this->parent = 0;
        
        this->desc = "";
	this->gfx = gim;
	this->parent = aParent;
	this->pos = defaultPos;
	this->size = defaultSize;
        this->mode = defaultMode;

}

OgreGFX::Widget::~Widget(){

};

Ogre::Vector2 OgreGFX::Widget::screenPos(){
	if (this->parent) {
		return this->parent->screenPos() + this->pos;
	} else {
		return this->pos;
	}
}

bool OgreGFX::Widget::cursonInMe(const Ogre::Vector2 &pos){
	Ogre::Vector2 myPos = screenPos();
        Ogre::Vector2 mySize = this->size;
        
        
        if(this->mode == Ogre::GMM_PIXELS)
        {                
        }
        else if(this->mode == Ogre::GMM_RELATIVE)
        {
            myPos.x *= this->gfx->mWindow->getWidth();        
            myPos.y *= this->gfx->mWindow->getHeight();
            mySize.x *= this->gfx->mWindow->getWidth();        
            mySize.y *= this->gfx->mWindow->getHeight();
            DEBUG(std::cout << "Adjusted pos/size: " << myPos.x << "," << myPos.y << "|" << mySize.x << "," << mySize.y <<  "|" << (pos.x >= myPos.x && pos.x <= myPos.x + mySize.x && pos.y >= myPos.y && pos.y <= myPos.y + mySize.y)  <<std::endl;)
        }
        return (pos.x >= myPos.x && pos.x <= myPos.x + mySize.x && pos.y >= myPos.y && pos.y <= myPos.y + mySize.y);
}


//--------------------------------------------------------------------------------------------------------------------------------

OgreGFX::UIOutline::UIOutline(GraphicsInteractionManager *gim) : OgreGFX::Widget(gim) {

	this->overlayManagerPtr = Ogre::OverlayManager::getSingletonPtr();
	std::string nextId = gfx->widgetMgr->getNextId("OutlinePanel");
	this->overlay = this->overlayManagerPtr->create("UIOutline");

	Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*> (this->overlayManagerPtr->createOverlayElement("Panel", nextId));


	this->pos = Ogre::Vector2(0.0, 0.0);
	this->size = Ogre::Vector2(gfx->mWindow->getWidth(), gfx->mWindow->getHeight());
	this->size = Ogre::Vector2(1.0, 1.0);
	panel->setPosition(pos.x, pos.y);
	panel->setDimensions(size.x, size.y);
	panel->setMaterialName("ECSLENT/UI");

	this->overlay->add2D(static_cast<Ogre::OverlayContainer *> (panel));
	this->overlay->add2D(panel);
	DEBUG(std::cout << "Added OvelayContainer Panel" << std::endl;)

	overlay->show();

}

void OgreGFX::UIOutline::show() {
	this->overlay->show();
}

void OgreGFX::UIOutline::hide() {
	this->overlay->hide();
}

//--------------------------------------------------------------------------------------------------------------------------------
//FEPanel(gim, caption1, aparent, apos, Ogre::Vector2(columnWidths.x + columnWidths.y, columnHeight), materialName)

OgreGFX::FEPanel::FEPanel(GraphicsInteractionManager *gim, std::string name, Widget* iparent,
		Ogre::Vector2 ipos, Ogre::Vector2 isize, std::string imaterial,Ogre::GuiMetricsMode defaultMode):
		Widget(gim, iparent, ipos, isize,defaultMode) {

		this->material = imaterial;
		this->name = name;
		this->overlayManagerPtr = Ogre::OverlayManager::getSingletonPtr();
		this->panel = static_cast<Ogre::OverlayContainer*>(this->overlayManagerPtr->createOverlayElement("Panel", gfx->widgetMgr->getNextId(name)));
		this->panel->setMetricsMode(this->mode);
		this->panel->setPosition(pos.x, pos.y);
		//size.y = 0; // or extra space when adding labels to this
		this->panel->setDimensions(size.x, size.y);
		this->panel->setMaterialName(material);
		this->panel->show();

		if (!this->parent){
			DEBUG(std::cout << "Creating panel overlay" << std::endl;)
			this->overlayPtr = this->overlayManagerPtr->create(gfx->widgetMgr->getNextId(name));
			this->overlayPtr->add2D(this->panel);
			this->overlayPtr->show();
		}

		this->belowPos = Ogre::Vector2(0.0, 0.0);
		this->rightPos = Ogre::Vector2(0.0, 0.0);
		this->gap      = Ogre::Vector2(0.0, 0.0);
		this->separatorHeight = 1;

};

OgreGFX::FEPanel::~FEPanel(){

}


void OgreGFX::FEPanel::show(){
	if (this->parent){
		this->overlayPtr->show();
	}
	this->panel->show();
	for(std::list<Widget*>::iterator it = this->items.begin(); it != this->items.end(); ++it){
		(*it)->show();
	}
}


void OgreGFX::FEPanel::hide(){
	for(std::list<Widget*>::iterator it = this->items.begin(); it != this->items.end(); ++it){ // do I need this?
		(*it)->hide();
	}
	this->panel->hide();
	if (this->parent){
		this->overlayPtr->hide();
	}
}

Ogre::OverlayElement * OgreGFX::FEPanel::makeSep(){

	Ogre::OverlayElement *sep = this->overlayManagerPtr->createOverlayElement("Panel", gfx->widgetMgr->getNextId("lineSep"));
	sep->setMetricsMode(Ogre::GMM_PIXELS);
	sep->setPosition(0, 0);
	sep->setDimensions(this->pos.x, this->separatorHeight);
	sep->setMaterialName("ECSLENT/line");
	sep->show();
	return sep;
}

void OgreGFX::FEPanel::addSep(int separation){
	Ogre::OverlayElement *sep = makeSep();
	this->separators.push_back(sep);
	sep->setPosition(0, separation);
	sep->setDimensions(this->size.x, this->separatorHeight);
	this->panel->addChild(sep);
}

void OgreGFX::FEPanel::adjustSeparators(){
	for(std::list<Ogre::OverlayElement*>::iterator it = this->separators.begin(); it != this->separators.end(); ++it){
		(*it)->setDimensions(this->size.x, this->separatorHeight);
	}
}

void OgreGFX::FEPanel::posChanged(){
	this->panel->setPosition(this->pos.x, this->pos.y);
	for(std::list<Widget*>::iterator it = this->items.begin(); it != this->items.end(); ++it){ // do I need this?
		(*it)->posChanged();
	}

}

void OgreGFX::FEPanel::addItem(Widget *item, Placement placement){
	this->items.push_back(item);
	if (this->size.x < item->size.x){
		this->size.x = item->size.x;
		adjustSeparators();
	}

	if (placement == Below){
		DEBUG(std::cout << "Adding below: " << std::endl;)
		item->pos = this->belowPos;
		item->posChanged();
		this->rightPos = this->belowPos + Ogre::Vector2(item->size.x + this->gap.x, 0.0);
		this->belowPos.y += item->size.y + this->gap.y;
		DEBUG(std::cout << "Below Pos: " << belowPos.x << ", " << belowPos.y << std::endl;)
	} else if (placement == Right){
		item->pos = this->rightPos;
		item->posChanged();
		this->rightPos.x += item->size.x + this->gap.x;
	}

	if (this->rightPos.x > this->size.x){
		this->size.x = this->rightPos.x;
	}
	if (this->belowPos.y > this->size.y){
		this->size.y = this->belowPos.y;
	}


	this->panel->addChild(item->getOverlayElementToAdd());
	this->panel->setDimensions(this->size.x, this->size.y);
	addSep(this->size.y);

}

Ogre::OverlayElement * OgreGFX::FEPanel::getOverlayElementToAdd(){
	return static_cast<Ogre::OverlayElement *>(this->panel);
}

//--------------------------------------------------------------------------------------------------------------------------------

OgreGFX::FELabel::FELabel(GraphicsInteractionManager *gim,
		std::string caption, Widget *aparent, Ogre::ColourValue acolor,
		Ogre::Vector2 apos, Ogre::Vector2 asize,Ogre::GuiMetricsMode defaultMode):
		Widget(gim, aparent, apos, asize,defaultMode) {

	this->overlayManagerPtr = Ogre::OverlayManager::getSingletonPtr();
	this->caption = caption;
	this->color = color;
	this->offset = Ogre::Vector2(5.0, 2.0);
	this->textArea = static_cast<Ogre::TextAreaOverlayElement*>(this->overlayManagerPtr->createOverlayElement("TextArea", gfx->widgetMgr->getNextId(caption)));
	this->textArea->setMetricsMode(this->mode);
	this->textArea->setCaption(caption);
	DEBUG(std::cout << "Label Size: " << size.x << ", " << size.y << std::endl;)
	this->textArea->setPosition(pos.x + offset.x, pos.y + offset.y);
	this->textArea->setDimensions(size.x, size.y);
	this->textArea->setFontName("SdkTrays/Value");
	this->textArea->setCharHeight(size.y);
	this->textArea->setColourBottom(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));
	this->textArea->setColourTop(color);
	this->textArea->show();
}

OgreGFX::FELabel::~FELabel(){

}

void OgreGFX::FELabel::setCaption(std::string val){
	this->caption = val;
	this->textArea->setCaption(val);
}


void OgreGFX::FELabel::show(){
	DEBUG(std::cout << "Showing label" << std::endl;)
	this->textArea->show();
}

void OgreGFX::FELabel::hide(){
	this->textArea->hide();
}

Ogre::OverlayElement* OgreGFX::FELabel::getOverlayElementToAdd(){
	return static_cast<Ogre::OverlayElement*>(this->textArea);
}

void OgreGFX::FELabel::posChanged(){
	this->textArea->setPosition(pos.x, pos.y);
}
//------------------------------------------------------------------------------
OgreGFX::FELineWrapLabel::FELineWrapLabel(GraphicsInteractionManager *gim, std::string caption,
            Widget *aParent, Ogre::ColourValue color,
            Ogre::Vector2 defaultPos, Ogre::Vector2 defaultSize, Ogre::GuiMetricsMode defaultMode, double lines): 
            FELabel(gim,caption, aParent, color, defaultPos,defaultSize,defaultMode),lines(lines) 
{
    this->textArea->setCharHeight(size.y / lines);
    this->setCaption(caption);
}

void OgreGFX::FELineWrapLabel::setCaption(std::string val){
    int charPerLine = 3*(this->textArea->getWidth() / this->textArea->getCharHeight());
    int linebreaks = val.size() / charPerLine;
    const int maxLineBreaks = lines -1; //to have x lines you need x-1 line breaks (last line doesnt have a line break)
    if(linebreaks > maxLineBreaks)
    {
        val.erase(val.begin()+(charPerLine*this->lines), val.end()); // truncates characters that wont fit in area
        linebreaks = maxLineBreaks;
    }
    for(int i = 1; i <= linebreaks; i++)
    {
        val.insert(val.begin()+(i*charPerLine), '\n');
    }
	this->caption = val;
	this->textArea->setCaption(val);
}
//--------------------------------------------------------------------------------------------------------------------------------

OgreGFX::FEButton::FEButton(GraphicsInteractionManager *gim, const OIS::KeyCode keyCode,
                            const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
                            std::string name, FastEcslent::CommandFactory* commandFactory, Widget *aparent,
                            Ogre::Vector2 apos, Ogre::Vector2 asize, std::string material, std::string desc, Ogre::GuiMetricsMode defaultMode) :
FEPanel(gim,
        name, aparent,
        apos, asize,material,defaultMode), commandFactory(commandFactory),
MouseHandler(gim, modifierKeys, oisMouseButtonId), KeyHandler(gim, modifierKeys, keyCode),
pressed(false), enabled(false)//, desc(desc)
{
    this->desc = desc;
}

OgreGFX::FEButton::~FEButton()
{
    delete commandFactory;

}

void OgreGFX::FEButton::handleKeyEvent()
{
    if (commandFactory != NULL)
    {
    bool firstEvent = processButtonFirstEvent(enabled); // if the key handler has been called, then we already know the keypresses match this event, but only run if the button is actually enabled
    processButtonSecondEvent(!firstEvent && enabled && commandFactory->requiredTarget == FastEcslent::None && this->gfx->uiMgr->buttonActive == NULL); //only activate the 2nd event for factories that dont need targets, and dont interrupt other active buttons
    }
}

bool OgreGFX::FEButton::processButtonFirstEvent(bool buttonActivated)
{
    //bool buttonWasClicked = enabled && buttonActivated;
        //DEBUG(std::cout << "Button checking:" << args.state.X.abs << "," << args.state.Y.abs << "|" << this->pos.x << "," << this->pos.y << std::endl;)
        if(buttonActivated && this->gfx->uiMgr->buttonActive == NULL && commandFactory->requiredTarget != FastEcslent::None) //need to wait for another mouse click
        {
            this->gfx->uiMgr->buttonActive = this;
            this->gfx->uiMgr->CreateGhostUnderMouse(commandFactory->previewMeshName);
            return true;
        }
        return false;
    
}

bool OgreGFX::FEButton::processButtonSecondEvent(bool buttonActivated)
{
            if (buttonActivated) //Either we do not require an ent to be found on mouse-click, or we found an ent on mouse-click regardless
            {
            	FastEcslent::Entity** selected = this->gfx->engine->selectionMgr->getSelectedEnts();
           		for(int i = 0; i < this->gfx->engine->selectionMgr->getNSelectedEnts(); i++){
           			std::deque<FastEcslent::Command*> ghostCommands = FastEcslent::addCommandForEnt(selected[i], commandFactory, this->gfx->uiMgr->entIndexUnderMouse());
                    this->gfx->uiMgr->buttonActive = NULL; //command fulfilled, button no longer needs to be active in UImgr
                    this->gfx->uiMgr->ChangeGhostTextureRBGA(this->gfx->uiMgr->ghostUnderMouse, Ogre::ColourValue(1.0,1.0,0,0.5));
                    this->gfx->uiMgr->waitingGhosts.push_back(std::pair<std::deque<FastEcslent::Command*>, Ogre::SceneNode*>(ghostCommands, this->gfx->uiMgr->ghostUnderMouse));
                    this->gfx->uiMgr->ghostUnderMouse = NULL;
           		}
            }
    
}

void OgreGFX::FEButton::handleMouseEvent(const OIS::MouseEvent &args)
{
    if (commandFactory != NULL)
    {
        std::pair<int, Ogre::Vector3 > mouseTargets = this->gfx->uiMgr->entIndexUnderMouse(); //check for required data
        bool buttonWasClicked = enabled && cursonInMe(Ogre::Vector2(args.state.X.abs, args.state.Y.abs));
        //(this->gfx->uiMgr->buttonActive == this || (buttonWasClicked && commandFactory->requiredTarget == FastEcslent::None && this->gfx->uiMgr->buttonActive == NULL))
        bool firstEvent = processButtonFirstEvent(buttonWasClicked);
        //only process the 2nd event if the button was clicked and requires no info, OR if the button was active AND has all the target info it needs
        //processButtonSecondEvent(!firstEvent && enabled && ((buttonWasClicked && commandFactory->requiredTarget == FastEcslent::None && this->gfx->uiMgr->buttonActive == NULL) || (this->gfx->uiMgr->buttonActive == this && ((commandFactory->requiredTarget != FastEcslent::Both && commandFactory->requiredTarget != FastEcslent::TargetEntity) || mouseTargets.first != -1))));
        processButtonSecondEvent(!firstEvent && enabled && ((buttonWasClicked && commandFactory->requiredTarget == FastEcslent::None && this->gfx->uiMgr->buttonActive == NULL) || (this->gfx->uiMgr->buttonActive == this && ((commandFactory->requiredTarget != FastEcslent::Both && commandFactory->requiredTarget != FastEcslent::TargetEntity) || (mouseTargets.first != -1 && ( (commandFactory->targetEntityType == FastEcslent::NENTITYTYPES) || (commandFactory->targetEntityType==this->gfx->gfxNodes[mouseTargets.first].entType) )) ))));
    }
}

void OgreGFX::FEButton::show()
{
    enabled = true;
    //posChanged();
    DEBUG(std::cout << "Name:" << this->name << std::endl;)
    //this->panel->show();
//    this->textArea->show();
}

void OgreGFX::FEButton::hide()
{
    enabled = false;
    //this->panel->hide();
    //this->textArea->hide();
}
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

//OgreGFX::FEButton::FEButton(GraphicsInteractionManager *gim,
//                            const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
//                            std::string name, Widget *aparent, Ogre::ColourValue acolor,
//                            Ogre::Vector2 apos, Ogre::Vector2 asize, bool (*pClickFunction)(), std::string material, std::string desc, Ogre::GuiMetricsMode defaultMode) :
//FEPanel(gim,
//        name, aparent,
//        apos, asize,material,defaultMode), pClickFunction(pClickFunction),
//MouseHandler(gim, modifierKeys, oisMouseButtonId),
//pressed(false), enabled(false)//, desc(desc)
//{
//    this->desc = desc;
//
//}
//
//OgreGFX::FEButton::~FEButton()
//{
//
//}
//
//void OgreGFX::FEButton::handleMouseEvent(const OIS::MouseEvent &args)
//{
//    DEBUG(std::cout << "Button checking:" << args.state.X.abs << "," << args.state.Y.abs << "|" << this->pos.x << "," << this->pos.y << std::endl;)
//    if (enabled && cursonInMe(Ogre::Vector2(args.state.X.abs, args.state.Y.abs)))
//    {
//        //this->color = Ogre::ColourValue(0.0, 0.0, 0.7, 0.0);
//        DEBUG(std::cout << "Button pressed" << std::endl;)
//        if (pClickFunction)
//            pClickFunction();
//    }
//}
//
//void OgreGFX::FEButton::show()
//{
//    enabled = true;
//    posChanged();
//    DEBUG(std::cout << "Name:" << this->name << std::endl;)
//    //this->panel->show();
////    this->textArea->show();
//}
//
//void OgreGFX::FEButton::hide()
//{
//    enabled = false;
//    //this->panel->hide();
//    //this->textArea->hide();
//}
////--------------------------------------------------------------------------------------------------------------------------------
//
//OgreGFX::FESCVButton::FESCVButton(GraphicsInteractionManager *gim,
//                                  const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
//                                  std::string name, Widget *aparent, Ogre::ColourValue acolor,
//                                  Ogre::Vector2 apos, Ogre::Vector2 asize, Ogre::GuiMetricsMode defaultMode) :
//FEButton(gim, modifierKeys, oisMouseButtonId,
//         name, aparent, acolor,
//         apos, asize, NULL,"Boats/SCV","SCV info",defaultMode)
////pressed(false), enabled(false)
//{
//
//}
//
//OgreGFX::FESCVButton::~FESCVButton()
//{
//
//}
//
//void OgreGFX::FESCVButton::handleMouseEvent(const OIS::MouseEvent &args)
//{
//    int rangex = 2000;
//    int rangez = rangex;
//    DEBUG(std::cout << "Button checking:" << args.state.X.abs << "," << args.state.Y.abs << "|" << this->pos.x << "," << this->pos.y << std::endl;)
//    if (enabled && cursonInMe(Ogre::Vector2(args.state.X.abs, args.state.Y.abs)))
//    {
//        Ogre::Vector3 spawnpos = gfx->engine->selectionMgr->primarySelection->pos;
//        spawnpos.z = 500.0f;
//        spawnpos.y = 0.0f;
//        spawnpos.x = 500.0f;
//
//        FastEcslent::Entity* ent = gfx->engine->entityMgr->createEntityAfterTime(FastEcslent::SCV, Ogre::Vector3(0, 0, 0), 0.0f);
//        ent->init();
//        ent->pos = spawnpos;
//        //z = rangez - random() % (2 * rangez);
//        //x = rangex - random() % (2 * rangex);
//        ent->heading = (random() % 180) * 0.0174532925;
//        ent->yaw = ent->heading;
//        ent->desiredHeading = ent->heading;
//        ent->desiredSpeed = 0.0f;
//        //std::cout << "SCVButton Manager: " << ent->uiname << std::endl;
//        //        ent->print();
//        //        gfx->engine->selectionMgr->primarySelection->print();
//        gfx->makeNode(ent);
//        gfx->nGFXNodes++;
//
//
//        //this->color = Ogre::ColourValue(0.0, 0.0, 0.7, 0.0);
//        //std::cout << "Button pressed" << std::endl;
//        //if(pClickFunction)
//        //    pClickFunction();
//    }
//}
//
////--------------------------------------------------------------------------------------------------------------------------------
//
//OgreGFX::FEMarineButton::FEMarineButton(GraphicsInteractionManager *gim,
//                                  const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
//                                  std::string name, Widget *aparent, Ogre::ColourValue acolor,
//                                  Ogre::Vector2 apos, Ogre::Vector2 asize, Ogre::GuiMetricsMode defaultMode) :
//FEButton(gim, modifierKeys, oisMouseButtonId,
//         name, aparent, acolor,
//         apos, asize, NULL,"Boats/MARINE", "Marine Info",defaultMode)
////pressed(false), enabled(false)
//{
//
//}
//
//OgreGFX::FEMarineButton::~FEMarineButton()
//{
//
//}
//
//void OgreGFX::FEMarineButton::handleMouseEvent(const OIS::MouseEvent &args)
//{
//    int rangex = 2000;
//    int rangez = rangex;
//    DEBUG(std::cout << "Button checking:" << args.state.X.abs << "," << args.state.Y.abs << "|" << this->pos.x << "," << this->pos.y << std::endl;)
//    if (enabled && cursonInMe(Ogre::Vector2(args.state.X.abs, args.state.Y.abs)))
//    {
//        Ogre::Vector3 spawnpos = gfx->engine->selectionMgr->primarySelection->pos;
//        spawnpos.z = 500.0f;
//        spawnpos.y = 0.0f;
//        spawnpos.x = 0.0f;
//
//        FastEcslent::Entity* ent = gfx->engine->entityMgr->createEntityAfterTime(FastEcslent::MARINE, Ogre::Vector3(0, 0, 0), 0.0f);
//        ent->init();
//        ent->pos = spawnpos;
//        //z = rangez - random() % (2 * rangez);
//        //x = rangex - random() % (2 * rangex);
//        ent->heading = (random() % 180) * 0.0174532925;
//        ent->yaw = ent->heading;
//        ent->desiredHeading = ent->heading;
//        ent->desiredSpeed = 0.0f;
//        //std::cout << "SCVButton Manager: " << ent->uiname << std::endl;
//        //        ent->print();
//        //        gfx->engine->selectionMgr->primarySelection->print();
//        gfx->makeNode(ent);
//        gfx->nGFXNodes++;
//
//
//        //this->color = Ogre::ColourValue(0.0, 0.0, 0.7, 0.0);
//        //std::cout << "Button pressed" << std::endl;
//        //if(pClickFunction)
//        //    pClickFunction();
//    }
//}

//--------------------------------------------------------------------------------------------------------------------------------

OgreGFX::FELabelPair::FELabelPair(GraphicsInteractionManager *gim, std::string caption1, std::string caption2, Widget *aparent,
		Ogre::ColourValue color, Ogre::Vector2 apos, Ogre::Vector2 columnWidths, int columnHeight, std::string materialName, Ogre::GuiMetricsMode defaultMode):
		OgreGFX::FEPanel(gim, caption1, aparent, apos, Ogre::Vector2(columnWidths.x + columnWidths.y, columnHeight), materialName, defaultMode) {

	this->left = new OgreGFX::FELabel(gim, caption1, this, color, apos, Ogre::Vector2(columnWidths.x, columnHeight), defaultMode);
	this->right = new OgreGFX::FELabel(gim, caption2, this, color, Ogre::Vector2(apos.x+columnWidths.x, apos.y), Ogre::Vector2(columnWidths.y, columnHeight),defaultMode);
	this->addItem(this->left, Below);
	this->addItem(this->right, Right);

}

void OgreGFX::FELabelPair::setLeft(std::string val){
	this->left->setCaption(val);
}


void OgreGFX::FELabelPair::setRight(std::string val){

	this->right->setCaption(val);
}

void OgreGFX::FELabelPair::show(){
	DEBUG(std::cout << "Showing labelpair" << std::endl;)
	this->panel->show();
	this->left->show();
	this->right->show();
}

void OgreGFX::FELabelPair::hide(){
	this->panel->hide();
	this->left->hide();
	this->right->hide();

}

/****************************/
OgreGFX::FEProgressBar::FEProgressBar(GraphicsInteractionManager *gim, std::string name, Widget* iparent,
		Ogre::Vector2 ipos, Ogre::Vector2 isize, std::string barMaterial, std::string fillMaterial,Ogre::GuiMetricsMode defaultMode):
		Widget(gim, iparent, ipos, isize,defaultMode)
{
                this->barMaterial = barMaterial;
                //this->fillMaterial = fillMaterial;
                this->percentFull = 0.0f;
		this->name = name;
		this->overlayManagerPtr = Ogre::OverlayManager::getSingletonPtr();
		this->progressBar = static_cast<Ogre::OverlayContainer*>(this->overlayManagerPtr->createOverlayElement("Panel", gfx->widgetMgr->getNextId(name)));
		this->progressBar->setMetricsMode(this->mode);
		this->progressBar->setPosition(pos.x, pos.y);
		//size.y = 0; // or extra space when adding labels to this
		this->progressBar->setDimensions(size.x, size.y);
		this->progressBar->setMaterialName(barMaterial);
		this->progressBar->show();
                this->UpdateFillPercent();
//                this->progressFill = static_cast<Ogre::OverlayContainer*>(this->overlayManagerPtr->createOverlayElement("Panel", gfx->widgetMgr->getNextId(name)));
//		this->progressFill->setMetricsMode(this->mode);
//		this->progressFill->setPosition(pos.x, pos.y);
//		//size.y = 0; // or extra space when adding labels to this
//		this->progressFill->setDimensions(size.x, size.y);
//		this->progressFill->setMaterialName(fillMaterial);
//		this->progressFill->show();


}

void OgreGFX::FEProgressBar::UpdateFillPercent()
{
    //progressFill->setWidth(progressBar->getWidth() * percentFull);
    //progressBar->setWidth(this->size.x * percentFull);
    progressBar->setWidth(this->parent->size.x * percentFull);
}

void OgreGFX::FEProgressBar::show()
{
    this->progressBar->show();
    //this->progressFill->show();
    
}
void OgreGFX::FEProgressBar::hide()
{
    this->progressBar->hide();
    //this->progressFill->hide();    
}
void OgreGFX::FEProgressBar::posChanged()
{
    this->progressBar->setPosition(this->pos.x, this->pos.y);
    //this->progressFill->setPosition(this->pos.x, this->pos.y);
}

Ogre::OverlayElement * OgreGFX::FEProgressBar::getOverlayElementToAdd(){
	return static_cast<Ogre::OverlayElement *>(this->progressBar);
}
