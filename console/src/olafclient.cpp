#include "olafclient.h"

Net::OlafClient::OlafClient(const QString &ip)
{
	socket.connectToHost(ip, Net::BoarPort);
}

Net::OlafClient::~OlafClient() = default;
