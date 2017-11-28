#include <micro/InfluenceMap3D.h>
using namespace std;
using namespace FastEcslent;

InfluenceMap3D::InfluenceMap3D(int type)
{
	m_influenceType = type;
}

InfluenceMap3D::~InfluenceMap3D()
{
	if(registeredObjects.size() == 0)
		return;
	registeredObjects.clear();
}

//--------------------
void InfluenceMap3D::Init(int sizeX, int sizeY, int sizeZ, int wSizeX, int wSizeY, int wSizeZ)
{
	m_dataSizeX = sizeX;
	m_dataSizeY = sizeY;
	m_dataSizeZ = sizeZ;

	m_numCels = m_dataSizeX*m_dataSizeY*m_dataSizeZ;
	m_map = new int[m_numCels];
	//clear out the map
	memset(m_map,0,m_numCels*sizeof(int));

	m_worldSizeX = wSizeX;
	m_worldSizeY = wSizeY;
	m_worldSizeZ = wSizeZ;

	m_celResX = m_worldSizeX / m_dataSizeX;//get cel resolution
	m_celResY = m_worldSizeY / m_dataSizeY;
	m_celResZ = m_worldSizeZ / m_dataSizeZ;

	debugMode = true;
}

//--------------------
void InfluenceMap3D::Reset()
{
	//clear out the map
	memset(m_map,0,m_numCels*sizeof(int));
	//get rid off all the objects
	if(registeredObjects.size() == 0)
		return;
	registeredObjects.clear();
}

//--------------------
void InfluenceMap3D::RegisterGameObj(FastEcslent::Entity* object)
{
	registeredObjects[object].m_lastPosX = object->pos.x;
	registeredObjects[object].m_lastPosY = object->pos.y;
	registeredObjects[object].m_lastPosZ = object->pos.z;
	registeredObjects[object].m_stamped = false;
	registeredObjects[object].exist = true;
}
//--------------------
void InfluenceMap3D::RemoveGameObj(FastEcslent::Entity* object)
{
	if(registeredObjects.size() == 0)
		return;
	registeredObjects[object].exist = false;
	this->registeredObjects.erase(object);
}

//--------------------
int InfluenceMap3D::GetInfluenceValue(int* pMap,Ogre::Vector3& location)
{
	int gridX = location.x/ m_celResX;
	int gridY = 100;//location.y/ m_celResY;
	int gridZ = location.z/ m_celResZ;

	//int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	int index = gridZ + m_dataSizeX * (gridY + m_dataSizeY * gridX);

	//std:: cout << index << ": " << pMap[index] << std::endl;

//	if(location.x < 0 ||location.y < 0||location.z < 0){
//		std:: cout << index << ": " << pMap[index]<<": "<< location<< std::endl;
//	}
	return pMap[index];
}

//--------------------
int InfluenceMap3D::GetInfluenceValue(int* pMap,Ogre::Vector3* location)
{
	int gridX = location->x/ m_celResX;
	int gridY = location->y/ m_celResY;
	int gridZ = location->z/ m_celResZ;

	//int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	int index = gridZ + m_dataSizeX * (gridY + m_dataSizeY * gridX);

	return pMap[index];
}


int InfluenceMap3D::GetInfluenceValue(int* pMap,int gridX, int gridY, int gridZ){
	//int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	int index = gridZ + m_dataSizeX * (gridY + m_dataSizeY * gridX);
	return pMap[index];
}

int InfluenceMap3D::GetInfluenceValueByGrid(int* pMap,int gridNumber){
	//int gridY = gridNumber/m_dataSizeX;
	//int gridX = gridNumber%m_dataSizeX;
	//int gridX = gridNumber;
	//int gridY = gridNumber + 1;
	//int gridZ = gridNumber + 2;
	int gridX = gridNumber / (m_dataSizeY * m_dataSizeZ) % m_dataSizeX;
	int gridY = gridNumber / (m_dataSizeZ) % m_dataSizeX;
	int gridZ = gridNumber % m_dataSizeZ;

	return pMap[gridNumber];//GetInfluenceValue(pMap, gridX, gridY, gridZ);
}

std::list<Ogre::Vector3> InfluenceMap3D::GetInfluenceValueHigher(int* pMap,Ogre::Vector3& location)
{
	list<Ogre::Vector3> postionlist;
	int gridX = location.x/m_celResX;
	int gridY = location.y/m_celResY;
	int gridZ = location.z/m_celResZ;

	//int center = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	int center = gridX + m_dataSizeX * (gridY + m_dataSizeY * gridZ);

	for (int y=gridY-1;y<=gridY+1;y++)
	{
		for (int x=gridX-1;x<=gridX+1;x++)
		{
			for(int z=gridZ-1;z<=gridZ+1;z++)
			{
				//int tmp = y*m_dataSizeX + (x%m_dataSizeX);
				int tmp = x + m_dataSizeX * (y + m_dataSizeY * z);
				if (pMap[tmp]>pMap[center])
				{
					int posX = x*m_celResX+m_celResX/2;
					int posY = y*m_celResY+m_celResY/2;
					int posZ = z*m_celResZ+m_celResZ/2;
					postionlist.push_back(Ogre::Vector3(posX, posY, posZ));
				}
			}
		}
	}

	return postionlist;
}

