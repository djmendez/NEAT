/*
 * netLobby.cpp
 *
 *  Created on: August 10, 2013
 *      Author: Siming
 */
#include <netLobby.h>
#include <netThread.h>
#include <engine.h>
#include <string>
#include <messages.h>

using boost::asio::ip::udp;

FastEcslent::NetLobby::NetLobby(Engine* eng):engine(eng) {
	init();
}

void FastEcslent::NetLobby::run(){
	netThread = boost::thread(&FastEcslent::NetLobby::runThread, this);
}

void FastEcslent::NetLobby::init() { //initialize sockets, public
	netCommon = new NetCommon();

	std::pair<boost::asio::ip::udp::endpoint, std::string> ippair = netCommon->getMyIP();

	udp::endpoint myIP =ippair.first;
	myIPAddress = ippair.second;

	listener = new Listener(myIP, engine->options.networkPort, engine->options.isServer);
	listener->init(myIPAddress);
	sender   = new Sender(myIP, engine->options.networkPort, engine->options.isServer); // if isServer -> broadcast
	sender->init();

	quit = false;
	startTime = getCurrentTime();
	TICK_SLEEP_TIME = 10;
	sleepTime = new boost::posix_time::milliseconds(100);
	lobbySleepTime = new boost::posix_time::milliseconds(300);
}

void FastEcslent::NetLobby::runThread(){ // run the netManager thread, private (each tick)
	listener->run();
	sender->run();

	while (!quit){
		if (engine->options.isServer) {
			serverTick();
			boost::this_thread::sleep(*(lobbySleepTime));
		} else {
			clientTick();
			boost::this_thread::sleep(*(sleepTime));
		}
	}
}

/**
 * Lobby broadcast message
 */
void FastEcslent::NetLobby::serverTick(){
	sendLobbyServerInfo();
	handleClientMessages();
}

void FastEcslent::NetLobby::sendLobbyServerInfo(){
	if(!(engine->gfx->gimPtr->lobbyMgr))
		return;

	Message *m = new Message();
	m->head.msgType = LOBBYSERVERTYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.sizeOfStruct    = LobbyServerSize;

	//add self as server
	unsigned int messageSize = 0;
	unsigned int offset = 0;

	for(std::map<std::string, OgreGFX::LobbyNode*>::iterator i= engine->gfx->gimPtr->lobbyMgr->nodeMap.begin();i!= engine->gfx->gimPtr->lobbyMgr->nodeMap.end();i++){
		if(i->second){
			LobbyServer *ls = fillLobbyClients(i->second->name,   i->second->ip, i->second->port, i->second->side, i->second->player, i->second->slot, i->second->isHost);
			memcpy((void*)(m->data + offset), (void *)ls, LobbyServerSize);
			offset += LobbyServerSize;
			delete ls;
			messageSize++;
		}
	}

	m->head.numberOfStructs = messageSize;
	sender->addMessage(m);
}

FastEcslent::LobbyServer* FastEcslent::NetLobby::fillLobbyClients(const std::string &gameName, const std::string &ip, int port, int side, int player, int slot, bool isHost){
	LobbyServer *l = new LobbyServer();
	l->port = port;

	char gName[64];
	sprintf(gName, "%63s", gameName.c_str());
	strcpy(l->name, gName);

	l->ip = ip_string_to_int(ip);
	l->side = side;
	l->player = player;
	l->slot = slot;
	l->isHost = isHost?'1':'0';
	return l;
}

void FastEcslent::NetLobby::joinRequest(Message *m){
	LobbyJoinRequest *req = new LobbyJoinRequest();
	memcpy((void *)req, (void*)(m->data), LobbyJoinRequestSize);

	if(ip_int_to_string(req->serverIP) == myIPAddress){
		engine->gfx->gimPtr->lobbyMgr->handleJoinRequest(trim(std::string(req->clientName)), ip_int_to_string(req->clientIP), req->port);
	}

	delete req;
}

void FastEcslent::NetLobby::startByRemote(Message *m){
	LobbyGameStart *ls = new LobbyGameStart();
	memcpy((void *)ls, (void*)(m->data), LobbyGameStartSize);

	engine->gfx->gimPtr->lobbyMgr->startGame(ip_int_to_string(ls->serverIP),ls->port);

	delete ls;
}

