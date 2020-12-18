#include <QApplication>
#include <QTextEdit>
#include <QSplitter>

#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Gui::MainWindow window;

	window.show();
	return app.exec();
}
