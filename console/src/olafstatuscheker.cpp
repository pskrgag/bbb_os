#include "olafstatuscheker.h"

#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#define QT_DESTRIPTOR_TO_UNIX(fd)	static_cast<sock_t>(fd->socketDescriptor())

Net::OlafStatusChecker::OlafStatusChecker():
	to_ping(100)
{
	for(int i = 0; i < 100; ++i)
		to_ping[i] = i + 1;
}

Net::OlafStatusChecker::~OlafStatusChecker() = default;

void Net::OlafStatusChecker::ping()
{
	const QString ip = "192.168.7.";
	QString name;
	QTcpSocket *socket;

	while (1) {
		for (int i = 0; i < to_ping.size(); ++i) {
			QString current_ip(ip + QString::number(to_ping[i]));

			socket = new QTcpSocket();

			socket->connectToHost(current_ip, BoarPort);

			if (socket->waitForConnected(10)) {
				name = get_device_name(socket);

				if (name.size() == 0)
					continue;

				to_ping.remove(i);

				QtConcurrent::run([this, socket, name](){
					this->device_keep_alive(socket, name);
				});

				found_device(name, current_ip);

				socket = nullptr;
				continue;
			}

			delete socket;
		}
	}
}

QString Net::OlafStatusChecker::get_device_name(QTcpSocket *socket)
{
	struct olaf_device_info info;
	int res;
	sock_t fd = QT_DESTRIPTOR_TO_UNIX(socket);

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

int Net::OlafStatusChecker::set_blocking(sock_t sock)
{
	int flags = fcntl(sock, F_GETFL);

	if (flags < 0)
		return flags;

	return fcntl(sock, F_SETFL, flags & (~O_NONBLOCK));
}

void Net::OlafStatusChecker::device_keep_alive(QTcpSocket *sock, const QString &name)
{
	uint64_t data = OLAF_KEEP_ALIVE;
	int res;

	while ((res = sock->write(reinterpret_cast<char *>(&data), sizeof(data))) == sizeof(data)) {
		if (!sock->flush())
			break;

		DEBUG_LOG << "res = " << QString::number(res);
		QThread::sleep(1);
	}

	DEBUG_LOG << "died" << QString::number(res);

	device_died(name);

	delete sock;
}
