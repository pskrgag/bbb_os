#ifndef OlafPinger_H
#define OlafPinger_H

#include <QTcpSocket>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <set>

#include "debug.h"
#include "net.h"
#include "olafstatuscheker.h"
#include <bone/api/olaf_api.h>
#include <olaf/olaf.h>

namespace Net{

class OlafPinger : public QObject
{
	Q_OBJECT

	QVector<int> to_ping;
	QVector<QThread> threads;

	QString get_device_name(QTcpSocket *sock);
	int set_blocking(sock_t);
	void device_keep_alive(QTcpSocket *, const QString &);
public:
	explicit OlafPinger();
	~OlafPinger();

	int connect_to_device();

public slots:
	void ping();
	void device_disconnected(int i);
signals:
	void found_device(const QString &, const QString &);
	void device_died(const QString &);
	void failed_to_get_name(const QString &);
	void device_not_responding(const QString &);
};

} /* namespace Net */

#endif // OlafPinger_H
