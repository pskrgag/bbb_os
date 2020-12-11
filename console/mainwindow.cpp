#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#define IMG_PATH			"/home/pskrgag/Pictures/1.png"		/* TODO: make this picture part of the project */

Gui::MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	horizontal_splitter(new QSplitter(Qt::Horizontal)),
	vertical_splitter(new QSplitter(Qt::Vertical)),
	avail_devices(new QListWidget(this)),
	device_state(new QWidget(this)),
	console_logger(new QTextEdit(this))
{
	if (!horizontal_splitter || !avail_devices || !device_state || !console_logger)
		throw std::bad_alloc();

	/* widgets set up */
	console_logger->setReadOnly(true);
	device_state->setStyleSheet("background-color: blue");

	/* splitters set up */
	horizontal_splitter->addWidget(avail_devices);
	horizontal_splitter->addWidget(vertical_splitter);

	vertical_splitter->addWidget(device_state);
	vertical_splitter->addWidget(console_logger);

	horizontal_splitter->setSizes(QList<int>() << 50 << 200);
	vertical_splitter->setSizes(QList<int>() << 200 << 50);

	setCentralWidget(horizontal_splitter);
	ui->setupUi(this);
}

Gui::MainWindow::~MainWindow()
{
	delete ui;
	delete horizontal_splitter;
	delete vertical_splitter;
	delete avail_devices;
	delete device_state;
	delete console_logger;
}

void Gui::MainWindow::add_device(QListWidgetItem *item)
{
	if (!item)
		return;

	device_list.push_back(QSharedPointer<QListWidgetItem>(item));
	avail_devices->addItem(device_list.back().data());
}

void Gui::MainWindow::remove_device(QListWidgetItem *item)
{
	auto item_iter = std::find(device_list.begin(), device_list.end(), QSharedPointer<QListWidgetItem>(item));

	if (!item)
		return;

	device_list.erase(item_iter);
	avail_devices->removeItemWidget((*item_iter).data());
}
