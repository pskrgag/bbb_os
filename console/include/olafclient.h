#ifndef OLAFCLIENT_H
#define OLAFCLIENT_H

#include <QObject>
#include <QTcpSocket>

#include "net.h"

namespace Net {

class OlafClient : public QObject
{
	Q_OBJECT
	QTcpSocket socket;
public:
	OlafClient(const QString &);
	~OlafClient();
};


} /* Net */

#endif // OLAFCLIENT_H
