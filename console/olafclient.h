#ifndef OLAFCLIENT_H
#define OLAFCLIENT_H

#include <QTcpSocket>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <set>

#include "debug.hpp"
#include <bone/olaf.h>
#include <bone/olaf_api.h>

namespace Net{

constexpr char BoardIp[] =	"192.168.7.2";
constexpr uint16_t BoarPort =	1234;

class OlafClient : public QObject
{
	Q_OBJECT

	QVector<int> to_ping;
	QVector<QThread> threads;

	QString get_device_name(QTcpSocket *sock);
	int set_blocking(sock_t);
	void device_keep_alive(QTcpSocket *, const QString &);
public:
	explicit OlafClient();
	~OlafClient();

public slots:
	void ping();

signals:
	void found_device(const QString &, const QString &);
	void device_died(const QString &);
};

} /* namespace Net */

#endif // OLAFCLIENT_H
