#include <micro/IMEnemy3D.h>
#include <cstring>
#include <sstream>  
#include <string> 
#include <algorithm>
#include <OgreColourValue.h>
#include <DebugDrawer.h>
#include <engine.h>

using namespace FastEcslent;
using namespace std;
//--------------------
IMEnemy3D::IMEnemy3D(Side s):InfluenceMap3D(IM_OCCUPANCE),side(s)
{
	if(this->side == RED){
		params = GA::getInstance()->getRedParams();
	}else{
		params = GA::getInstance()->getBlueParams();
	}

	value = params.UnitValue;
	radius = params.UnitRadius;

	this->updatedt = 3.0f;
}

void IMEnemy3D::Init(int sizeX, int sizeY, int sizeZ, int wSizeX, int wSizeY, int wSizeZ) {
	InfluenceMap3D::Init(sizeX, sizeY, sizeZ, wSizeX, wSizeY, wSizeZ);
	this->m_map_tr = new int[m_numCels];
	//initial 0
	memset(m_map_tr,0,m_numCels*sizeof(int));
}

void IMEnemy3D::Update()
{
	if(updatedt < 30){
		updatedt+= 1;
		return;
	}
	updatedt=0;
	//bail out if nobody to update
	if(registeredObjects.size() == 0)
		return;
	//based on terrain IM map.
	std::memcpy(m_map, m_map_tr, m_numCels*sizeof(int));

	//stamp obj locations
	std::map<Entity*, RegObj>::iterator listObj;
	for(listObj=registeredObjects.begin();listObj!=registeredObjects.end();++listObj)
	{
		if (!listObj->second.exist) continue;
		if (listObj->first->entityState != ALIVE) continue;

		int posX = listObj->first->pos.x;
		int posY = listObj->first->pos.y;
		int posZ = listObj->first->pos.z;
		
		if (posX > m_worldSizeX) posX = listObj->second.m_lastPosX;
		if (posY > m_worldSizeY) posY = listObj->second.m_lastPosY;
		if (posZ > m_worldSizeZ) posZ = listObj->second.m_lastPosZ;

		float hp = listObj->first->hitpoints;
		int hp_max = listObj->first->hitpointsmax;
		float hp_ratio = hp/hp_max;

		//unit's weight added according to hp ratio.
		StampInfluenceGradientSum(m_map, posX, posY, posZ, hp_ratio*value, radius);
	}
}

//--------------------
void IMEnemy3D::RegisterGameObj(Entity* object)
{
	if(object->entityId.side == this->side || object->entityId.side == NEUTRAL)     //self or neutral, ignore
		return;

	if(object->entityType==SC_ZEALOT){
		int i=0;
	}
	registeredObjects[object].m_lastPosX = object->pos.x;
	registeredObjects[object].m_lastPosY = object->pos.y;
	registeredObjects[object].m_lastPosZ = object->pos.z;
	registeredObjects[object].m_stamped = false;
	registeredObjects[object].m_objType = object->entityType;
	registeredObjects[object].m_typeName = object->entityType;
	registeredObjects[object].exist = true;
	
	registeredObjects[object].m_objSide = -1;
}

std::set<int> IMEnemy3D::getBorlderCellList(int ringCount)
{
	std::set<int> border;
	for(int i=0;i<m_numCels;i++)
	{
		if(ringCount ==1)
		{
			if(m_map[i] == 0 && hasEnemyInRange(m_map, i, 1))
				border.insert(i);
		}else if(ringCount==2)
		{
			if(m_map[i] == 0 && hasEnemyInRange(m_map, i, 2) && !hasEnemyInRange(m_map, i, 1))
				border.insert(i);
		}else if(ringCount ==3)
		{
			if(m_map[i] == 0 && hasEnemyInRange(m_map, i, 3)&& !hasEnemyInRange(m_map, i, 2) && !hasEnemyInRange(m_map, i, 1))
				border.insert(i);
		}
	}
	return border;
}



//--------------------
void IMEnemy3D::DrawTheInfluence(float dt)
{
	if(registeredObjects.size() == 0)
		return;

	//DrawTheGrid();
	//DrawGridValue();
	DrawUnit(dt);
}

void IMEnemy3D::DrawUnit(float dt){
}

Entity* IMEnemy3D::getLowestValueUnit(){
	//3D
	Entity* t;
	int min = 100000;
	for (std::map<Entity*, RegObj>::iterator i= registeredObjects.begin(); i!= registeredObjects.end();i++){
		int value = this->GetInfluenceValue(m_map, i->first->pos);
		if(value < min){
			min = value;
			t = i->first;
		}
	}
	return t;
}

