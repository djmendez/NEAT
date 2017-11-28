/*
 * lobbyMgr.cpp
 *
 */
#include <GraphicsInteractionManager.h>
#include <lobbyMgr.h>

#include "snd/SoundMgr.h"

OgreGFX::LobbyMgr::LobbyMgr(FastEcslent::Engine *eng, GraphicsInteractionManager *gim, OIS::Mouse *m, OIS::Keyboard *k)
							: GFXMgr(gim), engine(eng), mouse(m), keyboard(k){
	mouse->setEventCallback(this);
	keyboard->setEventCallback(this);

	startTime = getCurrentTime();
	state = GAMEMENU;

	init();
}

bool OgreGFX::LobbyMgr::frameStarted(const Ogre::FrameEvent& evt){
    keyboard->capture();
    mouse->capture();

   	//This is set by remote server, lauch by the same thread
    if(engine->gameState == FastEcslent::GAME){
    	loadGame();
    }

	return true;
}

bool OgreGFX::LobbyMgr::frameRenderingQueued(const Ogre::FrameEvent& evt){
   	this->refreshServerAndSlots();
    return true;
}

bool OgreGFX::LobbyMgr::frameEnded(const Ogre::FrameEvent& evt){
	return true;
}

/**
 * Key/Mouse handler
 */
bool OgreGFX::LobbyMgr::keyPressed( const OIS::KeyEvent &arg ){
	return true;
}
bool OgreGFX::LobbyMgr::keyReleased( const OIS::KeyEvent &arg ){
    return true;
}
bool OgreGFX::LobbyMgr::mouseMoved( const OIS::MouseEvent &arg ){
	mTrayMgr->injectMouseMove(arg);
    return true;
}
bool OgreGFX::LobbyMgr::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
	mTrayMgr->injectMouseDown(arg,id);
	return true;
}
bool OgreGFX::LobbyMgr::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
	mTrayMgr->injectMouseUp(arg,id);
    return true;
}

void OgreGFX::LobbyMgr::init(){
	exitQuestion = "Are you sure you want to exit?";
	engine->gameState = FastEcslent::LOBBY;
	LOBBY_TOLERANCE = 4000; //4 seconds

	initGUIGameMenu();
	loadOptions();

	netLobby = new FastEcslent::NetLobby(engine);
	netLobby->run();

	mTrayMgr->showBackdrop("FESdkTrays/FEShade");
	mTrayMgr->showAll();
}

void OgreGFX::LobbyMgr::initialize(){

}

void OgreGFX::LobbyMgr::buttonHit(OgreBites::Button* button){
	if (button == lobbyStartBt){   //start game button clicked
		this->startGame(netLobby->myIPAddress, engine->options.networkPort);
	}else if(button == createBt){  	//create a lobby button clicked
		this->clearPlayerLabel();

		this->mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->hide();
		this->mTrayMgr->getTrayContainer(OgreBites::TL_RIGHT)->hide();
		this->mTrayMgr->getTrayContainer(OgreBites::TL_NONE)->show();

		std::string lobbyMsg = this->getLobbyString("Lobby", netLobby->myIPAddress,  engine->options.networkPort);
		lobbyLb->setCaption(lobbyMsg);

		this->addNode("ServerName", netLobby->myIPAddress, engine->options.networkPort, FastEcslent::RED, 0, 0, true);

		this->switchState();
	}else if(button == joinBt){    //join a lobby button clicked
		//nothing in the server list.
		if(remoteServerMenu->getNumItems() <= 0){
			return;
		}
		this->joinGame();
		this->switchState();
	}else if(button == lobbyExitBt){  //exit lobby button clicked
		this->handleExitButton();
	}else if(button == exitBt){     //Exit system
		mTrayMgr->showYesNoDialog("Exit", exitQuestion);
	}
}

