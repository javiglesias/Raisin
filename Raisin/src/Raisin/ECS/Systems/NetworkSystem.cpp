#include "NetworkSystem.h"

NetworkSystem::NetworkSystem()
{
	serv_conn = new ServerConnection(m_port);
	client_conn = new ClientConnection(m_ip, m_port);
}

NetworkSystem::~NetworkSystem()
{
	free(serv_conn);
	free(client_conn);
}

void NetworkSystem::StartServer()
{
	serv_conn->init();
}

void NetworkSystem::StartClient()
{
	client_conn->init();
}
