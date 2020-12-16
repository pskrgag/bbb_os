#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>
#include <QListWidget>
#include <QSharedPointer>
#include <QThread>

#include <new>		/* std::bad_alloc */
#include <algorithm>

#include "debug.hpp"
#include "olafclient.h"

namespace Ui {
class MainWindow;
} /* Ui namspace */

namespace Gui {

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void add_device(QListWidgetItem *);
	void remove_device(QListWidgetItem *);
private:
	Net::OlafClient *pinger;
	Ui::MainWindow *ui;
	QSplitter *horizontal_splitter;
	QSplitter *vertical_splitter;
	QListWidget *avail_devices;
	QWidget *device_state;
	QTextEdit *console_logger;
	QList<QSharedPointer<QListWidgetItem>> device_list;
	QThread pinger_thread;

public slots:
	void new_device_found(const QString &name, const QString &ip);
};

} /* Gui namespace */

#endif // MAINWINDOW_HPP
