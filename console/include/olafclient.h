#ifndef OLAFCLIENT_H
#define OLAFCLIENT_H

#include <QObject>

class olafclient : public QObject
{
	Q_OBJECT
public:
	explicit olafclient(QObject *parent = nullptr);

signals:

public slots:
};

#endif // OLAFCLIENT_H