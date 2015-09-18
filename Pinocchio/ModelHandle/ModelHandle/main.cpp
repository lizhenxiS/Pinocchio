#include "modelhandle.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ModelHandle w;
	w.show();
	return a.exec();
}
