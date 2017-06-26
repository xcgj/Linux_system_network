
#include<QCoreApplication>
#include<MyTufao.h>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    new MyTufao;

    return app.exec();
}