void FastEcslent::NetLobby::sendLobbyClientResponse(const std::string &clientIP, int port, int side, int player){
	Message *m = new Message();
	LobbyJoinResponse *l = new LobbyJoinResponse();
	l->port = engine->options.networkPort;
	l->side = side;
	l->player = player;
	l->clientIP = ip_string_to_int(clientIP);
	l->serverIP = ip_string_to_int(myIPAddress);

	m->head.msgType = LOBBYJOINRESPONSETYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1;
	m->head.sizeOfStruct    = LobbyJoinResponseSize;

	memcpy((void*)(m->data) , (void *)l, LobbyJoinResponseSize);

	sender->addMessage(m);

	delete l;
}


void FastEcslent::NetLobby::clientTick(){
	handleServerMessages();
}

void FastEcslent::NetLobby::updateLobbyServer(Message *m){
	unsigned int offset = 0;
	for(int i=0;i<m->head.numberOfStructs;i++){
		LobbyServer *ls = new LobbyServer();
		memcpy((void *)ls, (void*)(m->data + offset), LobbyServerSize);
		offset += LobbyServerSize;

		std::string gName = ls->name;
		std::string ip =ip_int_to_string(ls->ip);
		int port = ls->port;
		int side = ls->side;
		int player = ls->player;
		int slot = ls->slot;
		bool isHost = ls->isHost=='1'?true:false;
		gName = trim(gName);

		if(engine->gfx->gimPtr->lobbyMgr){
			if(isHost){
				engine->gfx->gimPtr->lobbyMgr->addRemoteServer(gName, ip, port);
			}

			engine->gfx->gimPtr->lobbyMgr->addNode(gName, ip, port, side, player, slot, isHost);
		}

		delete ls;
	}
}

void FastEcslent::NetLobby::handleClientMessages(){
	Message *m = listener->dequeMessage();
	while (m) {
		if((int) m->head.msgType == FastEcslent::LOBBYJOINREQUESTTYPE) {
			joinRequest(m);
		}else if ((int) m->head.msgType == FastEcslent::LOBBYEXITTYPE){
			clientExit(m);
		}else if ((int) m->head.msgType == FastEcslent::LOBBYSLOTSWAPTYPE){
			clientSwap(m);
		}
		delete m;
		m = listener->dequeMessage();
	}
}

void FastEcslent::NetLobby::handleServerMessages(){
	Message *m = listener->dequeMessage();
	while (m) {
		if ((int)m->head.msgType == FastEcslent::LOBBYSERVERTYPE){  //lobby broadcast message
			updateLobbyServer(m);
		}else if ((int)m->head.msgType == FastEcslent::LOBBYJOINRESPONSETYPE){
			clientEnter(m);
		}else if ((int)m->head.msgType == FastEcslent::LOBBYGAMESTARTTYPE){
			startByRemote(m);
		}else if((int) m->head.msgType == FastEcslent::LOBBYEXITTYPE){
			serverExit(m);
		}
		delete m;
		m = listener->dequeMessage();
	}
}

void FastEcslent::NetLobby::clientEnter(Message *m){
	LobbyJoinResponse *ljr = new LobbyJoinResponse();
	memcpy((void *)ljr, (void *)(m->data), LobbyJoinResponseSize);
	if(engine->gfx->gimPtr->lobbyMgr){
		engine->gfx->gimPtr->lobbyMgr->lobbyEnter(ip_int_to_string(ljr->serverIP), ljr->port, ljr->side, ljr->player);
	}
	delete ljr;
}

void FastEcslent::NetLobby::clientExit(Message *m){
	LobbyExit *ljr = new LobbyExit();
	memcpy((void *)ljr, (void *)(m->data), LobbyExitSize);
	if(engine->gfx->gimPtr->lobbyMgr){
		std::pair<std::string, int> host = engine->gfx->gimPtr->lobbyMgr->getLobbyHost();
		if(ip_int_to_string(ljr->serverIP) == host.first && host.second == host.second){
			engine->gfx->gimPtr->lobbyMgr->lobbyExit(ip_int_to_string(ljr->clientIP), ljr->port);
		}
	}
	delete ljr;
}

