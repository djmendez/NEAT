#ifndef INFOMGR_H
#define	INFOMGR_H

#include <OgreVector3.h>
#include <enums.h>
#include <micro/IMEnemy3D.h>
#include <micro/IMTerrain.h>
#include "mgr.h"
#include "micro/SquadMgr.h"
#include "micro/IMParams.h"

namespace FastEcslent {
    class Engine;
    class Entity;
    class InfoMgr : public Mgr {
    public:
    	InfoMgr(Engine *eng);
        ~InfoMgr();
        
        int maxFrames;
        double positiveFitnessFactor;

        void registerEntity(Entity* ent);
		virtual void init();
		virtual void tick();

		IMEnemy3D* getIMEnemy(Side s);

		int getFrameCount(){return this->frame;};

		IMParams getRedParams(){return redParams;};
		IMParams getBlueParams(){return blueParams;};

		SquadMgr *squadmgr_red, *squadmgr_blue;
    private:
		int frame;
		double dt;
		IMEnemy3D   *eIM_red, *eIM_blue;
		IMTerrain *tIM_red, *tIM_blue;

		IMParams redParams;
		IMParams blueParams;
    };
}

#endif