//TODO fix two below
Ogre::Vector3 IMEnemy3D::getHidingPos(Ogre::Vector3* pos, Ogre::Vector3* enemyPos, int celldis){
	int grid = this->getGridFromPosition(*pos);
	for (int i = 0;i< celldis; i++){
		grid = this->getLowestNearbyGrid(grid, enemyPos);
		int value = m_map[grid];
		if(value <= 0){
			break;
		}
	}
	return this->getPositionFromGrid(grid);
}

Ogre::Vector3 IMEnemy3D::getLowestNearby(Entity* u, Ogre::Vector3* enemyPos){
	int grid = getGridFromPosition(u->pos);
	if(grid > m_dataSizeX*m_dataSizeY*m_dataSizeZ)
		return Ogre::Vector3(0,0,0);
	int count = 0;
	while (this->GetInfluenceValueByGrid(m_map, grid) > 0 && count++ < m_worldSizeX){
		grid = getLowestNearbyGrid(grid, enemyPos);
		Ogre::Vector3 p = this->getPositionFromGrid(grid);
		//BWAPI::Broodwar->drawCircleMap(p.x(), p.y(),3,Colors::Yellow, true);
	}

	return this->getPositionFromGrid(grid);
}

int IMEnemy3D::getLowestNearbyGrid3D(int grid, Ogre::Vector3* evadeLoc){
	int gridX = grid / (m_dataSizeY * m_dataSizeZ) % m_dataSizeX;
	int gridY = grid / (m_dataSizeZ) % m_dataSizeX;
	int gridZ = grid % m_dataSizeZ;
	
	int mv = 100000;
	int gridNumber = grid;

	int mX = min(gridX+1, m_dataSizeX-1);
	int mY = min(gridY+1, m_dataSizeY-1);
	int mZ = min(gridZ+1, m_dataSizeZ-1);

	//calculate direction preferred (us - them) * m_cellResX;
	//get position from vector?
	//collect list of lowest nearby cells
	//for each cell
		//if distance == 0 done
		//check distance from cell center to caculated preferred direction point
		//select lowest distance

	Ogre::Vector3 preferredPos = Ogre::Vector3(gridX * m_celResX, gridY * m_celResY, gridZ * m_celResZ);
	preferredPos = preferredPos + ((preferredPos - *evadeLoc).normalisedCopy() * m_celResX);

	std::vector<Ogre::Vector3> lowestCells;

	for(int j=gridY - 1; j<=mY;j++){
		for(int i=gridX-1;i<=mX;i++){
			for(int k=gridZ-1;k<=mZ;k++){
				if(i== gridX && j== gridY && k== gridZ) continue;//skips center cell

				if(i<0) i=0;
				if(j<0) j=0;
				if(k<0) k=0;

				int value = this->GetInfluenceValue(m_map, i, j, k);
				if(value < mv) {
					mv = value;
					lowestCells.clear();
					lowestCells.push_back(Ogre::Vector3(i, j, k));
					//lowestCells.push_back(k + m_dataSizeX * (j + m_dataSizeY * i));
				} else if(value == mv)  {
					lowestCells.push_back(Ogre::Vector3(i, j, k));
					//lowestCells.push_back(k + m_dataSizeX * (j + m_dataSizeY * i));
				}
			}
		}
	}

	double minDistGrid = 10000000;

	for(std::vector<Ogre::Vector3>::iterator it = lowestCells.begin(); it != lowestCells.end(); it++) {
		Ogre::Vector3 tempGrid = *it;

		double dist = tempGrid.distance(preferredPos);
		if(dist < minDistGrid) {
			minDistGrid = dist;
			gridNumber = tempGrid.z + m_dataSizeX * (tempGrid.y + m_dataSizeY * tempGrid.x);
		}
	}

	return gridNumber;

	//cout<<"Grid: " << gridNumber << ", x,y,z:" << gridX<<","<<gridY<<","<<gridZ << std::endl;
	//return gridNumber;
}

