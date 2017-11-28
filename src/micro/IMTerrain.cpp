#include <micro/IMTerrain.h>

using namespace FastEcslent;
using namespace std;
//--------------------
IMTerrain::IMTerrain(Side s):InfluenceMap(IM_OCCUPANCE),side(s)
{
	if(side == RED){
		params = GA::getInstance()->getRedParams();
	}else{
		params = GA::getInstance()->getBlueParams();
	}

	value = 0;//params.TerrainValue;
	radius = 0;//params.TerrainRadius;

	this->initiatied = false;
}

void IMTerrain::Init(int sizeX, int sizeY, int wSizeX, int wSizeY){
	InfluenceMap::Init(sizeX, sizeY, wSizeX, wSizeY);
	memset(m_map,0,m_numCels*sizeof(int));
}

void IMTerrain::Update(float dt)
{
	//bail out if nobody to update
	if(registeredObjects.size() == 0)
		return;

	std::map<Entity*, RegObj>::iterator listObj;
	for(listObj=registeredObjects.begin();listObj!=registeredObjects.end();++listObj)
	{
		if (!listObj->second.exist) continue;
		if (listObj->first->entityState != ALIVE) continue;

		int posX = listObj->first->pos.x;
		int posY = listObj->first->pos.z;

		if (posX > m_worldSizeX) posX = listObj->second.m_lastPosX;
		if (posY > m_worldSizeY) posY = listObj->second.m_lastPosY;

		StampInfluenceGradientSum(m_map,posX, posY, value, radius);
	}
}

//--------------------
void IMTerrain::RegisterGameObj(Entity* object)
{
	if(object->entityId.side != NEUTRAL)     //Only Neutral object will be an terrain object
		return;

	registeredObjects[object].m_lastPosX = object->pos.x;
	registeredObjects[object].m_lastPosY = object->pos.z;
	registeredObjects[object].m_stamped = false;
	registeredObjects[object].m_objType = object->entityType;
	registeredObjects[object].m_typeName = object->entityType;
	registeredObjects[object].exist = true;
	
	registeredObjects[object].m_objSide = 0;
}


IMTerrain::~IMTerrain()
{

}