void FastEcslent::NetLobby::serverExit(Message *m){
	LobbyExit *ljr = new LobbyExit();
	memcpy((void *)ljr, (void *)(m->data), LobbyExitSize);
	if(engine->gfx->gimPtr->lobbyMgr){
		std::pair<std::string, int> host = engine->gfx->gimPtr->lobbyMgr->getLobbyHost();
		if(ip_int_to_string(ljr->serverIP) == host.first && host.second == host.second){
			engine->gfx->gimPtr->lobbyMgr->lobbyExit(ip_int_to_string(ljr->clientIP), ljr->port);
		}
	}
	delete ljr;
}

void FastEcslent::NetLobby::clientSwap(Message *m){
	LobbySlotSwap *ls = new LobbySlotSwap();
	memcpy((void *)ls, (void *)(m->data), LobbySlotSwapSize);
	if(engine->gfx->gimPtr->lobbyMgr){
		if(ip_int_to_string(ls->serverIP) == myIPAddress && ls->port == engine->options.networkPort){
			engine->gfx->gimPtr->lobbyMgr->swapNodeSlot(ls->newpos, ls->oldpos, ls->newside, ls->newplayer);
		}
	}
	delete ls;
}


void FastEcslent::NetLobby::lobbyStart(){
	Message *m = new Message();
	LobbyGameStart *ls = new LobbyGameStart();
	ls->port = engine->options.networkPort;
	ls->serverIP = ip_string_to_int(myIPAddress);

	m->head.msgType = LOBBYGAMESTARTTYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1;
	m->head.sizeOfStruct    = LobbyGameStartSize;

	memcpy((void*)(m->data) , (void *)ls, LobbyGameStartSize);

	sender->addMessage(m);

	delete ls;
}

void FastEcslent::NetLobby::lobbyJoinGame(const std::string &ipAddress, int port){
	Message *m = new Message();
	LobbyJoinRequest *l = new LobbyJoinRequest();
	l->port = port;
	l->serverIP =  ip_string_to_int(ipAddress);
	l->clientIP = ip_string_to_int(myIPAddress);

	char clientName[64];
	sprintf(clientName, "%63s", "clientName");
	strcpy(l->clientName, clientName);

	m->head.msgType = LOBBYJOINREQUESTTYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1;
	m->head.sizeOfStruct    = LobbyJoinRequestSize;

	memcpy((void*)(m->data) , (void *)l, LobbyJoinRequestSize);

	sender->addMessage(m);

	delete l;
}

void FastEcslent::NetLobby::lobbyExitGame(const std::string &serverIP, int port){
	Message *m = new Message();
	LobbyExit *le = new LobbyExit();

	le->port = port;

	le->serverIP = ip_string_to_int(serverIP);
	le->clientIP = ip_string_to_int(myIPAddress);

	m->head.msgType = LOBBYEXITTYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1;
	m->head.sizeOfStruct    = LobbyExitSize;

	memcpy((void*)(m->data) , (void *)le, LobbyExitSize);

	sender->addMessage(m);

	delete le;
}

void FastEcslent::NetLobby::lobbySlotSwapReq(const std::string &serverip, int port, int newpos, int oldpos, int newside, int newplayer){
	Message *m = new Message();
	LobbySlotSwap *le = new LobbySlotSwap();

	le->serverIP = ip_string_to_int(serverip);
	le->port = port;

	le->newpos = newpos;
	le->oldpos = oldpos;

	le->newside = newside;
	le->newplayer = newplayer;

	m->head.msgType = LOBBYSLOTSWAPTYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1;
	m->head.sizeOfStruct    = LobbySlotSwapSize;

	memcpy((void*)(m->data) , (void *)le, LobbySlotSwapSize);

	sender->addMessage(m);

	delete le;
}
FastEcslent::NetLobby::~NetLobby(){
	delete netCommon;
	delete listener;
	delete sender;
	delete sleepTime;
	delete lobbySleepTime;
}
