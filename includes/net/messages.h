/*
 * state.h
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#ifndef STATE_H_
#define STATE_H_


namespace FastEcslent {

	const int MaxMessageSize = 65536; //udp limit

	struct __attribute__((__packed__)) PackedHeader{
		unsigned char msgType;
		int   millisecondsFromStart;
		int   numberOfStructs;
		int   sizeOfStruct;
	};

	typedef struct PackedHeader Header;

	enum MessageTypes {
	  INFOMESSAGETYPE              = 0,
	  STATEMESSAGETYPE             = 1,
	  REQUESTINFOMESSAGETYPE       = 2,
	  REQUESTSHIPSTATUSMESSAGETYPE = 3,
	  COMMANDENTITYMESSAGETYPE     = 4,
	  CREATEENTITYMESSAGETYPE      = 5,
	  REPOSITIONENTITYMESSAGETYPE  = 6,
	  SQUELCHMESSAGETYPE           = 7,
	  NUMBEROFMESSAGETYPES         = 8,

	  CREATEENTITYREQUESTMESSAGETYPE  = 20,
	  CREATEENTITYRESPONSEMESSAGETYPE = 21,
	  CREATEENTITYCANCELMESSAGETYPE   = 22,
	  CREATEENTITYPAUSEMESSAGETYPE    = 23,

	  LOBBYSERVERTYPE              = 30,
	  LOBBYJOINREQUESTTYPE         = 31,
	  LOBBYJOINRESPONSETYPE        = 32,
	  LOBBYGAMESTARTTYPE           = 33,
	  LOBBYSLOTSWAPTYPE            = 34,
	  LOBBYEXITTYPE                = 35,

	  MINERALPATCHIDREQUESTTYPE    = 40,
	  MINERALPATCHIDRESPONSETYPE   = 41
	};

	struct __attribute__((__packed__)) PackedReqInfo{
		int id;
	};

	typedef struct PackedReqInfo ReqInfo;

	struct __attribute__((__packed__)) PackedState{
		int   id;
		int hp;
		float timeLeftToBirth;
		float px, py, pz;
		float vx, vy, vz;
		float yaw;
		float rSpeed;//rotational speed
		float ds;
		float dh;
		int flag;
	};

	typedef struct PackedState State;

	struct __attribute__((__packed__)) PackedInfo {
		int id;
		char label[256];
		char type[256] ;
		float maxSpeed;
		float maxSpeedReverse;
		float length;
		float beam;
		float draft;
		int   playerId;
		int   side;

	};

	typedef struct PackedInfo Info;

	struct __attribute__((__packed__)) PackedCommandEntity {
		int id;
		float dh;
		float ds;
	} ;

	typedef struct PackedCommandEntity CommandEntity;

	struct __attribute__((__packed__)) PackedSquelchEntity {
		int id;
	} ;

	typedef struct PackedSquelchEntity SquelchEntity;

	struct __attribute__((__packed__)) PackedCreateEntity{
		char label[256];
		char entType[256];
		float px, py, pz;
		float yaw;
	} ;

	typedef struct PackedCreateEntity CreateEntity;

	struct __attribute__((__packed__)) PackedCreateEntityRequest{
		int builderid;
		int entType;
		int side;
		int player;
		float px, py, pz;
		float yaw;
	} ;

	typedef struct PackedCreateEntityRequest CreateEntityRequest;

	struct __attribute__((__packed__)) PackedCreateEntityCancel{
		int builderid;
		int builtid;
	} ;

	typedef struct PackedCreateEntityCancel CreateEntityCancel;

	struct __attribute__((__packed__)) PackedCreatePause{
		int builderid;
		int builtid;
	} ;

	typedef struct PackedCreatePause CreateEntityPause;

	struct __attribute__((__packed__)) PackedCreateEntityResponse{
		int builderid;
		int builtid;
	} ;

	typedef struct PackedCreateEntityResponse CreateEntityResponse;

	struct  __attribute__((__packed__)) PackedRepositionEntity { //Force move entities
		int id;
		float px, py, pz;
		float vx, vy, vz;
		float yaw;
		char  label[256]; // optional
	};

	typedef struct PackedRepositionEntity RepositionEntity;

	struct  __attribute__((__packed__)) PackedLobbyServer {
		int  port;
		char name[64];
		unsigned int ip;
		int  side;
		int  player;
		int  slot;
		char isHost;
	};

	typedef struct PackedLobbyServer LobbyServer;

	struct  __attribute__((__packed__)) PackedLobbyJoinRequest {
		int  port;
		unsigned int serverIP;
		unsigned int clientIP;
		char clientName[64];
	};

	typedef struct PackedLobbyJoinRequest LobbyJoinRequest;

	struct  __attribute__((__packed__)) PackedLobbyJoinResponse {
		int  port;
		unsigned int serverIP;
		unsigned int clientIP;
		int  side;
		int  player;
	};

	typedef struct PackedLobbyJoinResponse LobbyJoinResponse;

	struct  __attribute__((__packed__)) PackedLobbyGameStart {
		int  port;
		unsigned int serverIP;
	};

	typedef struct PackedLobbyGameStart LobbyGameStart;

	struct  __attribute__((__packed__)) PackedLobbyExit {
		unsigned int serverIP;
		unsigned int clientIP;
		int port;
	};

	typedef struct PackedLobbyExit LobbyExit;

	struct  __attribute__((__packed__)) PackedLobbySlotSwap {
		unsigned int serverIP;
		unsigned int newpos;
		unsigned int oldpos;
		unsigned int newside;
		unsigned int newplayer;
		int port;
	};

	typedef struct PackedLobbySlotSwap LobbySlotSwap;

	struct  __attribute__((__packed__)) PackedMineralPatchIDRequest {
		int  mineralid;
	};

	typedef struct PackedMineralPatchIDRequest MineralPatchIDRequest;

	struct  __attribute__((__packed__)) PackedMineralPatchIDResponse {
		int  mineralid;
		int  patchid;
	};

	typedef struct PackedMineralPatchIDResponse MineralPatchIDResponse;

	struct __attribute__((__packed__)) PackedMessage {
		Header head;
		char   data[MaxMessageSize - sizeof(Header)];
	};

	typedef struct PackedMessage Message;

	const int MessageSize          = sizeof(Message);
	const int HeaderSize           = sizeof(Header);
	const int StateSize            = sizeof(State) - sizeof(unsigned short);
	const int ReqInfoSize          = sizeof(ReqInfo);
	const int InfoSize             = sizeof(Info);
	const int CommandEntitySize    = sizeof(CommandEntity);
	const int CreateEntitySize     = sizeof(CreateEntity);
	const int CreateEntityRequestSize  = sizeof(CreateEntityRequest);
	const int CreateEntityResponseSize = sizeof(CreateEntityResponse);
	const int CreateEntityCancelSize   = sizeof(CreateEntityCancel);
	const int CreateEntityPauseSize    = sizeof(CreateEntityPause);
	const int RepositionEntitySize     = sizeof(RepositionEntity);
	const int SquelchEntitySize        = sizeof(SquelchEntity);

	const int LobbyServerSize      = sizeof(LobbyServer);
	const int LobbyJoinRequestSize = sizeof(LobbyJoinRequest);
	const int LobbyJoinResponseSize= sizeof(LobbyJoinResponse);
	const int LobbyGameStartSize   = sizeof(LobbyGameStart);
	const int LobbyExitSize        = sizeof(LobbyExit);
	const int LobbySlotSwapSize    = sizeof(LobbySlotSwap);

	const int MineralPatchIDRequestSize  = sizeof(MineralPatchIDRequest);
	const int MineralPatchIDResponseSize = sizeof(MineralPatchIDResponse);

	void printMessageHeader(Header h);
	void printMessageData(Message *m);
	void printMessage(Message *m);
	void printState(State *s);


}

#endif /* STATE_H_ */
