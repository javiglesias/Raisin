#pragma once
#include "../../Network/Server/ServerConnection.h"
#include "../../Network/Client/ClientConnection.h"
#include <cstdio>

class NetworkSystem
{
public:
	NetworkSystem();
	~NetworkSystem();
	void StartServer();
	void StartClient();
private:
	ServerConnection* serv_conn;
	ClientConnection* client_conn;
	unsigned int m_port = 6969;
	char m_ip[32] = "127.0.0.1";
};

