/*
 * netLobby.h
 *
 */

#ifndef NETLOBBY_H_
#define NETLOBBY_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <messages.h>
#include <listener.h>
#include <sender.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <netCommon.h>

using namespace boost::posix_time;
using boost::asio::ip::udp;

namespace FastEcslent {

	class Engine;

	class NetLobby {

	private:
		boost::thread netThread;

		int TICK_SLEEP_TIME;

		ptime  startTime;

		boost::posix_time::milliseconds *sleepTime;
		boost::posix_time::milliseconds *lobbySleepTime;

		NetCommon *netCommon;

		Engine*      engine;
		Listener*    listener;
		Sender*      sender;

		void init();
		void runThread();
		LobbyServer* fillLobbyClients(const std::string &gameName, const std::string &ip, int port, int side, int player, int slot, bool isHost);

		void handleClientMessages();
		void handleServerMessages();

		void clientTick();
		void serverTick();

		//helpers
		void sendLobbyServerInfo();
		void updateLobbyServer(Message *m);

		//server functions
		void joinRequest(Message *m);
		void clientEnter(Message *m);
		void serverExit(Message *m);
		void clientExit(Message *m);
		void clientSwap(Message *m);
		void startByRemote(Message *m);

	public:
		std::string myIPAddress;

		int port;
		bool quit;

		NetLobby(Engine* eng);

		void run();

		void sendLobbyClientResponse(const std::string &clientIP, int port, int side, int player);
		void lobbyStart();

		void lobbyJoinGame(const std::string &ip, int port);
		void lobbyExitGame(const std::string &ip, int port);
		void lobbySlotSwapReq(const std::string &serverip, int port, int newpos, int oldpos, int newside, int newplayer);
		virtual ~NetLobby();
	};

}

#endif /* NETLOBBY_H_ */
