
#include "levelMgr.h"
#include "engine.h"

#include "DEBUG.h"

using namespace FastEcslent;

Ogre::Vector3 LevelMgr::origin;

LevelMgr::LevelMgr(Engine* eng, Options opt) : Mgr(eng), options(opt)
{
}

LevelMgr::~LevelMgr()
{
}

void LevelMgr::init()
{
    currentLevel.width = currentLevel.height = 0;
    
    loadLevelFromOptions();
}

Level LevelMgr::loadLevelFromOptions()
{
    DEBUG(std::cout << "Loading Level..................................." << std::endl);
    switch(options.levelType) {
        case _64x64:
            currentLevel.width = currentLevel.height = 64;
            break;
            
        case _128x128:
            currentLevel.width = currentLevel.height = 128;
            break;
    }
    
    DEBUG(std::cout << "Level: " << currentLevel.width << ' ' << currentLevel.height << std::endl);

    Ogre::Vector3 bounds = getLevelBounds();
    setWorldOrigin(Ogre::Vector3(bounds.x / 2, 0, bounds.z / 2));
    
    
    DEBUG(std::cout << "Level Loaded!" << std::endl);
    
    return currentLevel;
}

bool LevelMgr::entityWithinLevel(Entity* ent) const
{
    return positionWithinLevel(ent->pos);
}

bool LevelMgr::positionWithinLevel(const Ogre::Vector3& pos) const
{
    Ogre::Vector3 bounds = getLevelBounds();
    if(pos.x < 0 || pos.x > bounds.x)
        return false;
    
    else if(pos.z < 0 || pos.z > bounds.z)
        return false;
    
    else
        return true;
}

Level LevelMgr::getCurrentLevel() const
{
    return currentLevel;
}

Ogre::Vector3 LevelMgr::getLevelBounds() const
{
    return Ogre::Vector3(currentLevel.width * 32, 0, currentLevel.height * 32);
}

// static functions
void LevelMgr::setWorldOrigin(const Ogre::Vector3& newOrigin)
{
    origin = newOrigin;
}

const Ogre::Vector3& LevelMgr::getWorldOrigin()
{
    return origin;
}