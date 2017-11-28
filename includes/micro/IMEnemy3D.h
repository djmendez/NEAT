

#ifndef IMENEMY3D_H
#define	IMENEMY3D_H

#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "GA.h"
#include "IMParams.h"
#include <stdio.h>
#include <micro/InfluenceMap3D.h>
//#include <IMTerrain.h>

namespace FastEcslent{
//	class DebugDrawer;

	class IMEnemy3D : public InfluenceMap3D
	{
	public:
		IMEnemy3D(Side side);
		~IMEnemy3D();
		virtual void Init(int sizeX, int sizeY, int sizeZ, int wSizeX, int wSizeY, int wSizeZ);
		virtual void Update() ;
		virtual void RegisterGameObj(Entity* object);
		virtual void DrawTheInfluence(float dt);
		virtual void DrawGridValue();
		void DrawUnit(float dt);
		Entity* getLowestValueUnit();
		Ogre::Vector3 getHidingPos(Ogre::Vector3* pos, Ogre::Vector3* enemyPos, int celldis);
		Ogre::Vector3 getLowestNearby(Entity* u, Ogre::Vector3* enemyPos);
		int getLowestNearbyGrid3D(int gridNumber, Ogre::Vector3* evadeLoc);
		int getLowestNearbyGrid(int gridNumber, Ogre::Vector3* evadeLoc);
		std::set<int> getBorlderCellList(int ringCount);

		int getNextTargetGroup();
		void setTerrainIM(int* map);

	private:
		void write2file(std::string w, std::string h, std::string d);

		IMParams params;
		int value;
		int radius;

		int* m_map_tr;
		float updatedt;

		Side side;
	};
}


#endif /* IMENEMY3D_H_ */


