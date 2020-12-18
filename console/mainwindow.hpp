#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>
#include <QListWidget>
#include <QSharedPointer>
#include <QThread>
#include <QHBoxLayout>
#include <QMenu>

#include <new>		/* std::bad_alloc */
#include <algorithm>
#include <cstring>

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

	void add_new_event(const QString &, Gui::EventLevels);
private:
	Net::OlafClient *pinger;
	QSplitter *vertical_splitter;
	QListWidget *avail_devices;
	QWidget *device_state;
	QListWidget *console_logger;
	QThread pinger_thread;
	QMap<EventLevels, QString> icons_map;

private slots:
	int connect_to_device(QListWidgetItem *item);
	void remove_device(const QString &);
public slots:
	void new_device_found(const QString &, const QString &);
	void device_clicked(const QPoint &);
};

} /* Gui namespace */

#endif // MAINWINDOW_HPP
