#ifndef OLAFCLIENT_H
#define OLAFCLIENT_H

#include <QTcpSocket>

namespace Net{

class OlafClient
{
	QTcpSocket socket;
public:
	explicit OlafClient();
};

} /* namespace Net */

#endif // OLAFCLIENT_H
