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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <new>
#include <algorithm>
#include <cstring>

#include "debug.h"
#include "olafstatuscheker.h"
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
	void ssh_connect(void);
private:
	Net::OlafStatusChecker *pinger;
	QSplitter *vertical_splitter;
	QListWidget *avail_devices;
	QWidget *device_state;
	QListWidget *console_logger;
	QThread pinger_thread;
	QVBoxLayout *rigth_box;
	QWidget *rigth_widget;
	QWidget *buttons;
	QPushButton *ssh_button;
	QHBoxLayout *buttons_layout;
	const QMap<EventLevels, QString> icons_map;
private slots:
	void connect_to_device(QListWidgetItem *item);
	void remove_device(const QString &);
public slots:
	void new_device_found(const QString &, const QString &);
	void device_clicked(const QPoint &);
};

} /* Gui namespace */

#endif // MAINWINDOW_HPP
