#include "mainwindow.h"

#define DEVICE_IMG_PATH			"1.png"		/* TODO: make this picture part of the project */
#define INFO_IMG_PATH			"2.png"		/* TODO: make this picture part of the project */
#define ERROR_IMG_PATH			"3.png"		/* TODO: make this picture part of the project */

static inline QString device_info_to_ip(const QString &info)
{
	::strtok(const_cast<char *>(info.toStdString().data()), ":");
	return ::strtok(nullptr, ":");
}

static inline QString device_info_to_name(const QString &info)
{
	return ::strtok(const_cast<char *>(info.toStdString().data()), ":");
}

Gui::MainWindow::MainWindow(QWidget *parent):
	QSplitter(Qt::Horizontal, parent),
	pinger(new Net::OlafStatusChecker),
	vertical_splitter(new QSplitter(Qt::Vertical)),
	avail_devices(new QListWidget(this)),
	device_state(new QWidget(this)),
	console_logger(new QListWidget(this)),
	icons_map({{INFO, INFO_IMG_PATH}, {ERROR, ERROR_IMG_PATH}})
{
	/* Connect pinger to MainWindow */
	connect(pinger, &Net::OlafStatusChecker::found_device, this, &Gui::MainWindow::new_device_found);
	connect(avail_devices, &QListWidget::customContextMenuRequested, this, &Gui::MainWindow::device_clicked);
	connect(pinger, &Net::OlafStatusChecker::device_died, this, &Gui::MainWindow::remove_device);

	pinger->moveToThread(&pinger_thread);

	/* Start ping routine */
	connect(&pinger_thread, &QThread::started, pinger, &Net::OlafStatusChecker::ping);
	pinger_thread.start();

	/* widgets set up */
	device_state->setStyleSheet("background-color: blue");

	/* splitters set up */
	addWidget(avail_devices);
	addWidget(vertical_splitter);

	vertical_splitter->addWidget(device_state);
	vertical_splitter->addWidget(console_logger);

	resize(1500, 1000);
	setSizes(QList<int>() << width() * 2/7 << width() * 5/7);
	vertical_splitter->setSizes(QList<int>() << height() * 5/7 << height() * 2/7);

	avail_devices->setContextMenuPolicy(Qt::CustomContextMenu);
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

void Gui::MainWindow::remove_device(const QString &name)
{
	QList<QListWidgetItem *> list = avail_devices->findItems(name, Qt::MatchContains);

	if (!list.size())
		return;

	avail_devices->removeItemWidget(list[0]);
	delete list[0];

	add_new_event("Device " + name + " disconnected", INFO);
}

void Gui::MainWindow::device_clicked(const QPoint &point)
{
	QPoint globalPos = avail_devices->mapToGlobal(point);
	QMenu device_menu;
	QListWidgetItem *item = avail_devices->itemAt(point);
	QAction *a_connect, *a_disconnect;
	QBrush state;

	if (!item)
		return;

	a_connect = device_menu.addAction("Connect");
	a_disconnect = device_menu.addAction("Disconnect");

	state = item->foreground();

	if (state.color() == Qt::black)
		a_disconnect->setEnabled(false);
	else
		a_connect->setEnabled(false);

	connect(a_connect, &QAction::triggered, this, [item, this](bool){
		this->connect_to_device(item);
	});

	device_menu.exec(globalPos);
}

void Gui::MainWindow::new_device_found(const QString &name, const QString &ip)
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(DEVICE_IMG_PATH), name + ":" + ip);

	avail_devices->addItem(item);
	add_new_event("Found new device " + name, Gui::INFO);
}

void Gui::MainWindow::connect_to_device(QListWidgetItem *item)
{
	DEBUG_LOG << "Connecting to device " + device_info_to_ip(item->text());
	item->setForeground(Qt::red);
}
