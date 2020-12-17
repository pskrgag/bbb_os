#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#define DEVICE_IMG_PATH			"/home/pskrgag/Pictures/1.png"		/* TODO: make this picture part of the project */
#define INFO_IMG_PATH			"/home/pskrgag/Pictures/2.png"		/* TODO: make this picture part of the project */

Gui::MainWindow::MainWindow(QWidget *parent):
	QSplitter(Qt::Horizontal, parent),
	pinger(new Net::OlafClient),
	vertical_splitter(new QSplitter(Qt::Vertical)),
	avail_devices(new QListWidget(this)),
	device_state(new QWidget(this)),
	console_logger(new QListWidget(this))
{
	/* fill icons map */
	icons_map[INFO] = INFO_IMG_PATH;

	/* Connect pinger to MainWindow */
	connect(pinger, &Net::OlafClient::found_device, this, &Gui::MainWindow::new_device_found);
	pinger->moveToThread(&pinger_thread);

	/* Start ping routine */
	connect(&pinger_thread, &QThread::started, pinger, &Net::OlafClient::ping);
	pinger_thread.start();

	/* widgets set up */
	device_state->setStyleSheet("background-color: blue");

	/* splitters set up */
	addWidget(avail_devices);
	addWidget(vertical_splitter);

	vertical_splitter->addWidget(device_state);
	vertical_splitter->addWidget(console_logger);

	//vertical_splitter->setSizes(QList<int>() << 200 << 50);

	new_device_found("aaa", "bbb");
	new_device_found("CCC", "CCC");
	resize(1500, 1000);
	setSizes(QList<int>() << width() * 1/7 << width() * 6/7);
	vertical_splitter->setSizes(QList<int>() << height() * 5/7 << height() * 2/7);
}

Gui::MainWindow::~MainWindow()
{
	delete vertical_splitter;
	delete avail_devices;
	delete device_state;
	delete console_logger;
}

void Gui::MainWindow::add_new_event(const QString &info, Gui::EventLevels level)
{
	console_logger->addItem(new QListWidgetItem(QIcon(icons_map[level]), info));
}

void Gui::MainWindow::remove_device(QListWidgetItem *item)
{

}

void Gui::MainWindow::new_device_found(const QString &name, const QString &ip)
{
	avail_devices->addItem(new QListWidgetItem(QIcon(DEVICE_IMG_PATH), name + ":" + ip));
	add_new_event("Found new device " + name, Gui::INFO);
}
