/*
 * netThread.cpp
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <netThread.h>

#include <engine.h>
#include <listener.h>
#include <sender.h>
#include <enums.h>
#include <messages.h>
#include <utils.h>
#include <GraphicsInteractionManager.h>

using boost::asio::ip::udp;


void FastEcslent::NetThread::init() { //initialize sockets, public
	std::cout << "Initializing Net before starting thread. Engine instance Id: " << engine->instanceId << std::endl;
	netCommon = new NetCommon();
	std::pair<udp::endpoint, std::string> ippair = netCommon->getMyIP();
	udp::endpoint myIP = ippair.first;
	myIPAddress = ippair.second;

	listener = new Listener(myIP, engine->options.networkPort, engine->options.isServer);
	listener->init(myIPAddress);
	sender   = new Sender(myIP, engine->options.networkPort, engine->options.isServer); // if isServer -> broadcast
	sender->init();
	quit = false;
	startTime = getCurrentTime();
	TICK_SLEEP_TIME = 10;
	sleepTime = new boost::posix_time::milliseconds(20);
	insideLobby = false;
}


void FastEcslent::NetThread::run() { //start thread, public
	netThread = boost::thread(&FastEcslent::NetThread::runThread, this);
	std::cout << "Running thread: " << netThread.get_id() << " Engine instance Id: " << engine->instanceId << std::endl;
}

void FastEcslent::NetThread::runThread(){ // run the netManaager thread, private (each tick)
	listener->run();
	sender->run();

	while (!quit){
		gameTick();
	}
}

void FastEcslent::NetThread::gameTick(){
	if(engine->gameState == LOBBY){
		return;
	}

	if (engine->options.isServer) {
		serverTick();
	} else {
		clientTick();
	}
}

namespace FastEcslent {

	State* fillStateFromEnt(FastEcslent::Entity *ent){
		State *s = new State();

		s->id = ent->entityId.id;

		s->hp = ent->hitpoints;
		s->timeLeftToBirth = ent->timeLeftToBirth;

		s->px = ent->pos.x;
		s->py = ent->pos.y;
		s->pz = ent->pos.z;

		s->vx = ent->vel.x;
		s->vy = ent->vel.y;
		s->vz = ent->vel.z;

		s->dh = ent->desiredHeading;
		s->ds = ent->desiredSpeed;

		s->rSpeed = 0.0;
		s->yaw    = ent->yaw;

		s->flag  = 0;//(unsigned short) 0;

		return s;
	}

	Info *fillInfoFromEnt(FastEcslent::Entity *ent){
		Info *info = new Info();
		char tmp[256];
		sprintf(tmp, "%255i", ent->entityType);
		strcpy(info->type, tmp);// = makeTypeStringFromEntityType(ent->entityType);
		sprintf(tmp, "%255s", ent->uiname.c_str()); //"%256s" convert to 256 + '/0'= 257, overlapped 1 char in type
		strcpy(info->label, tmp);
		//strcpy(info->label, ent->uiname.c_str());
		//pad(info->label, 256);
		info->beam = ent->width;
		info->draft = ent->depth;
		info->id    = ent->entityId.id;
		info->length = ent->length;
		info->playerId = ent->entityId.player;
		info->side = ent->entityId.side;
		info->maxSpeed = ent->maxSpeed;
		info->maxSpeedReverse = 0.0f;
		return info;
	}

	Message *makeMessageWithStateHeader(int nEnts, long dtime){
		Message *m = new Message();
		m->head.msgType = STATEMESSAGETYPE;
		m->head.millisecondsFromStart = dtime; //boost::posix_time::milliseconds(500)
		m->head.numberOfStructs = nEnts;
		m->head.sizeOfStruct    = StateSize;
		return m;
	}

	Message *makeMessageWithInfoHeader(int nEnts, long dtime){
		Message *m = new Message();
		m->head.msgType = INFOMESSAGETYPE;
		m->head.millisecondsFromStart = dtime; //boost::posix_time::milliseconds(500)
		m->head.numberOfStructs = nEnts;
		m->head.sizeOfStruct    = InfoSize;
		return m;
	}

}

void FastEcslent::NetThread::sendCommands(){
	sendUnknownEntQueries();
	combineCommandsIntoNetMessage();
	combineSquelchesIntoNetMessage();

}

void FastEcslent::NetThread::combineCommandsIntoNetMessage(){
	if (commandQueue.size() > 0){
		Message *m = new Message();

		m->head.msgType = COMMANDENTITYMESSAGETYPE;
		m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
		m->head.numberOfStructs = commandQueue.size() ;
		m->head.sizeOfStruct = CommandEntitySize;
		unsigned int offset = 0;
		for(int i=0; i< m->head.numberOfStructs; i++){
			CommandEntity* cmd = commandQueue.front();
			commandQueue.pop_front();
			memcpy((void*)(m->data + offset), (void *)cmd, CommandEntitySize);
			offset += CommandEntitySize;
			delete cmd;
		}

		sender->addMessage(m);
	}
}

void FastEcslent::NetThread::combineSquelchesIntoNetMessage(){
	if (squelchQueue.size() > 0){
		Message *m = new Message();

		m->head.msgType = SQUELCHMESSAGETYPE;
		m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
		m->head.numberOfStructs = squelchQueue.size() ;
		m->head.sizeOfStruct = SquelchEntitySize;
		unsigned int offset = 0;
		for(int i=0; i< m->head.numberOfStructs; i++){
			SquelchEntity* squelch = squelchQueue.front();
			squelchQueue.pop_front();
			memcpy((void*)(m->data + offset), (void *)squelch, SquelchEntitySize);
			offset += SquelchEntitySize;
			delete squelch;
		}

		sender->addMessage(m);
	}
}


void FastEcslent::NetThread::addCommand(CommandEntity* m){
	commandQueue.push_back(m);
}

void FastEcslent::NetThread::addSquelch(SquelchEntity* s){
	squelchQueue.push_back(s);
}


void FastEcslent::NetThread::sendUnknownEntQueries(){
	if(unknows.size()> 0) {
		std::cout<<"Unknowns entity count: "<< unknows.size() <<std::endl;

		unsigned int offset = 0;

		Message *m = new Message();
		m->head.msgType = REQUESTINFOMESSAGETYPE;
		m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
		m->head.numberOfStructs = unknows.size();
		m->head.sizeOfStruct    = ReqInfoSize;

		for (std::map<int, State*>::iterator i= unknows.begin(); i!=unknows.end();i++) {
			ReqInfo* reqInfo = new ReqInfo();
			reqInfo->id = i->first;
			memcpy((void*)(m->data + offset), (void *)reqInfo, ReqInfoSize);
			offset += ReqInfoSize;
			delete reqInfo;
		}

		sender->addPriorityMessage(m);
	}
}

void FastEcslent::NetThread::sendCommand(int id, float dh, float ds){
	Message *m = new Message();
	m->head.msgType = COMMANDENTITYMESSAGETYPE;
	m->head.millisecondsFromStart = (getCurrentTime() - startTime).total_milliseconds();
	m->head.numberOfStructs = 1;
	m->head.sizeOfStruct = CommandEntitySize;

	CommandEntity* cmd = new CommandEntity();
	cmd->id = id;
	cmd->dh = dh;
	cmd->ds = ds;
	memcpy((void*)(m->data), (void *)cmd, CommandEntitySize);
	delete cmd;

	sender->addMessage(m);
}

void FastEcslent::NetThread::serverTick(){
	handleClientMessages();
	serve();
	boost::this_thread::sleep(*sleepTime);
}

void FastEcslent::NetThread::clientTick(){
	this->handleServerMessages();
	sendCommands();
	boost::this_thread::sleep(*sleepTime);
}

void FastEcslent::NetThread::serve(){

	Message *message;
	State   *state;
	unsigned int offset = 0;

	int n = engine->entityMgr->nEnts;

	//std::cout << "Serving at dtime: " << dtime.total_milliseconds() << std::endl;
	long dt = netCommon->getCurrentTimeLong();
	message = makeMessageWithStateHeader(n, netCommon->getCurrentTimeLong());
	offset = 0;
	for (int i = 0; i < n; i++) {
		state = fillStateFromEnt(engine->entityMgr->ents[i]);
		//engine->entityMgr->dumpOne(i);
		memcpy((void*)(message->data + offset), (void *)state, StateSize);
		offset += StateSize;
		delete state;
	}
	//printMessage(message);
	sender->addMessage(message);
}

void FastEcslent::NetThread::handleClientMessages(){
	Message *m = listener->dequeMessage();
	while (m) {
		if((int) m->head.msgType != FastEcslent::STATEMESSAGETYPE) {
			handleMessage(m);
		}
		delete m;
		m = listener->dequeMessage();
	}
}

void FastEcslent::NetThread::handleServerMessages(){
	Message *m = listener->dequeMessage();
	while (m) {
		if((int) m->head.msgType == FastEcslent::STATEMESSAGETYPE) {
			updateState(m);
		}else if((int) m->head.msgType == FastEcslent::INFOMESSAGETYPE) {
			createEnt(m);
		}else if ((int) m->head.msgType == FastEcslent::SQUELCHMESSAGETYPE){
			squelchEnts(m);
		}else if ((int) m->head.msgType == FastEcslent::CREATEENTITYRESPONSEMESSAGETYPE){
			createEntFromClientRes(m);
		}else if ((int) m->head.msgType == FastEcslent::MINERALPATCHIDRESPONSETYPE){
			updateMineralPatchID(m);
		}
		delete m;
		m = listener->dequeMessage();
	}
}

void FastEcslent::NetThread::updateState(Message *m){
	unsigned int offset = 0;
	for(int i=0;i<m->head.numberOfStructs;i++){
		State *state = new State(); //remember to delete, now it is used in unknows
		memcpy((void *)state, (void*)(m->data + offset), StateSize);
		offset += StateSize;
		state->flag =  m->head.millisecondsFromStart;
		updateState(state);
		//delete state;
	}
}

void FastEcslent::NetThread::updateState(State *s){
	if(netIdToEntMap.find(s->id) != netIdToEntMap.end()){
		FastEcslent::Entity *ent= netIdToEntMap[s->id];
		NetAspect *na = dynamic_cast<NetAspect *> (ent->getAspect(NET));
		if(na){
			ent->netAspect->updateQueue.push_back(s);
		}else{
			delete s;
		}
	}
	else{
		State *tmp = unknows[s->id];
		unknows[s->id] = s;
		if (tmp != NULL)
			delete tmp;
	}
}

void FastEcslent::NetThread::createEntFromClientReq(int builderid, EntityType entType, Ogre::Vector3 pos){
	CreateEntityRequest* ent = new CreateEntityRequest();
	ent->builderid = builderid;
	ent->entType = entType;
	ent->yaw = 0.0;
	ent->side = this->engine->options.side;
	ent->player = this->engine->options.player;
	ent->px = pos.x;
	ent->py = pos.y;
	ent->pz = pos.z;

	Message *m = new Message();

	m->head.msgType = CREATEENTITYREQUESTMESSAGETYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1 ;
	m->head.sizeOfStruct = CreateEntityRequestSize;

	memcpy((void*)(m->data), (void *)ent, CreateEntityRequestSize);

	delete ent;

	sender->addMessage(m);
}

void FastEcslent::NetThread::createEntFromClientReq(Message *m){
	CreateEntityRequest *ent = new CreateEntityRequest();
	memcpy((void *)ent, (void*)(m->data), CreateEntityRequestSize);
	Entity* newEnt = this->engine->entityMgr->createEntityForPlayerAndSide((EntityType)(ent->entType), Ogre::Vector3(ent->px,ent->py,ent->pz), ent->yaw, (Side)(ent->side), (Player)(ent->player));

	this->createEntFromClientRes(ent->builderid, newEnt->entityId.id);
}

void FastEcslent::NetThread::createEntFromClientRes(int builderid, int builtid){
	CreateEntityResponse* ent = new CreateEntityResponse();
	ent->builderid = builderid;
	ent->builtid = builtid;

	Message *m = new Message();

	m->head.msgType = CREATEENTITYRESPONSEMESSAGETYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1 ;
	m->head.sizeOfStruct = CreateEntityResponseSize;

	memcpy((void*)(m->data), (void *)ent, CreateEntityResponseSize);

	delete ent;

	sender->addMessage(m);
}

void FastEcslent::NetThread::createEntFromClientRes(Message *m){
	CreateEntityResponse *ent = new CreateEntityResponse();
	memcpy((void *)ent, (void*)(m->data), CreateEntityResponseSize);
	int builderid = ent->builderid;
	int builtid = ent->builtid;
	Entity* builder = this->engine->entityMgr->getEntityById(builderid);
	builder->netAspect->setEntityBeingBuilt(builtid);
}

void FastEcslent::NetThread::createEnt(Message *m){
	unsigned int offset = 0;
	for(int i=0;i<m->head.numberOfStructs;i++){
		Info *info = new Info(); //remember to delete, now it is used in unknows
		memcpy((void *)info, (void*)(m->data + offset), InfoSize);
		offset += InfoSize;
		createEnt(info);
	}
}

void FastEcslent::NetThread::createEnt(Info *info){
	if(unknows.find(info->id) != unknows.end()){
		int type = atoi(info->type);
		EntityType et = (EntityType)type;
		Side side = (Side)(info->side);
		Player player = (Player)(info->playerId);
		State *s = unknows[info->id];

		Ogre::Vector3 pos(s->px,s->py,s->pz);

		Entity* ent = engine->entityMgr->createEntityForPlayerAndSide(et, pos, s->yaw, side, player );

		engine->gameMgr->pop[player] += engine->gameMgr->entTypeData[ent->entityType].supply;
		engine->gameMgr->currentEntityCounts[player][ent->entityType]++;
		engine->gameMgr->playerEnts[player][engine->gameMgr->playerNEnts[player]++] = ent;

		ent->width = info->beam;
		ent->depth = info->draft;
		ent->uiname = info->label;
		ent->length = info->length;
		ent->maxSpeed = info->maxSpeed;

		ent->vel.x = s->vx;
		ent->vel.y = s->vy;
		ent->vel.z = s->vz;

		ent->timeLeftToBirth = s->timeLeftToBirth;
		ent->hitpoints = s->hp;

		netIdToEntMap[info->id] = ent;
		netEntToIdMap[ent] = info->id;

		ent->desiredHeading = s->dh;
		ent->desiredSpeed = s->ds;

		ent->ai->state = NETSLAVE;


		delete s;
		unknows.erase(info->id);
		delete info;

		//DEBUG(std::cout<<"Create Entity from Network: "<< ent->entityId->id << ", " << ent->entityType << std::endl;)
	}
}

void FastEcslent::NetThread::squelchEnts(Message *m){
	unsigned int offset = 0;
	for(int i=0;i<m->head.numberOfStructs;i++){
		SquelchEntity *squelch = new SquelchEntity();
		memcpy((void *)squelch, (void*)(m->data + offset), SquelchEntitySize);
		offset += SquelchEntitySize;
		netIdToEntMap[squelch->id]->netAspect->squalch();
		delete squelch;
	}
}

void FastEcslent::NetThread::handleMessage(Message *m){
	switch((int) m->head.msgType) {
	case FastEcslent::REQUESTINFOMESSAGETYPE:
		sendInfo(m);
		break;
	case FastEcslent::COMMANDENTITYMESSAGETYPE:
		propagateCommand(m);
		break;
	case FastEcslent::SQUELCHMESSAGETYPE:
		//server does nothing about squelch
		break;
	case FastEcslent::CREATEENTITYREQUESTMESSAGETYPE:
		createEntFromClientReq(m);
		break;
	case FastEcslent::MINERALPATCHIDREQUESTTYPE:
		responseMineralPatchID(m);
		break;
	default:
		break;

	}
}

void FastEcslent::NetThread::sendInfo(Message *m){
	Entity *ent;
	Info   *info;
	ptime currentTime = getCurrentTime();
	time_duration diff = currentTime - startTime;
	Message *sendInfoMessage = makeMessageWithInfoHeader(m->head.numberOfStructs, diff.total_milliseconds());

	int offset = HeaderSize;
	//std::cout << "Readying info to be sent" << std::endl;
	FastEcslent::printMessageHeader(m->head);
	FastEcslent::printMessageData(m);
	for(int i = 0; i < m->head.numberOfStructs; i++){
		ent = engine->entityMgr->ents[*((int *)m->data + i)];//convert to int pointer, add i, then dereference
		info = fillInfoFromEnt(ent);
		std::cout << "Sending info message about ent with id: " << ent->entityId.id << std::endl;
		memcpy((void* ) ((char*)sendInfoMessage + offset), info, InfoSize);
		offset += InfoSize;

		delete info;
	}

	sender->addMessage(sendInfoMessage);
}

void FastEcslent::NetThread::propagateCommand(Message *m){
	unsigned int offset = 0;
	for(int i=0;i<m->head.numberOfStructs;i++){
		CommandEntity *cmd = new CommandEntity(); //remember to delete, now it is used in unknows
		memcpy((void *)cmd, (void*)(m->data + offset), CommandEntitySize);
		offset += CommandEntitySize;
		Entity* ent = engine->entityMgr->getEntityById(cmd->id);
		ent->desiredHeading = cmd->dh;
		ent->desiredSpeed = cmd->ds;
		if(ent->entityClass == SURFACE){ //building has no ai
			ent->ai->state = NETSLAVE;
		}
		delete cmd;
	}
}

void FastEcslent::NetThread::stop(){ // end thread, public
	quit = true;
}


void FastEcslent::NetThread::stopAndJoin(){ // end thread, public
	stop();
	std::cout << "NetThread Stopping...Quit = " << std::endl;
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	sender->stop();
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	listener->stop();
	boost::this_thread::sleep(boost::posix_time::seconds(2));

	std::cout << "NetThread Joining..." << std::endl;


	sender->join();
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	//listener->join();
	//boost::this_thread::sleep(boost::posix_time::seconds(2));
	listener->kill();
	sender->kill();

	boost::this_thread::sleep(boost::posix_time::seconds(2));
	//listener->listenerThread.interrupt();//I would like to join listener but that keeps crashing
	netThread.join();
}

int FastEcslent::NetThread::getNetId(Entity* ent){
	for(std::map<int, Entity*>::iterator i = netIdToEntMap.begin(); i!= netIdToEntMap.end(); i++){
		if(i->second == ent)
			return i->first;
	}
	return -1;
}

FastEcslent::Entity* FastEcslent::NetThread::getEntityFromNetId(int entID){
	for(std::map<int, Entity*>::iterator i = netIdToEntMap.begin(); i!= netIdToEntMap.end(); i++){
		if(i->first == entID)
			return i->second;
	}
	return NULL;
}

void FastEcslent::NetThread::requestMineralPatchID(int mineralid){
	MineralPatchIDRequest* req = new MineralPatchIDRequest();
	req->mineralid = mineralid;

	Message *m = new Message();

	m->head.msgType = MINERALPATCHIDREQUESTTYPE;
	m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
	m->head.numberOfStructs = 1 ;
	m->head.sizeOfStruct = MineralPatchIDRequestSize;

	memcpy((void*)(m->data), (void *)req, MineralPatchIDRequestSize);

	delete req;

	sender->addMessage(m);
}

void FastEcslent::NetThread::responseMineralPatchID(Message *m){
	MineralPatchIDRequest *req = new MineralPatchIDRequest();
	memcpy((void *)req, (void*)(m->data), MineralPatchIDRequestSize);
	Entity* ent = engine->entityMgr->getEntityById(req->mineralid);
	delete req;
	if(ent->entityType == MINERALS){
		Minerals* mineral = dynamic_cast<Minerals*>(ent);

		MineralPatchIDResponse* res = new MineralPatchIDResponse();
		res->mineralid = mineral->entityId.id;
		res->patchid = mineral->mineralPatchId;

		Message *m = new Message();

		m->head.msgType = MINERALPATCHIDRESPONSETYPE;
		m->head.millisecondsFromStart = netCommon->getCurrentTimeLong();
		m->head.numberOfStructs = 1 ;
		m->head.sizeOfStruct = MineralPatchIDResponseSize;

		memcpy((void*)(m->data), (void *)res, MineralPatchIDResponseSize);

		delete res;

		sender->addMessage(m);
	}
}

void FastEcslent::NetThread::updateMineralPatchID(Message *m){
	MineralPatchIDResponse *res = new MineralPatchIDResponse();
	memcpy((void *)res, (void*)(m->data), MineralPatchIDResponseSize);
	Entity* ent = this->getEntityFromNetId(res->mineralid);
	if(ent->entityType == MINERALS){
		Minerals* mineral = dynamic_cast<Minerals*>(ent);
		mineral->mineralPatchId = res->patchid;
	}

	delete res;
}