//--------------------
void InfluenceMap3D::StampInfluenceGradient(int* pMap,int pos_x, int pos_y, int pos_z, int initValue)
{
	int gridX = floor((float)pos_x/ (float)m_celResX);
	int gridY = floor((float)pos_y/ (float)m_celResY);
	int gridZ = floor((float)pos_z/ (float)m_celResZ);

	int stopDist = abs(initValue)*0.75f;
	int halfStopDist = stopDist / 2;

	int startX = gridX - halfStopDist;
	int stopX  = startX + stopDist;
	if(startX < 0) startX = 0;
	if(stopX > m_dataSizeX) stopX = m_dataSizeX;

	int startY = gridY - halfStopDist;
	int stopY  = startY + stopDist;
	if(startY < 0) startY = 0;
	if(stopY > m_dataSizeY) stopY = m_dataSizeY;

	int startZ = gridZ - halfStopDist;
	int stopZ = startZ + stopDist;
	if(startZ < 0) startZ = 0;
	if(stopZ > m_dataSizeZ) stopZ = m_dataSizeZ;

	for(int y = startY;y<stopY;y++)
	{
		for(int x = startX;x<stopX;x++)
		{
			for(int z = startZ; z < stopZ; z++)
			{
				int value = 0;
				int distX = fabs((float)(x - ((gridX - halfStopDist) + halfStopDist)));
				int distY = fabs((float)(y - ((gridY - halfStopDist) + halfStopDist)));
				int distZ = fabs((float)(z - ((gridZ - halfStopDist) + halfStopDist)));

				double destD = 0;
				if (!(x==gridX && y==gridY && z==gridZ))
				{
					destD = sqrt((double)((x-gridX)*(x-gridX) + (y-gridY)*(y-gridY) + (z-gridZ)*(z-gridZ)));
				}

				int destI = ceil(destD);
				if (halfStopDist >= destI)
				{
					value += initValue*( halfStopDist - destI)/halfStopDist;
				}

				//int index = y*m_dataSizeX + (x%m_dataSizeX);
				int index = x + m_dataSizeX * (y + m_dataSizeY * z);
				pMap[index] += value;
			}
		}
	}
}

//--------------------
void InfluenceMap3D::StampInfluenceGradientSum(int* pMap,int pos_x, int pos_y, int pos_z, int initValue, int radius)
{
	int gridX = floor((float)pos_x/ (float)m_celResX);
	int gridY = floor((float)pos_y/ (float)m_celResY);
	int gridZ = floor((float)pos_z/ (float)m_celResZ);
	//int gridX = (float)pos_x / m_celResX;
	//int gridY = (float)pos_y / m_celResY;
	//int gridZ = (float)pos_y / m_celResZ;

	int startX = gridX - radius;
	int stopX  = gridX + radius;
	if(startX < 0) startX = 0;
	if(stopX > m_dataSizeX) stopX = m_dataSizeX;

	int startY = gridY - radius;
	int stopY  = gridY + radius;
	if(startY < 0) startY = 0;
	if(stopY > m_dataSizeY) stopY = m_dataSizeY;

	int startZ = gridZ - radius;
	int stopZ = gridZ + radius;
	if(startZ < 0) startZ = 0;
	if(startZ > m_dataSizeZ) stopZ = m_dataSizeZ;

	for(int y = startY;y<stopY;y++)
	{
		for(int x = startX;x<stopX;x++)
		{
			for(int z = startZ; z < stopZ; z++)
			{
				int value = 0;
				int distX = fabs((float)(x - gridX));
				int distY = fabs((float)(y - gridY));
				int distZ = fabs((float)(z - gridZ));

				double destD = 0;
				if (!(x==gridX && y==gridY && z==gridZ))
				{
					destD = sqrt((double)((x-gridX)*(x-gridX) + (y-gridY)*(y-gridY) + (z-gridZ)*(z-gridZ)));
				}

				int destI = ceil(destD);
				if (radius >= destI)
				{
					value += initValue*(radius-destI)/radius;
				}

				//int index = y*m_dataSizeX + (x%m_dataSizeX);
				int index = z + m_dataSizeX * (y + m_dataSizeY * x);
				pMap[index] += value;
			}
		}
	}
}

