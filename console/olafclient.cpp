#include "olafclient.h"

#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

Net::OlafClient::OlafClient():
	socket(this),
	to_ping(100)
{
	for(int i = 0; i < 100; ++i)
		to_ping[i] = i + 1;
}

Net::OlafClient::~OlafClient()
{
	socket.close();
}

void Net::OlafClient::ping()
{
	const QString ip = "192.168.7.";
	QString name;

	while (1) {
		for (int i = 0; i < to_ping.size(); ++i) {
			QString current_ip(ip + QString::number(to_ping[i]));

			DEBUG_LOG << "Pinging " + current_ip;

			socket.connectToHost(current_ip, BoarPort);

			if (socket.waitForConnected(100)) {
				name = get_device_name();
				to_ping.remove(i);
				found_device(name, current_ip);
			}

			socket.close();
		}
	}
}

QString Net::OlafClient::get_device_name()
{
	struct olaf_device_info info;
	int res;
	int fd = socket.socketDescriptor();

	set_blocking(fd);

	res = olaf_call(static_cast<sock_t>(fd), OLAF_GET_DEVICE_INFO, static_cast<void *>(&info));
	if (res) {
		DEBUG_LOG << "Failed to olaf_call" << QString::number(res);
	}

	DEBUG_LOG << info.name;

	return QString(info.name);
}

void Net::OlafClient::set_blocking(sock_t sock)
{
	int flags = fcntl(sock, F_GETFL);

	fcntl(sock, F_SETFL, flags & (~O_NONBLOCK));
}
