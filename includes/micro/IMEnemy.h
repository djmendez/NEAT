

#ifndef IMENEMY_H
#define	IMENEMY_H

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
#include <micro/InfluenceMap.h>
//#include <IMTerrain.h>

namespace FastEcslent{
//	class DebugDrawer;

	class IMEnemy:public InfluenceMap
	{
	public:
		IMEnemy(Side side);
		~IMEnemy();
		virtual void Init(int sizeX, int sizeY, int wSizeX, int wSizeY);
		virtual void Update() ;
		virtual void RegisterGameObj(Entity* object);
		virtual void DrawTheInfluence(float dt);
		virtual void DrawGridValue();
		void DrawUnit(float dt);
		Entity* getLowestValueUnit();
		Ogre::Vector3 getHidingPos(Ogre::Vector3* pos, int celldis);
		Ogre::Vector3 getLowestNearby(Entity* u);
		int getLowestNearbyGrid(int gridNumber);
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


#endif /* IMENEMY_H_ */


