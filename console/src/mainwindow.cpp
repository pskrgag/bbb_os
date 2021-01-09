#include "mainwindow.h"

#define DEVICE_IMG_PATH			"1.png"
#define INFO_IMG_PATH			"2.png"
#define ERROR_IMG_PATH			"3.png"

#define PASSWD_SCRIPT_NAME		"passwd.sh"

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
	pinger(new Net::OlafPinger),
	vertical_splitter(new QSplitter(Qt::Vertical)),
	avail_devices(new QListWidget(this)),
	device_state(new QWidget(this)),
	console_logger(new QListWidget(this)),
	rigth_box(new QVBoxLayout(this)),
	rigth_widget(new QWidget(this)),
	buttons(new QWidget(this)),
	ssh_button(new QPushButton("SSH", this)),
	uart_button(new QPushButton("UART", this)),
	buttons_layout(new QHBoxLayout(this)),
	icons_map({{INFO, INFO_IMG_PATH}, {ERROR, ERROR_IMG_PATH}})
{
	connect(pinger, &Net::OlafPinger::found_device, this, &Gui::MainWindow::new_device_found);
	connect(pinger, &Net::OlafPinger::device_died, this, &Gui::MainWindow::remove_device);
	connect(pinger, &Net::OlafPinger::device_not_responding, this, &Gui::MainWindow::device_not_responding);
	connect(pinger, &Net::OlafPinger::failed_to_get_name, this, &Gui::MainWindow::call_error);

	connect(this, &Gui::MainWindow::device_disconnected, pinger, &Net::OlafPinger::device_disconnected, Qt::DirectConnection);
	connect(avail_devices, &QListWidget::customContextMenuRequested, this, &Gui::MainWindow::device_clicked);

	connect(ssh_button, &QPushButton::clicked, this, &Gui::MainWindow::ssh_connect);
	connect(uart_button, &QPushButton::clicked, this, &Gui::MainWindow::uart_connect);

	pinger->moveToThread(&pinger_thread);

	/* Start ping routine */
	connect(&pinger_thread, &QThread::started, pinger, &Net::OlafPinger::ping);
	pinger_thread.start();

	/* widgets set up */
	device_state->setStyleSheet("background-color: blue");
	rigth_box->addWidget(avail_devices, Qt::AlignBottom);
	rigth_box->addWidget(buttons, Qt::AlignTop);
	rigth_box->setStretch(1, 3);

	buttons_layout->addWidget(uart_button, Qt::AlignRight);
	buttons_layout->addWidget(ssh_button, Qt::AlignLeft);
	buttons_layout->setStretch(0, 0);
	buttons->setLayout(buttons_layout);

	rigth_widget->setLayout(rigth_box);

	/* splitters set up */
	addWidget(rigth_widget);
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

	device_disconnected(*(--list[0]->text().toStdString().end()) - '0');

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

void Gui::MainWindow::ssh_connect()
{
	QListWidgetItem *item;
	QProcess process;

	item = avail_devices->currentItem();
	if (!item)
		return;

	process.start("x-terminal-emulator -e \"ssh -i id_rsa.pub -l root 192.168.7.2\"");
	process.waitForFinished();
}

void Gui::MainWindow::uart_connect()
{
	QProcess process;

	process.start(QString("sh -c \"") + QString("/bin/echo -ne '#!/usr/bin/env sh\nzenity --password --title=Password\n' > ") + QString(PASSWD_SCRIPT_NAME) +
		      QString(" && chmod +x ") + QString(PASSWD_SCRIPT_NAME) + QString("\""));
	process.waitForFinished();

	process.start("x-terminal-emulator -e \"SUDO_ASKPASS='" + QDir::currentPath() + "/" + PASSWD_SCRIPT_NAME + "' sudo -A minicom\"");
	process.waitForFinished();
}

void Gui::MainWindow::call_error(const QString &err)
{
	add_new_event("Failed to olaf_call: ", Gui::ERROR);
}

void Gui::MainWindow::device_not_responding(const QString &name)
{
	QList<QListWidgetItem *> list = avail_devices->findItems(name, Qt::MatchContains);

	if (!list.size())
		return;

	list[0]->setForeground(Qt::gray);
}
