#include "olafstatuscheker.h"

void Net::OlafStatusChecker::keep_alive(const QString &name)
{
	uint64_t data = OLAF_KEEP_ALIVE;
	int res, is_dying = 0;
	QTcpSocket *sock = this;

	while ((res = sock->write(reinterpret_cast<char *>(&data), sizeof(data))) == sizeof(data)) {
		if (!sock->flush() && !is_dying) {
			device_not_responding(name);
			is_dying = 1;
			QThread::sleep(2);
		}

		QThread::sleep(1);
	}

	device_died(name);

	delete sock;
}