void OgreGFX::LobbyMgr::itemSelected(OgreBites::SelectMenu* menu){
	std::string menuCaption = menu->getCaption().substr(0,4);
	if(menuCaption == "Side" && menu->getSelectedItem().length() > 0){
		int index = menu->getSelectionIndex();
		std::string number = menu->getName().substr(4,1);
		int slot = atoi(number.c_str());
		nodeMap[slots[slot]]->side = index == 0?FastEcslent::RED:FastEcslent::BLUE;
		int slotNew = this->getEmptySlot((int)(nodeMap[slots[slot]]->side));
		if(slotNew != -1 && engine->options.isServer){
			this->slotSwap(slot, slotNew);
		}else{
			std::pair<std::string, int> host = this->getLobbyHost();
			int side = slotNew/2 > 0?FastEcslent::BLUE:FastEcslent::RED;
			int player = slotNew;
			netLobby->lobbySlotSwapReq(host.first, host.second, slotNew, slot, side, player);
		}
	}
}

void OgreGFX::LobbyMgr::labelHit(OgreBites::Label* label){
}

void OgreGFX::LobbyMgr::sliderMoved(OgreBites::Slider* slider){
}

void OgreGFX::LobbyMgr::checkBoxToggled(OgreBites::CheckBox* box){
	if(box == serverCB){
		if(box->isChecked()){
			engine->options.isServer = true;
			remoteServerMenu->clearItems();
			this->mTrayMgr->getTrayContainer(OgreBites::TL_RIGHT)->hide();
			this->createBt->show();
		}else{
			engine->options.isServer = false;
			remoteServerMenu->clearItems();
			this->mTrayMgr->getTrayContainer(OgreBites::TL_RIGHT)->show();
			this->createBt->hide();
		}
	}else if(box == soundCB){
		if(box->isChecked()){
                    engine->gfx->gimPtr->soundMgr->enable();
		}else{
                    engine->gfx->gimPtr->soundMgr->disable();
		}
	}
}
void OgreGFX::LobbyMgr::okDialogClosed(const Ogre::DisplayString& message) {
}
void OgreGFX::LobbyMgr::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit){
	if(question == exitQuestion && yesHit){
		exit(0);
	}
}

void OgreGFX::LobbyMgr::loadGame(){
	this->configureGame();
	mTrayMgr->hideAll();

	gfx->mRoot->removeFrameListener(this);

	//launch clients
	if(engine->options.isServer) {
		netLobby->lobbyStart();
	}

	//gfx thread start to render its own frame
	gfx->loadGameGFX();

	netLobby->quit = true;
}

void OgreGFX::LobbyMgr::joinGame(){
	this->lobbyStartBt->hide();

	std::string ip = remoteServerMenu->getSelectedItem();
	int port = remoteServerMap[ip]->port;
	netLobby->lobbyJoinGame(ip, port);
}

void OgreGFX::LobbyMgr::initGUIGameMenu(){
	//mTrayMgr = new OgreBites::SdkTrayManager("Game Menu", gfx->mWindow, mouse, this);
	Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();

	//Lobby, None Tray
	this->initGUILobby();
	//Calculate the positions of all the components in None Tray.
	this->setNoneTrayPosition();

	//Center Tray
	logo = mTrayMgr->createDecorWidget(OgreBites::TL_CENTER, "DebugRTTPanel", "FESdkTrays/FELogo");
	separatorCenter = mTrayMgr->createSeparator(OgreBites::TL_CENTER, "SeparatorCenter", GameNemuWidth);
	soundCB = mTrayMgr->createCheckBox(OgreBites::TL_CENTER, "SoundCB", "Sound", GameNemuWidth);
	serverCB = mTrayMgr->createCheckBox(OgreBites::TL_CENTER, "ServerCB", "Server", GameNemuWidth);
	createBt = mTrayMgr->createButton(OgreBites::TL_CENTER, "CreateButton", "Create", GameNemuWidth);
	exitBt = mTrayMgr->createButton(OgreBites::TL_CENTER, "ExitButton", "Exit",GameNemuWidth);

	//remote server menu, Right Tray
	remoteServerMenu = mTrayMgr->createThickSelectMenu(OgreBites::TL_RIGHT, "LobbyServer", "SERVER",GameNemuWidth,10);
	joinBt = mTrayMgr->createButton(OgreBites::TL_RIGHT, "JoinButton", "Join");

	//Hide None tray, none tray contains player slots
	this->mTrayMgr->getTrayContainer(OgreBites::TL_NONE)->hide();
        
        //default value for sound is "enabled"
        soundCB->setChecked(true);
}

