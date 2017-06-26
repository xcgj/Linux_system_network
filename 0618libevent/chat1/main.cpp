
#include <QApplication>
#include "MainWind.h"
//#include <iostream>

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    MainWind w;
    w.show();

    return app.exec();
}
