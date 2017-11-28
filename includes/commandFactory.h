/* 
 * File:   commandFactory.h
 * Author: chris
 *
 * Created on September 16, 2013, 10:25 AM
 */

#ifndef COMMANDFACTORY_H
#define	COMMANDFACTORY_H
#include "command.h"
#include "enums.h"
#include "commandHelp.h"
#include <deque>
#include "engine.h"

namespace FastEcslent {
    //class Entity;

    enum RequiredTargetType
{
    None,
    TargetPosition,
    TargetEntity,
    Either,
    Both
};

    /*! An interface for implementing factory classes to produce different commands*/

    class CommandFactory {
    public:
        CommandFactory(RequiredTargetType requiredTargetType, std::string previewMeshName = "", EntityType targetEntityType = NENTITYTYPES) : requiredTarget(requiredTargetType),previewMeshName(previewMeshName),targetEntityType(targetEntityType) {} ;
        CommandFactory(const CommandFactory& orig);
        virtual ~CommandFactory();
        /*!
         * Interface to a function which produces a new command
         * @param owner A pointer the Entity which the new command is being issued to.
         * @return Command*
         */
        virtual std::deque<Command*> MakeCommand(Entity* owner, std::pair<int, Ogre::Vector3 > mouseTargets) = 0;
        RequiredTargetType requiredTarget;
        EntityType targetEntityType;
        std::string previewMeshName;
    protected:
        //Engine   *engine;

    };

    class BuildFactory : public CommandFactory {
    public:

        /*!
     * Constructor for a Factory which produces commands to build different units.
     * @param spawnEnt The EntityType that BuildCommands produced from this factory should construct
     * @return void
     */
        BuildFactory(EntityType spawnEnt) :CommandFactory(None), spawnEnt(spawnEnt) {
        };
        BuildFactory(const CommandFactory& orig);

        virtual ~BuildFactory() {
        };

        /*!
         * Function to produce a new BuildCommand for a specified Entity.
         * @param owner A pointer the Entity which the new command is being issued to.
         * @return Command* Pointer to a new BuildCommand assigned to 'owner', to spawn a new unit of type 'spawnEnt'
         */
        virtual std::deque<Command*> MakeCommand(Entity* owner, std::pair<int, Ogre::Vector3 > mouseTargets) {
            std::deque<Command*> newCommands;
            newCommands.push_back(new BuildCommand(owner, spawnEnt, FastEcslent::ConstructCommand));
            return newCommands;
        }
    private:
        EntityType spawnEnt; /*! The entity type that a BuildCommand should produce */

    };
    
    class BuildStructureFactory : public CommandFactory {
    public:

        /*!
     * Constructor for a Factory which produces commands to build different units.
     * @param spawnEnt The EntityType that BuildCommands produced from this factory should construct
     * @return void
     */
        BuildStructureFactory(EntityType spawnEnt,std::string previewMeshName = "", RequiredTargetType requiredTargetType = TargetPosition,EntityType targetEntityType = NENTITYTYPES) :CommandFactory(requiredTargetType,previewMeshName,targetEntityType), spawnEnt(spawnEnt) {
        };
        BuildStructureFactory(const CommandFactory& orig);

        virtual ~BuildStructureFactory() {
        };

        /*!
         * Function to produce a new BuildCommand for a specified Entity.
         * @param owner A pointer the Entity which the new command is being issued to.
         * @return Command* Pointer to a new BuildCommand assigned to 'owner', to spawn a new unit of type 'spawnEnt'
         */
        virtual std::deque<Command*>  MakeCommand(Entity* owner, std::pair<int, Ogre::Vector3 > mouseTargets);
    private:
        EntityType spawnEnt; /*! The entity type that a BuildCommand should produce */

    };
    
    class MoveFactory : public CommandFactory {
    public:

        /*!
     * Constructor for a Factory which produces commands to move to a location/entity.
     * @param spawnEnt The EntityType that BuildCommands produced from this factory should construct
     * @return void
     */
        MoveFactory() :CommandFactory(TargetPosition) {
        };
        MoveFactory(const CommandFactory& orig);

        virtual ~MoveFactory() {
        };

        /*!
         * Function to produce a new MoveCommand for a specified Entity.
         * @param owner A pointer the Entity which the new command is being issued to.
         * @return Command* Pointer to a new BuildCommand assigned to 'owner', to spawn a new unit of type 'spawnEnt'
         */
        virtual std::deque<Command*> MakeCommand(Entity* owner, std::pair<int, Ogre::Vector3 > mouseTargets) {
            std::deque<Command*> newCommands;
            newCommands.push_back(createMoveForEnt(owner, mouseTargets.second));
            return newCommands;
        }
    private:
        

    };


    class AttackFactory : public CommandFactory {
    public:

        /*!
     * Constructor for a Factory which produces commands to move to a location/entity.
     * @param spawnEnt The EntityType that BuildCommands produced from this factory should construct
     * @return void
     */
    	AttackFactory() :CommandFactory(TargetPosition) {
        };
    	AttackFactory(const CommandFactory& orig);

        virtual ~AttackFactory() {
        };

        /*!
         * Function to produce a new MoveCommand for a specified Entity.
         * @param owner A pointer the Entity which the new command is being issued to.
         * @return Command* Pointer to a new BuildCommand assigned to 'owner', to spawn a new unit of type 'spawnEnt'
         */
        virtual std::deque<Command*> MakeCommand(Entity* owner, std::pair<int, Ogre::Vector3 > mouseTargets);
    private:


    };
}

#endif	/* COMMANDFACTORY_H */

