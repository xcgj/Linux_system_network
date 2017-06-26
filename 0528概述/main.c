include <QAppication>
#include "mainwindow.h"

int main(int argc, char * argv[])
{
	QApplicationn app(argc, argv);

	MainWindow manWnd;
	manWnd.show();

	return app.exec();

}


