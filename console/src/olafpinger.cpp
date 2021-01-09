#include "olafpinger.h"

#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#define QT_DESTRIPTOR_TO_UNIX(fd)	static_cast<sock_t>(fd->socketDescriptor())

Net::OlafPinger::OlafPinger():
	to_ping(100)
{
	for(int i = 0; i < 100; ++i)
		to_ping[i] = i + 1;
}

Net::OlafPinger::~OlafPinger() = default;

void Net::OlafPinger::ping()
{
	const QString ip = "192.168.7.";
	QString name;
	Net::OlafStatusChecker *checker;
	QThread *thread;

	while (1) {
		for (int i = 0; i < to_ping.size(); ++i) {
			QString current_ip(ip + QString::number(to_ping[i]));

			checker = new Net::OlafStatusChecker;

			checker->connectToHost(current_ip, BoarPort);

			if (checker->waitForConnected(30)) {
				name = get_device_name(checker);

				if (name.size() == 0)
					continue;

				to_ping.remove(i);

				{
					thread = new QThread;
					checker->moveToThread(thread);

					connect(thread, &QThread::started, checker, [name, checker](){checker->keep_alive(name);});
					connect(thread, &QThread::finished, thread, &QObject::deleteLater);
					connect(checker, &Net::OlafStatusChecker::device_died, this, &Net::OlafPinger::device_died, Qt::DirectConnection);
					connect(checker, &Net::OlafStatusChecker::device_not_responding, this, &Net::OlafPinger::device_not_responding, Qt::DirectConnection);

					thread->start();
				}

				found_device(name, current_ip);

				checker = nullptr;
				continue;
			}

			delete checker;
		}
	}
}

QString Net::OlafPinger::get_device_name(QTcpSocket *socket)
{
	struct olaf_device_info info;
	int res;
	sock_t fd = QT_DESTRIPTOR_TO_UNIX(socket);

	res = set_blocking(fd);
	if (res < 0)
		return {};

	res = olaf_call(fd, OLAF_GET_DEVICE_INFO, static_cast<void *>(&info));
	if (res) {
		failed_to_get_name("Result = " + QString::number(res));
		return {};
	}

	return QString(info.name);
}

int Net::OlafPinger::set_blocking(sock_t sock)
{
	int flags = fcntl(sock, F_GETFL);

	if (flags < 0)
		return flags;

	return fcntl(sock, F_SETFL, flags & (~O_NONBLOCK));
}

void Net::OlafPinger::device_disconnected(int i)
{
	to_ping.insert(i, i);
}
