/*
 * lobbyMgr.h
 *
 *  Created on: August 17, 2013
 *      Author: siming
 */

#ifndef LOBBYMGR_H_
#define LOBBYMGR_H_


#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreVector2.h>
#include <OgreColourValue.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <boost/lexical_cast.hpp>

#include <gfxMgr.h>
#include <SdkTrays.h>
#include <engine.h>
#include <stdlib.h>
#include <enums.h>
#include <const.h>
#include <netLobby.h>

namespace OgreGFX {
	class GraphicsInteractionManager;

	enum LobbyState {
		GAMEMENU    = 0,
		LOBBY = 1,

		NLOBBYSTATES
	};

	/*!
	 * LobbyNodeInfo, each host/client is a LobbyNode
	 */
	struct LobbyNode{
		std::string name;
		std::string ip;
		FastEcslent::Side side;
		FastEcslent::Player player;
		int slot;//enum this
		bool isHost;
		int port;
		long time;
	};

	/*!
	 * class LobbyMgr
	 * Inherited from GFXMgr, FrameListener
	 */
	class LobbyMgr : public GFXMgr, public Ogre::FrameListener, public OIS::KeyListener,
			 public OIS::MouseListener, public OgreBites::SdkTrayListener {
	public:
		LobbyMgr(FastEcslent::Engine *engine, GraphicsInteractionManager *gim, OIS::Mouse *m, OIS::Keyboard *k);
		~LobbyMgr();

		//FrameListener
	    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	    virtual bool frameStarted(const Ogre::FrameEvent& evt);
	    virtual bool frameEnded(const Ogre::FrameEvent& evt);

	    // OIS::KeyListener
	    virtual bool keyPressed( const OIS::KeyEvent &arg );
	    virtual bool keyReleased( const OIS::KeyEvent &arg );
	    // OIS::MouseListener
	    virtual bool mouseMoved( const OIS::MouseEvent &arg );
	    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	    //GFXMgr
	    virtual void initialize();

	    //OgreBites::SdkTrayListener
		virtual void buttonHit(OgreBites::Button* button);
		virtual void itemSelected(OgreBites::SelectMenu* menu);
		virtual void labelHit(OgreBites::Label* label);
		virtual void sliderMoved(OgreBites::Slider* slider);
		virtual void checkBoxToggled(OgreBites::CheckBox* box);
		virtual void okDialogClosed(const Ogre::DisplayString& message);
		virtual void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

		/*!
		 * Update servers who currently created a Lobby in the LAN.
		 * @param gName: Game name
		 * @param ip: server ip
		 * @param port: server port
		 */
		void addRemoteServer(const std::string &gName, const std::string &ip, int port);

		/*!
		 * Add a node to lobby
		 * 1. Update node information
		 * 2. Update slots
		 * @param gName: Game name
		 * @param nodeip: node ip address
		 * @param port: lobby port
		 * @param side: node side
		 * @param player: node player
		 * @param slot: node slot in the lobby
		 * @param isHost: is this node the host of lobby
		 */
		//use paramter LobbyNode instead
		//addNodeToSlot
		void addNode(const std::string &gName, const std::string &nodeip, int port, int side, int player, int slot, bool isHost);

		/*!
		 * Slot changed from the server, remove the node information from the old slot
		 * @param nodeip: node ip to be removed from the slot.
		 */
		void removeNodeFromSlot(const std::string &nodeip);

		/*!
		 * Function as a SERVER - Accept the request from client of join the lobby.
		 * Assign side and player to the new node, and send confirmation to the client node.
		 * @param clientName: client name
		 * @param ip: client ip
		 * @param port: server port
		 */
		void handleJoinRequest(const std::string &clientName, const std::string &ip, int port);

		/*!
		 * Function as a CLIENT - Receive the response from the server, and enter the lobby
		 * @param serverip: ip address of the server lobby
		 * @param port: port of the server lobby
		 * @param side: side assigned by the server
		 * @param player: player assigned by the server
		 */
		void lobbyEnter(const std::string &serverip, int port, int side, int player);

		/*!
		 * Function as a SERVER - Handle client swap slot request, swap 2 slots
		 * @param newslot: slot 1
		 * @param oldslot: slot 2
		 * @param newside: new side
		 * @param newplayer: new player
		 */
		void swapNodeSlot(int newslot, int oldslot, int newside, int newplayer);

		/*!
		 * Function as a CLIENT - Exit from a lobby
		 */
		void lobbyExit(const std::string &clientip, int port);

		/*!
		 * Set the game started flag to true.
		 */
		void startGame(const std::string &ip, int port);

		/*!
		 * Store all nodes which are inside the lobby.
		 */
		std::map<std::string, LobbyNode*> nodeMap;

		/*!
		 * Store all servers in the LAN.
		 */
		std::map<std::string, LobbyNode*> remoteServerMap;

		std::pair<std::string, int> getLobbyHost();

		LobbyState state;

	private:
		FastEcslent::NetLobby *netLobby;
		FastEcslent::Engine *engine;
	    OgreBites::SdkTrayManager* mTrayMgr;
	    OIS::Mouse*    mouse;
	    OIS::Keyboard* keyboard;

	    std::map<int, std::string> slots;

	    //none tray
	    OgreBites::Label *lobbyLb;
	    OgreBites::Separator *separatorTop;
	    OgreBites::Label *playerLb[FastEcslent::MaxPlayers];
	    OgreBites::SelectMenu *playerColor[FastEcslent::MaxPlayers];
	    OgreBites::SelectMenu *playerSide[FastEcslent::MaxPlayers];
	    OgreBites::Button *lobbyStartBt;
	    OgreBites::Button *lobbyExitBt;

	    const static int LobbyTitleWidth = 400;
	    const static int LobbySepWidth = 800;
	    const static int LobbyPlayerWidth =400;
	    const static int LobbyPlayerColorWidth = 100;
	    const static int LobbyPlayerSideWidth = 100;
	    const static int LobbyBtnWidth = 150;
	    const static int LobbyGapH = 10;
	    const static int LobbyGapV = 50;
	    //menu
	    const static int GameNemuWidth = 200;

	    std::string exitQuestion;

	    //center tray
	    OgreBites::DecorWidget *logo;
	    OgreBites::Separator *separatorCenter;
	    OgreBites::CheckBox* soundCB;
	    OgreBites::CheckBox* serverCB;
	    OgreBites::Button * createBt;
	    OgreBites::Button * exitBt;

	    //top right
	    OgreBites::TextBox* networkTB;

	    //bottom
	    OgreBites::TextBox* messageTB;
	    OgreBites::Button* sendBt;

	    //menu
	    OgreBites::SelectMenu *remoteServerMenu;
	    OgreBites::Button * joinBt;

		ptime startTime;

		long LOBBY_TOLERANCE;

		//Update server list and slot list
		void refreshServerAndSlots();

		//initialize GUI component.
		void init();
	    void initGUIGameMenu();
	    void initGUILobby();
	    void loadOptions();

	    void setNoneTrayPosition();
	    void clearPlayerLabel();
	    void handleExitButton();

	    //Player slots related functions
	    int getEmptySlot();
	    int getEmptySlot(int side);
	    std::string getLobbyString(const std::string &lobbyName, const std::string &ip, int port);
	    std::string getPlayerString(const std::string &playerName, const std::string &ip);

	    //Game functions
	    void loadGame();
	    void joinGame();
	    void configureGame();

	    void switchState();
	    void slotSwap(int slot1,int slot2);
	    //Clean up
	    void clearServerMap();
	    void clearNodeMap();
	};
}



#endif /* LOBBYMGR_H_ */
