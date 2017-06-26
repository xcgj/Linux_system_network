#ifndef MAINWIND_H
#define MAINWIND_H

#include <QWidget>
#include <QTextEdit>    //多行输入
#include <QTextBrowser> //输出显示
#include <QPushButton>
#include <QListWidget>
#include <QTcpSocket>
#include <QHostAddress> //保存ip地址
#include <QByteArray>//Qt字节数组

//客户端
class MainWind : public QWidget
{
    Q_OBJECT
public:
    explicit MainWind(QWidget *parent = 0);

    QListWidget * leftWindow;
    QWidget * rightWindow;

    QTextEdit * input;
    QTextBrowser * output;
    QPushButton * send;

    QTcpSocket * socket;
    int packetSize;
    QByteArray buf;

    void readData();

signals:

public slots:
    void onReadyRead();
    void onSend();
};

#endif // MAINWIND_H
