#ifndef IMTERRAIN_H
#define	IMTERRAIN_H

#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "IMParams.h"
#include <stdio.h>
#include <micro/InfluenceMap.h>
#include "micro/GA.h"

class IMTerrain:public FastEcslent::InfluenceMap
{
public:
	IMTerrain(FastEcslent::Side side);
	~IMTerrain();
	virtual void Init(int sizeX, int sizeY, int wSizeX, int wSizeY);
	virtual void Update(float dt);
	virtual void RegisterGameObj(FastEcslent::Entity* object);


private:
	bool initiatied;
	IMParams params;
	int value;
	int radius;

	FastEcslent::Side side;
};

#endif


