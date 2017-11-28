/* 
 * File:   levelMgr.h
 * Author: crowe
 *
 * Created on June 2, 2014, 4:34 PM
 */

#ifndef LEVELMGR_H
#define	LEVELMGR_H

#include <OgreVector3.h>
#include <enums.h>

#include "mgr.h"
#include "options.h"

namespace FastEcslent {
    typedef struct {
        int width;
        int height;
    } Level;
    
    class Engine;
    class Entity;
    
    class LevelMgr : public Mgr {
    public:
        LevelMgr(Engine *eng, Options opt);
        ~LevelMgr();
        
        void init();
        Level loadLevelFromOptions();
        bool entityWithinLevel(Entity* ent) const;
        bool positionWithinLevel(const Ogre::Vector3& pos) const;
        
        Level getCurrentLevel() const;
        Ogre::Vector3 getLevelBounds() const;
        
        static const Ogre::Vector3& getWorldOrigin();
        
        Options options;
    private:
        static void setWorldOrigin(const Ogre::Vector3& newOrigin);
        static Ogre::Vector3 origin;
        Level currentLevel;
    };
}

#endif	/* LEVELMGR_H */

