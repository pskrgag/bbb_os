#include <QApplication>
#include <QTextEdit>
#include <QSplitter>

#include "mainwindow.hpp"
#include "olafclient.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Gui::MainWindow window;

	window.show();
	return app.exec();
}
