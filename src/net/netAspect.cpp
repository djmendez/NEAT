/*
 * netAspect.cpp
 *
 */

#include <netAspect.h>
#include <ent.h>
#include <engine.h>

void FastEcslent::NetAspect::init(){
	SPEED_TOLERANCE = 0.1;
	HEADING_TOLERANCE = 3.14159/360.0;

	firstTick = true;
	pendingEntityId = -1;
};

void FastEcslent::NetAspect::initNetworking(){
	int size = updateQueue.size();
	if(size > 0){
		statusData = *(updateQueue.back()); //get the latest throw out the rest
		this->clearQueue(&updateQueue, size);

		remotePos = Ogre::Vector3(statusData.px,statusData.py,statusData.pz);
		entity->pos = remotePos;

		entity->yaw = statusData.yaw;
		remoteVel = Ogre::Vector3(statusData.vx, statusData.vy, statusData.vz);
		entity->speed = remoteVel.length();
		entity->desiredSpeed = statusData.ds;
		entity->desiredHeading = statusData.dh;
	}

	oldTime = 0;
    latency = 0;
    nSteps = 0;

    ds = 0;
    dh = 0;

    rotFactor     = 0;
    rotProgress   = 1.0;

    if(entity->entityType == MINERALS){
    	Minerals* mineral = dynamic_cast<Minerals*>(entity);
    	if(mineral->mineralPatchId == 0){
    		requestMineralPatchID();
    	}
    }
}

void FastEcslent::NetAspect::requestMineralPatchID(){
	entity->engine->net->requestMineralPatchID(entity->engine->net->getNetId(entity));
}

void FastEcslent::NetAspect::tick(double dt){
	if(firstTick){
		initNetworking();
		firstTick = false;
	}

	if(entity->ai->state == NETSLAVE){
		updateEcslent(dt);
	}else if(entity->ai->state == MANUAL && !entity->engine->options.isServer){
		updateServer(entity->desiredSpeed, entity->desiredHeading);
	}

	if(this->checkPendingEntityId()){
		Entity* ent = this->entity->engine->net->getEntityFromNetId(pendingEntityId);
		setEntityBeingBuilt(this->entity->engine->net->getEntityFromNetId(pendingEntityId));
	}
};

void FastEcslent::NetAspect::updateEcslent(double dt){
	int size = updateQueue.size();
	if(size > 0){
		statusData = *(updateQueue.back()); //get the latest throw out the rest
		this->clearQueue(&updateQueue, size);

		nSteps = 0;

		if(statusData.flag > oldTime){
			latency = (statusData.flag - oldTime)/1000.0;
			oldTime = statusData.flag;
			nSteps = latency/dt;

			if(nSteps < 1){
				nSteps = 1;
			}

			lerpPos();
			lerpRot();
			entity->desiredSpeed = statusData.ds;
			entity->desiredHeading = statusData.dh;

			entity->hitpoints = statusData.hp;
			entity->timeLeftToBirth = statusData.timeLeftToBirth;
		}else{
			rotProgress = 0;
			nSteps = 0;
		}

		if (nSteps > 0){
			entity->pos -= diffPosFrac;
			if(rotProgress< 1.0){
				qDelta = Ogre::Quaternion::Slerp(rotProgress, srcOrie, destOrie,true);
				deltaYaw = qDelta.getYaw().valueRadians();
				entity->yaw = deltaYaw;
				rotProgress += rotFactor;
			}
			nSteps -= 1;
		}
	}
}

void FastEcslent::NetAspect::createEntity(EntityType entType, Ogre::Vector3 pos){
	entity->engine->net->createEntFromClientReq(this->entity->entityId.id, entType, pos);
}

void FastEcslent::NetAspect::lerpPos(){
	remotePos = Ogre::Vector3(statusData.px, statusData.py, statusData.pz);
	remoteVel = Ogre::Vector3(statusData.vx, statusData.vy, statusData.vz);
	nextRemotePos = remotePos + (remoteVel * latency);
	nextPos = entity->pos + (entity->vel * latency);
	diffPos = nextPos - nextRemotePos;
	diffPosFrac = diffPos/nSteps;
}

void FastEcslent::NetAspect::lerpRot(){
	remoteQuat = pitchYawRoll(0.0f, toDegrees(statusData.yaw), 0.0f);
	destOrie = remoteQuat * pitchYawRoll(0.0f, toDegrees(statusData.rSpeed * latency), 0.0f);
	srcOrie = pitchYawRoll(0.0, toDegrees(entity->yaw), 0.0f);
	finalDestOrie = destOrie * srcOrie;
	if(destOrie.equals(srcOrie, Ogre::Radian(HEADING_TOLERANCE))){
		rotFactor = 1.0;
		rotProgress = 1.0;
	}else{
		rotProgress = 1.0/nSteps;
		rotProgress = rotFactor;
	}
}

bool FastEcslent::NetAspect::withinSpeedTolerance(float a, float b){
	return std::abs(a - b) < SPEED_TOLERANCE;
}

bool FastEcslent::NetAspect::withinHeadingTolerance(float a, float b){
	return std::abs(a - b) < HEADING_TOLERANCE;
}

void FastEcslent::NetAspect::updateServer(float newDS, float newDH){
	if(withinSpeedTolerance(ds,newDS) and withinHeadingTolerance(dh, newDH)){
		return;
	}else{
		ds = newDS;
		dh = newDH;
		if(dh >= -pi and dh <= pi and ds <= entity->maxSpeed and ds >=0){
			CommandEntity* cmd = new CommandEntity();
			cmd->id = entity->engine->net->getNetId(entity);
			cmd->ds = ds;
			cmd->dh = dh;
			SquelchEntity* squelch = new SquelchEntity();
			squelch->id = cmd->id;
			entity->engine->net->addCommand(cmd);
			entity->engine->net->addSquelch(squelch);
		}
	}
}

bool FastEcslent::NetAspect::isValid(State *s){
	if(s->id == 0 && s->dh == 0 && s->ds == 0
			&& s->px == 0 && s->py == 0 && s->pz == 0
			&& s->vx == 0 && s->vy == 0 && s->vz==0)
		return false;
	else
		return true;
}

void FastEcslent::NetAspect::squalch(){
	if(entity->ai){
		entity->ai->state = NETSLAVE;
	}
}

void FastEcslent::NetAspect::squalchOtherClients(){
	if(entity->ai){
		entity->ai->state = NETSLAVE;
	}
}

void FastEcslent::NetAspect::clearQueue(std::deque<State*> * queue, int size){
	for(int i=0; i< size; i++){
		State * s = queue->front();
		queue->pop_front();
		delete s;
	}
}

void FastEcslent::NetAspect::setEntityBeingBuilt(int entId){
	this->pendingEntityId = entId;
}

bool FastEcslent::NetAspect::checkPendingEntityId(){

	if(this->entity->engine->net->getEntityFromNetId(pendingEntityId) != NULL){
		return true;
	}
	return false;
}

void FastEcslent::NetAspect::setEntityBeingBuilt(Entity* ent){

	if(this->entity->builder != NULL){
		this->entity->builder->entityBeingBuilt = ent;
		this->entity->builder->waitNetworkResponse = false;
		this->pendingEntityId = -1;
	}
}
