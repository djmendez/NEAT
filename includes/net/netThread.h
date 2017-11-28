/*
 * netThread.h
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#ifndef NETTHREAD_H_
#define NETTHREAD_H_

#include <deque>
#include <map>

//#include <engine.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include <messages.h>
#include <listener.h>
#include <sender.h>
#include <ent.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <netCommon.h>

using namespace boost::posix_time;
using boost::asio::ip::udp;

namespace FastEcslent {

	class Engine;

	class NetThread {

	private:
		void runThread();
		void sendUnknownEntQueries();
		boost::thread netThread;
		int TICK_SLEEP_TIME;

		ptime  startTime;
		bool quit;

		std::map<int, State*> unknows;
		std::map<int, Entity*> netIdToEntMap;
		std::map<Entity*, int> netEntToIdMap;

		std::deque<CommandEntity*> commandQueue;
		std::deque<SquelchEntity*> squelchQueue;

		udp::endpoint myIP;
		NetCommon *netCommon;

	public:
		Engine* engine;

		Listener*    listener;
		Sender*      sender;

		bool insideLobby;
		bool isServer;
		int port;

		std::string myIPAddress;
		boost::posix_time::milliseconds *sleepTime;

		NetThread(Engine* eng) {
			engine = eng;
		}
		void run();
		void init();
		void stopAndJoin();
		void stop();

		int getNetId(Entity* ent);
		Entity* getEntityFromNetId(int entID);

		//helpers
		void clientTick();
		void handleServerMessages();
		void updateState(Message *m);
		void updateState(State *s);
		void createEntFromClientReq(int builderid, FastEcslent::EntityType ent, Ogre::Vector3 pos);
		void createEntFromClientReq(Message *m);
		void createEntFromClientRes(int builderid, int builtid);
		void createEntFromClientRes(Message *m);
		void createEnt(Message *m);
		void createEnt(Info *info);
		void sendCommands();
		void sendCommand(int id, float dh, float ds);
		void squelchEnts(Message *m);

		void serverTick();
		void handleClientMessages();
		void handleMessage(Message *m);
		void sendInfo(Message *m);
		void propagateCommand(Message *m);

		void combineCommandsIntoNetMessage();
		void combineSquelchesIntoNetMessage();

		void sendLobbyClientResponse(std::string clientIP, int port, int side, int player);
		void lobbyStart();

		void addCommand(CommandEntity *m);
		void addSquelch(SquelchEntity *m);

		void requestMineralPatchID(int mineralid);
		void responseMineralPatchID(Message *m);
		void updateMineralPatchID(Message *m);
		void serve();

		void gameTick();
	};

}

#endif /* NETTHREAD_H_ */
