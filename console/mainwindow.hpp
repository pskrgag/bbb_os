#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>
#include <QListWidget>
#include <QSharedPointer>
#include <QThread>
#include <QHBoxLayout>

#include <new>		/* std::bad_alloc */
#include <algorithm>

#include "debug.hpp"
#include "olafclient.h"

namespace Gui {

enum EventLevels {
	INFO,
	WARNING,
	ERROR,
};

class MainWindow : public QSplitter
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void add_new_event(const QString &info, Gui::EventLevels level);
	void remove_device(QListWidgetItem *);
private:
	Net::OlafClient *pinger;
	QSplitter *vertical_splitter;
	QListWidget *avail_devices;
	QWidget *device_state;
	QListWidget *console_logger;
	QThread pinger_thread;
	QMap<EventLevels, QString> icons_map;
public slots:
	void new_device_found(const QString &name, const QString &ip);
};

} /* Gui namespace */

#endif // MAINWINDOW_HPP
