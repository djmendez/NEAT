#ifndef INFLUENCEMAP_H_
#define INFLUENCEMAP_H_

#include <map>
#include <algorithm>
#include <math.h>
#include "IMParams.h"
#include "ent.h"

namespace FastEcslent {

	/*struct RegObj
	{
	//	BWAPI::Unit* m_pObject;
		Entity* m_pObject;
		int m_objSizeX;
		int m_objSizeY;
		int m_objType;
		int m_lastPosX;
		int m_lastPosY;
		bool m_stamped;
		bool isBuilding;
		int m_objSide;
		std::string m_typeName;
		int currentGrid;
		bool exist;
	};*/

	class InfluenceMap
	{
	public:
		//constructor/functions
		InfluenceMap(int type);
		~InfluenceMap();
		virtual void Update() {}
		virtual void DrawInfluenceMap(){};
		virtual void DrawTheGrid();
		virtual void DrawTheInfluence(){};
		virtual void Init(int sizeX, int sizeY, int wSizeX, int wSizeY);
		virtual void Reset();
		virtual void RegisterGameObj(Entity* object);
		virtual void RemoveGameObj(FastEcslent::Entity* object);
		virtual void StampInfluenceGradientSum(int* pMap,int pos_x, int pos_y, int initValue, int radius);
		virtual void StampInfluenceGradient(int* pMap,int pos_x, int pos_y, int initValue);
		int GetInfluenceValue(int* pMap,Ogre::Vector3& location);
		int GetInfluenceValue(int* pMap,Ogre::Vector3* location);
		int GetInfluenceValue(int* pMap,int gridX, int gridY);
		int GetInfluenceValueByGrid(int* pMap,int gridNumber);
		std::list<Ogre::Vector3> GetInfluenceValueHigher(int* pMap,Ogre::Vector3& location);
		void SetType(int type) {m_influenceType = type;}
		int* GetIMValue(){return m_map;};
		int GetSizeX(){return m_dataSizeX;}
		int GetSizeY(){return m_dataSizeY;}
		void setDebugMode(bool debugMode);
		Ogre::Vector3 getPositionFromGrid(int gridNumber);
		Ogre::Vector3 getPositionFromGrid(int x, int y);
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
		int m_numCels;
		int m_worldSizeX;
		int m_worldSizeY;
		int m_celResX;
		int m_celResY;
		int m_influenceType;

		int* m_map;
	protected:
		//data members


		//std::list<RegObj*> registeredObjects;


		bool debugMode;
	};

}

#endif /* INFLUENCEMAP_H_ */
