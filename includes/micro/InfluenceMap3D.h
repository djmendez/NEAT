#ifndef INFLUENCEMAP3D_H_
#define INFLUENCEMAP3D_H_

#include <map>
#include <algorithm>
#include <math.h>
#include "ent.h"

namespace FastEcslent {

	/*struct RegObj
	{
	//	BWAPI::Unit* m_pObject;
		Entity* m_pObject;

		int m_objSizeX;
		int m_objSizeY;
		int m_objSizeZ;

		int m_objType;

		int m_lastPosX;
		int m_lastPosY;
		int m_lastPosZ;

		bool m_stamped;
		bool isBuilding;
		int m_objSide;
		std::string m_typeName;
		int currentGrid;
		bool exist;
	};*/

	class InfluenceMap3D
	{
	public:
		//constructor/functions
		InfluenceMap3D(int type);
		~InfluenceMap3D();
		virtual void Update(float dt) {}
		virtual void DrawInfluenceMap(){};
		virtual void DrawTheGrid();
		virtual void DrawTheInfluence(){};
		virtual void Init(int sizeX, int sizeY, int sizeZ, int wSizeX, int wSizeY, int wSizeZ);
		virtual void Reset();
		virtual void RegisterGameObj(Entity* object);
		virtual void RemoveGameObj(FastEcslent::Entity* object);
		virtual void StampInfluenceGradientSum(int* pMap,int pos_x, int pos_y, int pos_z, int initValue, int radius);
		virtual void StampInfluenceGradient(int* pMap,int pos_x, int pos_y, int pos_z, int initValue);
		int GetInfluenceValue(int* pMap,Ogre::Vector3& location);
		int GetInfluenceValue(int* pMap,Ogre::Vector3* location);
		int GetInfluenceValue(int* pMap,int gridX, int gridY, int gridZ);
		int GetInfluenceValueByGrid(int* pMap,int gridNumber);
		std::list<Ogre::Vector3> GetInfluenceValueHigher(int* pMap,Ogre::Vector3& location);
		void SetType(int type) {m_influenceType = type;}
		int* GetIMValue(){return m_map;}
		int GetSizeX(){return m_dataSizeX;}
		int GetSizeY(){return m_dataSizeY;}
		int GetSizeZ(){return m_dataSizeZ;}
		void setDebugMode(bool debugMode);
		Ogre::Vector3 getPositionFromGrid(int gridNumber);
		Ogre::Vector3 getPositionFromGrid(int x, int y, int z);
		int getDistanceFrom2Grid(int grid1, int grid2);
		int getGridFromPosition(Ogre::Vector3 position);
		bool hasEnemyInRange(int* pMap, int gridNumber, int radius);

		//influence map types
		enum
		{
			IM_NONE,
			IM_OCCUPANCE,
			IM_CONTROL,
			IM_BITWISE
		};

		std::map<FastEcslent::Entity*, RegObj> registeredObjects;

		int m_dataSizeX;
		int m_dataSizeY;
		int m_dataSizeZ;

		int m_numCels;

		int m_worldSizeX;
		int m_worldSizeY;
		int m_worldSizeZ;

		int m_celResX;
		int m_celResY;
		int m_celResZ;

		int m_influenceType;

		int* m_map;
	protected:
		//data members


		//std::list<RegObj*> registeredObjects;


		bool debugMode;
	};

}

#endif /* INFLUENCEMAP_H_ */
