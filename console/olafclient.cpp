#include "olafclient.h"

#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#define QT_DESTRIPTOR_TO_UNIX(fd)	static_cast<sock_t>(fd)

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

			if (socket.waitForConnected(30)) {
				name = get_device_name();

				if (name.size() == 0)
					continue;

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
	sock_t fd = QT_DESTRIPTOR_TO_UNIX(socket.socketDescriptor());

	res = set_blocking(fd);
	if (res < 0)
		return {};

	res = olaf_call(fd, OLAF_GET_DEVICE_INFO, static_cast<void *>(&info));
	if (res) {
		DEBUG_LOG << "Failed to olaf_call" << QString::number(res);
	}

	DEBUG_LOG << info.name;

	return QString(info.name);
}

int Net::OlafClient::set_blocking(sock_t sock)
{
	int flags = fcntl(sock, F_GETFL);

	if (flags < 0)
		return flags;

	return fcntl(sock, F_SETFL, flags & (~O_NONBLOCK));
}
