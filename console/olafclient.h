#ifndef OLAFCLIENT_H
#define OLAFCLIENT_H

#include <QTcpSocket>

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

	QTcpSocket socket;
	QVector<int> to_ping;

	QString get_device_name();
	void set_blocking(sock_t sock);
public:
	explicit OlafClient();
	~OlafClient();

public slots:
	void ping();

signals:
	void found_device(const QString &, const QString &);
};

} /* namespace Net */

#endif // OLAFCLIENT_H