void OgreGFX::LobbyMgr::initGUILobby(){
	lobbyLb = mTrayMgr->createLabel(OgreBites::TL_NONE, "LobbyName", "LobbyName", LobbyTitleWidth);
	separatorTop = mTrayMgr->createSeparator(OgreBites::TL_NONE, "SeparatorTop", LobbySepWidth);

	for(int i=0; i< FastEcslent::MaxPlayers; i++){
		std::string player = "Player";
		std::string side = "Side";
		playerLb[i] = mTrayMgr->createLabel(OgreBites::TL_NONE, player.append(int_to_string(i)), "", LobbyPlayerWidth);
		playerSide[i] = mTrayMgr->createLongSelectMenu(OgreBites::TL_NONE, side.append(int_to_string(i)), "Side :", LobbyPlayerSideWidth,10 );
		playerSide[i]->addItem("RED");
		playerSide[i]->addItem("BLUE");
	}

	lobbyStartBt = mTrayMgr->createButton(OgreBites::TL_NONE, "LobbyStartButton", "Start Game", LobbyBtnWidth);
	lobbyExitBt = mTrayMgr->createButton(OgreBites::TL_NONE, "LobbyExitButton", "Exit Lobby", LobbyBtnWidth);
}

void OgreGFX::LobbyMgr::setNoneTrayPosition(){
	int height = gfx->mWindow->getHeight();
	int width = gfx->mWindow->getWidth();

	int gapVinit = (height - LobbyGapV * 10)/2;
	lobbyLb->getOverlayElement()->setPosition((width - LobbyTitleWidth)/2, gapVinit+LobbyGapV);
	separatorTop->getOverlayElement()->setPosition((width - LobbySepWidth)/2, gapVinit+LobbyGapV*2);

	for(int i=0; i< FastEcslent::MaxPlayers; i++){
		int left = (width - LobbyPlayerWidth - LobbyPlayerSideWidth*2 - LobbyGapH)/2;
		int top = (i <= 1)?(gapVinit+LobbyGapV*2+LobbyGapV*(i+1)):(gapVinit+LobbyGapV*3+LobbyGapV*(i+1));
		playerLb[i]->getOverlayElement()->setPosition(left, top);
		playerSide[i]->getOverlayElement()->setPosition(left+LobbyPlayerWidth+ LobbyGapH, top);
	}

	lobbyStartBt->getOverlayElement()->setPosition((width- LobbyBtnWidth*2 - LobbyGapH)/2, gapVinit+LobbyGapV*9);
	lobbyExitBt->getOverlayElement()->setPosition((width + LobbyGapH)/2 , gapVinit+LobbyGapV*9);
}

void OgreGFX::LobbyMgr::loadOptions(){
	if(engine->options.isServer){
		serverCB->setChecked(true);
		this->mTrayMgr->getTrayContainer(OgreBites::TL_RIGHT)->hide();
	}else{
		serverCB->setChecked(false);
		this->mTrayMgr->getTrayContainer(OgreBites::TL_RIGHT)->show();
	}
}

void OgreGFX::LobbyMgr::addRemoteServer(const std::string &gName, const std::string &ip, int port){
	LobbyNode *ln = new LobbyNode();
	ln->name = gName;
	ln->ip = ip;
	ln->port = port;

	ln->time = (getCurrentTime() - startTime).total_milliseconds();
	remoteServerMap[ip] = ln;
}

void OgreGFX::LobbyMgr::addNode(const std::string &gName, const std::string &clientip, int port, int side, int player, int slot, bool isHost){
	//already in position
	if(nodeMap.find(clientip)!=nodeMap.end() ){
		if(nodeMap[clientip]->slot == slot){
			return;
		}else{
			this->removeNodeFromSlot(clientip);
		}
	}

	LobbyNode *lci = new LobbyNode();
	lci->name = gName;
	lci->ip = clientip;
	lci->port = port;
	lci->side = (FastEcslent::Side)side;
	lci->player =  (FastEcslent::Player)player;
	lci->isHost = isHost;
	lci->slot = slot;

	lci->time = (getCurrentTime() - startTime).total_milliseconds();
	nodeMap[clientip] = lci;
	slots[slot] = clientip;
}

void OgreGFX::LobbyMgr::removeNodeFromSlot(const std::string &clientip){
	LobbyNode *l = nodeMap[clientip];
	int slot = l->slot;
	nodeMap.erase(clientip);
	delete l;
	slots[slot] = "";
}