void InfluenceMap3D::DrawTheGrid()
{
//	for (int i=0; i< m_dataSizeX; i++)
//	{
//		BWAPI::Broodwar->drawLineMap(i*m_celResX,0,i*m_celResX, m_worldSizeY,BWAPI::Colors::Grey);
//		BWAPI::Broodwar->drawTextMap(i*m_celResX,0,"%d",i);
//	}
//
//	for (int i=0; i< m_dataSizeY; i++)
//	{
//		BWAPI::Broodwar->drawLineMap(0,i*m_celResY,m_worldSizeX,i*m_celResY,BWAPI::Colors::Grey);
//		BWAPI::Broodwar->drawTextMap(0, i*m_celResY,"%d",i);
//	}
}

void InfluenceMap3D::setDebugMode(bool debugMode)
{
	this->debugMode = debugMode;
}

Ogre::Vector3 InfluenceMap3D::getPositionFromGrid(int gridNumber)
{
	//int gridX = gridNumber/m_dataSizeX;
	//int gridY = gridNumber%m_dataSizeX;
	//int gridX = gridNumber + 2;
	//int gridY = gridNumber + 1;
	//int gridZ = gridNumber;
	int gridX = gridNumber / (m_dataSizeY * m_dataSizeZ) % m_dataSizeX;
	int gridY = gridNumber / (m_dataSizeZ) % m_dataSizeX;
	int gridZ = gridNumber % m_dataSizeZ;

	return this->getPositionFromGrid(gridX, gridY, gridZ);
}

Ogre::Vector3 InfluenceMap3D::getPositionFromGrid(int x, int y, int z)
{
	//int pos_x = col*m_celResY + m_celResY/2;
	//int pos_y = row*m_celResX + m_celResX/2;
	int pos_x = x*m_celResX + m_celResX/2;
	int pos_y = y*m_celResY + m_celResY/2;
	int pos_z = z*m_celResZ + m_celResZ/2;

	Ogre::Vector3 pos(pos_x, pos_y, pos_z);
	return pos;	
}
int InfluenceMap3D::getGridFromPosition(Ogre::Vector3 position)
{
	int gridX = floor((float)position.x/ (float)m_celResX);
	int gridZ = floor((float)position.z/ (float)m_celResZ);
	int gridY = floor((float)position.y/ (float)m_celResY);


	//int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	int index = gridZ + m_dataSizeX * (gridY + m_dataSizeY * gridX);

	return index;
}

int InfluenceMap3D::getDistanceFrom2Grid(int grid1, int grid2)
{
	Ogre::Vector3 p1 = getPositionFromGrid(grid1);
	Ogre::Vector3 p2 = getPositionFromGrid(grid2);
	return p1.distance(p2);
}
bool InfluenceMap3D::hasEnemyInRange(int* pMap, int gridNumber, int radius)
{
	//int gridX = gridNumber/m_dataSizeX;
	//int gridY = gridNumber%m_dataSizeX;
	/*int gridX = gridNumber;
	int gridY = gridNumber + 1;
	int gridZ = gridNumber + 2;*/
	int gridX = gridNumber / (m_dataSizeY * m_dataSizeZ) % m_dataSizeX;
	int gridY = gridNumber / (m_dataSizeZ) % m_dataSizeX;
	int gridZ = gridNumber % m_dataSizeZ;

	int startX = gridX - radius;
	int stopX  = gridX + radius;
	if(startX < 0) startX = 0;
	if(stopX > m_dataSizeX-1) stopX = m_dataSizeX-1;

	int startY = gridY - radius;
	int stopY  = gridY + radius;
	if(startY < 0) startY = 0;
	if(stopY > m_dataSizeY-1) stopY = m_dataSizeY-1;

	int startZ = gridZ - radius;
	int stopZ = gridZ + radius;
	if(startZ < 0) startZ = 0;
	if(stopZ > m_dataSizeZ-1) stopZ = m_dataSizeZ-1;

	//for(int y = startY; y <= stopY;y++)
	//{
	//	for(int x = startX;x <=stopX;x++)
	//	{	
	//		int index = x*m_dataSizeX + y;
	//		int value = pMap[index];
	//		if(value < 0 ) return true;
	//	}
	//}
	int indexU = startX*m_dataSizeX + gridZ;
	int indexD = stopX* m_dataSizeX + gridZ;
	int indexL = gridX *m_dataSizeX + startZ;
	int indexR = gridX *m_dataSizeX + stopZ;
	int indexT = gridY *m_dataSizeZ + gridY;
	int indexB = gridY *m_dataSizeZ + stopY;
	
	return pMap[indexU] < 0 || pMap[indexD] < 0 ||pMap[indexL] < 0 ||pMap[indexR] < 0 ||pMap[indexB] < 0 ||pMap[indexT] < 0;
}