int IMEnemy3D::getLowestNearbyGrid(int grid, Ogre::Vector3* evadeLoc){
	int gridX = grid / (m_dataSizeY * m_dataSizeZ) % m_dataSizeX;
	int gridY = grid / (m_dataSizeZ) % m_dataSizeX;
	int gridZ = grid % m_dataSizeZ;

	int mv = 100000;
	int gridNumber = grid;

	int mX = min(gridX+1, m_dataSizeX-1);
	int mY = min(gridY+1, m_dataSizeY-1);
	int mZ = min(gridZ+1, m_dataSizeZ-1);

	//calculate direction preferred (us - them) * m_cellResX;
	//get position from vector?
	//collect list of lowest nearby cells
	//for each cell
		//if distance == 0 done
		//check distance from cell center to caculated preferred direction point
		//select lowest distance

	Ogre::Vector3 preferredPos = Ogre::Vector3(gridX * m_celResX, gridY * m_celResY, gridZ * m_celResZ);
	preferredPos = preferredPos + ((preferredPos - *evadeLoc).normalisedCopy() * m_celResX);

	std::vector<Ogre::Vector3> lowestCells;

//	for(int j=gridY - 1; j<=mY;j++){
		for(int i=gridX-1;i<=mX;i++){
			for(int k=gridZ-1;k<=mZ;k++){
				if(i== gridX && k== gridZ) continue;//skips center cell

				if(i<0) i=0;
				//if(j<0) j=0;
				if(k<0) k=0;

				int value = this->GetInfluenceValue(m_map, i, gridY, k);
				if(value < mv) {
					mv = value;
					lowestCells.clear();
					lowestCells.push_back(Ogre::Vector3(i, gridY, k));
					//lowestCells.push_back(k + m_dataSizeX * (j + m_dataSizeY * i));
				} else if(value == mv)  {
					lowestCells.push_back(Ogre::Vector3(i, gridY, k));
					//lowestCells.push_back(k + m_dataSizeX * (j + m_dataSizeY * i));
				}
			}
//		}
	}

	double minDistGrid = 10000000;

	for(std::vector<Ogre::Vector3>::iterator it = lowestCells.begin(); it != lowestCells.end(); it++) {
		Ogre::Vector3 tempGrid = *it;

		double dist = tempGrid.distance(preferredPos);
		if(dist < minDistGrid) {
			minDistGrid = dist;
			gridNumber = tempGrid.z + m_dataSizeX * (tempGrid.y + m_dataSizeY * tempGrid.x);
		}
	}

	return gridNumber;

	//cout<<"Grid: " << gridNumber << ", x,y,z:" << gridX<<","<<gridY<<","<<gridZ << std::endl;
	//return gridNumber;
}

void IMEnemy3D::setTerrainIM(int* m){
	std::memcpy(m_map_tr, m, m_numCels*sizeof(int));
}
//--------------------
void IMEnemy3D::DrawGridValue()
{
	
	for(int i=0;i<m_numCels - 2;i++)
	{
		//int gridX = i%m_dataSizeX;
		//int gridY = i/m_dataSizeX;
		int gridX = i;
		int gridY = i + 1;
		int gridZ = i + 2;

		int x = gridX*m_celResX+m_celResX/2;
		int y = gridY*m_celResY+m_celResY/2;
		
		if (m_map[i]==0) continue;

		//BWAPI::Broodwar->drawTextMap(x-10,y-10,"%d",m_map[i]);
		//BWAPI::Broodwar->drawTextMap(x-10,y-10,"%d",i);
	}
	int total = 0;
	std::map<Entity*, RegObj>::iterator listObj;
	for(listObj=registeredObjects.begin();
		listObj!=registeredObjects.end();++listObj)
	{
		if (listObj->second.exist) total++ ;
	}

	int cells  = m_numCels;
	int cellWidth = m_celResX;
	int cellHeight = m_celResY;
	char color = '\x01';

	//Broodwar->drawTextScreen(250,20, "Total   : %d",total);
	//Broodwar->drawTextScreen(250,30, "Width   : %d",m_dataSizeX);
	//Broodwar->drawTextScreen(250,40, "Height  : %d",m_dataSizeY);
	//Broodwar->drawTextScreen(250,50, "Cells   : %d",cells);
	//Broodwar->drawTextScreen(250,60, "CellW   : %d",cellWidth);
	//Broodwar->drawTextScreen(250,70, "CellH   : %d",cellHeight);

//	if (BWAPI::Broodwar->getFrameCount() % 20 == 0){
	{
		stringstream width;
		stringstream height;
		stringstream data;
		string w;
		string h;
		string d;
		
		width << "width="<<m_dataSizeX;
		height << "height="<<m_dataSizeY;
		data <<"data=";
		for (int i=0;i<m_numCels;i++)
		{
			data << m_map[i];
			data <<",";
		}

		width >> w;
		height >> h;
		data >> d;
		write2file(w,h,d);
	}
}

IMEnemy3D::~IMEnemy3D()
{
	
}

void IMEnemy3D::write2file(string w, string h, string d)
{
	const char filename[] = "c:\\lsm\\im.properties";
	ofstream o_file;
	ifstream i_file;

	//o_file.open(filename, ios::app);
	o_file.open(filename);
	o_file << w << endl; 
	o_file << h << endl; 
	o_file << d << endl; 
	o_file.close();
}