void OgreGFX::LobbyMgr::handleJoinRequest(const std::string &clientName, const std::string &clientip, int port){
	LobbyNode *lci = new LobbyNode();
	lci->name = clientName;
	lci->ip = clientip;
	lci->port = port;
	lci->time = (getCurrentTime() - startTime).total_milliseconds();

	nodeMap[clientip] = lci;

	int slot = this->getEmptySlot();
	if(slot != -1){
		//assign a new side and player
		lci->side = (slot < FastEcslent::MaxPlayers/2)? (FastEcslent::RED):(FastEcslent::BLUE);
		lci->player = (FastEcslent::Player)slot;
		lci->slot = slot;
		slots[slot] = lci->ip;
		netLobby->sendLobbyClientResponse(clientip, port, lci->side, lci->player );
	}
}

void OgreGFX::LobbyMgr::lobbyEnter(const std::string &serverip, int port, int side, int player){
	mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->hide();
	mTrayMgr->getTrayContainer(OgreBites::TL_RIGHT)->hide();
	mTrayMgr->getTrayContainer(OgreBites::TL_NONE)->show();
	std::string txt = this->getLobbyString(remoteServerMap[serverip]->name, serverip, port);
	lobbyLb->setCaption(txt);

	engine->options.side = (FastEcslent::Side)side;
	engine->options.player = (FastEcslent::Player)player;
}

void OgreGFX::LobbyMgr::swapNodeSlot(int newslot, int oldslot, int newside, int newplayer){
	this->nodeMap[this->slots[oldslot]]->side = (FastEcslent::Side)newside;
	this->nodeMap[this->slots[oldslot]]->player = (FastEcslent::Player)newplayer;
	this->slotSwap(newslot, oldslot);
}

void OgreGFX::LobbyMgr::lobbyExit(const std::string &clientip, int port){
	if(engine->options.isServer){
		for(int i=0;i<FastEcslent::MaxPlayers;i++){
			if(slots[i] == clientip){
				slots[i] = "";
			}
		}

		LobbyNode *info = nodeMap[clientip];
		nodeMap.erase(clientip);
		delete info;
	}else if(state == LOBBY){
		this->handleExitButton();
	}
}

void OgreGFX::LobbyMgr::startGame(const std::string &ip, int port){
	std::pair<std::string, int> host = this->getLobbyHost();
	if(host.first.size() > 0 && host.first == ip && host.second == port){
		engine->gameState = FastEcslent::GAME;
	}
}

std::pair<std::string, int> OgreGFX::LobbyMgr::getLobbyHost(){
	std::string hostip = "";
	int port = 0;
	for(std::map<std::string, LobbyNode*>::iterator i = nodeMap.begin(); i!= nodeMap.end();i++){
		LobbyNode *info = i->second;
		if (info && info->isHost) {
			hostip = info->ip;
			port = info->port;
			break;
		}
	}
	return std::pair<std::string, int>(hostip, port);
}
std::string OgreGFX::LobbyMgr::getLobbyString(const std::string &lobbyName, const std::string &ip, int port){
	std::string txt = lobbyName;
	txt.append(" - ").append(ip).append(" : ").append(int_to_string(port));
	return txt;
}

void OgreGFX::LobbyMgr::clearServerMap(){
	for(std::map<std::string, LobbyNode*>::iterator i = remoteServerMap.begin(); i!= remoteServerMap.end();i++){
		LobbyNode *info = i->second;
		remoteServerMap.erase(i);
		delete info;
	}
}

void OgreGFX::LobbyMgr::clearNodeMap(){
	for(int i=0; i< FastEcslent::MaxPlayers;i++){
		slots[i] = "";
	}

	for(std::map<std::string, LobbyNode*>::iterator i = nodeMap.begin(); i!= nodeMap.end();i++){
		LobbyNode *info = i->second;
		nodeMap.erase(i);
		delete info;
	}
}

std::string OgreGFX::LobbyMgr::getPlayerString(const std::string &playerName, const std::string &ip){
	std::string txt = playerName;
	txt.append(" - ").append(ip);
	return txt;
}

