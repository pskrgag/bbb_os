#ifndef OlafStatusChecker_H
#define OlafStatusChecker_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

#include "net.h"
#include "debug.h"
#include <bone/api/olaf_api.h>

namespace Net {

class OlafStatusChecker : public QTcpSocket
{
	Q_OBJECT
public:
	OlafStatusChecker() = default;
	~OlafStatusChecker() = default;
	void keep_alive(const QString &);
signals:
	void device_died(const QString &);
	void device_not_responding(const QString &);
};

} /* Net */

#endif // OlafStatusChecker_H
