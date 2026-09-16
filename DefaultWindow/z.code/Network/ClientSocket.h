#pragma once

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	ClientSocket operator=(ClientSocket&) = delete;
	ClientSocket operator()(ClientSocket&) = delete;

public:
	SOCKET Socket;
	SOCKADDR_IN SocketInfo = { 0 };
	int InfoSize = sizeof(SocketInfo);
	int Index;


};