void OgreGFX::LobbyMgr::refreshServerAndSlots(){
	if(state == GAMEMENU){
		//client needs to update server list
		if(!engine->options.isServer){
			//refresh server map, remove the timeout node
			for(std::map<std::string, LobbyNode*>::iterator i = remoteServerMap.begin(); i!= remoteServerMap.end();i++){
				LobbyNode *info = i->second;
				if((getCurrentTime() - startTime).total_milliseconds() - info->time >= LOBBY_TOLERANCE ){
					remoteServerMap.erase(i);
					delete info;
					remoteServerMenu->clearItems();
					break;
				}
			}

			if(remoteServerMenu->getNumItems() != remoteServerMap.size()){
				for(std::map<std::string, LobbyNode*>::iterator i = remoteServerMap.begin(); i!= remoteServerMap.end();i++){
					remoteServerMenu->addItem(i->first);
				}
			}
		}
	}else if(state == LOBBY){
		for(int i=0; i<FastEcslent::MaxPlayers; i++){
			if(slots.find(i) != slots.end() && slots[i].length() > 0){
				if(nodeMap.find(slots[i]) != nodeMap.end()){
					std::string cap = this->getPlayerString(nodeMap[slots[i]]->name, nodeMap[slots[i]]->ip);
					playerLb[i]->setCaption(cap);
					FastEcslent::Side s = nodeMap[slots[i]]->side;
					playerSide[i]->selectItem(s==FastEcslent::RED?"RED":"BLUE", false);
					playerSide[i]->show();
				}
			}else{
				playerLb[i]->setCaption("");
				playerSide[i]->hide();
			}
		}
	}
}

int OgreGFX::LobbyMgr::getEmptySlot(){
	for(int i=0;i<FastEcslent::MaxPlayers;i++){
		if(slots.find(i) == slots.end() || slots[i].length() == 0){
			return i;
		}
	}
	return -1;
}

int OgreGFX::LobbyMgr::getEmptySlot(int side){
	for(int i=0; i< FastEcslent::MaxPlayers/2;i++){
		if(!(slots.find((side-1)*FastEcslent::MaxPlayers/2+i) != slots.end() && slots[(side-1)*FastEcslent::MaxPlayers/2+i].length() > 0)){
			return (side-1)*FastEcslent::MaxPlayers/2+i;
		}
	}
	return -1;
}

void OgreGFX::LobbyMgr::slotSwap(int slot1,int slot2){
	std::string p1 = slots[slot1];
	std::string p2 = slots[slot2];
	if(nodeMap[p1]){
		nodeMap[p1]->slot = slot2;
	}
	if(nodeMap[p2]){
		nodeMap[p2]->slot = slot1;
	}

	slots[slot2] = p1;
	slots[slot1] = p2;
}

void OgreGFX::LobbyMgr::clearPlayerLabel(){
	for(int i=0; i< FastEcslent::MaxPlayers;i++){
		playerLb[i]->setCaption("");
	}
}

void  OgreGFX::LobbyMgr::configureGame(){
	int slot = -1;
	for(int i=0;i<FastEcslent::MaxPlayers;i++){
		if(slots[i] == this->netLobby->myIPAddress){
			slot = i;
			break;
		}
	}

	engine->options.side = ((slot+1) / (FastEcslent::MaxPlayers/2)==0?FastEcslent::RED:FastEcslent::BLUE);
	engine->options.player = (FastEcslent::Player)slot;
}

void OgreGFX::LobbyMgr::switchState(){
	if(state == GAMEMENU){
		state = LOBBY;
	}else{
		state = GAMEMENU;
	}
}

void OgreGFX::LobbyMgr::handleExitButton(){
	this->mTrayMgr->getTrayContainer(OgreBites::TL_NONE)->hide();
	this->mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->show();
	if(!engine->options.isServer){
		this->mTrayMgr->getTrayContainer(OgreBites::TL_RIGHT)->show();
	}

	this->lobbyStartBt->show();

	//Send exit lobby to server
	std::pair<std::string, int> hostNode = this->getLobbyHost();
	if(hostNode.first.length() > 0){
		netLobby->lobbyExitGame(hostNode.first, hostNode.second);
	}
	this->clearServerMap();
	this->clearNodeMap();

	this->switchState();
}

OgreGFX::LobbyMgr::~LobbyMgr(){
	delete mTrayMgr;

	mTrayMgr = 0;

	delete netLobby;
}



