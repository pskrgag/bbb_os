#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>
#include <QListWidget>
#include <QSharedPointer>

#include <new>		/* std::bad_alloc */
#include <algorithm>

#include "debug.hpp"

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
	/* members */
	Ui::MainWindow *ui;
	QSplitter *horizontal_splitter;
	QSplitter *vertical_splitter;
	QListWidget *avail_devices;
	QWidget *device_state;
	QTextEdit *console_logger;
	QList<QSharedPointer<QListWidgetItem>> device_list;
};

} /* Gui namespace */
#endif // MAINWINDOW_HPP
